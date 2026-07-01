#include "AStarPathFinder.h"

#include <array>
#include <numbers>
#include <windows.h>

#include "../../Stage/StageQuadTree.h"
#include "../../Stage/StageObject.h"
#include "../../System/GameInstance.h"

CAStarPathFinder::CAStarPathFinder(float cellSize)
{
    m_cellSize = cellSize;
}

std::vector<VECTOR2> CAStarPathFinder::SearchRoute(VECTOR2 start, VECTOR2 goal)
{
    // 座標をグリッドにスナップ（計算の一貫性を保つため）//
    start = Snap(start);
    goal = Snap(goal);

    // Openリスト：未探索ノードをf値の昇順で管理するプライオリティキュー//
    std::priority_queue<AStarNode, std::vector<AStarNode>, std::greater<AStarNode>> open;

    // gScore：スタートから各ノードへの最小コスト//
    // cameFrom：経路復元用の親ノード座標マップ//
    std::unordered_map<Vec2Int, float, Vec2IntKeyHash> gScore;
    std::unordered_map<Vec2Int, VECTOR2, Vec2IntKeyHash> cameFrom;

    constexpr float DIAGONAL_COST = std::numbers::sqrt2_v<float>;

    // ヒューリスティック関数：オクタイル距離//
    // 斜め移動コスト√2を考慮した、実コストに近い楽観的な距離推定//
    auto heuristic = [](const VECTOR2& a, const VECTOR2& b)
    {
        const float dx = std::abs(a.x - b.x);
        const float dy = std::abs(a.y - b.y);
        // 軸方向の差が大きい方 + 斜め分のコスト追加（√2 - 1 ≒ 0.414）//
        return (std::max)(dx, dy) + (DIAGONAL_COST - 1.0f) * (std::min)(dx, dy);
    };

    // QuadTreeを1回だけ取得してA*全体で使いまわす（毎回検索するとコスト大）//
    CStageQuadTree* pQuadTree = ObjectManager::FindQuadTree<CStageQuadTree>();

    // スタートノードをOpenリストに追加
    const Vec2Int startKey = ToKey(start);
    gScore[startKey] = 0;
    const float startH = heuristic(start, goal);
    open.push({start, 0, startH, startH});
    const Vec2Int goalKey = ToKey(goal);

    // 8方向への移動ベクター（上下左右 + 斜め4方向）//
    const std::array<VECTOR2, 8> DIRS = {
        {
            {0, -m_cellSize}, {0, m_cellSize},
            {-m_cellSize, 0}, {m_cellSize, 0},
            {-m_cellSize, -m_cellSize}, {m_cellSize, -m_cellSize},
            {-m_cellSize, m_cellSize}, {m_cellSize, m_cellSize}
        }
    };

    int mainLoopCount = 0;


    // メインループ：Openリストが空になるか、ゴールに到達するまで繰り返す//
    while (!open.empty())
    {
        constexpr int MAX_MAIN_LOOP = 10000;
        // 無限ループ防止
        if (++mainLoopCount > MAX_MAIN_LOOP)
        {
            OutputDebugStringA("AStarPathFinder: Main loop limit exceeded!\n");
            break;
        }

        // f値が最小のノードを取り出す（最も有望な候補から探索）//
        AStarNode cur = open.top();
        open.pop();

        const Vec2Int curKey = ToKey(cur.pos);

        // staleなエントリをスキップ
        // 同一ノードが複数回Openに積まれることがあるため、
        // 記録済みのgScoreより大きい場合は古いエントリなので無視する//
        if (gScore.contains(curKey) && cur.g > gScore[curKey]) continue;

        // ゴール到達チェック//
        // キーの完全一致、または座標が十分に近い場合（障害物でゴールに乗れない場合を考慮）//
        const float dx = cur.pos.x - goal.x;
        const float dy = cur.pos.y - goal.y;
        const bool goalBlocked = HasObstacle(goal, pQuadTree);
        const bool reachedGoal = (curKey == goalKey) || (goalBlocked && dx * dx + dy * dy <= m_cellSize * m_cellSize *
            4.0f);
        if (reachedGoal)
        {
            // cur.pos を実効ゴールとして経路を復元して返す//
            return ReconstructPath(cur.pos, start, startKey, cameFrom);
        }

        for (auto& d : DIRS)
        {
            const VECTOR2 next = {cur.pos.x + d.x, cur.pos.y + d.y};
            const Vec2Int nextKey = ToKey(next);

            // 障害物があるセルはスキップ（ゴール自体は障害物判定から除外）//
            if (nextKey != goalKey && HasObstacle(next, pQuadTree)) continue;

            // 斜め移動は√2倍のコスト、直線移動はそのままm_cellSize//
            const float cost = (d.x != 0 && d.y != 0) ? m_cellSize * DIAGONAL_COST : m_cellSize;
            const float ng = gScore[curKey] + cost;

            if (!gScore.contains(nextKey) || ng < gScore[nextKey])
            {
                gScore[nextKey] = ng;
                cameFrom[nextKey] = cur.pos;
                const float nh = heuristic(next, goal);
                open.push({next, ng, nh, ng + nh});
            }
        }
    }

    // Openリストが空になっても到達できなかった場合は空を返す//
    return {};
}

