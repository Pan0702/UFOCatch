#include "FbxParser.h"

#include <cmath>
#include <fstream>
#include <stdexcept>
#include <unordered_map>

#include <windows.h>
#include <zlib.h>

#include "../../Utils/MyMath.h"


// ----------------------------------------------------------------
//  FBX バイナリファイルを読み込む
// ----------------------------------------------------------------
bool FbxParser::Load(const std::string& path)
{
    std::ifstream file(path, std::ios::binary);
    if (!file)
    {
        MessageBoxA(nullptr, ("FBX ファイルを開けません: " + path).c_str(),
                    "FbxParser", MB_OK | MB_ICONERROR);
        return false;
    }

    file.seekg(0, std::ios::end);
    m_data.resize(static_cast<size_t>(file.tellg()));
    file.seekg(0);
    file.read(reinterpret_cast<char*>(m_data.data()),
              static_cast<std::streamsize>(m_data.size()));

    constexpr char kFbxMagic[] = "Kaydara FBX Binary  \x00\x1a\x00";
    constexpr int kFbxMagicSize = 23;
    if (m_data.size() < 27 ||
        memcmp(m_data.data(), kFbxMagic, kFbxMagicSize) != 0)
    {
        MessageBoxA(nullptr, "FBX バイナリフォーマットではありません",
                    "FbxParser", MB_OK | MB_ICONERROR);
        return false;
    }

    constexpr uint64_t kVersionOffset = 23;
    m_version = Read<uint32_t>(kVersionOffset);
    constexpr uint32_t kVersion64bit = 7500;
    m_is64bit = (m_version >= kVersion64bit);

    m_roots = ParseChildren(27, m_data.size());
    return true;
}

// ----------------------------------------------------------------
//
//  仕様
//    - Objects 直下の全 Geometry を結合して1つのメッシュとして出力する
//    - 各 Geometry に対応する Model の ローカル Transform (T/R/S) を適用する
//      （親 Model の Transform は適用しない）
//    - 頂点の一意性は (pos_idx, uv_idx) の組み合わせで判定する（法線は含めない）
//    - UV の V 軸反転は行わない（シェーダー側で処理する）
// ----------------------------------------------------------------
bool FbxParser::ExtractMesh(std::vector<MeshVertex>& outVerts, std::vector<uint32_t>& outIndices)
{
    Node* objects = FindNode(m_roots, "Objects");
    if (!objects)
    {
        MessageBoxA(nullptr, "FBX: Objects ノードが見つかりません",
                    "FbxParser", MB_OK | MB_ICONERROR);
        return false;
    }

    // --- Model ID → Node のマップを作成 ---
    m_modelMap.clear();
    for (auto& child : objects->children)
    {
        if (child.name == "Model")
            m_modelMap[ReadNodeId(child.propStart)] = &child;
    }

    // --- Geometry ID セットを作成 ---
    std::unordered_map<int64_t, Node*> geo_map;
    for (auto& child : objects->children)
    {
        if (child.name == "Geometry")
            geo_map[ReadNodeId(child.propStart)] = &child;
    }

    // --- Connections から接続情報を収集 ---
    //   geo_to_model      : Geometry ID → 直接の親 Model ID
    //   model_parent_map_ : Model ID    → 親 Model ID（グローバル行列の再帰計算用）
    std::unordered_map<int64_t, int64_t> geo_to_model;
    m_modelParentMap.clear();
    const Node* conn_node = FindNode(m_roots, "Connections");
    if (conn_node)
    {
        for (auto& c : conn_node->children)
        {
            if (c.name != "C") continue;
            uint64_t off = c.propStart;
            // conn_type (S) をスキップ
            if (off < m_data.size() && m_data[off] == 'S')
            {
                uint32_t l = Read<uint32_t>(off + 1);
                off += 5 + l;
            }
            // タグを実際に読んでオフセットを正確に進める
            const uint8_t tag1 = m_data[off];
            const int64_t id1 = ReadNodeId(off);
            off += (tag1 == 'L') ? 9u : 5u;
            const int64_t id2 = ReadNodeId(off);

            if (geo_map.count(id1))
                geo_to_model[id1] = id2;
            else if (m_modelMap.contains(id1) && m_modelMap.contains(id2))
                m_modelParentMap[id1] = id2;
        }
    }
    // // --- デバッグ: model_map_ / model_parent_map_ の内容を確認 ---
    // {
    //     char buf[256];
    //     sprintf_s(buf, "[FbxParser] model_map_ size=%zu  model_parent_map_ size=%zu\n",
    //               model_map_.size(), model_parent_map_.size());
    //     OutputDebugStringA(buf);
    // }
    //
    // // --- デバッグ: geo_to_model の内容を確認 ---
    // {
    //     char buf[256];
    //     sprintf_s(buf, "[FbxParser] geo_map size=%zu  geo_to_model size=%zu\n",
    //               geo_map.size(), geo_to_model.size());
    //     OutputDebugStringA(buf);
    // }
    // --- 全 Geometry を展開して結合 ---
    outVerts.clear();
    outIndices.clear();

    if (geo_map.empty())
    {
        MessageBoxA(nullptr, "FBX: Geometry ノードが見つかりません",
                    "FbxParser", MB_OK | MB_ICONERROR);
        return false;
    }

    for (auto& child : objects->children)
    {
        if (child.name != "Geometry") continue;

        const int64_t geo_id = ReadNodeId(child.propStart);
        Node* model = nullptr;
        const auto it = geo_to_model.find(geo_id);
        if (it != geo_to_model.end())
        {
            // // --- デバッグ: ID の照合を確認 ---
            // char buf[256];
            // sprintf_s(buf, "[FbxParser] geo_id=%lld  model_id_from_conn=%lld  in_model_map=%s\n",
            //           geo_id, it->second,
            // OutputDebugStringA(buf);


            const auto mit = m_modelMap.find(it->second);
            if (mit != m_modelMap.end()) model = mit->second;
        }

        if (!ExtractGeometry(&child, model, outVerts, outIndices))
            return false;
    }

    return true;
}

