#include "SheperdDogComp.h"
#include "../System/EnemyManager.h"
#include "../AnimalDog//ShepherdDog.h"  // パスは適宜修正してください
#include "../System/Flog.h"


CCollecting::CCollecting(CAShepherdDog* dog, float speed)
    : m_moveSpeed(speed)
{
    m_pOwner = dog;
}

void CCollecting::Enter()
{
    m_isFinish = false;
    m_repathTimer = 0.0f;
    m_pOwner->GetAnimator()->MergePlay(AnimationType::A_WALK);
    m_pOwner->GetAnimator()->SetPlaySpeed(4.0f);
    RecomputePath();
}


void CCollecting::RecomputePath()
{
    CFlog* flog = m_pOwner->GetFlog();
    if (flog == nullptr)
    {
        m_isFinish = true;
        return;
    }

    //はぐれ羊を再選定
    CSheep* targetSheep = nullptr;
    float maxDisSq = 0.0f;
    const VECTOR3 flogCenter = flog->GetFlockCenter();
    for (CSheep* s : m_pOwner->GetSheeps())
    {
        if (s == nullptr) continue;
        if (flog->ContainPos(s->GetTransform().position)) continue;
        VECTOR3 diff = flogCenter - s->GetTransform().position;
        diff.y = 0;
        float disSq = diff.LengthSquare();
        if (disSq > maxDisSq)
        {
            maxDisSq = disSq;
            targetSheep = s;
        }
    }
    if (targetSheep == nullptr)
    {
        m_isFinish = true;
        return;
    }

    //群れの中心と逆方向に 1m 離れた地点を目標にする
    const VECTOR3 sheepPos = targetSheep->GetTransform().position;
    VECTOR3 toCentroid = flogCenter - sheepPos;
    toCentroid.y = 0;
    if (toCentroid.LengthSquare() < NEAR_ZERO_LENSQ)
    {
        m_isFinish = true;
        return;
    }
    toCentroid = normalize(toCentroid);
    constexpr float BEHIND_DIS = 2.0f;
    const VECTOR3 behindPos = sheepPos - toCentroid * BEHIND_DIS;
    m_targetPos = behindPos;

    // A* で経路計算
    VECTOR2 pos, size;
    m_pOwner->GetBounds2D(pos, size);
    m_pathFinder.SetAgentSize(size);
    const VECTOR2 start = ToVec2XZ(m_pOwner->GetTransform().position);
    const VECTOR2 end = ToVec2XZ(behindPos);
    m_pathIndex = 1;
    m_path = m_pathFinder.SearchRoute(start, end);

    targetSheep->ChangeState(CBaseState::State::HERDED);
}

void CCollecting::Update()
{
    if (m_isFinish) return;

    m_repathTimer += SceneManager::DeltaTime();
    if (m_repathTimer >= REPATH_INTERVAL)
    {
        m_repathTimer = 0.0f;
        RecomputePath();
        if (m_isFinish) return;
    }
    if (m_path.empty() || m_pathIndex >=
        static_cast<int>(m_path.size()))
    {
        m_isFinish = true;
        return;
    }
    const VECTOR2 nextPoint = m_path[m_pathIndex];
    const VECTOR3 currentPos = m_pOwner->GetTransform().position;
    const VECTOR3 nextPos = {nextPoint.x, currentPos.y, nextPoint.y};
    // 目標位置へ移動

    VECTOR3 direction = nextPos - currentPos;
    direction.y = 0;
    const float targetAngle = atan2f(direction.x, direction.z);

    const float current = m_pOwner->GetTransform().rotation.y;
    const float t = 3.0f * SceneManager::DeltaTime();
    float angleDiff = targetAngle - current;
    angleDiff = std::remainder(angleDiff, XM_2PI);
    const float newAngle = current + angleDiff * t;
    m_pOwner->SetRotateY(newAngle);

    VECTOR3 moveVec = VECTOR3(0, 0, m_moveSpeed * SceneManager::DeltaTime())
        * XMMatrixRotationY(newAngle);
    moveVec = m_pOwner->CalcSlideMove(moveVec);
    m_pOwner->AddPosition(moveVec);

    VECTOR3 toTarget = nextPos - m_pOwner->GetTransform().position;
    toTarget.y = 0;
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
            VECTOR2 toNext = m_path[m_pathIndex] - ToVec2XZ(m_pOwner->GetTransform().position);
            VECTOR2 toAfter = m_path[m_pathIndex + 1] - ToVec2XZ(m_pOwner->GetTransform().position);
            const float angleCos = dot(normalize(toNext), normalize(toAfter));
            const float threshold = cos(30.0f * DegToRad);
            if (angleCos > threshold) m_pathIndex++;
            else break;
        }
    }
}


CDriving::CDriving(CAShepherdDog* dog, float speed)
    : m_moveSpeed(speed)
{
    m_pOwner = dog;
}