void CAStarPathFinder::SetAgentSize(const VECTOR2& size)
{
    m_agentSize = size;
}

VECTOR2 CAStarPathFinder::Snap(const VECTOR2& pos) const
{
    // 各軸を独立してグリッドの最近傍セル中心に丸める//
    return {
        std::round(pos.x / m_cellSize) * m_cellSize,
        std::round(pos.y / m_cellSize) * m_cellSize
    };
}

Vec2Int CAStarPathFinder::ToKey(const VECTOR2& pos) const
{
    // ワールド座標をグリッド整数インデックスに変換する//
    // roundで浮動小数点誤差による不一致を防ぐ//
    return {
        .x = static_cast<int>(std::round(pos.x / m_cellSize)),
        .y = static_cast<int>(std::round(pos.y / m_cellSize))
    };
}

bool CAStarPathFinder::HasObstacle(const VECTOR2& pos, const CStageQuadTree* pQuadTree) const
{
    if (pQuadTree == nullptr) return false;

    // エージェントサイズとセルサイズを合わせた半径でAABBを作成する//
    // エージェントが通れるかどうかを正確に判定するため//
    const VECTOR2 half = {
        (m_cellSize + m_agentSize.x) * 0.5f,
        (m_cellSize + m_agentSize.y) * 0.5f
    };
    const VECTOR2 topLeft = {pos.x - half.x, pos.y - half.y};

    return !pQuadTree->GetOverlappingObjects(topLeft, {half.x * 2.0f, half.y * 2.0f}).empty();
}

std::vector<VECTOR2> CAStarPathFinder::ReconstructPath(
    const VECTOR2& effectiveGoal, const VECTOR2& start, const Vec2Int& startKey,
    const std::unordered_map<Vec2Int, VECTOR2, Vec2IntKeyHash>& cameFrom) const
{
    std::vector<VECTOR2> path;
    VECTOR2 c = effectiveGoal;
    int reconstructCount = 0;
    constexpr int MAX_RECONSTRUCT = 1000;

    // ゴールからスタートへ cameFrom を逆にたどってウェイポイントを収集する//
    while (ToKey(c) != startKey)
    {
        // 無限ループ防止
        if (++reconstructCount > MAX_RECONSTRUCT)
        {
            OutputDebugStringA("AStarPathFinder: Reconstruct loop limit exceeded!\n");
            return {};
        }

        path.push_back(c);
        const Vec2Int cKey = ToKey(c);

        // 対応する親ノードが存在しない場合は空のベクターを返す//
        if (!cameFrom.contains(cKey))
        {
            OutputDebugStringA("AStarPathFinder: cameFrom key not found during reconstruction!\n");
            return {};
        }

        VECTOR2 nextC = cameFrom.at(cKey);

        // 自己参照チェック（無限ループ防止）//
        if (ToKey(nextC) == cKey)
        {
            OutputDebugStringA("AStarPathFinder: Self-reference in cameFrom during reconstruction!\n");
            break;
        }

        c = nextC;
    }

    path.push_back(start);

    // 収集した経路はゴール→スタート順なので逆順にしてスタート→ゴール順にする//
    std::ranges::reverse(path);
    return path;
}
