#pragma once
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>
#include "../../Utils/MyMath.h"

// .mesh ファイルの頂点構造体（stride = 32 bytes）
struct MeshVertex
{
    float position[3];  // 12 bytes
    float normal[3];    // 12 bytes
    float uv[2];        //  8 bytes
};

// FBX Binary ファイルを解析して頂点・インデックスデータを取得するクラス
class FbxParser
{
private:
    // FBX ノード情報
    struct Node
    {
        std::string name;
        uint64_t    propStart  = 0;
        uint64_t    propLen    = 0;
        uint64_t    childStart = 0;
        uint64_t    end         = 0;
        std::vector<Node> children;
    };

    std::vector<uint8_t> m_data;
    uint32_t             m_version = 0;
    bool                 m_is64bit = false;
    std::vector<Node>    m_roots;
    // Connections から構築するルックアップテーブル
    std::unordered_map<int64_t, Node*>   m_modelMap;          // Model ID → Node*
    std::unordered_map<int64_t, int64_t> m_modelParentMap;   // Model ID → 親 Model ID

public:
    /// <summary>
    /// FBX バイナリファイルを読み込む
    /// </summary>
    /// <param name="path">FBX ファイルのパス</param>
    /// <returns>成功で true</returns>
    bool Load(const std::string& path);

    /// <summary>
    /// 読み込んだ FBX から頂点・インデックスデータを展開する。
    /// Objects 直下の全 Geometry を結合して1つのメッシュとして出力する。
    /// 各 Geometry に対応する Model のローカル Transform (T/R/S) を適用する。
    /// </summary>
    /// <param name="outVerts">出力頂点配列</param>
    /// <param name="outIndices">出力インデックス配列</param>
    /// <returns>成功で true</returns>
    bool ExtractMesh(
        std::vector<MeshVertex>& outVerts,
        std::vector<uint32_t>&   outIndices);
    
    // FBX に埋め込まれたテクスチャのファイル名を取得する。
    // RelativeFilename が空なら FileName のファイル名部分を返す
    std::string GetTextureFileName() const;

private:
    /// <summary>
    /// 1つの Geometry ノードをローカル Transform を適用しながら展開する。
    /// out_verts / out_indices の末尾に追記する。
    /// </summary>
    /// <param name="geometry">展開する Geometry ノード</param>
    /// <param name="model">対応する Model ノード。nullptr の場合は Transform なし</param>
    /// <param name="outVerts">追記先の頂点配列</param>
    /// <param name="outIndices">追記先のインデックス配列</param>
    /// <returns>成功で true。Vertices のない Geometry はスキップして true を返す</returns>
    bool ExtractGeometry(
        Node*                    geometry,
        Node*                    model,
        std::vector<MeshVertex>& outVerts,
        std::vector<uint32_t>&   outIndices);

    /// <summary>
    /// ノードの最初のプロパティ（ID）を int64_t で読む。
    /// FBX 7400 以前は 'I'(int32)、7500 以降は 'L'(int64)。
    /// </summary>
    int64_t ReadNodeId(uint64_t offset) const;

    /// <summary>
    /// Model ノードからグローバル変換行列 (親チェーンを再帰的に結合) を取得する。
    /// 行列要素 4x4。Euler 回転順序は FBX 標準の XYZ。
    /// model が nullptr の場合は単位行列を返す。
    /// </summary>
    MATRIX4X4 GetGlobalMatrix(Node* model) const;

    /// <summary>
    /// Model ノードからローカル変換行列 (TRS) を取得する。
    /// 行列要素 4x4。Euler 回転順序は FBX 標準の XYZ。
    /// model が nullptr の場合は単位行列を返す。
    /// </summary>
    MATRIX4X4 GetLocalMatrix(Node* model) const;

    // バイナリデータから型 T の値を読み込む
    template<typename T>
    T Read(uint64_t offset) const
    {
        T v{};
        memcpy(&v, m_data.data() + offset, sizeof(T));
        return v;
    }

    // 型付き配列プロパティを読み込む（非圧縮 / zlib 圧縮の両方に対応）
    // 実装は FbxParser.cpp に記述。double / int32_t のみ使用可能。
    template<typename T>
    std::vector<T> ReadTypedArray(uint64_t offset);

    Node              ReadNodeHeader(uint64_t offset, uint64_t& outEnd) const;
    std::vector<Node> ParseChildren(uint64_t start, uint64_t end);
    static Node*             FindNode(std::vector<Node>& nodes, const std::string& name);

    /// <summary>指定ノードの子から double 配列プロパティを読み込む</summary>
    std::vector<double>  ReadDoubleArray(Node* parent, const std::string& nodeName);

    /// <summary>指定ノードの子から int32 配列プロパティを読み込む</summary>
    std::vector<int32_t> ReadIntArray(Node* parent, const std::string& nodeName);

    /// <summary>指定ノードの子から文字列プロパティを読み込む</summary>
    std::string          ReadString(Node* parent, const std::string& nodeName) const;
};
