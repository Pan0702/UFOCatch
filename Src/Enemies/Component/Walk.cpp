#include "Walk.h"

#include <algorithm>
#include "../../System/GameInstance.h"
#include "../AnimalSheep/Sheep.h"
#include "../Base/EnemyBase.h"
#include "../System/EnemyManager.h"
#include "../System/Flock.h"


CWalk::CWalk(CEnemyBase* e, float speed)
{
    m_pOwner = e;
    m_moveSpeed = speed;
}

void CWalk::Enter()
{
    m_isFinish = false;

    // 現在の Transform を取得
    Transform trans = m_pOwner->GetTransform();
    m_position = trans.position;

    //目標位置の決定//
    bool foundValidMove = CalcRandomMove();

    //妥当な移動目標が見つからなかった場合は移動せず回転だけランダムに決める//
    if (!foundValidMove)
    {
        m_turnAmount = Randomf(-TURN_ANGLE_DEG, TURN_ANGLE_DEG) * DegToRad;
    }

    //目標が障害物上ならスナップして空きセルへずらす//
    CStageQuadTree* pTree = ObjectManager::FindQuadTree<CStageQuadTree>();
    AdjustTargetToFreeCell(pTree, CGameInstance::Get()->GetMapSize(), m_pathFinder.GetCellSize());

    //A*で経路を計算//
    VECTOR2 pos, size;
    m_pOwner->GetBounds2D(pos, size);
    m_pathFinder.SetAgentSize(size);
    const VECTOR2 start = {ToVec2XZ(m_position)};
    const VECTOR2 end = {ToVec2XZ(m_targetPos)};
    m_path = m_pathFinder.SearchRoute(start, end);
    m_pathIndex = 1;

    //回転の初期値と目標値をセット//
    m_currentRotation = trans.rotation.y;
    m_targetRotation = m_currentRotation + m_turnAmount;
    m_rotation = true;

    PlayWalkAnimation();
}

bool CWalk::CalcRandomMove()
{
    CSheep* sheep = dynamic_cast<CSheep*>(m_pOwner);
    //羊が群れに所属している場合は、群れ範囲を基準に移動目標を決める//
    if (sheep != nullptr && sheep->GetFlock() != nullptr)
    {
        return CalcSheepRandomMove(sheep);
    }
    return CalcDefaultRandomMove();
}

bool CWalk::CalcSheepRandomMove(const CSheep* sheep)
{
    CFlock* flock = sheep->GetFlock();

    //群れ内とリングのどちらに目標を取るか決める//
    constexpr float LEAVE_FLOCK_RATE = 0.2f;
    float minRadius = 0.0f;
    float maxRadius = flock->GetFlockRadius();
    if (Randomf(0.0f, 1.0f) < LEAVE_FLOCK_RATE)
    {
        //20%の確率で群れの外側リング（FlockRadius〜MoveRadius）を狙う//
        constexpr float MARGIN = 0.5f;
        minRadius = flock->GetFlockRadius() + MARGIN;
        maxRadius = flock->GetMoveRadius() - MARGIN;
        maxRadius = (std::max)(maxRadius, minRadius);
    }

    //極座標で目標位置を決定//
    const float angle = Randomf(0.0f, XM_2PI);
    const float dist = Randomf(minRadius, maxRadius);
    m_targetPos = flock->GetFlockCenter() + VECTOR3(
        cosf(angle) * dist,
        0.0f,
        sinf(angle) * dist
    );

    //現在向きから目標方向への回転量を計算//
    VECTOR3 toTarget = m_targetPos - m_position;
    toTarget.y = 0.0f;
    if (toTarget.LengthSquare() > NEAR_ZERO_LENSQ)
    {
        const float targetAngle = atan2f(toTarget.x, toTarget.z);
        m_turnAmount = targetAngle - m_pOwner->GetTransform().rotation.y;
    }
    else
    {
        m_turnAmount = 0.0f;
    }

    return true;
}

