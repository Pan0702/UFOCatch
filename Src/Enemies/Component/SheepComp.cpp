#include "SheepComp.h"
#include "../System/Flock.h"
#include "../System/EnemyManager.h"

CHerded::CHerded(CSheep* sheep)
    : m_wanderTimer(0.0f), m_walkTimer(0.0f), m_walkDuration(0.0f), m_currentRotation(0.0f)
{
    m_pOwner = sheep;
    m_pPlayer = ObjectManager::FindGameObject<CPlayer>();

    // 初期のWander方向をランダムに設定
    float randomAngle = Randomf(0.0f, XM_2PI);
    m_wanderTarget = VECTOR3(sinf(randomAngle), 0, cosf(randomAngle));
}

void CHerded::Enter()
{
    m_isFinish = false;
    m_pOwner->GetAnimator()->MergePlay(AnimationType::A_WALK);

    // 状態に入るたびにWander方向をリセット
    float randomAngle = Randomf(0.0f, XM_2PI);
    m_wanderTarget = VECTOR3(sinf(randomAngle), 0, cosf(randomAngle));
    m_wanderTimer = 0.0f;

    // 移動時間をランダムに設定（3～7秒）
    m_walkDuration = Randomf(3.0f, 7.0f);
    m_walkTimer = 0.0f;

    // 現在の回転を保存
    m_currentRotation = m_pOwner->GetTransform().rotation.y;
}

void CHerded::Update()
{
    m_walkTimer += SceneManager::DeltaTime();

    //群れの十分内側まで戻れたら通常待機に戻す//
    if (IsDeepInsideFlock())
    {
        m_isFinish = true;
        m_pOwner->ChangeState(CBaseState::State::IDLE);
        return;
    }

    // 一定時間経過したら徘徊時間をリフレッシュ
    if (m_walkTimer >= m_walkDuration)
    {
        m_walkTimer = 0.0f;
        m_walkDuration = Randomf(3.0f, 7.0f);
    }

    const VECTOR3 totalForce = CalcRetrunToFlock() + CalcBoids();
    ApplyMovement(totalForce);
    m_pOwner->IsSuctionCheck();
}

bool CHerded::IsDeepInsideFlock() const
{
    CFlock* flock = m_pOwner->GetFlock();
    if (flock == nullptr) return false;

    VECTOR3 diff = flock->GetFlockCenter() - m_pOwner->GetTransform().position;
    diff.y = 0;
    constexpr float INNER_RADIUS_RATIO = 0.75f;
    const float innerRadius = flock->GetFlockRadius() * INNER_RADIUS_RATIO;
    return diff.LengthSquare() < innerRadius * innerRadius;
}

void CHerded::ApplyMovement(const VECTOR3& force)
{
    if (force.LengthSquare() <= NEAR_ZERO_LENSQ) return;

    //回転計算：力の向きに向かって現在角度を滑らかに近づける//
    const VECTOR3 dir = normalize(force);
    const float targetAngle = atan2f(dir.x, dir.z);

    constexpr float rotationSpeed = 3.0f;
    const float angleDiff = std::remainder(targetAngle - m_currentRotation, XM_2PI);
    const float rotationDelta = angleDiff * rotationSpeed * SceneManager::DeltaTime();
    if (fabsf(angleDiff) < fabsf(rotationDelta))
    {
        m_currentRotation = targetAngle;
    }
    else
    {
        m_currentRotation += rotationDelta;
    }
    m_pOwner->SetRotateY(m_currentRotation);

    //移動計算：合成力の方向へ一定速度で進める//
    constexpr float moveSpeed = 1.5f;
    m_pOwner->AddPosition(dir * moveSpeed * SceneManager::DeltaTime());
}

