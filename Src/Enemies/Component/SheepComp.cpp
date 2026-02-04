#include "SheepComp.h"
#include "../System/Flog.h"
CHerded::CHerded(CSheep* sheep)
{
    m_pOwner = sheep;
    m_pPlayer = ObjectManager::FindGameObject<CPlayer>();
}

void CHerded::Enter()
{
    m_isFinish = false;
    m_pOwner->GetAnimator()->MergePlay(AnimationType::A_WALK);
}

void CHerded::Update()
{
    const VECTOR3 boidsForce = CalculateBoids();
    const VECTOR3 escapeForce = CalculateEscapeFromDog();

    VECTOR3 totalForce = boidsForce + escapeForce;
    totalForce.y = 0;

    // デバッグ出力
    ImGui::Begin("Sheep Debug");
    ImGui::Text("Boids: %.2f, %.2f, %.2f", boidsForce.x, boidsForce.y, boidsForce.z);
    ImGui::Text("Escape: %.2f, %.2f, %.2f", escapeForce.x, escapeForce.y, escapeForce.z);
    ImGui::Text("Total: %.2f, %.2f, %.2f", totalForce.x, totalForce.y, totalForce.z);
    ImGui::Text("HasMaster: %s", m_pOwner->GetMaster() ? "YES" : "NULL");
    if (m_pOwner->GetMaster())
    {
        VECTOR3 dogPos = m_pOwner->GetMaster()->GetTransform().position;
        VECTOR3 myPos = m_pOwner->GetTransform().position;
        float dist = sqrtf((dogPos - myPos).LengthSquare());
        ImGui::Text("Dog Distance: %.2f", dist);
    }
    ImGui::End();

    if (totalForce.LengthSquare() > 0.0001f)
    {
        normalize(totalForce);

        // 移動方向に回転
        float targetAngle = atan2f(totalForce.x, totalForce.z);
        m_pOwner->SetRotateY(targetAngle);

        // 移動
        const float moveSpeed = 1.5f;
        m_pOwner->AddPosition(totalForce * moveSpeed * SceneManager::DeltaTime());
    }
    m_pOwner->IsSuctionCheck();
}

VECTOR3 CHerded::CalculateBoids() const
{
    CFlog* flog = ObjectManager::FindGameObject<CFlog>();
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

    constexpr float neighborRadius = 30.0f;   // 仲間と認識する距離（ストロンボム: 30m）
    constexpr float separationRadius = 2.0f;  // 近すぎると判定する距離

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
            normalize(cohesion);
            // 吸い込み中は凝集力が強くなる（ストロンボムモデル）
            float cohesionWeight = isSucking ? 1.05f : 0.2f;
            cohesion *= cohesionWeight;
        }
    }

    if (separationCount > 0 && separation.LengthSquare() > 0.0001f)
    {
        normalize(separation);
        constexpr float separationWeight = 1.5f;  // 分離（近すぎる仲間から離れる）
        separation *= separationWeight;
    }

    return cohesion + separation;
}

VECTOR3 CHerded::CalculateEscapeFromDog() const
{
    // UFOが吸い込み中でなければ逃げない
    if (m_pPlayer == nullptr || !m_pPlayer->GetIsSuckUp())
    {
        return {0, 0, 0};
    }

    CAShepherdDog* dog = m_pOwner->GetMaster();
    if (dog == nullptr) return {0, 0, 0};

    const VECTOR3 dogPos = dog->GetTransform().position;
    const VECTOR3 myPos = m_pOwner->GetTransform().position;

    VECTOR3 diff = myPos - dogPos;
    diff.y = 0;
    const float distanceSq = diff.LengthSquare();

    // 犬を感知する距離（ストロンボムモデル: 50m）
    constexpr float detectionRadiusSq = 50.0f * 50.0f;

    // 犬が検知範囲内にいる場合のみ逃げる
    if (distanceSq < detectionRadiusSq && distanceSq > 0.0001f)
    {
        normalize(diff);
        constexpr float dogEscapeWeight = 2.5f;
        return diff * dogEscapeWeight;
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
    
    // ランダムな方向を決定
   const float randomAngle = Randomf(0.0f, XM_2PI);
    m_panicDirection = VECTOR3(sinf(randomAngle), 0, cosf(randomAngle));
    
    m_changeDirectionTimer = 0.0f;
    
    // アニメーションを走りに変更
    m_pOwner->GetAnimator()->MergePlay(AnimationType::A_RUN);
    m_pOwner->GetAnimator()->SetPlaySpeed(1.5f);  // 速めに再生
}

void CPanic::Update()
{
    m_changeDirectionTimer += SceneManager::DeltaTime();
    
    constexpr float changeDirectionInterval = 2.0f;  // 2秒ごとに方向転換
    // 一定時間ごとに方向をランダムに変更（ジグザグに逃げる）
    if (m_changeDirectionTimer >= changeDirectionInterval)
    {
        const float randomAngle = Randomf(0.0f, XM_2PI);
        m_panicDirection = VECTOR3(sinf(randomAngle), 0, cosf(randomAngle));
        m_changeDirectionTimer = 0.0f;
    }
    
    // 移動方向に回転
    float targetAngle = atan2f(m_panicDirection.x, m_panicDirection.z);
    m_pOwner->SetRotateY(targetAngle);

    // パニック状態で移動
    const float moveSpeed = 2.0f;  // HERDEDより速く（パニック状態）
    m_pOwner->AddPosition(m_panicDirection * moveSpeed * SceneManager::DeltaTime());
    
    // エリア外に出たら方向転換
    const VECTOR3 currentPos = m_pOwner->GetTransform().position;
    const VECTOR2 areaSize = m_pOwner->GetAreaSize();
    
    if (!IsInsideAreaXZ(currentPos, areaSize))
    {
        // エリア中心方向に向き直す
        VECTOR3 toCenter = VECTOR3(0, 0, 0) - currentPos;
        toCenter.y = 0; 
        if (toCenter.LengthSquare() > 0.0001f)
        {
            normalize(toCenter);
            m_panicDirection = toCenter;
        }
    }
    
    // 吸い込みチェック
    m_pOwner->IsSuctionCheck();
}