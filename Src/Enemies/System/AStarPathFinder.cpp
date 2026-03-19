#include "AStarPathFinder.h"

#include <queue>

#include "StageQuadTree.h"
#include "../../Stage/StageObject.h"
#include "../../System/GameInstance.h"

CAStarPathFinder::CAStarPathFinder(float cellSize)
{
    VECTOR4 size = CGameInstance::Get()->GetMapSize();
    float mapRange = size.x + size.z;
    mapRange /= Pow(2, cellSize);
    m_cellSize = mapRange;
}

Vec2IntKey CAStarPathFinder::ToKey(const VECTOR2& pos) const
{
    return { static_cast<int>(pos.x), static_cast<int>(pos.y) };
}

std::vector<VECTOR2> CAStarPathFinder::SerchRoute(VECTOR2 start, VECTOR2 goal)
{
    start = Snap(start);
    goal  = Snap(goal);

    // Openリスト（f値が小さい順）
    std::priority_queue<AStarNode, std::vector<AStarNode>, std::greater<AStarNode>> open;

    // Closedリスト（探索済み）
    std::unordered_set<Vec2IntKey, Vec2IntKeyHash> closed;

    // コストと親の管理（文字列キーをやめて整数ペアキーに変更）
    std::unordered_map<Vec2IntKey, float,   Vec2IntKeyHash> gScore;
    std::unordered_map<Vec2IntKey, VECTOR2, Vec2IntKeyHash> cameFrom;

    // ヒューリスティック（マンハッタン距離）
    auto heuristic = [](VECTOR2 a, VECTOR2 b) {
        return std::abs(a.x - b.x) + std::abs(a.y - b.y);
    };

    // QuadTreeを1回だけ取得してA*全体で使いまわす
    CStageQuadTree* pQuadTree = ObjectManager::FindGameObject<CStageQuadTree>();

    // スタートノードを追加
    Vec2IntKey startKey = ToKey(start);
    gScore[startKey] = 0;
    open.push({start, 0, heuristic(start, goal), heuristic(start, goal), start});

    Vec2IntKey goalKey = ToKey(goal);

    while (!open.empty())
    {
        // f値が最小のノードを取り出す
        AStarNode cur = open.top();
        open.pop();

        Vec2IntKey curKey = ToKey(cur.pos);

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

        const VECTOR2 DIRS[8] = {
            {0, -m_cellSize}, {0,  m_cellSize},
            {-m_cellSize, 0}, {m_cellSize,  0},
            {-m_cellSize, -m_cellSize}, {m_cellSize, -m_cellSize},
            {-m_cellSize,  m_cellSize}, {m_cellSize,  m_cellSize}
        };

        for (auto& d : DIRS)
        {
            VECTOR2 next = {cur.pos.x + d.x, cur.pos.y + d.y};
            Vec2IntKey nextKey = ToKey(next);

            // 探索済みならスキップ
            if (closed.contains(nextKey)) continue;

            // 障害物チェック（QuadTreeはループ外でキャッシュ済み）
            if (HasObstacle(next, pQuadTree)) continue;

            // gコストを計算
            float cost = (d.x != 0 && d.y != 0) ? m_cellSize * 1.414f : m_cellSize;
            float ng = gScore[curKey] + cost;

            // より良いルートなら更新
            if (!gScore.contains(nextKey) || ng < gScore[nextKey])
            {
                gScore[nextKey]   = ng;
                cameFrom[nextKey] = cur.pos;
                float nh = heuristic(next, goal);
                open.push({next, ng, nh, ng + nh, cur.pos});
            }
        }
    }
    return {};
}

VECTOR2 CAStarPathFinder::Snap(const VECTOR2& pos)
{
    return {
        std::round(pos.x / m_cellSize) * m_cellSize,
        std::round(pos.y / m_cellSize) * m_cellSize
    };
}

bool CAStarPathFinder::HasObstacle(const VECTOR2& pos, CStageQuadTree* pQuadTree)
{
    if (pQuadTree == nullptr) return false;
    VECTOR2 checkSize = {m_cellSize, m_cellSize};
    return !pQuadTree->GetNearbyObjects(pos, checkSize).empty();
}