std::string FbxParser::GetTextureFileName() const
{
    // Objects 直下の最初の Texture ノードを探す
    Node* objects = FindNode(const_cast<std::vector<Node>&>(m_roots), "Objects");
    if (!objects) return "";

    Node* tex = FindNode(objects->children, "Texture");
    if (!tex) return "";

    // RelativeFilename を優先して使う
    Node* rel = FindNode(tex->children, "RelativeFilename");
    if (rel)
    {
        std::string s = ReadString(rel, ""); // ノード自身のプロパティを読む
        // ノード自身のプロパティを読む//
        const uint64_t off = rel->propStart;
        if (m_data[off] == 'S')
        {
            const uint32_t len = Read<uint32_t>(off + 1);
            s = std::string(reinterpret_cast<const char*>(m_data.data() + off + 5), len);
        }
        if (!s.empty())
        {
            // 絶対パス（"X:\" や "/" 始まり）ならファイル名部分だけ返す
            const bool is_abs = (s.size() >= 2 && s[1] == ':') || (s[0] == '/' || s[0] == '\\');
            if (is_abs)
            {
                size_t pos = s.find_last_of("/\\");
                return (pos != std::string::npos) ? s.substr(pos + 1) : s;
            }
            return s;
        }
    }

    // RelativeFilename がなければ FileName のファイル名部分だけ返す
    const Node* fn = FindNode(tex->children, "FileName");
    if (fn)
    {
        uint64_t off = fn->propStart;
        if (m_data[off] == 'S')
        {
            const uint32_t len = Read<uint32_t>(off + 1);
            const std::string full(reinterpret_cast<const char*>(m_data.data() + off + 5), len);
            // パスの最後のファイル名だけ切り出す
            const size_t pos = full.find_last_of("/\\");
            if (pos != std::string::npos) return full.substr(pos + 1);
            return full;
        }
    }

    return "";
}


