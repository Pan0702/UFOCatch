#include "Walk.h"

#include "../../System/GameInstance.h"
#include "../Base/EnemyBase.h"
#include "../Human/Human.h"
#include "../AnimalSheep/Sheep.h"
#include "../System/Flog.h"

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

    bool foundValidMove = CalcRandomMove();

    // 最後まで見つからなかった場合、
    // 「移動しない」にして、回転だけランダムに決める
    if (!foundValidMove)
    {
        m_turnAmount = Randomf(-TURN_ANGLE_DEG, TURN_ANGLE_DEG) * DegToRad;
    }
    
    CStageQuadTree* pTree = ObjectManager::FindQuadTree<CStageQuadTree>();
    AdjustTargetToFreeCell(pTree,CGameInstance::Get()->GetMapSize(),m_pathFinder.GetCellSize());
    
    VECTOR2 pos, size;
    m_pOwner->GetBounds2D(pos, size);
    m_pathFinder.SetAgentSize(size);
    const VECTOR2 start = {ToVec2XZ(m_position)};
    const VECTOR2 end = {ToVec2XZ(m_targetPos)};
    
    m_path = m_pathFinder.SearchRoute(start, end);

    m_pathIndex = 1;

    m_currentRotation = trans.rotation.y;

    //目標回転
    m_targetRotation = m_currentRotation + m_turnAmount;

    //回転処理を終わらせるためのフラグ
    m_rotation = true;
    PlayWalkAnimation();
}