bool CWalk::CalcDefaultRandomMove()
{
    constexpr int MAX_RETRY = 50;

    //50回動けるか試してできなかったら動くのをやめる//
    for (int retry = 0; retry < MAX_RETRY; ++retry)
    {
        //回転量と移動距離をランダム決定し、回転後の前方ベクトルを目標位置にする//
        m_turnAmount = Randomf(-TURN_ANGLE_DEG, TURN_ANGLE_DEG) * DegToRad;
        constexpr float MIN_MOVE = 1.5f;
        constexpr float MAX_MOVE = 4.0f;
        const float moveAmount = Randomf(MIN_MOVE, MAX_MOVE);
        m_targetPos = m_position + VECTOR3(0, 0, moveAmount) * XMMatrixRotationY(m_turnAmount);

        //エリア内に収まる目標が見つかれば確定//
        if (IsInsideAreaXZ(m_targetPos, m_pOwner->GetAreaSize()))
        {
            return true;
        }
    }

    return false;
}

void CWalk::PlayWalkAnimation() const
{
    Animator* animator = m_pOwner->GetAnimator();
    animator->MergePlay(AnimationType::A_WALK);
    animator->SetPlaySpeed(1.0f);
}


void CWalk::Update()
{
    if (m_isFinish) return;

    m_pOwner->IsSuctionCheck();

    //経路が無いとき：その場で回転だけ補間して終了//
    if (m_path.empty())
    {
        UpdateRotationOnly();
        return;
    }
    //経路を最後まで踏んだら終了//
    if (m_pathIndex >= static_cast<int>(m_path.size()))
    {
        m_isFinish = true;
        return;
    }

    const VECTOR2 nextPoint = m_path[m_pathIndex];
    const VECTOR3 nextPos = {nextPoint.x, m_position.y, nextPoint.y};

    //回転計算：次ウェイポイント方向へ角度補間//
    VECTOR3 dir = nextPos - m_pOwner->GetTransform().position;
    const float targetAngle = atan2f(dir.x, dir.z);
    const float current = m_pOwner->GetTransform().rotation.y;
    const float t = 10.0f * SceneManager::DeltaTime();
    const float angleDiff = ClampRotateY(targetAngle - current);
    const float newAngle = current + angleDiff * t;
    m_pOwner->SetRotateY(newAngle);

    //移動計算：前方ベクトル＋スライド補正＋群れ境界チェック//
    VECTOR3 moveVec = VECTOR3(0, 0, m_moveSpeed * SceneManager::DeltaTime()) * XMMatrixRotationY(newAngle);
    moveVec = m_pOwner->CalcSlideMove(moveVec);
    if (IsBlockedByFlockBoundary(moveVec)) return;
    m_pOwner->AddPosition(moveVec);

    //到達判定→次のウェイポイントへ//
    VECTOR3 toTarget = nextPos - m_pOwner->GetTransform().position;
    toTarget.y = 0;
    const float cellSize = m_pathFinder.GetCellSize();
    const float reachDistSq = Pow2(cellSize * 0.5f);
    if (toTarget.LengthSquare() < reachDistSq)
    {
        AdvancePathIndex();
    }
}

void CWalk::UpdateRotationOnly()
{
    //移動経路が無い場合は、目標角度までその場で回転補間する//
    static constexpr float ROTATION_LERP_SPEED = 10.0f;
    const float t = ROTATION_LERP_SPEED * SceneManager::DeltaTime();
    m_currentRotation += (m_targetRotation - m_currentRotation) * t;
    if (abs(m_targetRotation - m_currentRotation) < 0.01f)
    {
        m_isFinish = true;
    }
    m_pOwner->SetRotateY(ClampRotateY(m_currentRotation));
}

bool CWalk::IsBlockedByFlockBoundary(const VECTOR3& moveVec) const
{
    CSheep* sheep = dynamic_cast<CSheep*>(m_pOwner);
    if (sheep == nullptr) return false;
    CFlock* flock = sheep->GetFlock();
    if (flock == nullptr) return false;
    return !flock->ContainMoveArea(m_pOwner->GetTransform().position + moveVec);
}