// ----------------------------------------------------------------
//  1つの Geometry ノードをローカル Transform を適用しながら展開する
// ----------------------------------------------------------------
bool FbxParser::ExtractGeometry(
    Node* geometry,
    Node* model,
    std::vector<MeshVertex>& outVerts,
    std::vector<uint32_t>& outIndices)
{
    const std::vector<double> raw_pos = ReadDoubleArray(geometry, "Vertices");
    const std::vector<int32_t> poly_idx = ReadIntArray(geometry, "PolygonVertexIndex");
    if (raw_pos.empty() || poly_idx.empty()) return true;

    Node* le_normal = FindNode(geometry->children, "LayerElementNormal");
    if (!le_normal)
    {
        MessageBoxA(nullptr, "FBX: LayerElementNormal が見つかりません",
                    "FbxParser", MB_OK | MB_ICONERROR);
        return false;
    }
    const std::string norm_ref = ReadString(le_normal, "ReferenceInformationType");
    const std::vector<double> raw_norm = ReadDoubleArray(le_normal, "Normals");
    std::vector<int32_t> norm_idx = {};
    if (norm_ref == "IndexToDirect")
        norm_idx = ReadIntArray(le_normal, "NormalsIndex");

    Node* le_uv = FindNode(geometry->children, "LayerElementUV");
    if (!le_uv)
    {
        MessageBoxA(nullptr, "FBX: LayerElementUV が見つかりません",
                    "FbxParser", MB_OK | MB_ICONERROR);
        return false;
    }
    const std::string uv_ref = ReadString(le_uv, "ReferenceInformationType");
    const std::vector<double> raw_uv = ReadDoubleArray(le_uv, "UV");
    std::vector<int32_t> uv_idx = {};
    if (uv_ref == "IndexToDirect")
        uv_idx = ReadIntArray(le_uv, "UVIndex");

    MATRIX4X4 mat = GetGlobalMatrix(model);

    // --- デバッグ: 行列の移動成分を確認 ---
    {
        char buf[256];
        sprintf_s(buf, "[FbxParser] GlobalMatrix T=(%.3f, %.3f, %.3f)\n",
                  mat.m[3][0], mat.m[3][1], mat.m[3][2]);
        OutputDebugStringA(buf);
    }

    const uint32_t vertex_offset = static_cast<uint32_t>(outVerts.size());
    uint32_t pv_counter = 0;
    std::vector<std::pair<uint32_t, uint32_t>> face;

    // ポリゴン頂点ごとに必ず新規頂点を生成し、インデックスを 0 から振り直す
    auto emit_vertex = [&](uint32_t pos_idx, uint32_t pv_idx)
    {
        const uint32_t n_idx = (norm_ref == "IndexToDirect")
                                   ? static_cast<uint32_t>(norm_idx[pv_idx])
                                   : pv_idx;
        const uint32_t u_idx = (uv_ref == "IndexToDirect")
                                   ? static_cast<uint32_t>(uv_idx[pv_idx])
                                   : pv_idx;

        const float px = static_cast<float>(raw_pos[pos_idx * 3 + 0]);
        const float py = static_cast<float>(raw_pos[pos_idx * 3 + 1]);
        const float pz = static_cast<float>(raw_pos[pos_idx * 3 + 2]);
        const float nx = static_cast<float>(raw_norm[n_idx * 3 + 0]);
        const float ny = static_cast<float>(raw_norm[n_idx * 3 + 1]);
        const float nz = static_cast<float>(raw_norm[n_idx * 3 + 2]);

        MeshVertex v{};
        // 位置にグローバル変換行列を適用する（行優先）
        v.position[0] = (mat.m[0][0] * px + mat.m[1][0] * py + mat.m[2][0] * pz + mat.m[3][0]) / 100.0f;
        v.position[1] = (mat.m[0][1] * px + mat.m[1][1] * py + mat.m[2][1] * pz + mat.m[3][1]) / 100.0f;
        v.position[2] = (mat.m[0][2] * px + mat.m[1][2] * py + mat.m[2][2] * pz + mat.m[3][2]) / 100.0f;
        // 法線に回転のみ適用して再正規化する
        float wnx = mat.m[0][0] * nx + mat.m[1][0] * ny + mat.m[2][0] * nz;
        float wny = mat.m[0][1] * nx + mat.m[1][1] * ny + mat.m[2][1] * nz;
        float wnz = mat.m[0][2] * nx + mat.m[1][2] * ny + mat.m[2][2] * nz;
        const float len = std::sqrt(wnx * wnx + wny * wny + wnz * wnz);
        if (len > 1e-6f)
        {
            wnx /= len;
            wny /= len;
            wnz /= len;
        }
        v.normal[0] = wnx;
        v.normal[1] = wny;
        v.normal[2] = wnz;
        // UV はそのまま（ V 軸反転はシェーダー側で行う）
        v.uv[0] = static_cast<float>(raw_uv[u_idx * 2 + 0]);
        v.uv[1] = 1.0f - static_cast<float>(raw_uv[u_idx * 2 + 1]);

        const uint32_t new_idx = static_cast<uint32_t>(outVerts.size() - vertex_offset);
        outVerts.push_back(v);
        outIndices.push_back(vertex_offset + new_idx);
    };

    for (int32_t raw : poly_idx)
    {
        const bool is_end = (raw < 0);
        const uint32_t pos_idx = is_end ? static_cast<uint32_t>(~raw) : static_cast<uint32_t>(raw);
        face.push_back({pos_idx, pv_counter++});

        if (is_end)
        {
            for (size_t i = 1; i + 1 < face.size(); ++i)
            {
                emit_vertex(face[0].first, face[0].second);
                emit_vertex(face[i].first, face[i].second);
                emit_vertex(face[i + 1].first, face[i + 1].second);
            }
            face.clear();
        }
    }

    return true;
}

