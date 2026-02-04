#include "Sheep.h"
#include "../System/Flog.h"
CHerded::CHerded(CSheep* sheep)
{
    m_pOwner = sheep;
    m_pPlayer = ObjectManager::FindGameObject<CPlayer>();
}

void CHerded::Enter()
{
    m_isFinish = false;
    m_pOwner->GetAnimator()->MergePlay(AnimationType::A_RUN);
}

void CHerded::Update()
{
    const VECTOR3 boidsForce = CalculateBoids();
    const VECTOR3 escapeForce = CalculateEscapeFromUFO();
    
    const VECTOR3 totalForce = boidsForce + escapeForce;
    if (totalForce.LengthSquare() > 0.0001f)
    {
        normalize(totalForce);
    }
    float moveSpeed = 3.0f;
    m_pOwner->AddPosition(totalForce * moveSpeed * SceneManager::DeltaTime());
    m_pOwner->IsSuctionCheck();
}

void CHerded::Exit()
{
    CComponentBase::Exit();
}

VECTOR3 CHerded::CalculateBoids()
{
    CFlog* flog = ObjectManager::FindGameObject<CFlog>();
    const auto& allSheep = flog->GetAllSheeps();
    
    VECTOR3 cohesion(0, 0, 0);
    VECTOR3 separation(0, 0, 0);
    VECTOR3 alignment(0, 0, 0);
    
    int neighborCount = 0;
    int separationCount = 0;
    
    VECTOR3 myPos = m_pOwner->GetTransform().position;
    
    constexpr float neighborRadius = 5.0f;    // 仲間と認識する距離
    constexpr float separationRadius = 2.0f;  // 近すぎると判定する距離
    
    float neighborRadiusSq = Pow2(neighborRadius);
    float separationRadiusSq = Pow2(separationRadius);
    
    for (const auto& other : allSheep)
    {
        if (other == m_pOwner) continue;
        
        VECTOR3 otherPos = other->GetTransform().position;
        VECTOR3 diff = otherPos - myPos;
        float distanceSq = diff.LengthSquare();
        
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
        cohesion /= neighborCount;
        cohesion -= myPos;
        if (cohesion.LengthSquare() > 0.0001f)
        {
            normalize(cohesion);
            constexpr float cohesionWeight = 1.0f;    // 凝集（群れの中心に向かう）
            cohesion *= cohesionWeight;
        }
        
        if (alignment.LengthSquare() > 0.0001f)
        {
            normalize(alignment);
           constexpr float alignmentWeight = 1.0f;   // 整列（仲間と同じ方向に向かう）
            alignment *= alignmentWeight;
        }
    }
    
    if (separationCount > 0 && separation.LengthSquare() > 0.0001f)
    {
        normalize(separation);
        constexpr float separationWeight = 1.5f;  // 分離（近すぎる仲間から離れる）
        separation *= separationWeight;
    }
    
    return cohesion + separation + alignment;
}

VECTOR3 CHerded::CalculateEscapeFromUFO()const 
{
    if (m_pPlayer == nullptr) return VECTOR3(0, 0, 0);
    
    VECTOR3 ufoPos = m_pPlayer->GetTransform().position;
    VECTOR3 myPos = m_pOwner->GetTransform().position;
    
    VECTOR3 escapeDir = myPos - ufoPos;

    if (escapeDir.LengthSquare() > 0.0001f)
    {
        
        normalize(escapeDir);
        constexpr float escapeWeight = 2.0f;      // UFOから逃げる
        return escapeDir * escapeWeight;
    }
    
    return VECTOR3(0, 0, 0);
}


CPanic::CPanic(CSheep* sheep)
{
    m_pOwner = sheep;
}

void CPanic::Enter()
{
    m_isFinish = false;
    
    // ランダムな方向を決定
    float randomAngle = Randomf(0.0f, XM_2PI);
    m_panicDirection = VECTOR3(sinf(randomAngle), 0, cosf(randomAngle));
    
    m_changeDirectionTimer = 0.0f;
    
    // アニメーションを走りに変更
    m_pOwner->GetAnimator()->MergePlay(AnimationType::A_RUN);
    m_pOwner->GetAnimator()->SetPlaySpeed(1.5f);  // 速めに再生
}

void CPanic::Update()
{
    m_changeDirectionTimer += SceneManager::DeltaTime();
    
    float changeDirectionInterval = 2.0f;  // 2秒ごとに方向転換
    // 一定時間ごとに方向をランダムに変更（ジグザグに逃げる）
    if (m_changeDirectionTimer >= changeDirectionInterval)
    {
        float randomAngle = Randomf(0.0f, XM_2PI);
        m_panicDirection = VECTOR3(sinf(randomAngle), 0, cosf(randomAngle));
        m_changeDirectionTimer = 0.0f;
    }
    
    // パニック状態で移動
    float moveSpeed = 4.0f;  // HERDEDより速く（パニック状態）
    m_pOwner->AddPosition(m_panicDirection * moveSpeed * SceneManager::DeltaTime());
    
    // エリア外に出たら方向転換
    VECTOR3 currentPos = m_pOwner->GetTransform().position;
    VECTOR2 areaSize = m_pOwner->GetAreaSize();
    
    if (!IsInsideAreaXZ(currentPos, areaSize))
    {
        // エリア中心方向に向き直す
        VECTOR3 toCenter = VECTOR3(0, 0, 0) - currentPos;
        if (toCenter.LengthSquare() > 0.0001f)
        {
            normalize(toCenter);
            m_panicDirection = toCenter;
        }
    }
    
    // 吸い込みチェック
    m_pOwner->IsSuctionCheck();
}

void CPanic::Exit()
{
    CComponentBase::Exit();
}