/// 境界内に収まるランダムな回転量と移動距離を決定する
/// ランダムに回転量（-180°～180°）と移動距離（1.5～4.0）を決定し、境界チェックに通るまで最大50回リトライする。
/// 妥当な組み合わせが見つかった場合、m_turnAmountとm_moveAmountに設定される。
/// @return 妥当な移動パラメータが見つかった場合true、最大試行回数を超えた場合false
bool CWalk::CalcRandomMove()
{
    static constexpr int MAX_RETRY = 50; // ランダム移動の試行回数上限（境界外に出ない組み合わせが見つかるまで最大 N 回試す）
    static constexpr float MIN_MOVE = 1.5f; // ランダム移動距離の範囲（最小）
    static constexpr float MAX_MOVE = 4.0f; // ランダム移動距離の範囲（最大）

    // Sheep専用の範囲チェック
    CSheep* sheep = dynamic_cast<CSheep*>(m_pOwner);
    CFlog* flog = nullptr;
    if (sheep != nullptr)
    {
        flog = ObjectManager::FindGameObject<CFlog>();
    }

    // ランダムに「回転量」「移動距離」を作って、境界内に収まるまでリトライ
    for (int retry = 0; retry < MAX_RETRY; ++retry)
    {
        // 回転量[-180°, +180°] をランダムに選んでラジアンに変換
        m_turnAmount = Randomf(-TURN_ANGLE_DEG, TURN_ANGLE_DEG) * DegToRad;

        // 移動距離[1.0, 3.5] をランダムに選ぶ
        const float moveAmount = Randomf(MIN_MOVE, MAX_MOVE);

        m_targetPos = m_position + VECTOR3(0, 0,
                                           moveAmount) * XMMatrixRotationY(m_turnAmount);

        // Sheepの場合：Flogの範囲内かチェック
        if (flog != nullptr)
        {
            VECTOR3 toCenter = flog->GetFlockCenter() - m_targetPos;
            toCenter.y = 0;
            float distanceToCenter = sqrtf(toCenter.LengthSquare());
            if (distanceToCenter <= flog->GetFlockRadius())
            {
                return true;
            }
        }
        // それ以外のエネミーの場合、自身のAreaSizeチェック
        else if (IsInsideAreaXZ(m_targetPos, m_pOwner->GetAreaSize()))
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
    if (m_isFinish)return;
    if (m_pOwner->IsHuman())
    {
        m_pOwner->IsSuctionCheck();
    }
    if (m_path.empty())
    {
        static constexpr float ROTATION_LERP_SPEED = 10.0f;
        const float t = ROTATION_LERP_SPEED * SceneManager::DeltaTime();
        m_currentRotation = m_currentRotation + (m_targetRotation - m_currentRotation) * t;
        if (abs(m_targetRotation - m_currentRotation) < 0.01f)
        {
            m_isFinish = true;
        }
        m_pOwner->SetRotateY(ClampRotateY(m_currentRotation));
        return;
    }
    if (m_pathIndex >= static_cast<int>(m_path.size()))
    {
        m_isFinish = true;
        return;
    }
    const VECTOR2 nextPoint = m_path[m_pathIndex];
    const VECTOR3 nextPos = {nextPoint.x, m_position.y, nextPoint.y};

    // ウェイポイントの方向を向く
    VECTOR3 dir = nextPos - m_pOwner->GetTransform().position;
    dir.y = 0;
    const float targetAngle = atan2f(dir.x, dir.z);

    // 回転
    const float current = m_pOwner->GetTransform().rotation.y;
    const float t = 10.0f * SceneManager::DeltaTime();
    float angleDiff = targetAngle - current;
    angleDiff = ClampRotateY(angleDiff);
    const float newAngle = current + angleDiff * t;
    m_pOwner->SetRotateY(newAngle);

    // 移動
    VECTOR3 moveVec = VECTOR3(0, 0, m_moveSpeed * SceneManager::DeltaTime())
        * XMMatrixRotationY(newAngle);
    moveVec = m_pOwner->CalcSlideMove(moveVec);
    m_pOwner->AddPosition(moveVec);

    //目的地の計算
    VECTOR3 toTarget = nextPos - m_pOwner->GetTransform().position;
    toTarget.y = 0;
    //距離の計算
    const float disSq = toTarget.LengthSquare();
    const float cellSize = m_pathFinder.GetCellSize();
    const float reachDistSq = cellSize * 0.5f * cellSize * 0.5f;
    if (disSq < reachDistSq)
    {
        m_pathIndex++;
        if (m_pathIndex >= static_cast<int>(m_path.size()))
        {
            m_isFinish = true;
            return;
        }
        while (m_pathIndex < static_cast<int>(m_path.size()) - 1)
        {
            // 現在地 → 次のウェイポイントの方向
            VECTOR2 toNext = m_path[m_pathIndex] - ToVec2XZ(m_pOwner->GetTransform().position);
            // 現在地 → さらに先のウェイポイントの方向
            VECTOR2 toAfter = m_path[m_pathIndex + 1] - ToVec2XZ(m_pOwner->GetTransform().position);

            // 2つの方向の角度差が小さければ次をスキップ
            const float angleCos = dot(normalize(toNext), normalize(toAfter));
            const float threshold = cos(30.0f * DegToRad);
            if (angleCos > threshold) m_pathIndex++;
            else break;
        }
    }
}

float CWalk::ClampRotateY(float angle)
{
    return std::remainder(angle, XM_2PI);
}

void CWalk::AdjustTargetToFreeCell(CStageQuadTree* pTree, const VECTOR4& mapBounds, float cellSize)
{
    if (pTree == nullptr) return;
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
    while (!openQueue.empty())
    {
        Vec2Int cur = openQueue.front();
        openQueue.pop();
        const int ring = (std::max)(std::abs(cur.x - startIX), std::abs(cur.y - startIZ));
        if (ring > MAX_RING) continue;
        const VECTOR2 c = {static_cast<float>(cur.x) * cellSize,static_cast<float>(cur.y) * cellSize};
        const bool inMap =(c.x > mapBounds.x + half && c.x <mapBounds.z - half &&
                           c.y > mapBounds.y + half && c.y < mapBounds.w - half);
        if (!inMap) continue;
        const bool inArea = IsInsideAreaXZ(VECTOR3(c.x,0.0f,c.y),m_pOwner->GetAreaSize());
        VECTOR2 topLeft = c - half;
        VECTOR2 size2d = {cellSize,cellSize};
        const bool onObstacle = pTree->GetOverlappingObjects(topLeft, size2d).empty();
        if (onObstacle && inArea)
        {
            m_targetPos = VECTOR3(c.x, 0.0f, c.y);
            return;
        }
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
