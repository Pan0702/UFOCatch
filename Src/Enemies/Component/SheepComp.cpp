#include "SheepComp.h"
#include "../System/Flog.h"
CHerded::CHerded(CSheep* sheep)
    : m_wanderTimer(0.0f), m_walkTimer(0.0f), m_walkDuration(0.0f), m_currentRotation(0.0f)
{
    m_pOwner = sheep;
    m_pPlayer = ObjectManager::FindGameObject<CPlayer>();

    // 蛻晄悄縺ｮWander譁ｹ蜷代ｒ繝ｩ繝ｳ繝繝縺ｫ險ｭ螳・
    float randomAngle = Randomf(0.0f, XM_2PI);
    m_wanderTarget = VECTOR3(sinf(randomAngle), 0, cosf(randomAngle));
}

void CHerded::Enter()
{
    m_isFinish = false;
    m_pOwner->GetAnimator()->MergePlay(AnimationType::A_WALK);

    // 迥ｶ諷九↓蜈･繧九◆縺ｳ縺ｫWander譁ｹ蜷代ｒ繝ｪ繧ｻ繝・ヨ
    float randomAngle = Randomf(0.0f, XM_2PI);
    m_wanderTarget = VECTOR3(sinf(randomAngle), 0, cosf(randomAngle));
    m_wanderTimer = 0.0f;

    // 遘ｻ蜍墓凾髢薙ｒ繝ｩ繝ｳ繝繝縺ｫ險ｭ螳夲ｼ・・・遘抵ｼ・
    m_walkDuration = Randomf(3.0f, 7.0f);
    m_walkTimer = 0.0f;

    // 迴ｾ蝨ｨ縺ｮ蝗櫁ｻ｢繧剃ｿ晏ｭ・
    m_currentRotation = m_pOwner->GetTransform().rotation.y;
}

