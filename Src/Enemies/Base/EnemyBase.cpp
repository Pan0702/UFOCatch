#define NOMINMAX
#include "EnemyBase.h"
#include "../../Stage/Ground.h"
#include "../../Stage/StageObject.h"
#include "../System/EnemyManager.h"

CEnemyBase::CEnemyBase()
    : m_velocityY(0.0f)
{
    m_pPlayer = ObjectManager::FindGameObject<CPlayer>();
    m_pEnemyManager = ObjectManager::FindGameObject<CEnemyManager>();
    if (m_pEnemyManager)
    {
        m_pEnemyManager->RegisterEnemy(this);
    }

}


std::unique_ptr<CBBox> CEnemyBase::CreateBBox()
{
    if (m_pBBox == nullptr && m_pMesh != nullptr)
    {
        m_pBBox = std::make_unique<CBBox>(m_pMesh->m_vMin, m_pMesh->m_vMax);
    }
    return move(m_pBBox);
}



void CEnemyBase::ChangeState(CBaseState::State type)
{
    // 蜷後§迥ｶ諷九↑繧我ｽ輔ｂ縺励↑縺・
    if (m_pComponent == m_components[type].get()) return;

    m_pState->Exit();
    m_pComponent = m_components[type].get();
    m_pState->Enter(type);
}

CEnemyBase::~CEnemyBase()
{
    if (m_pEnemyManager)
    {
        m_pEnemyManager->UnregisterEnemy(this);
    }

}

void CEnemyBase::Update()
{
    ApplyGravity();
    m_pAnimator->Update();
    if (m_pState)
    {
        m_pState->Update();
    }
    //霑代￥縺ｫ縺・ｋ繧ｪ繝悶ず繧ｧ繧ｯ繝医ｒ蜿門ｾ励＠縲∝ｽ薙◆縺｣縺ｦ縺・◆繧画款縺玲綾縺・
    ResolveOBBCollisions();
    UpdateBBox();
    // 繧ｹ繝・・繧ｸ繧ｪ繝悶ず繧ｧ繧ｯ繝医→縺ｮ陦晉ｪ∝愛螳壹→謚ｼ縺玲綾縺暦ｼ域怙蠕後↓螳溯｡鯉ｼ・
    ResolveStageCollisions();
}

void CEnemyBase::ApplyGravity()
{
    if (!ShouldApplyGravity())
    {
        m_velocityY = 0.0f;
        return;
    }
    static constexpr float GRAVITY = 9.8f;

    static constexpr float GROUND_SKIN = 0.02f; // 蝨ｰ髱｢縺ｨ縺ｮ謗･隗ｦ譎ゅ・繧√ｊ霎ｼ縺ｿ髦ｲ豁｢逕ｨ縺ｮ繧ｹ繧ｭ繝ｳ蛟､ //
    m_velocityY -= GRAVITY * SceneManager::DeltaTime();
    const float dt = SceneManager::DeltaTime();
    const float nextY = transform.position.y + m_velocityY * dt;

    if (m_pGround != nullptr)
    {
        static constexpr float GROUND_CHECK_OFFSET = 0.1f;
        // 迴ｾ蝨ｨ縺ｮ菴咲ｽｮ縺ｨ谺｡縺ｮ菴咲ｽｮ繧剃ｽｿ縺｣縺ｦ縲√が繝輔そ繝・ヨ繧定・・縺励◆繝ｬ繧､繧ｭ繝｣繧ｹ繝医・遽・峇繧呈ｱｺ螳・//
        const float fromY = transform.position.y + GROUND_CHECK_OFFSET;
        const float toY = nextY - GROUND_CHECK_OFFSET;
        // 荳句髄縺阪・蝨ｰ髱｢繝√ぉ繝・け //
        if (toY < fromY)
        {
            const VECTOR3 rayStart = VECTOR3(transform.position.x, fromY, transform.position.z);
            const VECTOR3 rayEnd = VECTOR3(transform.position.x, toY, transform.position.z);

            MeshCollider::CollInfo collInfo;
            bool hit = m_pGround->HitLineToMesh(rayStart, rayEnd, &collInfo);

            if (hit)
            {
                // 陦晉ｪ√＠縺溷ｴ蜷医∝慍髱｢縺ｮ菴咲ｽｮ縺ｫ繧ｹ繧ｭ繝ｳ蛟､繧貞刈縺医※菴咲ｽｮ繧定ｪｿ謨ｴ //
                transform.position.y = collInfo.hitPosition.y + GROUND_SKIN;
                m_velocityY = 0.0f;
                return;
            }
        }
    }

    transform.position.y = nextY;
}

