#include "PlayerLevel.h"

CPlayerLevel::CPlayerLevel(float initialConeTopPos, float coneDegree)
    : m_coneDegree(coneDegree)
    , m_coneTopPos(initialConeTopPos)
    , m_allExp(1.0f)
    , m_exp(0.001f)
    , m_lv(1)
{
    m_coneRadius = m_coneTopPos * tan(DegToRad * m_coneDegree);
}

void CPlayerLevel::Update()
{
    CheckLevel();
    UpdateHeightAndRadiusLerp();
}

////////////////////
// 荳螳壽焚蜷ｸ縺・ｾｼ繧薙□縺九メ繧ｧ繝・け //
////////////////////
void CPlayerLevel::CheckLevel()
{
    if (m_exp >= m_allExp)
    {
        float tmp = m_exp - m_allExp;
        m_allExp *= 1.3f;
        m_exp = tmp;
        m_lv++;
        IncreaseSuctionConeHeight();
    }
}

////////////////////
// 蜷ｸ縺・ｾｼ縺ｿ繧ｳ繝ｼ繝ｳ縺ｮ鬮倥＆繧貞｢怜刈縺輔○繧・//
////////////////////
void CPlayerLevel::IncreaseSuctionConeHeight()
{
    const float targetHeight = m_coneTopPos + 1.0f;
    const float targetRadius = targetHeight * tan(DegToRad * m_coneDegree);

    m_heightLerp.Start(m_coneTopPos, targetHeight, 0.05f);
    m_radiusLerp.Start(m_coneRadius, targetRadius, 0.05f);
}

////////////////////
// 鬮倥＆縺ｨ蜊雁ｾ・・Lerp蜃ｦ逅・//
////////////////////
void CPlayerLevel::UpdateHeightAndRadiusLerp()
{
    if (m_heightLerp.IsAnimating())
    {
        m_coneTopPos = m_heightLerp.Update(SceneManager::DeltaTime());
    }
    if (m_radiusLerp.IsAnimating())
    {
        m_coneRadius = m_radiusLerp.Update(SceneManager::DeltaTime());
    }
}