void CDriving::Enter()
{
    m_isFinish = false;

    // デバッグ・バランスの数を把握
    size_t sheepCount = m_pOwner->GetSheeps().size();

    FlogInfo info = CFlog::CalcFlogInfoStatic(m_pOwner->GetSheeps());
    CPlayer* player = ObjectManager::FindGameObject<CPlayer>();
    if (player == nullptr) return;

    VECTOR3 ufoPos = player->GetTransform().position;
    VECTOR3 centroid = info.centroid;

    // UFOから群れへの方向（逃げる方向）
    VECTOR3 escapeDir = centroid - ufoPos;
    escapeDir.y = 0;

    // ゼロベクトル対策（UFOと群れが同じ位置ならデフォルト方向）
    float lengthSq = escapeDir.LengthSquare();
    if (lengthSq < 0.0001f)
    {
        escapeDir = VECTOR3(0, 0, 1); // デフォルト：Z+方向
    }
    else
    {
        escapeDir = escapeDir / sqrtf(lengthSq); // 正規化
    }

    // 群れの後ろ（UFOから見て群れの向こう側）に回り込む
    float pushDistance = 5.0f;
    m_targetPos = centroid + escapeDir * pushDistance;
}

void CDriving::Update()
{
    // 目標位置へ移動
    const VECTOR3 currentPos = m_pOwner->GetTransform().position;
    VECTOR3 direction = m_targetPos - currentPos;
    direction.y = 0;
    const float distanceSq = direction.LengthSquare();

    constexpr float arrivalThresholdSq = 0.25f;
    // 目標に到着したら終了
    if (distanceSq < arrivalThresholdSq)
    {
        m_isFinish = true;
        return;
    }

    direction = normalize(direction);

    // 移動方向に回転
    float targetAngle = atan2f(direction.x, direction.z);
    m_pOwner->SetRotateY(targetAngle);

    m_pOwner->AddPosition(direction * m_moveSpeed * SceneManager::DeltaTime());
}

CRescue::CRescue(CAShepherdDog* dog)
{
    m_pOwner = dog;
    m_phase = Phase::APPROACH_SHEEP;
}

void CRescue::Enter()
{
    m_isFinish = false;
    m_pOwner->GetAnimator()->MergePlay(AnimationType::A_RUN);
    m_pOwner->GetAnimator()->SetPlaySpeed(1.5f);

    // 救出キューから対象を取得
    if (m_pOwner->GetRescueQueue().empty())
    {
        m_isFinish = true;
        return;
    }

    m_targetSheep = m_pOwner->GetRescueQueue().front();

    // 群れの重心を計算
    FlogInfo info = CFlog::CalcFlogInfoStatic(m_pOwner->GetSheeps());
    m_centroid = info.centroid;

    // フェーズ1: 羊に近づくところから開始
    m_phase = Phase::APPROACH_SHEEP;
}

void CRescue::Update()
{
    if (m_targetSheep == nullptr)
    {
        m_isFinish = true;
        return;
    }

    const VECTOR3 myPos = m_pOwner->GetTransform().position;
    const VECTOR3 sheepPos = m_targetSheep->GetTransform().position;
    constexpr float MOVE_SPEED = 4.0f;

    switch (m_phase)
    {
    case Phase::APPROACH_SHEEP:
        {
            // 羊への方向
            VECTOR3 toSheep = sheepPos - myPos;
            const float distance = toSheep.LengthSquare();
            constexpr float ARRIVAL_DIS = 1.0f; // 重心到着判定距離

            // 羊の近くに到着したらフェーズ2へ
            if (distance < Pow2(ARRIVAL_DIS))
            {
                m_phase = Phase::GUIDE_TO_CENTER;
                break;
            }

            // 羊に近づく
            toSheep = normalize(toSheep);
            // 移動方向に回転
            float targetAngle = atan2f(toSheep.x, toSheep.z);
            m_pOwner->SetRotateY(targetAngle);
            m_pOwner->AddPosition(toSheep * MOVE_SPEED * SceneManager::DeltaTime());
        }
        break;

    case Phase::GUIDE_TO_CENTER:
        {
            // 羊から重心への方向
            VECTOR3 toCentroid = m_centroid - sheepPos;
            toCentroid.y = 0;
            const float distance = toCentroid.LengthSquare();

            // 重心に到着したら完了
            constexpr float m_approachDistance = 2.0f; // 羊に近づく距離`
            if (distance < Pow2(m_approachDistance))
            {
                m_pOwner->PopRescueQueue(); // キューから削除
                m_isFinish = true;
                break;
            }

            // 羊の背後から重心方向へプレッシャーをかける位置
            toCentroid = normalize(toCentroid);
            static constexpr float m_behindDistance = 2.0f; // 羊の背後に回り込む距離
            const VECTOR3 behindPos = sheepPos - toCentroid * m_behindDistance;

            // その位置へ移動
            VECTOR3 toBehind = behindPos - myPos;
            toBehind.y = 0;
            toBehind = normalize(toBehind);
            // 移動方向に回転
            float targetAngle = atan2f(toBehind.x, toBehind.z);
            m_pOwner->SetRotateY(targetAngle);
            m_pOwner->AddPosition(toBehind * MOVE_SPEED * SceneManager::DeltaTime());
        }
        break;
    }
}


CDestroyShepherdDog::CDestroyShepherdDog(CAShepherdDog* dog, int score, float exp)
    : CDestroy(dog, score, exp)
{
    m_pDog = dog;
}

void CDestroyShepherdDog::Enter()
{
    // 羊達をPANIC状態に
    for (auto sheep : m_pDog->GetSheeps())
    {
        if (sheep != nullptr)
        {
            sheep->ChangeState(CBaseState::State::PANIC);
        }
    }

    // 親のDestroy処理
    CDestroy::Enter();
}