VECTOR3 CHerded::CalcBoids() const
{
    CFlock* flock = m_pOwner->GetFlock();
    if (flock == nullptr) return {0, 0, 0};
    const std::vector<CSheep*>& allSheep = flock->GetAllSheeps();

    VECTOR3 cohesion(0, 0, 0);
    VECTOR3 separation(0, 0, 0);

    int neighborCount = 0;
    int separationCount = 0;

    const VECTOR3 myPos = m_pOwner->GetTransform().position;

    // UFOが吸い込み中かどうかで凝集の強さを変える
    bool isSucking = false;
    if (m_pPlayer != nullptr)
    {
        isSucking = m_pPlayer->GetIsSuckUp();
    }

    constexpr float neighborRadius = 30.0f; // 仲間と認識する距離（ストロングボム: 30m）
    constexpr float separationRadius = 2.0f; // 近すぎると判定する距離

    const float neighborRadiusSq = Pow2(neighborRadius);
    const float separationRadiusSq = Pow2(separationRadius);

    //全ての羊を走査し、近隣の平均位置と近すぎる羊から離れる方向を集計する//
    for (const auto& other : allSheep)
    {
        if (other == m_pOwner) continue;

        const VECTOR3 otherPos = other->GetTransform().position;
        VECTOR3 diff = otherPos - myPos;
        const float distanceSq = diff.LengthSquare();
        diff.y = 0;
        // 近隣の仲間
        if (distanceSq < neighborRadiusSq)
        {
            cohesion += otherPos;
            neighborCount++;
        }

        // 近すぎる仲間から離れる
        if (distanceSq < separationRadiusSq && distanceSq > 0.0001f)
        {
            float distance = sqrtf(distanceSq);
            separation -= diff / distance;
            separationCount++;
        }
    }

    //凝集計算：近隣の平均位置へ向かう方向に重みをかける//
    if (neighborCount > 0)
    {
        cohesion = cohesion / static_cast<float>(neighborCount);
        cohesion -= myPos;
        cohesion.y = 0;
        if (cohesion.LengthSquare() > NEAR_ZERO_LENSQ)
        {
            cohesion = normalize(cohesion);
            float cohesionWeight;
            if (isSucking)
            {
                cohesionWeight = 0.7f;
            }
            else if (m_pOwner->GetFlock() && !m_pOwner->GetFlock()->ContainPos(myPos))
            {
                cohesionWeight = 0.5f;
            }
            else
            {
                cohesionWeight = 0.1f;
            }
            cohesion *= cohesionWeight;
        }
    }

    //分離計算：近すぎる羊がいれば離れる方向を強める//
    if (separationCount > 0 && separation.LengthSquare() > NEAR_ZERO_LENSQ)
    {
        separation = normalize(separation);
        constexpr float separationWeight = 2.0f; // 分離を強めて、近づきすぎを防ぐ
        separation *= separationWeight;
    }

    return cohesion + separation;
}

VECTOR3 CHerded::CalcEscapeFromDog() const
{
    CFlock* flock = m_pOwner->GetFlock();
    if (flock == nullptr) return {0, 0, 0};
    CAShepherdDog* dog = flock->GetShepherdDog();
    if (dog == nullptr) return {0, 0, 0};

    const VECTOR3 dogPos = dog->GetTransform().position;
    const VECTOR3 myPos = m_pOwner->GetTransform().position;

    VECTOR3 diff = myPos - dogPos;
    diff.y = 0;
    const float distanceSq = diff.LengthSquare();

    // 犬を感知する距離（ストロングボムモデル: 50m）
    constexpr float detectionRadiusSq = 5.0f * 5.0f;

    // 犬が感知範囲内にいる場合のみ逃げる
    if (distanceSq < detectionRadiusSq && distanceSq > NEAR_ZERO_LENSQ)
    {
        diff = normalize(diff);
        constexpr float dogEscapeWeight = 2.5f;
        return diff * dogEscapeWeight;
    }

    return {0, 0, 0};
}


VECTOR3 CHerded::CalcWandering()
{
    // 一定時間ごとにランダムな方向を変更
    m_wanderTimer += SceneManager::DeltaTime();

    constexpr float changeDirectionInterval = 3.0f; // 3秒ごとに方向変更
    if (m_wanderTimer >= changeDirectionInterval)
    {
        // ランダムな角度を生成（現在の方向から±60度の範囲）
        float currentAngle = atan2f(m_wanderTarget.x, m_wanderTarget.z);
        float randomOffset = Randomf(-XM_PI / 3.0f, XM_PI / 3.0f); // ±60度
        float newAngle = currentAngle + randomOffset;

        m_wanderTarget = VECTOR3(sinf(newAngle), 0, cosf(newAngle));
        m_wanderTimer = 0.0f;
    }

    // Wander方向に正規化された力を返す
    VECTOR3 wanderForce = m_wanderTarget;
    if (wanderForce.LengthSquare() > NEAR_ZERO_LENSQ)
    {
        wanderForce = normalize(wanderForce);
        constexpr float wanderWeight = 1.0f; // Wanderの強さ
        return wanderForce * wanderWeight;
    }

    return {0, 0, 0};
}

