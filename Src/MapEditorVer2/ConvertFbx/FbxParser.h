#pragma once
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>
#include "../../MyMath.h"

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
        uint64_t    prop_start  = 0;
        uint64_t    prop_len    = 0;
        uint64_t    child_start = 0;
        uint64_t    end         = 0;
        std::vector<Node> children;
    };

    std::vector<uint8_t> data_;
    uint32_t             version_ = 0;
    bool                 is64bit_ = false;
    std::vector<Node>    roots_;
    // Connections から構築するルックアップテーブル
    std::unordered_map<int64_t, Node*>   model_map_;          // Model ID → Node*
    std::unordered_map<int64_t, int64_t> model_parent_map_;   // Model ID → 親 Model ID

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
    /// <param name="out_verts">出力頂点配列</param>
    /// <param name="out_indices">出力インデックス配列</param>
    /// <returns>成功で true</returns>
    bool ExtractMesh(
        std::vector<MeshVertex>& out_verts,
        std::vector<uint32_t>&   out_indices);
    
    // FBX に埋め込まれたテクスチャのファイル名を取得する
    // RelativeFilename が空なら FileName のファイル名部分を返す
    std::string GetTextureFileName() const;

private:
    /// <summary>
    /// 1つの Geometry ノードをローカル Transform を適用しながら展開する。
    /// out_verts / out_indices の末尾に追記する。
    /// </summary>
    /// <param name="geometry">展開する Geometry ノード</param>
    /// <param name="model">対応する Model ノード（nullptr の場合は Transform なし）</param>
    /// <param name="out_verts">追記先の頂点配列</param>
    /// <param name="out_indices">追記先のインデックス配列</param>
    /// <returns>成功で true。Vertices のない Geometry はスキップして true を返す</returns>
    bool ExtractGeometry(
        Node*                    geometry,
        Node*                    model,
        std::vector<MeshVertex>& out_verts,
        std::vector<uint32_t>&   out_indices);

    /// <summary>
    /// ノードの最初のプロパティ（ID）を int64_t で読む。
    /// FBX 7400 以前は 'I'(int32)、7500 以降は 'L'(int64)。
    /// </summary>
    int64_t ReadNodeId(uint64_t offset) const;

    /// <summary>
    /// Model ノードからグローバル変換行列 (親チェーンを再帰的に結合) を取得する。
    /// 行優先 4x4、Euler 回転順序は FBX 標準の XYZ。
    /// model が nullptr の場合は単位行列を返す。
    /// </summary>
    MATRIX4X4 GetGlobalMatrix(Node* model) const;

    /// <summary>
    /// Model ノードからローカル変換行列 (TRS) を取得する。
    /// 行優先 4x4、Euler 回転順序は FBX 標準の XYZ。
    /// model が nullptr の場合は単位行列を返す。
    /// </summary>
    MATRIX4X4 GetLocalMatrix(Node* model) const;

    // バイナリデータから型 T の値を読み込む
    template<typename T>
    T Read(uint64_t offset) const
    {
        T v{};
        memcpy(&v, data_.data() + offset, sizeof(T));
        return v;
    }

    // 型付き配列プロパティを読み込む（非圧縮 / zlib 圧縮の両方に対応）
    // 実装は FbxParser.cpp に記述（double / int32_t のみ使用可）
    template<typename T>
    std::vector<T> ReadTypedArray(uint64_t offset);

    Node              ReadNodeHeader(uint64_t offset, uint64_t& out_end) const;
    std::vector<Node> ParseChildren(uint64_t start, uint64_t end);
    static Node*             FindNode(std::vector<Node>& nodes, const std::string& name);

    /// <summary>指定ノードの子から double 配列プロパティを読み込む</summary>
    std::vector<double>  ReadDoubleArray(Node* parent, const std::string& node_name);

    /// <summary>指定ノードの子から int32 配列プロパティを読み込む</summary>
    std::vector<int32_t> ReadIntArray(Node* parent, const std::string& node_name);

    /// <summary>指定ノードの子から文字列プロパティを読み込む</summary>
    std::string          ReadString(Node* parent, const std::string& node_name) const;
};