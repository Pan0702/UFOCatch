#include "SheepComp.h"
#include "../System/Flog.h"
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
    // 移動時間をカウント
    m_walkTimer += SceneManager::DeltaTime();

    // 一定時間経過したらIDLEに戻る
    if (m_walkTimer >= m_walkDuration)
    {
        m_isFinish = true;
        m_pOwner->ChangeState(CBaseState::State::IDLE);
        return;
    }

    const VECTOR3 boidsForce = CalculateBoids();
    const VECTOR3 wanderForce = CalculateWandering();

    // 力を合成（Wandering優先で自由な動き）
    VECTOR3 totalForce = wanderForce + boidsForce + CalculateEscapeFromDog();
    totalForce.y = 0;

    if (totalForce.LengthSquare() > 0.0001f)
    {
        totalForce = normalize(totalForce);

        // 目標の回転角度を計算
        float targetAngle = atan2f(totalForce.x, totalForce.z);

        // 現在の回転から目標の回転へ滑らかに補間（回転速度: 3.0 rad/s）
        constexpr float rotationSpeed = 3.0f;
        float angleDiff = targetAngle - m_currentRotation;

        // 角度を-π～πの範囲に正規化
        while (angleDiff > XM_PI) angleDiff -= XM_2PI;
        while (angleDiff < -XM_PI) angleDiff += XM_2PI;

        // 回転を補間
        float rotationDelta = angleDiff * rotationSpeed * SceneManager::DeltaTime();
        if (fabsf(angleDiff) < fabsf(rotationDelta))
        {
            m_currentRotation = targetAngle;
        }
        else
        {
            m_currentRotation += rotationDelta;
        }

        // 回転を適用
        m_pOwner->SetRotateY(m_currentRotation);

        // 移動
        const float moveSpeed = 1.5f;
        m_pOwner->AddPosition(totalForce * moveSpeed * SceneManager::DeltaTime());
    }
    m_pOwner->IsSuctionCheck();
}

VECTOR3 CHerded::CalculateBoids() const
{
    CFlog* flog = m_pOwner->GetFlog();
    if (flog == nullptr) return {0, 0, 0};
    const std::vector<CSheep*>& allSheep = flog->GetAllSheeps();

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

    // 平均化
    if (neighborCount > 0)
    {
        cohesion = cohesion / static_cast<float>(neighborCount);
        cohesion -= myPos;
        cohesion.y = 0;
        if (cohesion.LengthSquare() > 0.0001f)
        {
            cohesion = normalize(cohesion);
            // 吸い込み中は凝集力が強くなる。通常時は弱く（自由な動きを優先）
            float cohesionWeight = isSucking ? 1.5f : 0.1f;
            cohesion *= cohesionWeight;
        }
    }

    if (separationCount > 0 && separation.LengthSquare() > 0.0001f)
    {
        separation = normalize(separation);
        constexpr float separationWeight = 2.0f; // 分離を強めて、近づきすぎを防ぐ
        separation *= separationWeight;
    }

    return cohesion + separation;
}

VECTOR3 CHerded::CalculateEscapeFromDog() const
{
    CFlog* flog = m_pOwner->GetFlog();
    if (flog == nullptr) return {0, 0, 0};
    CAShepherdDog* dog = flog->GetShepherdDog();
    if (dog == nullptr) return {0, 0, 0};

    const VECTOR3 dogPos = dog->GetTransform().position;
    const VECTOR3 myPos = m_pOwner->GetTransform().position;

    VECTOR3 diff = myPos - dogPos;
    diff.y = 0;
    const float distanceSq = diff.LengthSquare();

    // 犬を感知する距離（ストロングボムモデル: 50m）
    constexpr float detectionRadiusSq = 5.0f * 5.0f;

    // 犬が感知範囲内にいる場合のみ逃げる
    if (distanceSq < detectionRadiusSq && distanceSq > 0.0001f)
    {
        diff = normalize(diff);
        constexpr float dogEscapeWeight = 2.5f;
        return diff * dogEscapeWeight;
    }

    return {0, 0, 0};
}


VECTOR3 CHerded::CalculateWandering()
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
    if (wanderForce.LengthSquare() > 0.0001f)
    {
        wanderForce = normalize(wanderForce);
        constexpr float wanderWeight = 1.0f; // Wanderの強さ
        return wanderForce * wanderWeight;
    }

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

void CPanic::UpdateMovement()
{
    float targetAngle = atan2f(m_panicDirection.x, m_panicDirection.z);
    m_pOwner->SetRotateY(targetAngle);

    const float moveSpeed = 2.0f;
    m_pOwner->AddPosition(m_panicDirection * moveSpeed * SceneManager::DeltaTime());
}

void CPanic::CheckBoundaryAndTransition()
{
    if (m_panicTimer < m_panicDuration) return;

    const VECTOR3 currentPos = m_pOwner->GetTransform().position;
    CFlog* nearest = nullptr;
    float nearestDistSq = FLT_MAX;

    for (CFlog* f : ObjectManager::FindGameObjects<CFlog>())
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

    if (nearest != nullptr)
    {
        m_pOwner->SetFlog(nearest);

        m_pOwner->ChangeState(CBaseState::State::HERDED);
    }
    else
    {
        m_pOwner->SetFlog(nullptr);

        m_pOwner->ChangeState(CBaseState::State::IDLE);
    }
    m_isFinish = true;
}