// ----------------------------------------------------------------
//  ノードの最初のプロパティ（ID）を int64_t で読む
//  FBX 7400 以前は 'I'(int32)、7500 以降は 'L'(int64)
// ----------------------------------------------------------------
int64_t FbxParser::ReadNodeId(const uint64_t offset) const
{
    const uint8_t tc = m_data[offset];
    if (tc == 'L') return Read<int64_t>(offset + 1);
    if (tc == 'I') return Read<int32_t>(offset + 1);
    return 0;
}

// ----------------------------------------------------------------
//  Model ノードからグローバル変換行列を計算する
//  Connections の親チェーンを再帰的に辿り、各祖先のローカル行列を結合する
//  （FbxNode::EvaluateGlobalTransform() 相当）
// ----------------------------------------------------------------
MATRIX4X4 FbxParser::GetGlobalMatrix(Node* model) const
{
    const MATRIX4X4 identity(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
    if (!model) return identity;

    int64_t modelId = ReadNodeId(model->propStart);

    auto parentIt = m_modelParentMap.find(modelId);
    if (parentIt != m_modelParentMap.end())
    {
        auto nodeIt = m_modelMap.find(parentIt->second);
        if (nodeIt != m_modelMap.end())
        {
            // 親のグローバル行列 × 自身のローカル行列
            const MATRIX4X4 parentGlobal = GetGlobalMatrix(nodeIt->second);
            const MATRIX4X4 local = GetLocalMatrix(model);
            return parentGlobal * local;
        }
    }

    // 親がいない（ルートノード）ならローカル行列がそのままグローバル行列
    return GetLocalMatrix(model);
}

// ----------------------------------------------------------------
//  Model ノードからローカル変換行列 (TRS) を計算する
//  行列要素 4x4, Euler 回転順序は FBX 標準の XYZ
// ----------------------------------------------------------------
MATRIX4X4 FbxParser::GetLocalMatrix(Node* model) const
{
    MATRIX4X4 m(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
    if (!model) return m;
    const Node* props70 = FindNode(model->children, "Properties70");
    if (!props70) return m;

    double tx = 0, ty = 0, tz = 0;
    double rx = 0, ry = 0, rz = 0;
    double sx = 1, sy = 1, sz = 1;

    for (auto& p : props70->children)
    {
        if (p.name != "P") continue;
        uint64_t off = p.propStart;

        // 1つ目: プロパティ名 (S)
        if (m_data[off] != 'S') continue;
        const uint32_t slen = Read<uint32_t>(off + 1);
        const std::string pname(reinterpret_cast<const char*>(m_data.data() + off + 5), slen);
        off += 5 + slen;

        if (pname != "Lcl Translation" &&
            pname != "Lcl Rotation" &&
            pname != "Lcl Scaling")
            continue;

        // 2～4つ目（型名, ラベル, フラグ）をスキップ
        for (int i = 0; i < 3; ++i)
        {
            if (off >= m_data.size()) break;
            const uint8_t tc = m_data[off++];
            if (tc == 'S')
            {
                uint32_t l = Read<uint32_t>(off);
                off += 4 + l;
            }
            else if (tc == 'I') off += 4;
            else if (tc == 'L') off += 8;
            else if (tc == 'F') off += 4;
            else if (tc == 'D') off += 8;
        }

        // 5～7つ目: X, Y, Z 値
        auto read_val = [&]() -> double
        {
            if (off >= m_data.size()) return 0.0;
            const uint8_t tc = m_data[off++];
            if (tc == 'D')
            {
                double v = Read<double>(off);
                off += 8;
                return v;
            }
            if (tc == 'F')
            {
                float v = Read<float>(off);
                off += 4;
                return static_cast<double>(v);
            }
            return 0.0;
        };
        double vx = read_val(), vy = read_val(), vz = read_val();
        if (pname == "Lcl Translation")
        {
            tx = vx;
            ty = vy;
            tz = vz;
        }
        else if (pname == "Lcl Rotation")
        {
            rx = vx * DegToRad;
            ry = vy * DegToRad;
            rz = vz * DegToRad;
        }
        else if (pname == "Lcl Scaling")
        {
            sx = vx;
            sy = vy;
            sz = vz;
        }
    }

    // rx/ry/rz はすでに DegToRad 済みなのでそのまま渡す
    const double crx = cos(rx), srx = sin(rx);
    const double cry = cos(ry), sry = sin(ry);
    const double crz = cos(rz), srz = sin(rz);

    // 行列（ TRS 行列。 M = Rz * Ry * Rx）
    m.m[0][0] = static_cast<float>(sx * (cry * crz));
    m.m[0][1] = static_cast<float>(sx * (cry * srz));
    m.m[0][2] = static_cast<float>(sx * (-sry));
    m.m[0][3] = 0.0f;
    m.m[1][0] = static_cast<float>(sy * (srx * sry * crz - crx * srz));
    m.m[1][1] = static_cast<float>(sy * (srx * sry * srz + crx * crz));
    m.m[1][2] = static_cast<float>(sy * (srx * cry));
    m.m[1][3] = 0.0f;
    m.m[2][0] = static_cast<float>(sz * (crx * sry * crz + srx * srz));
    m.m[2][1] = static_cast<float>(sz * (crx * sry * srz - srx * crz));
    m.m[2][2] = static_cast<float>(sz * (crx * cry));
    m.m[2][3] = 0.0f;
    m.m[3][0] = static_cast<float>(tx);
    m.m[3][1] = static_cast<float>(ty);
    m.m[3][2] = static_cast<float>(tz);
    m.m[3][3] = 1.0f;
    return m;
}

// ----------------------------------------------------------------
//  ノードのヘッダーを読み込む（32bit / 64bit オフセットに対応）
// ----------------------------------------------------------------
FbxParser::Node FbxParser::ReadNodeHeader(uint64_t offset, uint64_t& outEnd) const
{
    Node n;
    uint64_t numProps;
    uint64_t propLen;

    if (m_is64bit)
    {
        outEnd = Read<uint64_t>(offset);
        numProps = Read<uint64_t>(offset + 8);
        propLen = Read<uint64_t>(offset + 16);
        const uint8_t nameLen = m_data[offset + 24];
        n.name.assign(reinterpret_cast<const char*>(m_data.data() + offset + 25), nameLen);
        n.propStart = offset + 25 + nameLen;
        n.propLen = propLen;
        n.childStart = n.propStart + propLen;
    }
    else
    {
        outEnd = Read<uint32_t>(offset);
        numProps = Read<uint32_t>(offset + 4);
        propLen = Read<uint32_t>(offset + 8);
        const uint8_t name_len = m_data[offset + 12];
        n.name.assign(reinterpret_cast<const char*>(m_data.data() + offset + 13), name_len);
        n.propStart = offset + 13 + name_len;
        n.propLen = propLen;
        n.childStart = n.propStart + propLen;
    }
    n.end = outEnd;
    (void)numProps;
    return n;
}

// ----------------------------------------------------------------
//  指定範囲の子ノードを再帰的に解析する
// ----------------------------------------------------------------
std::vector<FbxParser::Node> FbxParser::ParseChildren(uint64_t start, uint64_t end)
{
    std::vector<Node> nodes;
    uint64_t offset = start;
    const size_t header_sz = m_is64bit ? 25 : 13;

    while (offset + header_sz <= end)
    {
        uint64_t node_end = 0;
        Node n = ReadNodeHeader(offset, node_end);
        if (node_end == 0 && n.name.empty()) break;
        n.children = ParseChildren(n.childStart, n.end);
        nodes.push_back(std::move(n));
        offset = node_end;
    }
    return nodes;
}

// ----------------------------------------------------------------
//  ノードリストから指定名のノードを再帰的に探索する
// ----------------------------------------------------------------
FbxParser::Node* FbxParser::FindNode(std::vector<Node>& nodes, const std::string& name)
{
    for (auto& n : nodes)
    {
        if (n.name == name) return &n;
        Node* r = FindNode(n.children, name);
        if (r) return r;
    }
    return nullptr;
}

// ----------------------------------------------------------------
//  型付き配列プロパティを読み込む（非圧縮 / zlib 圧縮の両方に対応）
// ----------------------------------------------------------------
template <typename T>
std::vector<T> FbxParser::ReadTypedArray(uint64_t offset)
{
    const uint32_t count = Read<uint32_t>(offset);
    const uint32_t encoding = Read<uint32_t>(offset + 4);
    uint32_t comp_len = Read<uint32_t>(offset + 8);
    const uint8_t* src = m_data.data() + offset + 12;

    std::vector<T> result(count);
    if (encoding == 0)
    {
        memcpy(result.data(), src, sizeof(T) * count);
    }
    else if (encoding == 1)
    {
        uLongf dest_len = static_cast<uLongf>(sizeof(T) * count);
        if (uncompress(
            reinterpret_cast<Bytef*>(result.data()), &dest_len,
            src, comp_len) != Z_OK)
            throw std::runtime_error("zlib 解凍に失敗しました");
    }
    return result;
}

template std::vector<double> FbxParser::ReadTypedArray<double>(uint64_t);
template std::vector<int32_t> FbxParser::ReadTypedArray<int32_t>(uint64_t);

std::vector<double> FbxParser::ReadDoubleArray(Node* parent, const std::string& nodeName)
{
    const Node* n = FindNode(parent->children, nodeName);
    if (!n) return {};
    const uint64_t off = n->propStart;
    if (static_cast<char>(m_data[off]) != 'd') return {};
    return ReadTypedArray<double>(off + 1);
}

std::vector<int32_t> FbxParser::ReadIntArray(Node* parent, const std::string& nodeName)
{
    const Node* n = FindNode(parent->children, nodeName);
    if (!n) return {};
    const uint64_t off = n->propStart;
    if (static_cast<char>(m_data[off]) != 'i') return {};
    return ReadTypedArray<int32_t>(off + 1);
}

std::string FbxParser::ReadString(Node* parent, const std::string& nodeName) const
{
    const Node* n = FindNode(parent->children, nodeName);
    if (!n) return "";
    const uint64_t off = n->propStart;
    if (static_cast<char>(m_data[off]) != 'S') return "";
    const uint32_t len = Read<uint32_t>(off + 1);
    return std::string(reinterpret_cast<const char*>(m_data.data() + off + 5), len);
}
