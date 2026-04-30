#include "ShepherdDogComp.h"

#include <algorithm>
#include "../System/EnemyManager.h"
#include "../AnimalDog/ShepherdDog.h"
#include "../System/Flock.h"


CShepherdDogWalk::CShepherdDogWalk(CAShepherdDog* dog, float speed)
    : m_pOwner(dog), m_moveSpeed(speed)
{
}

void CShepherdDogWalk::Enter()
{
    m_isFinish = false;

    if (m_pOwner == nullptr || !CalcRandomMoveInRing())
    {
        m_isFinish = true;
        return;
    }

    m_pOwner->GetAnimator()->MergePlay(AnimationType::A_WALK);
    m_pOwner->GetAnimator()->SetPlaySpeed(1.0f);
}

void CShepherdDogWalk::Update()
{
    if (m_isFinish || m_pOwner == nullptr)
    {
        return;
    }

    //回転計算：目標方向に滑らかに向く（到着済みなら false で終了）//
    if (!TryRotateToward(m_targetPos))
    {
        return;
    }

    //移動計算：前方ベクトル＋スライド補正＋内側円の接線スライド//
    const float newAngle = m_pOwner->GetTransform().rotation.y;
    VECTOR3 moveVec = VECTOR3(0, 0, m_moveSpeed * SceneManager::DeltaTime()) * XMMatrixRotationY(newAngle);
    moveVec = m_pOwner->CalcSlideMove(moveVec);
    moveVec = SlideAlongInnerCircle(moveVec);

    //移動先が許容範囲外なら WALK を再エントリしてやり直す//
    const VECTOR3 nextPos = m_pOwner->GetTransform().position + moveVec;
    if (!CanMoveTo(nextPos))
    {
        m_pOwner->ChangeState(CBaseState::State::WALK);
        m_isFinish = true;
        return;
    }

    m_pOwner->AddPosition(moveVec);
    m_pOwner->IsSuctionCheck();
}

bool CShepherdDogWalk::TryRotateToward(const VECTOR3& targetPos)
{
    VECTOR3 direction = targetPos - m_pOwner->GetTransform().position;
    direction.y = 0.0f;

    constexpr float ARRIVAL_DISTANCE = 0.5f;
    if (direction.LengthSquare() < Pow2(ARRIVAL_DISTANCE))
    {
        m_isFinish = true;
        return false;
    }

    direction = normalize(direction);
    const float targetAngle = atan2f(direction.x, direction.z);
    const float currentAngle = m_pOwner->GetTransform().rotation.y;

    float angleDiff = std::remainder(targetAngle - currentAngle, XM_2PI);

    constexpr float ROTATION_SPEED = 6.0f;
    const float newAngle = currentAngle + angleDiff * ROTATION_SPEED * SceneManager::DeltaTime();
    m_pOwner->SetRotateY(newAngle);
    return true;
}

VECTOR3 CShepherdDogWalk::SlideAlongInnerCircle(const VECTOR3& moveVec) const
{
    //内側円に侵入しないなら補正不要でそのまま返す//
    const VECTOR3 currentPos = m_pOwner->GetTransform().position;
    const VECTOR3 nextPos = currentPos + moveVec;
    if (!IsInsideInnerCircle(nextPos))
    {
        return moveVec;
    }

    //中心ぴったりに居る稀ケースは方向が決まらないので補正できない//
    CFlock* flock = m_pOwner->GetFlock();
    VECTOR3 toCurrent = currentPos - flock->GetFlockCenter();
    toCurrent.y = 0.0f;
    if (toCurrent.LengthSquare() < NEAR_ZERO_LENSQ)
    {
        return moveVec;
    }

    //中心方向への成分を除去して接線方向だけ残す//
    const VECTOR3 radialDir = normalize(toCurrent);
    VECTOR3 slid = moveVec;
    const float radialComponent = dot(slid, radialDir);
    if (radialComponent < 0.0f)
    {
        slid -= radialDir * radialComponent;
    }

    //接線成分だけにしてもまだ内側に残るなら境界外へ押し出す//
    if (IsInsideInnerCircle(currentPos + slid))
    {
        constexpr float BOUNDARY_EPSILON = 0.01f;
        const float pushRadius = flock->GetFlockRadius() + BOUNDARY_EPSILON;
        VECTOR3 boundaryPos = flock->GetFlockCenter() + radialDir * pushRadius;
        boundaryPos.y = currentPos.y;
        slid = boundaryPos - currentPos;
    }
    return slid;
}

