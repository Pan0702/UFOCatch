#include "AStarPathFinder.h"

#include <array>
#include <queue>
#include <numbers>
#include "StageQuadTree.h"
#include "../../Stage/StageObject.h"
#include "../../System/GameInstance.h"

CAStarPathFinder::CAStarPathFinder(float cellSize)
{
    m_cellSize = cellSize;
}

std::vector<VECTOR2> CAStarPathFinder::SearchRoute(VECTOR2 start, VECTOR2 goal)
{
    start = Snap(start);
    goal = Snap(goal);

    // Openリスト（f値が小さい順）
    std::priority_queue<AStarNode, std::vector<AStarNode>, std::greater<AStarNode>> open;

    // Closedリスト（探索済み）
    std::unordered_set<Vec2IntKey, Vec2IntKeyHash> closed;

    // コストと親の管理
    std::unordered_map<Vec2IntKey, float, Vec2IntKeyHash> gScore;
    std::unordered_map<Vec2IntKey, VECTOR2, Vec2IntKeyHash> cameFrom;

    constexpr float DIAGONAL_COST = std::numbers::sqrt2_v<float>;
    //オクタイル距離を求める
    auto heuristic = [this](const VECTOR2& a, const VECTOR2& b)
    {
        const float dx = std::abs(a.x - b.x);
        const float dy = std::abs(a.y - b.y);
        return (std::max)(dx, dy) + (DIAGONAL_COST - 1.0f) * (std::min)(dx, dy);
    };

    // QuadTreeを1回だけ取得してA*全体で使いまわす
    CStageQuadTree* pQuadTree = ObjectManager::FindGameObject<CStageQuadTree>();

    // スタートノードを追加
    Vec2IntKey startKey = ToKey(start);
    gScore[startKey] = 0;
    open.push({start, 0, heuristic(start, goal), heuristic(start, goal), start});

    Vec2IntKey goalKey = ToKey(goal);


    const std::array<VECTOR2, 8> DIRS = {{
        {0, -m_cellSize}, {0,  m_cellSize},                
        {-m_cellSize, 0}, {m_cellSize,  0},
        {-m_cellSize, -m_cellSize}, {m_cellSize,-m_cellSize},   
        {-m_cellSize,  m_cellSize}, {m_cellSize, m_cellSize}                                            
    }};
    
    while (!open.empty())
    {
        // f値が最小のノードを取り出す
        AStarNode cur = open.top();
        open.pop();

        const Vec2IntKey curKey = ToKey(cur.pos);

        // 探索済みならスキップ
        if (closed.contains(curKey)) continue;
        closed.insert(curKey);

        // ゴール到達チェック
        if (curKey == goalKey)
        {
            // 経路復元
            std::vector<VECTOR2> path;
            VECTOR2 c = goal;
            while (ToKey(c) != startKey)
            {
                path.push_back(c);
                c = cameFrom[ToKey(c)];
            }
            path.push_back(start);
            std::ranges::reverse(path);
            return path;
        }
        

        for (auto& d : DIRS)
        {
            const VECTOR2 next = {cur.pos.x + d.x, cur.pos.y + d.y};
            const Vec2IntKey nextKey = ToKey(next);

            // 探索済みならスキップ
            if (closed.contains(nextKey)) continue;

            // 障害物チェック
            if (HasObstacle(next, pQuadTree)) continue;

            // gコストを計算
            const float cost = (d.x != 0 && d.y != 0) ? m_cellSize * DIAGONAL_COST : m_cellSize;
            const float ng = gScore[curKey] + cost;

            // より良いルートなら更新
            if (!gScore.contains(nextKey) || ng < gScore[nextKey])
            {
                gScore[nextKey] = ng;
                cameFrom[nextKey] = cur.pos;
                const float nh = heuristic(next, goal);
                open.push({next, ng, nh, ng + nh, cur.pos});
            }
        }
    }
    return {};
}

void CAStarPathFinder::SetAnimSize(const VECTOR2& size)
{
    m_agentSize = size;
}

VECTOR2 CAStarPathFinder::Snap(const VECTOR2& pos) const
{
    return {
         std::round(pos.x / m_cellSize) * m_cellSize,
          std::round(pos.y / m_cellSize) * m_cellSize
    };
}
Vec2IntKey CAStarPathFinder::ToKey(const VECTOR2& pos) const
{
    return {
        .x = static_cast<int>(std::round(pos.x / m_cellSize)),
        .y = static_cast<int>(std::round(pos.y / m_cellSize))
    };
}

bool CAStarPathFinder::HasObstacle(const VECTOR2& pos, const CStageQuadTree* pQuadTree) const
{
    if (pQuadTree == nullptr) return false;
    const VECTOR2 checkSize = 
        {m_cellSize + m_agentSize.x,
        m_cellSize + m_agentSize.y};
    return !pQuadTree->GetNearbyObjects(pos, checkSize).empty();
}
