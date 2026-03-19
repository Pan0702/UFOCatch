#pragma once
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "../../Utils/MyMath.h"

class CStageQuadTree;

struct AStarNode
{
    //ワールド座標
    VECTOR2 pos;
    //スタートからのコスト
    float g;
    //ゴールまでの予測コスト
    float h;
    //g + h
    float f;
    //どこから来たか
    VECTOR2 parent;
    bool operator>(const AStarNode& o) const { return f > o.f; }
};

// VECTOR2をハッシュマップのキーに使うためのヘルパー
struct Vec2IntKey
{
    int x, y;
    bool operator==(const Vec2IntKey& o) const { return x == o.x && y == o.y; }
};
struct Vec2IntKeyHash
{
    size_t operator()(const Vec2IntKey& k) const
    {
        return std::hash<int>()(k.x) ^ (std::hash<int>()(k.y) << 16);
    }
};

class CAStarPathFinder
{
public:
    CAStarPathFinder(float cellSize = 2.0f);

    ///経路探索
    /// @param start 自身のいる場所
    /// @param goal 目的地
    /// @return
    std::vector<VECTOR2> SerchRoute(VECTOR2 start, VECTOR2 goal);

    float GetCellSize() const { return m_cellSize; }

private:
    VECTOR2 Snap(const VECTOR2& pos);
    Vec2IntKey ToKey(const VECTOR2& pos) const;
    bool HasObstacle(const VECTOR2& pos, CStageQuadTree* pQuadTree);

private:
    //セルの大きさ
    float m_cellSize = 0;
};