bool CShepherdDogWalk::CalcRandomMoveInRing()
{
    CFlock* flock = m_pOwner->GetFlock();
    if (flock == nullptr) return false;

    //群れの内側円に入らないよう、外側リングの半径範囲を作る//
    constexpr float MARGIN = 0.5f;

    float minRadius = flock->GetFlockRadius() + MARGIN;
    float maxRadius = flock->GetMoveRadius() - MARGIN;

    maxRadius = (std::max)(maxRadius, minRadius);

    //極座標でリング内のランダムな目標位置を決定する//
    const float angle = Randomf(0.0f, XM_2PI);
    const float dist = Randomf(minRadius, maxRadius);

    m_targetPos = flock->GetFlockCenter() + VECTOR3(
        cosf(angle) * dist,
        0.0f,
        sinf(angle) * dist
    );

    return true;
}

bool CShepherdDogWalk::CanMoveTo(const VECTOR3& nextPos) const
{
    CFlock* flock = m_pOwner->GetFlock();
    if (flock == nullptr) return true;

    //内側円へ侵入する移動は不可//
    if (flock->ContainPos(nextPos))
    {
        return false;
    }

    //外側の移動許容範囲を超える移動も不可//
    if (!flock->ContainMoveArea(nextPos))
    {
        return false;
    }

    return true;
}

bool CShepherdDogWalk::IsInsideInnerCircle(const VECTOR3& nextPos) const
{
    CFlock* flock = m_pOwner->GetFlock();
    if (flock == nullptr)
    {
        return false;
    }

    return flock->ContainPos(nextPos);
}

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
    m_pOwner->GetAnimator()->SetPlaySpeed(2.0f);
    RecomputePath();
}


void CCollecting::RecomputePath()
{
    //回収に必要な群れ・対象羊・目標位置を再評価する//
    if (m_pOwner->GetFlock() == nullptr)
    {
        m_isFinish = true;
        return;
    }

    CSheep* targetSheep = FindFurthestStraySheep();
    if (targetSheep == nullptr)
    {
        m_isFinish = true;
        return;
    }

    if (!CalcCollectTargetPos(targetSheep))
    {
        m_isFinish = true;
        return;
    }
    m_targetSheep = targetSheep;

    BuildPath();

    VECTOR3 toTarget = m_targetPos - m_pOwner->GetTransform().position;
    toTarget.y = 0;
    if (toTarget.LengthSquare() < Pow2(1.0f))
    {
        FinishAndHerdTarget();
    }
}

CSheep* CCollecting::FindFurthestStraySheep() const
{
    CFlock* flock = m_pOwner->GetFlock();
    const VECTOR3 flockCenter = flock->GetFlockCenter();

    CSheep* result = nullptr;
    float maxDisSq = 0.0f;
    //群れ内にいる羊は除外し、最も遠い羊を線形探索//
    for (CSheep* s : m_pOwner->GetSheeps())
    {
        if (s == nullptr) continue;
        if (flock->ContainPos(s->GetTransform().position)) continue;

        VECTOR3 diff = flockCenter - s->GetTransform().position;
        diff.y = 0;
        const float disSq = diff.LengthSquare();
        if (disSq > maxDisSq)
        {
            maxDisSq = disSq;
            result = s;
        }
    }
    return result;
}

bool CCollecting::CalcCollectTargetPos(CSheep* targetSheep)
{
    CFlock* flock = m_pOwner->GetFlock();

    //羊から群れ中心へのベクトルを取り、距離と方向を求める//
    const VECTOR3 sheepPos = targetSheep->GetTransform().position;
    VECTOR3 toCentroid = flock->GetFlockCenter() - sheepPos;
    toCentroid.y = 0;
    const float disFromCenterSq = toCentroid.LengthSquare();
    if (disFromCenterSq < NEAR_ZERO_LENSQ) return false;

    toCentroid = normalize(toCentroid);
    constexpr float SMALL_OUTSIDE_MARGIN = 3.0f;
    const float outsideThreshold = flock->GetFlockRadius() + SMALL_OUTSIDE_MARGIN;

    //はぐれ具合で「手前へ近づく」か「背後へ回る」かを切り替える//
    if (disFromCenterSq <= outsideThreshold * outsideThreshold)
    {
        // 少し外れただけなら、犬は羊の近くへ行くだけ
        constexpr float APPROACH_DIS = 1.5f;
        m_targetPos = sheepPos - toCentroid * APPROACH_DIS;
    }
    else
    {
        // 大きく外れたなら、犬は背後へ回って群れ方向へ押し戻す
        constexpr float BEHIND_DIS = 2.0f;
        m_targetPos = sheepPos - toCentroid * BEHIND_DIS;
    }
    return true;
}

void CCollecting::BuildPath()
{
    //A*用に犬の当たりサイズを設定し、現在位置から目標位置までの経路を作る//
    VECTOR2 pos, size;
    m_pOwner->GetBounds2D(pos, size);
    m_pathFinder.SetAgentSize(size);

    const VECTOR2 start = ToVec2XZ(m_pOwner->GetTransform().position);
    const VECTOR2 end = ToVec2XZ(m_targetPos);
    m_pathIndex = 1;
    m_path = m_pathFinder.SearchRoute(start, end);
}