void CHerded::Update()
{
    // 遘ｻ蜍墓凾髢薙ｒ繧ｫ繧ｦ繝ｳ繝・
    m_walkTimer += SceneManager::DeltaTime();

    // 荳螳壽凾髢鍋ｵ碁℃縺励◆繧迂DLE縺ｫ謌ｻ繧・
    if (m_walkTimer >= m_walkDuration)
    {
        m_isFinish = true;
        m_pOwner->ChangeState(CBaseState::State::IDLE);
        return;
    }

    const VECTOR3 boidsForce = CalculateBoids();
    const VECTOR3 wanderForce = CalculateWandering();
    const VECTOR3 boundaryForce = CalculateBoundaryForce();

    // 蜉帙ｒ蜷域・・・andering蜆ｪ蜈医〒閾ｪ逕ｱ縺ｪ蜍輔″・・
    VECTOR3 totalForce = wanderForce + boidsForce + boundaryForce;
    totalForce.y = 0;

    if (totalForce.LengthSquare() > 0.0001f)
    {
        normalize(totalForce);

        // 逶ｮ讓吶・蝗櫁ｻ｢隗貞ｺｦ繧定ｨ育ｮ・
        float targetAngle = atan2f(totalForce.x, totalForce.z);

        // 迴ｾ蝨ｨ縺ｮ蝗櫁ｻ｢縺九ｉ逶ｮ讓吶・蝗櫁ｻ｢縺ｸ貊代ｉ縺九↓陬憺俣・亥屓霆｢騾溷ｺｦ: 3.0 rad/s・・
        constexpr float rotationSpeed = 3.0f;
        float angleDiff = targetAngle - m_currentRotation;

        // 隗貞ｺｦ繧・ﾏ・槃縺ｮ遽・峇縺ｫ豁｣隕丞喧
        while (angleDiff > XM_PI) angleDiff -= XM_2PI;
        while (angleDiff < -XM_PI) angleDiff += XM_2PI;

        // 蝗櫁ｻ｢繧定｣憺俣
        float rotationDelta = angleDiff * rotationSpeed * SceneManager::DeltaTime();
        if (fabsf(angleDiff) < fabsf(rotationDelta))
        {
            m_currentRotation = targetAngle;
        }
        else
        {
            m_currentRotation += rotationDelta;
        }

        // 蝗櫁ｻ｢繧帝←逕ｨ
        m_pOwner->SetRotateY(m_currentRotation);

        // 遘ｻ蜍・
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

    // UFO縺悟精縺・ｾｼ縺ｿ荳ｭ縺九←縺・°縺ｧ蜃晞寔縺ｮ蠑ｷ縺輔ｒ螟峨∴繧・
    bool isSucking = false;
    if (m_pPlayer != nullptr)
    {
        isSucking = m_pPlayer->GetIsSuckUp();
    }

    constexpr float neighborRadius = 30.0f;   // 莉ｲ髢薙→隱崎ｭ倥☆繧玖ｷ晞屬・医せ繝医Ο繝ｳ繝懊Β: 30m・・
    constexpr float separationRadius = 2.0f;  // 霑代☆縺弱ｋ縺ｨ蛻､螳壹☆繧玖ｷ晞屬

    const float neighborRadiusSq = Pow2(neighborRadius);
    const float separationRadiusSq = Pow2(separationRadius);

    for (const auto& other : allSheep)
    {
        if (other == m_pOwner) continue;

        const VECTOR3 otherPos = other->GetTransform().position;
        VECTOR3 diff = otherPos - myPos;
        const float distanceSq = diff.LengthSquare();
        diff.y = 0;
        // 霑鷹團縺ｮ莉ｲ髢・
        if (distanceSq < neighborRadiusSq)
        {
            cohesion += otherPos;
            neighborCount++;
        }

        // 霑代☆縺弱ｋ莉ｲ髢薙°繧蛾屬繧後ｋ
        if (distanceSq < separationRadiusSq && distanceSq > 0.0001f)
        {
            float distance = sqrtf(distanceSq);
            separation -= diff / distance;
            separationCount++;
        }
    }

    // 蟷ｳ蝮・喧
    if (neighborCount > 0)
    {
        cohesion = cohesion / static_cast<float>(neighborCount);
        cohesion -= myPos;
        cohesion.y = 0;
        if (cohesion.LengthSquare() > 0.0001f)
        {
            normalize(cohesion);
            // 蜷ｸ縺・ｾｼ縺ｿ荳ｭ縺ｯ蜃晞寔蜉帙′蠑ｷ縺上↑繧九・壼ｸｸ譎ゅ・蠑ｱ縺擾ｼ郁・逕ｱ縺ｪ蜍輔″繧貞━蜈茨ｼ・
            float cohesionWeight = isSucking ? 1.5f : 0.1f;
            cohesion *= cohesionWeight;
        }
    }

    if (separationCount > 0 && separation.LengthSquare() > 0.0001f)
    {
        normalize(separation);
        constexpr float separationWeight = 2.0f;  // 蛻・屬繧貞ｼｷ繧√※縲∬ｿ代▼縺阪☆縺弱ｒ髦ｲ縺・
        separation *= separationWeight;
    }

    return cohesion + separation;
}

VECTOR3 CHerded::CalculateEscapeFromDog() const
{
    // UFO縺悟精縺・ｾｼ縺ｿ荳ｭ縺ｧ縺ｪ縺代ｌ縺ｰ騾・￡縺ｪ縺・
    if (m_pPlayer == nullptr || !m_pPlayer->GetIsSuckUp())
    {
        return {0, 0, 0};
    }

    CAShepherdDog* dog = nullptr;
    if (dog == nullptr) return {0, 0, 0};

    const VECTOR3 dogPos = dog->GetTransform().position;
    const VECTOR3 myPos = m_pOwner->GetTransform().position;

    VECTOR3 diff = myPos - dogPos;
    diff.y = 0;
    const float distanceSq = diff.LengthSquare();

    // 迥ｬ繧呈─遏･縺吶ｋ霍晞屬・医せ繝医Ο繝ｳ繝懊Β繝｢繝・Ν: 50m・・
    constexpr float detectionRadiusSq = 50.0f * 50.0f;

    // 迥ｬ縺梧､懃衍遽・峇蜀・↓縺・ｋ蝣ｴ蜷医・縺ｿ騾・￡繧・
    if (distanceSq < detectionRadiusSq && distanceSq > 0.0001f)
    {
        normalize(diff);
        constexpr float dogEscapeWeight = 2.5f;
        return diff * dogEscapeWeight;
    }

    return {0, 0, 0};
}

VECTOR3 CHerded::CalculateBoundaryForce() const
{
    CFlog* flog = ObjectManager::FindGameObject<CFlog>();
    if (flog == nullptr) return {0, 0, 0};

    const VECTOR3 flockCenter = flog->GetFlockCenter();
    const float flockRadius = flog->GetFlockRadius();
    const VECTOR3 myPos = m_pOwner->GetTransform().position;

    VECTOR3 toCenter = flockCenter - myPos;
    toCenter.y = 0;

    const float distanceToCenter = sqrtf(toCenter.LengthSquare());

    // 蜊雁ｾ・､悶↓蜃ｺ縺溷ｴ蜷医・縺ｿ縲∝ｼｷ縺・鴨縺ｧ荳ｭ蠢・↓謌ｻ縺・
    if (distanceToCenter > flockRadius)
    {
        if (toCenter.LengthSquare() > 0.0001f)
        {
            normalize(toCenter);
            // 蜊雁ｾ・ｒ雜・∴繧九⊇縺ｩ蠑ｷ縺・鴨縺ｧ蠑輔″謌ｻ縺・
            const float overDistance = distanceToCenter - flockRadius;
            const float boundaryWeight = 5.0f + overDistance * 1.0f;
            return toCenter * boundaryWeight;
        }
    }

    // 蜊雁ｾ・・縺ｧ縺ｯ螳悟・縺ｫ閾ｪ逕ｱ・亥｢・阜蜉帙↑縺暦ｼ・
    return {0, 0, 0};
}

VECTOR3 CHerded::CalculateWandering()
{
    // 荳螳壽凾髢薙＃縺ｨ縺ｫ繝ｩ繝ｳ繝繝縺ｪ譁ｹ蜷代ｒ螟画峩
    m_wanderTimer += SceneManager::DeltaTime();

    constexpr float changeDirectionInterval = 3.0f;  // 3遘偵＃縺ｨ縺ｫ譁ｹ蜷大､画峩
    if (m_wanderTimer >= changeDirectionInterval)
    {
        // 繝ｩ繝ｳ繝繝縺ｪ隗貞ｺｦ繧堤函謌撰ｼ育樟蝨ｨ縺ｮ譁ｹ蜷代°繧可ｱ60蠎ｦ縺ｮ遽・峇・・
        float currentAngle = atan2f(m_wanderTarget.x, m_wanderTarget.z);
        float randomOffset = Randomf(-XM_PI / 3.0f, XM_PI / 3.0f);  // ﾂｱ60蠎ｦ
        float newAngle = currentAngle + randomOffset;

        m_wanderTarget = VECTOR3(sinf(newAngle), 0, cosf(newAngle));
        m_wanderTimer = 0.0f;
    }

    // Wander譁ｹ蜷代↓豁｣隕丞喧縺輔ｌ縺溷鴨繧定ｿ斐☆
    VECTOR3 wanderForce = m_wanderTarget;
    if (wanderForce.LengthSquare() > 0.0001f)
    {
        normalize(wanderForce);
        constexpr float wanderWeight = 1.0f;  // Wander縺ｮ蠑ｷ縺・
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
    
    // 繝ｩ繝ｳ繝繝縺ｪ譁ｹ蜷代ｒ豎ｺ螳・
   const float randomAngle = Randomf(0.0f, XM_2PI);
    m_panicDirection = VECTOR3(sinf(randomAngle), 0, cosf(randomAngle));
    
    m_changeDirectionTimer = 0.0f;
    
    // 繧｢繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ繧定ｵｰ繧翫↓螟画峩
    m_pOwner->GetAnimator()->MergePlay(AnimationType::A_RUN);
    m_pOwner->GetAnimator()->SetPlaySpeed(1.5f);  // 騾溘ａ縺ｫ蜀咲函
}

void CPanic::Update()
{
    m_changeDirectionTimer += SceneManager::DeltaTime();
    
    constexpr float changeDirectionInterval = 2.0f;  // 2遘偵＃縺ｨ縺ｫ譁ｹ蜷題ｻ｢謠・
    // 荳螳壽凾髢薙＃縺ｨ縺ｫ譁ｹ蜷代ｒ繝ｩ繝ｳ繝繝縺ｫ螟画峩・医ず繧ｰ繧ｶ繧ｰ縺ｫ騾・￡繧具ｼ・
    if (m_changeDirectionTimer >= changeDirectionInterval)
    {
        const float randomAngle = Randomf(0.0f, XM_2PI);
        m_panicDirection = VECTOR3(sinf(randomAngle), 0, cosf(randomAngle));
        m_changeDirectionTimer = 0.0f;
    }
    
    // 遘ｻ蜍墓婿蜷代↓蝗櫁ｻ｢
    float targetAngle = atan2f(m_panicDirection.x, m_panicDirection.z);
    m_pOwner->SetRotateY(targetAngle);

    // 繝代ル繝・け迥ｶ諷九〒遘ｻ蜍・
    const float moveSpeed = 2.0f;  // HERDED繧医ｊ騾溘￥・医ヱ繝九ャ繧ｯ迥ｶ諷具ｼ・
    m_pOwner->AddPosition(m_panicDirection * moveSpeed * SceneManager::DeltaTime());

    // 鄒､繧後・遽・峇螟悶↓蜃ｺ縺溘ｉ譁ｹ蜷題ｻ｢謠・
    CFlog* flog = ObjectManager::FindGameObject<CFlog>();
    if (flog != nullptr)
    {
        const VECTOR3 flockCenter = flog->GetFlockCenter();
        const float flockRadius = flog->GetFlockRadius();
        const VECTOR3 currentPos = m_pOwner->GetTransform().position;

        VECTOR3 toCenter = flockCenter - currentPos;
        toCenter.y = 0;
        const float distanceToCenter = sqrtf(toCenter.LengthSquare());

        // 蜊雁ｾ・､悶↓蜃ｺ縺溷ｴ蜷医∽ｸｭ蠢・婿蜷代↓蜷代″逶ｴ縺・
        if (distanceToCenter > flockRadius)
        {
            if (toCenter.LengthSquare() > 0.0001f)
            {
                normalize(toCenter);
                m_panicDirection = toCenter;
            }
        }
    }
    
    // 蜷ｸ縺・ｾｼ縺ｿ繝√ぉ繝・け
    m_pOwner->IsSuctionCheck();
}