VECTOR3 CHerded::CalcRetrunToFlock()
{
    CFlock* flock = m_pOwner->GetFlock();
    if (flock == nullptr) return {0, 0, 0};

    const VECTOR3 sheepPos = m_pOwner->GetTransform().position;
    VECTOR3 toCenter = flock->GetFlockCenter() - sheepPos;
    toCenter.y = 0;

    const float disSq = toCenter.LengthSquare();
    if (disSq < NEAR_ZERO_LENSQ) return {0, 0, 0};

    const float dis = sqrtf(disSq);
    const float radius = flock->GetFlockRadius();

    constexpr float OUTSIDE_FORCE = 3.0f;
    constexpr float EDGE_RATE = 0.75f;
    constexpr float EDGE_FORCE = 1.2f;
    //群れの外なら強く、端付近なら弱く中心方向へ戻す//
    if (dis > radius) return normalize(toCenter) * OUTSIDE_FORCE;
    if (dis > radius * EDGE_RATE)return normalize(toCenter) * EDGE_FORCE;
    return {0, 0, 0};
}


CPanic::CPanic(CSheep* sheep)
{
    m_pOwner = sheep;
    m_panicDirection = VECTOR3(0, 0, 0);
}

void CPanic::Enter()
{
    m_isFinish = false;

    m_panicTimer = 0.0f;
    m_panicDuration = Randomf(4.0f, 7.0f);
    m_startPos = m_pOwner->GetTransform().position;
    // ランダムな方向を決定
    const float randomAngle = Randomf(0.0f, XM_2PI);
    m_panicDirection = VECTOR3(sinf(randomAngle), 0, cosf(randomAngle));

    m_changeDirectionTimer = 0.0f;

    // アニメーションを走りに変更
    m_pOwner->GetAnimator()->MergePlay(A_WALK);
    m_pOwner->GetAnimator()->SetPlaySpeed(3.0f); // 速めに再生
}

void CPanic::Update()
{
    m_panicTimer += SceneManager::DeltaTime();
    m_changeDirectionTimer += SceneManager::DeltaTime();

    //方向更新、移動、終了後の所属群れ判定を順に処理する//
    UpdateDirection();
    UpdateMovement();
    CheckBoundaryAndTransition();
    m_pOwner->IsSuctionCheck();
}

void CPanic::UpdateDirection()
{
    constexpr float changeDirectionInterval = 2.0f;
    if (m_changeDirectionTimer >= changeDirectionInterval)
    {
        const float randomAngle = Randomf(0.0f, XM_2PI);
        m_panicDirection = VECTOR3(sinf(randomAngle), 0, cosf(randomAngle));
        m_changeDirectionTimer = 0.0f;
    }
}

void CPanic::UpdateMovement() const
{
    //回転計算：パニック方向へ即座に向きを合わせる//
    float targetAngle = atan2f(m_panicDirection.x, m_panicDirection.z);
    m_pOwner->SetRotateY(targetAngle);

    //移動計算：決定済みの逃走方向へ一定速度で進む//
    constexpr float MOVE_SPEED = 2.0f;
    m_pOwner->AddPosition(m_panicDirection * MOVE_SPEED * SceneManager::DeltaTime());
}

void CPanic::CheckBoundaryAndTransition()
{
    if (m_panicTimer < m_panicDuration) return;

    const VECTOR3 currentPos = m_pOwner->GetTransform().position;
    CFlock* nearest = nullptr;
    float nearestDistSq = FLT_MAX;

    //現在位置を含む群れの中から、最も中心が近い群れを探す//
    for (CFlock* f : ObjectManager::FindGameObjects<CFlock>())
    {
        if (!f->ContainPos(currentPos)) continue;
        VECTOR3 diff = f->GetFlockCenter() - currentPos;
        diff.y = 0;
        float distSq = diff.LengthSquare();
        if (distSq < nearestDistSq)
        {
            nearestDistSq = distSq;
            nearest = f;
        }
    }

    //群れ内に戻っていればHERDED、どの群れにもいなければIDLEに戻す//
    if (nearest != nullptr)
    {
        m_pOwner->SetFlock(nearest);

        m_pOwner->ChangeState(CBaseState::State::HERDED);
    }
    else
    {
        m_pOwner->SetFlock(nullptr);

        m_pOwner->ChangeState(CBaseState::State::IDLE);
    }
    m_isFinish = true;
}