void CCollecting::FinishAndHerdTarget()
{
    if (m_targetSheep != nullptr)
    {
        m_targetSheep->ChangeState(CBaseState::State::HERDED);
    }
    m_isFinish = true;
}

void CCollecting::Update()
{
    if (m_isFinish) return;

    //一定間隔ではぐれ羊を再選定して経路を更新//
    m_repathTimer += SceneManager::DeltaTime();
    if (m_repathTimer >= REPATH_INTERVAL)
    {
        m_repathTimer = 0.0f;
        RecomputePath();
        if (m_isFinish) return;
    }

    //経路が空・最後まで到達していたら完了処理//
    if (m_path.empty() || m_pathIndex >= static_cast<int>(m_path.size()))
    {
        FinishAndHerdTarget();
        return;
    }

    const VECTOR2 nextPoint = m_path[m_pathIndex];
    const VECTOR3 currentPos = m_pOwner->GetTransform().position;
    const VECTOR3 nextPos = {nextPoint.x, currentPos.y, nextPoint.y};

    //回転計算：次ウェイポイント方向へ角度補間//
    VECTOR3 direction = nextPos - currentPos;
    direction.y = 0;
    const float targetAngle = atan2f(direction.x, direction.z);
    const float current = m_pOwner->GetTransform().rotation.y;
    const float t = 3.0f * SceneManager::DeltaTime();
    const float angleDiff = std::remainder(targetAngle - current, XM_2PI);
    const float newAngle = current + angleDiff * t;
    m_pOwner->SetRotateY(newAngle);

    //移動計算：前方ベクトル＋スライド補正で移動を適用//
    VECTOR3 moveVec = VECTOR3(0, 0, m_moveSpeed * SceneManager::DeltaTime())
        * XMMatrixRotationY(newAngle);
    moveVec = m_pOwner->CalcSlideMove(moveVec);
    m_pOwner->AddPosition(moveVec);

    //到達判定→次ポイントへ//
    VECTOR3 toTarget = nextPos - m_pOwner->GetTransform().position;
    toTarget.y = 0;
    const float cellSize = m_pathFinder.GetCellSize();
    const float reachDistSq = Pow2(cellSize * 0.5f);
    if (toTarget.LengthSquare() < reachDistSq)
    {
        AdvancePathIndex();
    }
}

void CCollecting::AdvancePathIndex()
{
    m_pathIndex++;
    if (m_pathIndex >= static_cast<int>(m_path.size()))
    {
        FinishAndHerdTarget();
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


CDriving::CDriving(CAShepherdDog* dog, float speed)
    : m_moveSpeed(speed)
{
    m_pOwner = dog;
}

void CDriving::Enter()
{
    m_isFinish = false;

    //群れの重心と UFO の位置を取得//
    FlockInfo info = CFlock::CalCFlockInfoStatic(m_pOwner->GetSheeps());
    CPlayer* player = ObjectManager::FindGameObject<CPlayer>();
    if (player == nullptr) return;

    VECTOR3 ufoPos = player->GetTransform().position;
    VECTOR3 centroid = info.centroid;

    //UFO→群れの方向ベクトル（＝群れの逃げる向き）//
    VECTOR3 escapeDir = centroid - ufoPos;
    escapeDir.y = 0;

    //ゼロベクトル対策：UFOと群れが同じ位置ならデフォルト方向//
    float lengthSq = escapeDir.LengthSquare();
    if (lengthSq < 0.0001f)
    {
        escapeDir = VECTOR3(0, 0, 1); // デフォルト：Z+方向
    }
    else
    {
        escapeDir = escapeDir / sqrtf(lengthSq); // 正規化
    }

    //群れの後ろ（UFOから見て群れの向こう側）に回り込む位置を目標にする//
    float pushDistance = 5.0f;
    m_targetPos = centroid + escapeDir * pushDistance;
}

void CDriving::Update()
{
    //目標位置までの方向と距離を計算//
    const VECTOR3 currentPos = m_pOwner->GetTransform().position;
    VECTOR3 direction = m_targetPos - currentPos;
    direction.y = 0;
    const float distanceSq = direction.LengthSquare();

    //目標に到着したら終了//
    constexpr float arrivalThresholdSq = 0.25f;
    if (distanceSq < arrivalThresholdSq)
    {
        m_isFinish = true;
        return;
    }

    direction = normalize(direction);

    //回転計算：移動方向にスナップ（補間なし）//
    float targetAngle = atan2f(direction.x, direction.z);
    m_pOwner->SetRotateY(targetAngle);

    //移動を適用//
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
    FlockInfo info = CFlock::CalCFlockInfoStatic(m_pOwner->GetSheeps());
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

    //現在フェーズに応じて、羊へ接近するか重心方向へ誘導する//
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
