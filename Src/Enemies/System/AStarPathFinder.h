#pragma once
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "../../Utils/MyMath.h"

class CStageQuadTree;

/// @brief A*探索で使用するノード情報//
struct AStarNode
{
    VECTOR2 pos; //!< ワールド座標//
    float g;     //!< スタートからのコスト（実コスト）//
    float h;     //!< ゴールまでの予測コスト（ヒューリスティック）//
    float f;     //!< 総コスト（g + h）//
    bool operator>(const AStarNode& o) const { return f > o.f; }
};

/// @brief VECTOR2をハッシュマップのキーに使うためのグリッド座標キー//
struct Vec2Int
{
    int x, y;
    bool operator==(const Vec2Int& o) const { return x == o.x && y == o.y; }
};

/// @brief Vec2IntKey 用のハッシュ関数
struct Vec2IntKeyHash
{
    size_t operator()(const Vec2Int& k) const
    {
        return std::hash<long long>()(static_cast<long long>(k.x) << 32 | static_cast<unsigned int>(k.y));
    }
};

/// @brief A*アルゴリズムによる経路探索クラス
///
/// グリッドベースのA*探索で、障害物を回避しながらスタートからゴールまでの
/// 最短経路を求める。斜め移動対応（8方向）//
class CAStarPathFinder
{
public:
    /// @brief コンストラクタ
    /// @param cellSize グリッド1セルのサイズ（ワールド単位）//
    CAStarPathFinder(float cellSize = 0.4f);

    /// @brief A*アルゴリズムで経路を探索する
    /// @param start 開始位置（ワールド座標）
    /// @param goal  目標位置（ワールド座標）
    /// @return スタートからゴールへのウェイポイント列。経路が見つからない場合は空ベクター//
    std::vector<VECTOR2> SearchRoute(VECTOR2 start, VECTOR2 goal);

    /// @brief グリッドのセルサイズを返す
    float GetCellSize() const { return m_cellSize; }

    /// @brief 衝突判定に使うエージェントのサイズを設定する
    /// @param size エージェントのAABBサイズ（幅・高さ）//
    void SetAgentSize(const VECTOR2& size);

private:
    /// @brief 座標を最近傍グリッドセル中心にスナップする
    /// @param pos スナップ前のワールド座標
    /// @return スナップ後のワールド座標//
    VECTOR2 Snap(const VECTOR2& pos) const;

    /// @brief ワールド座標をグリッド整数キーに変換する
    /// @param pos ワールド座標
    /// @return グリッドキー
    Vec2Int ToKey(const VECTOR2& pos) const;

    /// @brief 指定セルが障害物と重なっているか判定する
    /// @param pos        判定するセルのワールド座標（中心）
    /// @param pQuadTree  ステージの四分木（nullptr の場合は障害物なしとみなす）//
    /// @return 障害物があれば true
    bool HasObstacle(const VECTOR2& pos, const CStageQuadTree* pQuadTree) const;

    /// @brief ゴールからスタートへ遡って経路を復元する
    /// @param effectiveGoal 到達したゴールのワールド座標
    /// @param start         スタートのワールド座標
    /// @param startKey      スタートのグリッドキー
    /// @param cameFrom      各ノードの親ノード座標マップ
    /// @return スタートからゴール順に並んだウェイポイント列//
    std::vector<VECTOR2> ReconstructPath(
        const VECTOR2& effectiveGoal,
        const VECTOR2& start,
        const Vec2Int& startKey,
        const std::unordered_map<Vec2Int, VECTOR2, Vec2IntKeyHash>& cameFrom) const;

private:
    float m_cellSize = 0;    //!< グリッド1セルのサイズ//
    VECTOR2 m_agentSize = {}; //!< 障害物判定に使うエージェントのAABBサイズ//
};