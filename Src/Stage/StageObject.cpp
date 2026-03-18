#define NOMINMAX
#include "StageObject.h"
#include "../Enemies/Base/EnemyBase.h"
#include "../Framework/ResourceManager.h"

////////////////////
// 繧ｳ繝ｳ繧ｹ繝医Λ繧ｯ繧ｿ
// @param meshPath 繝｡繝・す繝･繝輔ぃ繧､繝ｫ縺ｮ繝代せ
// @param pos 繧ｪ繝悶ず繧ｧ繧ｯ繝医・菴咲ｽｮ
// @param scale 繧ｪ繝悶ず繧ｧ繧ｯ繝医・繧ｵ繧､繧ｺ
// @param useOBB OBB繧剃ｽｿ逕ｨ縺吶ｋ縺・//
////////////////////
CStageObject::CStageObject(const char* meshPath, const VECTOR3& pos, float scale, bool useOBB)
{
    m_bUseOBB = useOBB;
    m_pOBB = nullptr;

    // ResourceManager縺九ｉ繝｡繝・す繝･繧貞叙蠕暦ｼ医く繝｣繝・す繝･縺輔ｌ繧具ｼ・
    m_pMesh = ResourceManager::LoadFbx(meshPath, TODO);

    // OBB縺ｮ菴懈・
    if (m_bUseOBB)
    {
        // 繝｡繝・す繝･縺九ｉ逶ｴ謗･繝舌え繝ｳ繝・ぅ繝ｳ繧ｰ繝懊ャ繧ｯ繧ｹ繧貞叙蠕・
        VECTOR3 vMin = m_pMesh->m_vMin;
        VECTOR3 vMax = m_pMesh->m_vMax;

        // OBB繧剃ｽ懈・
        m_pOBB = new CBBox(vMin, vMax);
    }

    // 蛻晄悄菴咲ｽｮ
    transform.position = pos;
    transform.scale = VECTOR3(1.0f, 1.0f, 1.0f) * scale;
}

//------------------------------------------------------------------------
// 繝・せ繝医Λ繧ｯ繧ｿ
//------------------------------------------------------------------------
CStageObject::~CStageObject()
{
    if (m_pOBB)
    {
        delete m_pOBB;
        m_pOBB = nullptr;
    }
}

//------------------------------------------------------------------------
// 譖ｴ譁ｰ蜃ｦ逅・
//------------------------------------------------------------------------
void CStageObject::Update()
{
    Object3D::Update();

    // OBB縺ｮ繝ｯ繝ｼ繝ｫ繝芽｡悟・繧呈峩譁ｰ
    if (m_pOBB)
    {
        m_pOBB->m_mWorld = transform.matrix();
    }
}

//------------------------------------------------------------------------
// 謠冗判蜃ｦ逅・
//------------------------------------------------------------------------
void CStageObject::Draw()
{
    Object3D::Draw();


    // // 繝・ヰ繝・げ逕ｨ: OBB繧呈緒逕ｻ・亥ｿ・ｦ√↓蠢懊§縺ｦ繧ｳ繝｡繝ｳ繝医い繧ｦ繝茨ｼ・
    // if (m_pOBB)
    // {
    //     m_pOBB->Render();
    // }
}

bool CStageObject::GetBounds2D(VECTOR2& outPos, VECTOR2& outSize) const
{
    if (m_pMesh == nullptr) return false;

    const VECTOR3 min = m_pMesh->m_vMin;
    const VECTOR3 max = m_pMesh->m_vMax;

    VECTOR3 corners[4] = {
        VECTOR3(min.x, 0, min.z),
        VECTOR3(max.x, 0, min.z),
        VECTOR3(min.x, 0, max.z),
        VECTOR3(max.x, 0, max.z)
    };

    MATRIX4X4 rotY = XMMatrixRotationY(transform.rotation.y);
    MATRIX4X4 scaleM = XMMatrixScaling(transform.scale.x, transform.scale.y, transform.scale.z);
    MATRIX4X4 mat = scaleM * rotY;

    float minX = FLT_MAX, maxX = -FLT_MAX;
    float minZ = FLT_MAX, maxZ = -FLT_MAX;
    for (const auto& c : corners)
    {
        VECTOR3 t = XMVector3TransformCoord(c, mat);
        minX = std::min(minX, t.x);
        maxX = std::max(maxX, t.x);
        minZ = std::min(minZ, t.z);
        maxZ = std::max(maxZ, t.z);
    }

    outPos = VECTOR2((minX + maxX) * 0.5f + transform.position.x,
                     (minZ + maxZ) * 0.5f + transform.position.z);
    outSize = VECTOR2(maxX - minX, maxZ - minZ);
    return true;
}

