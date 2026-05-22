#pragma once
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>
#include "../../Utils/MyMath.h"

// .mesh ファイルの頂点構造体（stride = 32 bytes）
struct MeshVertex
{
    float position[3]; // 12 bytes
    float normal[3]; // 12 bytes
    float uv[2]; //  8 bytes
};

/// <summary>ステージエディタで使う Fbx Parser の情報と処理をまとめる型</summary>
class FbxParser
{
private:
    // FBX ノード情報
    struct Node
    {
        std::string name;
        uint64_t propStart = 0;
        uint64_t propLen = 0;
        uint64_t childStart = 0;
        uint64_t end = 0;
        std::vector<Node> children;
    };

    std::vector<uint8_t> m_data;
    uint32_t m_version = 0;
    bool m_is64bit = false;
    std::vector<Node> m_roots;
    // Connections から構築するルックアップテーブル
    std::unordered_map<int64_t, Node*> m_modelMap; // Model ID → Node*
    std::unordered_map<int64_t, int64_t> m_modelParentMap; // Model ID → 親 Model ID

public:
    /// 読み込む
    /// @param path パス
    /// @return 成功または条件を満たす場合 true
    bool Load(const std::string& path);

    /// Extract Mesh を返す
    /// @param outVerts outVerts に渡す値
    /// @param outIndices outIndices に渡す値
    /// @return 成功または条件を満たす場合 true
    bool ExtractMesh(
        std::vector<MeshVertex>& outVerts,
        std::vector<uint32_t>& outIndices);

    // FBX に埋め込まれたテクスチャのファイル名を取得する。
    // RelativeFilename が空なら FileName のファイル名部分を返す
    /// Texture File Name を取得する
    /// @return 取得した文字列
    std::string GetTextureFileName() const;

private:
    /// Extract Geometry を返す
    /// @param geometry geometry に渡す値
    /// @param model model に渡す値
    /// @param outVerts outVerts に渡す値
    /// @param outIndices outIndices に渡す値
    /// @return 成功または条件を満たす場合 true
    bool ExtractGeometry(
        Node* geometry,
        Node* model,
        std::vector<MeshVertex>& outVerts,
        std::vector<uint32_t>& outIndices);

    /// Node Id を読み込む
    /// @param offset offset に渡す値
    /// @return 処理結果
    int64_t ReadNodeId(uint64_t offset) const;

    /// Global Matrix を取得する
    /// @param model model に渡す値
    /// @return 行列
    MATRIX4X4 GetGlobalMatrix(Node* model) const;

    /// Local Matrix を取得する
    /// @param model model に渡す値
    /// @return 行列
    MATRIX4X4 GetLocalMatrix(Node* model) const;

    // バイナリデータから型 T の値を読み込む
    template <typename T>
    /// 読み込む
    /// @param offset offset に渡す値
    /// @return 処理結果
    T Read(uint64_t offset) const
    {
        T v{};
        memcpy(&v, m_data.data() + offset, sizeof(T));
        return v;
    }

    // 型付き配列プロパティを読み込む（非圧縮 / zlib 圧縮の両方に対応）
    // 実装は FbxParser.cpp に記述。double / int32_t のみ使用可能。
    template <typename T>
    /// Typed Array を読み込む
    /// @param offset offset に渡す値
    /// @return 取得した要素一覧
    std::vector<T> ReadTypedArray(uint64_t offset);

    /// Node Header を読み込む
    /// @param offset offset に渡す値
    /// @param outEnd outEnd に渡す値
    /// @return 処理結果
    Node ReadNodeHeader(uint64_t offset, uint64_t& outEnd) const;
    /// Parse Children を返す
    /// @param start start に渡す値
    /// @param end end に渡す値
    /// @return 取得した要素一覧
    std::vector<Node> ParseChildren(uint64_t start, uint64_t end);
    /// Node を検索する
    /// @param nodes nodes に渡す値
    /// @param name 名前
    /// @return 対象のポインタ
    static Node* FindNode(std::vector<Node>& nodes, const std::string& name);

    /// Double Array を読み込む
    /// @param parent 親オブジェクト
    /// @param nodeName 名前
    /// @return 取得した要素一覧
    std::vector<double> ReadDoubleArray(Node* parent, const std::string& nodeName);

    /// Int Array を読み込む
    /// @param parent 親オブジェクト
    /// @param nodeName 名前
    /// @return 取得した要素一覧
    std::vector<int32_t> ReadIntArray(Node* parent, const std::string& nodeName);

    /// String を読み込む
    /// @param parent 親オブジェクト
    /// @param nodeName 名前
    /// @return 取得した文字列
    std::string ReadString(Node* parent, const std::string& nodeName) const;
};
