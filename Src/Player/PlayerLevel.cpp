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
// 一定数吸引したかチェック //
////////////////////
void CPlayerLevel::CheckLevel()
{
    if (m_exp >= m_allExp)
    {
        m_raito = 1.0f;
        float tmp = m_exp - m_allExp;
        m_allExp *= 1.3f;
        m_exp = tmp;
        m_raito += m_exp / m_allExp;
        m_lv++;
        IncreaseSuctionConeHeight();
    }
    else
    {
        m_raito = m_exp / m_allExp;
    }
}

////////////////////
// 吸引コーンの高さを増加させる //
////////////////////
void CPlayerLevel::IncreaseSuctionConeHeight()
{
    const float targetHeight = m_coneTopPos + 1.0f;
    const float targetRadius = targetHeight * tan(DegToRad * m_coneDegree);

    m_heightLerp.Start(m_coneTopPos, targetHeight, 0.05f);
    m_radiusLerp.Start(m_coneRadius, targetRadius, 0.05f);
}

////////////////////
// 高さと半径のLerp処理 //
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
