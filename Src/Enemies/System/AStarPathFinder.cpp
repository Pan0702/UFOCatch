#include "AStarPathFinder.h"

#include <array>
#include <numbers>
#include <windows.h>
#include <string>

#include "../../Stage/StageQuadTree.h"
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

    // コストと親の管理
    std::unordered_map<Vec2IntKey, float, Vec2IntKeyHash> gScore;
    std::unordered_map<Vec2IntKey, VECTOR2, Vec2IntKeyHash> cameFrom;

    constexpr float DIAGONAL_COST = std::numbers::sqrt2_v<float>;
    //オクタイル距離を求める
    auto heuristic = [](const VECTOR2& a, const VECTOR2& b)
    {
        const float dx = std::abs(a.x - b.x);
        const float dy = std::abs(a.y - b.y);
        return (std::max)(dx, dy) + (DIAGONAL_COST - 1.0f) * (std::min)(dx, dy);
    };

    // QuadTreeを1回だけ取得してA*全体で使いまわす
    CStageQuadTree* pQuadTree = ObjectManager::FindQuadTree<CStageQuadTree>();

    // スタートノードを追加
    const Vec2IntKey startKey = ToKey(start);
    gScore[startKey] = 0;
    open.push({start, 0, heuristic(start, goal), heuristic(start, goal)});
    const Vec2IntKey goalKey = ToKey(goal);

    const std::array<VECTOR2, 8> DIRS = {
        {
            {0, -m_cellSize}, {0, m_cellSize},
            {-m_cellSize, 0}, {m_cellSize, 0},
            {-m_cellSize, -m_cellSize}, {m_cellSize, -m_cellSize},
            {-m_cellSize, m_cellSize}, {m_cellSize, m_cellSize}
        }
    };

    int mainLoopCount = 0;
    constexpr int MAX_MAIN_LOOP = 10000;
    while (!open.empty())
    {
        if (++mainLoopCount > MAX_MAIN_LOOP)
        {
            OutputDebugStringA("AStarPathFinder: Main loop limit exceeded!\n");
            break;
        }

        // f値が最小のノードを取り出す
        AStarNode cur = open.top();
        open.pop();

        const Vec2IntKey curKey = ToKey(cur.pos);

        // staleなエントリをスキップ（より良いパスが既に見つかっている）
        if (gScore.contains(curKey) && cur.g > gScore[curKey]) continue;

        // ゴール到達チェック（完全一致 or ゴールに十分近い）
        const float dx = cur.pos.x - goal.x;
        const float dy = cur.pos.y - goal.y;
        const bool reachedGoal = (curKey == goalKey) || (dx * dx + dy * dy <= m_cellSize * m_cellSize * 4.0f);
        if (reachedGoal)
        {
            // 経路復元（cur.posを実効ゴールとして使う）
            std::vector<VECTOR2> path;
            VECTOR2 c = cur.pos;
            int reconstructCount = 0;
            constexpr int MAX_RECONSTRUCT = 1000;
            while (ToKey(c) != startKey)
            {
                if (++reconstructCount > MAX_RECONSTRUCT)
                {
                    OutputDebugStringA("AStarPathFinder: Reconstruct loop limit exceeded!\n");
                    break;
                }
                path.push_back(c);
                const Vec2IntKey cKey = ToKey(c);
                if (!cameFrom.contains(cKey))
                {
                    OutputDebugStringA("AStarPathFinder: cameFrom key not found during reconstruction!\n");
                    break;
                }
                VECTOR2 nextC = cameFrom[cKey];
                if (ToKey(nextC) == cKey)
                {
                    OutputDebugStringA("AStarPathFinder: Self-reference in cameFrom during reconstruction!\n");
                    break;
                }
                c = nextC;
            }
            path.push_back(start);
            std::ranges::reverse(path);
            return path;
        }

        for (auto& d : DIRS)
        {
            const VECTOR2 next = {cur.pos.x + d.x, cur.pos.y + d.y};
            const Vec2IntKey nextKey = ToKey(next);

            // 障害物チェック（ゴールは除外）
            if (nextKey != goalKey && HasObstacle(next, pQuadTree)) continue;

            // gコストを計算
            const float cost = (d.x != 0 && d.y != 0) ? m_cellSize * DIAGONAL_COST : m_cellSize;
            const float ng = gScore[curKey] + cost;

            // より良いルートなら更新
            if (!gScore.contains(nextKey) || ng < gScore[nextKey])
            {
                gScore[nextKey] = ng;
                cameFrom[nextKey] = cur.pos;
                const float nh = heuristic(next, goal);
                open.push({next, ng, nh, ng + nh});
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
    const VECTOR2 half = { (m_cellSize + m_agentSize.x) * 0.5f,
                           (m_cellSize + m_agentSize.y) * 0.5f };
    const VECTOR2 topLeft = { pos.x - half.x, pos.y - half.y };
    return !pQuadTree->GetOverlappingObjects(topLeft, { half.x * 2.0f, half.y * 2.0f }).empty();
}