////////////////////
// OBB縺ｨ縺ｮ陦晉ｪ∝愛螳壹ｒ陦後≧
// @param other 逶ｸ謇九・OBB
// @param vHit 陦晉ｪ∽ｽ咲ｽｮ・・ut・・
// @param vNormal 陦晉ｪ∵ｳ慕ｷ夲ｼ・ut・俄ｻXZ蟷ｳ髱｢縺ｮ縺ｿ・・謌仙・縺ｯ0・・
// @return 陦晉ｪ√＠縺ｦ縺・◆繧液rue //
////////////////////
bool CStageObject::HitOBB(CBBox* other, VECTOR3* vHit, VECTOR3* vNormal)
{
    if (!m_pOBB || !other || !m_bUseOBB)
    {
        return false;
    }

    VECTOR3 vHitTemp, vNormalTemp;
    bool bHit = m_pOBB->OBBCollisionDetection(other, &vHitTemp, &vNormalTemp);

    // 邨先棡繧呈ｼ邏・
    if (bHit)
    {
        if (vHit) *vHit = vHitTemp;

        if (vNormal)
        {
            // XZ蟷ｳ髱｢縺ｮ縺ｿ縺ｧ謚ｼ縺玲綾縺暦ｼ・霆ｸ繧貞性繧√ｋ縺ｨ荳九↓繧√ｊ霎ｼ繧・・
            vNormalTemp.y = 0.0f;
            *vNormal = XMVector3Normalize(vNormalTemp);
        }
    }

    return bHit;
}

////////////////////
// 繧ｨ繝阪Α繝ｼ縺ｨ縺ｮ陦晉ｪ√ｒ隗｣豸医☆繧・
// @param pEnemy 蛻､螳壼ｯｾ雎｡縺ｮ繧ｨ繝阪Α繝ｼ //
////////////////////
void CStageObject::ResolveEnemyCollision(CEnemyBase* pEnemy)
{
    if (!m_pOBB || !pEnemy || !pEnemy->GetBBox()) return;

    VECTOR3 hitPos, hitNormal;
    if (HitOBB(pEnemy->GetBBox(), &hitPos, &hitNormal))
    {
        // OBBCollisionDetection 縺ｯ豕慕ｷ壹ｒ (0,1,0) 蝗ｺ螳壹〒霑斐☆縺溘ａ hitNormal 縺ｯ菴ｿ逕ｨ荳榊庄
        // 繧ｹ繝・・繧ｸ荳ｭ蠢・竊・繧ｨ繝阪Α繝ｼ荳ｭ蠢・縺ｮ譁ｹ蜷代〒謚ｼ縺玲綾縺・
        MATRIX4X4 stageCenterMat = XMMatrixTranslation(
            m_pOBB->m_fLengthX + m_pOBB->m_vMin.x,
            m_pOBB->m_fLengthY + m_pOBB->m_vMin.y,
            m_pOBB->m_fLengthZ + m_pOBB->m_vMin.z
        );
        stageCenterMat = stageCenterMat * m_pOBB->m_mWorld;
        VECTOR3 stageCenter = GetPositionVector(stageCenterMat);

        CBBox* enemyBBox = pEnemy->GetBBox();
        MATRIX4X4 enemyCenterMat = XMMatrixTranslation(
            enemyBBox->m_fLengthX + enemyBBox->m_vMin.x,
            enemyBBox->m_fLengthY + enemyBBox->m_vMin.y,
            enemyBBox->m_fLengthZ + enemyBBox->m_vMin.z
        );
        enemyCenterMat = enemyCenterMat * enemyBBox->m_mWorld;
        VECTOR3 enemyCenter = GetPositionVector(enemyCenterMat);

        VECTOR3 pushDir = enemyCenter - stageCenter;
        pushDir.y = 0.0f;
        float dist = magnitude(pushDir);
        if (dist > 0.001f)
        {
            pushDir = normalize(pushDir);
            static constexpr float STAGE_PUSHBACK_DIST = 0.2f;
            pEnemy->AddPosition(pushDir * STAGE_PUSHBACK_DIST);
        }
    }
}