bool CEnemyBase::GetBounds2D(VECTOR2& outPos, VECTOR2& outSize) const
{
    if (m_pMesh == nullptr)
    {
        //Mesh縺後↑縺代ｌ縺ｰ邨ゅｏ繧・/
        return false;
    }
    
    // XZ蟷ｳ髱｢縺ｧ縺ｮ4鬆らせ繧貞叙蠕・/
    const VECTOR3 min = m_pMesh->m_vMin;
    const VECTOR3 max = m_pMesh->m_vMax;
    
    VECTOR3 corners[4] = {
        VECTOR3(min.x, 0, min.z),
        VECTOR3(max.x, 0, min.z),
        VECTOR3(min.x, 0, max.z),
        VECTOR3(max.x, 0, max.z)
    };

    // 蝗櫁ｻ｢陦悟・繧貞叙蠕・/
    MATRIX4X4 rotY = XMMatrixRotationY(transform.rotation.y);
    MATRIX4X4 scaleM = XMMatrixScaling(transform.scale.x, transform.scale.y, transform.scale.z);
    MATRIX4X4 transformMatrix = scaleM * rotY;

    // 螟画鋤蠕後・鬆らせ縺ｮ譛蟆上・譛螟ｧ蛟､繧呈ｱゅａ繧・//
    float minX = FLT_MAX, maxX = -FLT_MAX;
    float minZ = FLT_MAX, maxZ = -FLT_MAX;

    for (const auto& corner : corners)
    {
        VECTOR3 transformed = XMVector3TransformCoord(corner, transformMatrix);
        minX = std::min(minX, transformed.x);
        maxX = std::max(maxX, transformed.x);
        minZ = std::min(minZ, transformed.z);
        maxZ = std::max(maxZ, transformed.z);
    }

    // 繝ｯ繝ｼ繝ｫ繝牙ｺｧ讓吶↓螟画鋤
    outPos = VECTOR2(
        (minX + maxX) * 0.5f + transform.position.x,
        (minZ + maxZ) * 0.5f + transform.position.z
    );
    outSize = VECTOR2(maxX - minX, maxZ - minZ);

    return true;
}

std::vector<CEnemyBase*> CEnemyBase::GetNearbyEnemies() const
{
    // AnimalManager繧貞叙蠕・//
    if (m_pEnemyManager == nullptr)
    {
        return std::vector<CEnemyBase*>();
    }

    // 閾ｪ蛻・・2D蠅・阜繝懊ャ繧ｯ繧ｹ・・Z蟷ｳ髱｢縺ｧ縺ｮ菴咲ｽｮ縺ｨ繧ｵ繧､繧ｺ・峨ｒ蜿門ｾ・//
    VECTOR2 pos, size;
    if (!GetBounds2D(pos, size))
    {
        return std::vector<CEnemyBase*>();
    }

    // 繝槭ロ繝ｼ繧ｸ繝｣繝ｼ縺ｮ蝗帛・譛ｨ繧剃ｽｿ縺｣縺ｦ縲∝捉霎ｺ縺ｮ繧ｨ繝阪Α繝ｼ繧貞柑邇・噪縺ｫ蜿門ｾ・//
    return m_pEnemyManager->GetNearbyEnemies(const_cast<CEnemyBase*>(this), pos, size);
}

void CEnemyBase::UpdateBBox() const
{
    if (m_pBBox != nullptr)
    {
        m_pBBox->m_mWorld = transform.matrix();
    }
}

void CEnemyBase::ResolveOBBCollisions()
{
    if (m_pBBox == nullptr) return;

    // 蝗帛・譛ｨ縺九ｉ蜻ｨ霎ｺ縺ｮ繧ｨ繝阪Α繝ｼ繧貞柑邇・噪縺ｫ蜿門ｾ・/
    std::vector<CEnemyBase*> nearbyEnemies = GetNearbyEnemies();

    // 蜻ｨ霎ｺ繧ｨ繝阪Α繝ｼ縺ｨ蠖薙◆繧雁愛螳・/
    for (auto* enemy : nearbyEnemies)
    {
        if (enemy == this) continue;  // 閾ｪ蛻・・霄ｫ縺ｯ繧ｹ繧ｭ繝・・//
        if (enemy->GetBBox() == nullptr) continue;

        VECTOR3 hitPos, hitNormal;
        if (m_pBBox->OBBCollisionDetection(enemy->GetBBox(), &hitPos, &hitNormal))
        {
            // 陦晉ｪ√＠縺溷ｴ蜷医∵款縺玲綾縺怜・逅・ｒ螳溯｡・/
            CalcApplyPushback(enemy);
        }
    }
}