void CWalk::AdvancePathIndex()
{
    m_pathIndex++;
    if (m_pathIndex >= static_cast<int>(m_path.size()))
    {
        m_isFinish = true;
        return;
    }

    // 進行方向がほぼ同じ次ポイントはスキップして滑らかに進む
    const float threshold = cos(30.0f * DegToRad);
    while (m_pathIndex < static_cast<int>(m_path.size()) - 1)
    {
        const VECTOR2 myXZ = ToVec2XZ(m_pOwner->GetTransform().position);
        const VECTOR2 toNext = normalize(m_path[m_pathIndex] - myXZ);
        const VECTOR2 toAfter = normalize(m_path[m_pathIndex + 1] - myXZ);
        if (dot(toNext, toAfter) > threshold) m_pathIndex++;
        else break;
    }
}

float CWalk::ClampRotateY(float angle)
{
    return std::remainder(angle, XM_2PI);
}

void CWalk::AdjustTargetToFreeCell(const CStageQuadTree* pTree, const VECTOR4& mapBounds, float cellSize)
{
    if (pTree == nullptr) return;
    //目標座標をグリッドセル番号に変換して探索開始地点にする//
    const int startIX = static_cast<int>(std::round(m_targetPos.x / cellSize));
    const int startIZ = static_cast<int>(std::round(m_targetPos.z / cellSize));

    auto makeKey = [](int ix, int iz) -> long long
    {
        return (static_cast<long long>(ix) << 32) |
            static_cast<unsigned int>(iz);
    };
    //障害物チェックAABB

    const float half = cellSize * 0.5f;
    {
        const VECTOR2 c = {static_cast<float>(startIX) * cellSize, static_cast<float>(startIZ) * cellSize};
        const VECTOR2 topLeft = {c.x - half, c.y - half};
        const VECTOR2 size = {cellSize, cellSize};
        //trueなら障害物無し
        if (pTree->GetOverlappingObjects(topLeft, size).empty()) return;
    }

    //最大探索リング数
    constexpr int MAX_RING = 10;
    constexpr int DIR[8][2] = {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1},
        {1, 1}, {1, -1}, {-1, 1}, {-1, -1}
    };
    std::queue<Vec2Int> openQueue;
    std::unordered_set<long long> visited;
    openQueue.push({startIX, startIZ});
    visited.insert(makeKey(startIX, startIZ));
    //周囲のセルを幅優先で探索し、最初に見つかった空きセルへ目標をずらす//
    while (!openQueue.empty())
    {
        Vec2Int cur = openQueue.front();
        openQueue.pop();
        const int ring = (std::max)(std::abs(cur.x - startIX), std::abs(cur.y - startIZ));
        if (ring > MAX_RING) continue;
        const VECTOR2 c = {static_cast<float>(cur.x) * cellSize, static_cast<float>(cur.y) * cellSize};
        const bool inMap = (c.x > mapBounds.x + half && c.x < mapBounds.z - half &&
            c.y > mapBounds.y + half && c.y < mapBounds.w - half);
        if (!inMap) continue;
        const bool inArea = IsInsideAreaXZ(VECTOR3(c.x, 0.0f, c.y), m_pOwner->GetAreaSize());
        VECTOR2 topLeft = c - half;
        VECTOR2 size2d = {cellSize, cellSize};
        const bool onObstacle = pTree->GetOverlappingObjects(topLeft, size2d).empty();
        if (onObstacle && inArea)
        {
            m_targetPos = VECTOR3(c.x, 0.0f, c.y);
            return;
        }
        //8方向の隣接セルを未訪問なら探索候補に追加する//
        for (auto& d : DIR)
        {
            Vec2Int n = {cur.x + d[0], cur.y + d[1]};
            long long key = makeKey(n.x, n.y);
            if (visited.contains(key)) continue;
            visited.insert(key);
            openQueue.push(n);
        }
    }
}
