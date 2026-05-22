#pragma once
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "../../Utils/MyMath.h"

/// <summary>敵AIで使う Stage Quad Tree の情報と処理をまとめる型</summary>
class CStageQuadTree;

/// @brief A*探索で使用するノード情報//
struct AStarNode
{
    VECTOR2 pos; //!< ワールド座標//
    float g; //!< スタートからのコスト（実コスト）//
    float h; //!< ゴールまでの予測コスト（ヒューリスティック）//
    float f; //!< 総コスト（g + h）//
    /// operator> の処理を行う
    /// @param o o に渡す値
    /// @return 成功または条件を満たす場合 true
    bool operator>(const AStarNode& o) const { return f > o.f; }
};

/// @brief VECTOR2をハッシュマップのキーに使うためのグリッド座標キー//
struct Vec2Int
{
    int x, y;
    /// operator== の処理を行う
    /// @param o o に渡す値
    /// @return 成功または条件を満たす場合 true
    bool operator==(const Vec2Int& o) const { return x == o.x && y == o.y; }
};

/// @brief Vec2IntKey 用のハッシュ関数
struct Vec2IntKeyHash
{
    /// operator の処理を行う
    /// @return 処理結果
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
    /// CAStarPathFinder を初期化する
    /// @param cellSize サイズ
    CAStarPathFinder(float cellSize = 0.4f);

    /// Search Route を返す
    /// @param start start に渡す値
    /// @param goal goal に渡す値
    /// @return 取得した要素一覧
    std::vector<VECTOR2> SearchRoute(VECTOR2 start, VECTOR2 goal);

    /// Cell Size を取得する
    /// @return 計算結果の値
    float GetCellSize() const { return m_cellSize; }

    /// Agent Size を設定する
    /// @param size サイズ
    void SetAgentSize(const VECTOR2& size);

private:
    /// Snap を返す
    /// @param pos 座標
    /// @return 2次元ベクトル
    VECTOR2 Snap(const VECTOR2& pos) const;

    /// To Key を返す
    /// @param pos 座標
    /// @return 処理結果
    Vec2Int ToKey(const VECTOR2& pos) const;

    /// Obstacle を保持しているか判定する
    /// @param pos 座標
    /// @param pQuadTree pQuadTree に渡す値
    /// @return 成功または条件を満たす場合 true
    bool HasObstacle(const VECTOR2& pos, const CStageQuadTree* pQuadTree) const;

    /// Reconstruct Path を返す
    /// @param effectiveGoal effectiveGoal に渡す値
    /// @param start start に渡す値
    /// @param startKey startKey に渡す値
    /// @param cameFrom cameFrom に渡す値
    /// @return 取得した要素一覧
    std::vector<VECTOR2> ReconstructPath(
        const VECTOR2& effectiveGoal,
        const VECTOR2& start,
        const Vec2Int& startKey,
        const std::unordered_map<Vec2Int, VECTOR2, Vec2IntKeyHash>& cameFrom) const;

private:
    float m_cellSize = 0; //!< グリッド1セルのサイズ//
    VECTOR2 m_agentSize = {}; //!< 障害物判定に使うエージェントのAABBサイズ//
};