void CEnemyBase::CalcApplyPushback(CEnemyBase* other)
{
    if (m_pBBox == nullptr || other == nullptr || other->GetBBox() == nullptr) return;

    // 閾ｪ蛻・→逶ｸ謇九・OBB荳ｭ蠢・ｺｧ讓吶ｒ險育ｮ・/
    MATRIX4X4 myCenterMat = XMMatrixTranslation(
        m_pBBox->m_fLengthX + m_pBBox->m_vMin.x,
        m_pBBox->m_fLengthY + m_pBBox->m_vMin.y,
        m_pBBox->m_fLengthZ + m_pBBox->m_vMin.z
    );
    // 繝ｯ繝ｼ繝ｫ繝臥ｩｺ髢薙↓螟画鋤//
    myCenterMat = myCenterMat * m_pBBox->m_mWorld;
    // 陦悟・縺九ｉ菴咲ｽｮ謌仙・繧呈歓蜃ｺ縺励※閾ｪ蛻・・荳ｭ蠢・ｺｧ讓吶ｒ蜿門ｾ・/
    VECTOR3 myCenter =GetPositionVector(myCenterMat);

    // 逶ｸ謇九・OBB荳ｭ蠢・ｺｧ讓吶ｒ險育ｮ・/
    CBBox* otherBBox = other->GetBBox();
    MATRIX4X4 otherCenterMat = XMMatrixTranslation(
        otherBBox->m_fLengthX + otherBBox->m_vMin.x,
        otherBBox->m_fLengthY + otherBBox->m_vMin.y,
        otherBBox->m_fLengthZ + otherBBox->m_vMin.z
    );
    // 繝ｯ繝ｼ繝ｫ繝臥ｩｺ髢薙↓螟画鋤//
    otherCenterMat = otherCenterMat * otherBBox->m_mWorld;
    // 陦悟・縺九ｉ菴咲ｽｮ謌仙・繧呈歓蜃ｺ縺励※逶ｸ謇九・荳ｭ蠢・ｺｧ讓吶ｒ蜿門ｾ・/
    VECTOR3 otherCenter =  GetPositionVector(otherCenterMat);

    // 謚ｼ縺玲綾縺励・繧ｯ繝医Ν繧定ｨ育ｮ・/
    VECTOR3 pushDirection = myCenter - otherCenter;
    pushDirection.y = 0.0f;  // Y謌仙・繧堤┌隕悶＠縺ｦXZ蟷ｳ髱｢縺ｮ縺ｿ縺ｧ謚ｼ縺玲綾縺・/
    float distance = magnitude(pushDirection);

    if (distance > 0.001f)  // 0髯､邂励ｒnorm縺ｧ襍ｷ縺薙＆縺ｪ縺・◆繧√↓蛟､縺後＞蟆上＆縺・ｴ蜷医・繧ｹ繧ｭ繝・・//
    {
        pushDirection = normalize(pushDirection);

        static constexpr float PUSHBACK_SPEED = 6.0f;  // 1遘偵≠縺溘ｊ縺ｮ謚ｼ縺玲綾縺鈴溷ｺｦ

        // 謚ｼ縺玲綾縺苓ｷ晞屬//
        float pushDistance = PUSHBACK_SPEED * SceneManager::DeltaTime();

        // 菴咲ｽｮ繧呈峩譁ｰ//
        transform.position += pushDirection * pushDistance;
    }
}

