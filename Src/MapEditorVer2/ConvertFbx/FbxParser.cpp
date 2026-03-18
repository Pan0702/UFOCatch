#include "FbxParser.h"

#include <cmath>
#include <fstream>
#include <stdexcept>
#include <unordered_map>

#include <windows.h>
#include <zlib.h>

#include "../../Utils/MyMath.h"


// ----------------------------------------------------------------
//  FBX 繝舌う繝翫Μ繝輔ぃ繧､繝ｫ繧定ｪｭ縺ｿ霎ｼ繧
// ----------------------------------------------------------------
bool FbxParser::Load(const std::string& path)
{
    std::ifstream file(path, std::ios::binary);
    if (!file)
    {
        MessageBoxA(nullptr, ("FBX 繝輔ぃ繧､繝ｫ繧帝幕縺代∪縺帙ｓ: " + path).c_str(),
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
        MessageBoxA(nullptr, "FBX 繝舌う繝翫Μ繝輔か繝ｼ繝槭ャ繝医〒縺ｯ縺ゅｊ縺ｾ縺帙ｓ",
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
//  隱ｭ縺ｿ霎ｼ繧薙□ FBX 縺九ｉ鬆らせ繝ｻ繧､繝ｳ繝・ャ繧ｯ繧ｹ繝・・繧ｿ繧貞ｱ暮幕縺吶ｋ
//
//  莉墓ｧ・
//    - Objects 逶ｴ荳九・蜈ｨ Geometry 繧堤ｵ仙粋縺励※1縺､縺ｮ繝｡繝・す繝･縺ｨ縺励※蜃ｺ蜉帙☆繧・
//    - 蜷・Geometry 縺ｫ蟇ｾ蠢懊☆繧・Model 縺ｮ 繝ｭ繝ｼ繧ｫ繝ｫ Transform (T/R/S) 繧帝←逕ｨ縺吶ｋ
//      ・郁ｦｪ Model 縺ｮ Transform 縺ｯ驕ｩ逕ｨ縺励↑縺・ｼ・
//    - 鬆らせ縺ｮ荳諢乗ｧ縺ｯ (pos_idx, uv_idx) 縺ｮ邨・∩蜷医ｏ縺帙〒蛻､螳壹☆繧具ｼ域ｳ慕ｷ壹・蜷ｫ繧√↑縺・ｼ・
//    - UV 縺ｮ V 霆ｸ蜿崎ｻ｢縺ｯ陦後ｏ縺ｪ縺・ｼ医す繧ｧ繝ｼ繝繝ｼ蛛ｴ縺ｧ蜃ｦ逅・☆繧具ｼ・
// ----------------------------------------------------------------
bool FbxParser::ExtractMesh(std::vector<MeshVertex>& outVerts,std::vector<uint32_t>& outIndices)
{
    Node* objects = FindNode(m_roots, "Objects");
    if (!objects)
    {
        MessageBoxA(nullptr, "FBX: Objects 繝弱・繝峨′隕九▽縺九ｊ縺ｾ縺帙ｓ",
                    "FbxParser", MB_OK | MB_ICONERROR);
        return false;
    }

    // --- Model ID 竊・Node 縺ｮ繝槭ャ繝励ｒ菴懈・ ---
    m_modelMap.clear();
    for (auto& child : objects->children)
    {
        if (child.name == "Model")
            m_modelMap[ReadNodeId(child.propStart)] = &child;
    }

    // --- Geometry ID 繧ｻ繝・ヨ繧剃ｽ懈・ ---
    std::unordered_map<int64_t, Node*> geo_map;
    for (auto& child : objects->children)
    {
        if (child.name == "Geometry")
            geo_map[ReadNodeId(child.propStart)] = &child;
    }

    // --- Connections 縺九ｉ謗･邯壽ュ蝣ｱ繧貞庶髮・---
    //   geo_to_model      : Geometry ID 竊・逶ｴ謗･縺ｮ隕ｪ Model ID
    //   model_parent_map_ : Model ID    竊・隕ｪ Model ID・医げ繝ｭ繝ｼ繝舌Ν陦悟・縺ｮ蜀榊ｸｰ險育ｮ礼畑・・
    std::unordered_map<int64_t, int64_t> geo_to_model;
    m_modelParentMap.clear();
    const Node* conn_node = FindNode(m_roots, "Connections");
    if (conn_node)
    {
        for (auto& c : conn_node->children)
        {
            if (c.name != "C") continue;
            uint64_t off = c.propStart;
            // conn_type (S) 繧偵せ繧ｭ繝・・
            if (off < m_data.size() && m_data[off] == 'S')
            {
                uint32_t l = Read<uint32_t>(off + 1);
                off += 5 + l;
            }
            // 繧ｿ繧ｰ繧貞ｮ滄圀縺ｫ隱ｭ繧薙〒繧ｪ繝輔そ繝・ヨ繧呈ｭ｣遒ｺ縺ｫ騾ｲ繧√ｋ
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
    // // --- 繝・ヰ繝・げ: model_map_ / model_parent_map_ 縺ｮ蜀・ｮｹ繧堤｢ｺ隱・---
    // {
    //     char buf[256];
    //     sprintf_s(buf, "[FbxParser] model_map_ size=%zu  model_parent_map_ size=%zu\n",
    //               model_map_.size(), model_parent_map_.size());
    //     OutputDebugStringA(buf);
    // }
    //
    // // --- 繝・ヰ繝・げ: geo_to_model 縺ｮ蜀・ｮｹ繧堤｢ｺ隱・---
    // {
    //     char buf[256];
    //     sprintf_s(buf, "[FbxParser] geo_map size=%zu  geo_to_model size=%zu\n",
    //               geo_map.size(), geo_to_model.size());
    //     OutputDebugStringA(buf);
    // }
    // --- 蜈ｨ Geometry 繧貞ｱ暮幕縺励※邨仙粋 ---
    outVerts.clear();
    outIndices.clear();

    if (geo_map.empty())
    {
        MessageBoxA(nullptr, "FBX: Geometry 繝弱・繝峨′隕九▽縺九ｊ縺ｾ縺帙ｓ",
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
            // // --- 繝・ヰ繝・げ: ID 縺ｮ辣ｧ蜷医ｒ遒ｺ隱・---
            // char buf[256];
            // sprintf_s(buf, "[FbxParser] geo_id=%lld  model_id_from_conn=%lld  in_model_map=%s\n",
            //           geo_id, it->second,
            //           model_map_.count(it->second) ? "YES" : "NO");
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
    // Objects 逶ｴ荳九・譛蛻昴・ Texture 繝弱・繝峨ｒ謗｢縺・
     Node* objects = FindNode(const_cast<std::vector<Node>&>(m_roots), "Objects");
    if (!objects) return "";

    Node* tex = FindNode(objects->children, "Texture");
    if (!tex) return "";

    // RelativeFilename 繧貞━蜈医＠縺ｦ菴ｿ縺・
    Node* rel = FindNode(tex->children, "RelativeFilename");
    if (rel)
    {
        std::string s = ReadString(rel, ""); // 繝弱・繝芽・霄ｫ縺ｮ繝励Ο繝代ユ繧｣繧定ｪｭ繧
        // 繝弱・繝芽・霄ｫ縺ｮ繝励Ο繝代ユ繧｣繧定ｪｭ繧//
        const uint64_t off = rel->propStart;
        if (m_data[off] == 'S')
        {
            const uint32_t len = Read<uint32_t>(off + 1);
            s = std::string(reinterpret_cast<const char*>(m_data.data() + off + 5), len);
        }
        if (!s.empty())
        {
            // 邨ｶ蟇ｾ繝代せ・・X:\" 繧・"/" 蟋九∪繧奇ｼ峨↑繧峨ヵ繧｡繧､繝ｫ蜷埼Κ蛻・□縺題ｿ斐☆
            const bool is_abs = (s.size() >= 2 && s[1] == ':') || (s[0] == '/' || s[0] == '\\');
            if (is_abs)
            {
                size_t pos = s.find_last_of("/\\");
                return (pos != std::string::npos) ? s.substr(pos + 1) : s;
            }
            return s;
        }
    }

    // RelativeFilename 縺後↑縺代ｌ縺ｰ FileName 縺ｮ繝輔ぃ繧､繝ｫ蜷埼Κ蛻・□縺題ｿ斐☆
    const Node* fn = FindNode(tex->children, "FileName");
    if (fn)
    {
        uint64_t off = fn->propStart;
        if (m_data[off] == 'S')
        {
            const uint32_t len = Read<uint32_t>(off + 1);
            const std::string full(reinterpret_cast<const char*>(m_data.data() + off + 5), len);
            // 繝代せ縺ｮ譛蠕後・繝輔ぃ繧､繝ｫ蜷阪□縺大叙繧雁・縺・
            const size_t pos = full.find_last_of("/\\");
            if (pos != std::string::npos) return full.substr(pos + 1);
            return full;
        }
    }

    return "";
}


// ----------------------------------------------------------------
//  1縺､縺ｮ Geometry 繝弱・繝峨ｒ繝ｭ繝ｼ繧ｫ繝ｫ Transform 繧帝←逕ｨ縺励↑縺後ｉ螻暮幕縺吶ｋ
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
        MessageBoxA(nullptr, "FBX: LayerElementNormal 縺瑚ｦ九▽縺九ｊ縺ｾ縺帙ｓ",
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
        MessageBoxA(nullptr, "FBX: LayerElementUV 縺瑚ｦ九▽縺九ｊ縺ｾ縺帙ｓ",
                    "FbxParser", MB_OK | MB_ICONERROR);
        return false;
    }
    const std::string uv_ref = ReadString(le_uv, "ReferenceInformationType");
    const std::vector<double> raw_uv = ReadDoubleArray(le_uv, "UV");
    std::vector<int32_t> uv_idx = {};
    if (uv_ref == "IndexToDirect")
        uv_idx = ReadIntArray(le_uv, "UVIndex");

    MATRIX4X4 mat = GetGlobalMatrix(model);

    // --- 繝・ヰ繝・げ: 陦悟・縺ｮ遘ｻ蜍墓・蛻・ｒ遒ｺ隱・---
    {
        char buf[256];
        sprintf_s(buf, "[FbxParser] GlobalMatrix T=(%.3f, %.3f, %.3f)\n",
                  mat.m[3][0], mat.m[3][1], mat.m[3][2]);
        OutputDebugStringA(buf);
    }

    const uint32_t vertex_offset = static_cast<uint32_t>(outVerts.size());
    uint32_t pv_counter = 0;
    std::vector<std::pair<uint32_t, uint32_t>> face;

    // 繝昴Μ繧ｴ繝ｳ鬆らせ縺斐→縺ｫ蠢・★譁ｰ隕城らせ繧堤函謌舌＠縲√う繝ｳ繝・ャ繧ｯ繧ｹ繧・0 縺九ｉ謖ｯ繧顔峩縺・
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
        // 菴咲ｽｮ縺ｫ繧ｰ繝ｭ繝ｼ繝舌Ν螟画鋤陦悟・繧帝←逕ｨ縺吶ｋ・郁｡悟━蜈茨ｼ・
        v.position[0] = (mat.m[0][0] * px + mat.m[1][0] * py + mat.m[2][0] * pz + mat.m[3][0]) / 100.0f;
        v.position[1] = (mat.m[0][1] * px + mat.m[1][1] * py + mat.m[2][1] * pz + mat.m[3][1]) / 100.0f;
        v.position[2] = (mat.m[0][2] * px + mat.m[1][2] * py + mat.m[2][2] * pz + mat.m[3][2]) / 100.0f;
        // 豕慕ｷ壹↓蝗櫁ｻ｢縺ｮ縺ｿ驕ｩ逕ｨ縺励※蜀肴ｭ｣隕丞喧縺吶ｋ
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
        // UV 縺ｯ縺昴・縺ｾ縺ｾ・・ 霆ｸ蜿崎ｻ｢縺ｯ繧ｷ繧ｧ繝ｼ繝繝ｼ蛛ｴ縺ｧ陦後≧・・
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
//  繝弱・繝峨・譛蛻昴・繝励Ο繝代ユ繧｣・・D・峨ｒ int64_t 縺ｧ隱ｭ繧
//  FBX 7400 莉･蜑阪・ 'I'(int32)縲・500 莉･髯阪・ 'L'(int64)
// ----------------------------------------------------------------
int64_t FbxParser::ReadNodeId(const uint64_t offset) const
{
    const uint8_t tc = m_data[offset];
    if (tc == 'L') return Read<int64_t>(offset + 1);
    if (tc == 'I') return Read<int32_t>(offset + 1);
    return 0;
}

// ----------------------------------------------------------------
//  Model 繝弱・繝峨°繧峨げ繝ｭ繝ｼ繝舌Ν螟画鋤陦悟・繧貞叙蠕励☆繧・
//  Connections 縺ｮ隕ｪ繝√ぉ繝ｼ繝ｳ繧貞・蟶ｰ逧・↓霎ｿ繧翫∝・逾門・縺ｮ繝ｭ繝ｼ繧ｫ繝ｫ陦悟・繧堤ｵ仙粋縺吶ｋ
//  ・・bxNode::EvaluateGlobalTransform() 逶ｸ蠖難ｼ・
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
            // 隕ｪ縺ｮ繧ｰ繝ｭ繝ｼ繝舌Ν陦悟・ ﾃ・閾ｪ蛻・・繝ｭ繝ｼ繧ｫ繝ｫ陦悟・
            const MATRIX4X4 parentGlobal = GetGlobalMatrix(nodeIt->second);
            const MATRIX4X4 local = GetLocalMatrix(model);
            return parentGlobal * local;
        }
    }

    // 隕ｪ縺後＞縺ｪ縺・ｼ医Ν繝ｼ繝医ヮ繝ｼ繝会ｼ峨↑繧峨Ο繝ｼ繧ｫ繝ｫ陦悟・縺後◎縺ｮ縺ｾ縺ｾ繧ｰ繝ｭ繝ｼ繝舌Ν陦悟・
    return GetLocalMatrix(model);
}

// ----------------------------------------------------------------
//  Model 繝弱・繝峨°繧峨Ο繝ｼ繧ｫ繝ｫ螟画鋤陦悟・ (TRS) 繧貞叙蠕励☆繧・
//  陦悟━蜈・4x4, Euler 蝗櫁ｻ｢鬆・ｺ上・ FBX 讓呎ｺ悶・ XYZ
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

        // 1縺､逶ｮ: 繝励Ο繝代ユ繧｣蜷・(S)
        if (m_data[off] != 'S') continue;
        const uint32_t slen = Read<uint32_t>(off + 1);
        const std::string pname(reinterpret_cast<const char*>(m_data.data() + off + 5), slen);
        off += 5 + slen;

        if (pname != "Lcl Translation" &&
            pname != "Lcl Rotation" &&
            pname != "Lcl Scaling")
            continue;

        // 2縲・縺､逶ｮ・亥梛蜷・ 繝ｩ繝吶Ν, 繝輔Λ繧ｰ・峨ｒ繧ｹ繧ｭ繝・・
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

        // 5縲・縺､逶ｮ: X, Y, Z 蛟､
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

    // rx/ry/rz 縺ｯ縺吶〒縺ｫ DegToRad 貂医∩縺ｪ縺ｮ縺ｧ縺昴・縺ｾ縺ｾ貂｡縺・
    const double crx = cos(rx), srx = sin(rx);
    const double cry = cos(ry), sry = sin(ry);
    const double crz = cos(rz), srz = sin(rz);

    // 陦悟━蜈・TRS 陦悟・・・ = Rz * Ry * Rx・・
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
//  繝弱・繝峨・繝・ム繝ｼ繧定ｪｭ縺ｿ霎ｼ繧・・2bit / 64bit 繧ｪ繝輔そ繝・ヨ縺ｫ蟇ｾ蠢懶ｼ・
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
//  謖・ｮ夂ｯ・峇縺ｮ蟄舌ヮ繝ｼ繝峨ｒ蜀榊ｸｰ逧・↓隗｣譫舌☆繧・
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
//  繝弱・繝峨Μ繧ｹ繝医°繧画欠螳壼錐縺ｮ繝弱・繝峨ｒ蜀榊ｸｰ逧・↓讀懃ｴ｢縺吶ｋ
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
//  蝙倶ｻ倥″驟榊・繝励Ο繝代ユ繧｣繧定ｪｭ縺ｿ霎ｼ繧・磯撼蝨ｧ邵ｮ / zlib 蝨ｧ邵ｮ縺ｮ荳｡譁ｹ縺ｫ蟇ｾ蠢懶ｼ・
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
            throw std::runtime_error("zlib 隗｣蜃阪↓螟ｱ謨励＠縺ｾ縺励◆");
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