VECTOR3 CEnemyBase::CalcSlideMove(const VECTOR3& desiredMove) const
{
    if (m_pBBox == nullptr) return desiredMove;


    // 縺薙％縺ｧ迴ｾ蝨ｨ縺ｮ transform 繧剃ｽｿ縺｣縺ｦ BBox 繧呈峩譁ｰ
    m_pBBox->m_mWorld = transform.matrix();

    VECTOR3 moveVec = desiredMove;

    // 蝗帛・譛ｨ縺ｧ霑大ｍ繧ｪ繝悶ず繧ｧ繧ｯ繝医・縺ｿ蜿門ｾ・
    VECTOR2 pos2d, size2d;
    std::vector<CStageObject*> stageObjects;
    if (m_pEnemyManager && GetBounds2D(pos2d, size2d))
    {
        stageObjects = m_pEnemyManager->GetNearbyStageObjects(pos2d, size2d);
    }

    for (CStageObject* stage : stageObjects)
    {
        if (stage == nullptr || stage->GetOBB() == nullptr) continue;

        VECTOR3 hitPos, hitNormal;
        if (stage->HitOBB(m_pBBox.get(), &hitPos, &hitNormal))
        {
            // OBBCollisionDetection 縺ｯ豕慕ｷ壹ｒ (0,1,0) 蝗ｺ螳壹〒霑斐☆縺溘ａ hitNormal 縺ｯ菴ｿ逕ｨ荳榊庄
            // 繧ｹ繝・・繧ｸ荳ｭ蠢・竊・謨ｵ荳ｭ蠢・縺ｮ譁ｹ蜷代ｒ螢∵ｳ慕ｷ壹・莉｣繧上ｊ縺ｫ菴ｿ縺・
            CBBox* stageOBB = stage->GetOBB();
            MATRIX4X4 stageCenterMat = XMMatrixTranslation(
                stageOBB->m_fLengthX + stageOBB->m_vMin.x,
                stageOBB->m_fLengthY + stageOBB->m_vMin.y,
                stageOBB->m_fLengthZ + stageOBB->m_vMin.z
            );
            stageCenterMat = stageCenterMat * stageOBB->m_mWorld;
            VECTOR3 stageCenter = GetPositionVector(stageCenterMat);

            MATRIX4X4 enemyCenterMat = XMMatrixTranslation(
                m_pBBox->m_fLengthX + m_pBBox->m_vMin.x,
                m_pBBox->m_fLengthY + m_pBBox->m_vMin.y,
                m_pBBox->m_fLengthZ + m_pBBox->m_vMin.z
            );
            enemyCenterMat = enemyCenterMat * m_pBBox->m_mWorld;
            VECTOR3 enemyCenter = GetPositionVector(enemyCenterMat);

            VECTOR3 awayDir = enemyCenter - stageCenter;
            awayDir.y = 0.0f;
            float dist = magnitude(awayDir);
            if (dist > 0.001f)
            {
                awayDir = normalize(awayDir);
                // awayDir 縺ｯ縲梧惠縺九ｉ髮｢繧後ｋ譁ｹ蜷代・ 螢∵ｳ慕ｷ壹・霑台ｼｼ
                // moveVec 縺ｮ縲梧惠縺ｫ蜷代°縺・・蛻・阪□縺代ｒ髯､蜴ｻ縺吶ｋ
                float d = Dot(moveVec, awayDir);
                if (d < 0.0f)  // 譛ｨ譁ｹ蜷代↓蜷代°縺｣縺ｦ縺・ｋ縺ｨ縺阪□縺・
                {
                    moveVec -= awayDir * d;
                }
            }
        }
    }
    return moveVec;
}

//
// 繧ｹ繝・・繧ｸ繧ｪ繝悶ず繧ｧ繧ｯ繝医→縺ｮ陦晉ｪ∝愛螳壹→謚ｼ縺玲綾縺怜・逅・
//
void CEnemyBase::ResolveStageCollisions()
{
    if (m_pBBox == nullptr) return;

    // 蝗帛・譛ｨ縺ｧ霑大ｍ繧ｪ繝悶ず繧ｧ繧ｯ繝医・縺ｿ蜿門ｾ・
    if (m_pEnemyManager == nullptr) return;

    VECTOR2 pos, size;
    if (!GetBounds2D(pos, size)) return;

    std::vector<CStageObject*> stageObjects = m_pEnemyManager->GetNearbyStageObjects(pos, size);

    for (CStageObject* stage : stageObjects)
    {
        if (stage == nullptr) continue;
        stage->ResolveEnemyCollision(this);
    }
}

CComponentBase* CEnemyBase::GetComponent(CBaseState::State type) const
{
    auto itr = m_components.find(type);
    if (itr == m_components.end())
    {
        return nullptr;
    }
    return itr->second.get();
}

VECTOR3 CEnemyBase::SuctionSpeed() const
{
    return {0, 0, 0};
}

void CEnemyBase::IsSuctionCheck() 
{
    if (m_pPlayer == nullptr)return;
    if (m_pPlayer->GetIsSuckUp() && m_pPlayer->IsInsideSuctionCircle(transform.position))
    {
        ChangeState(CBaseState::State::SUCTION);
    }
}



