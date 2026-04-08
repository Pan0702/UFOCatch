#include "UIRingGauge.h"

#include "../Framework/sceneManager.h"

CUIRingGauge::CUIRingGauge(CSpriteImage* pBg, CSpriteImage* pFill, const VECTOR2& pos, const VECTOR4& size)
    : m_maxWidth(size.z), m_t(0.0f),m_srcSize(size)
{
    m_position = pos;
    m_size = VECTOR2(size.z, size.w);
    auto bg = std::make_unique<CUIImage>(pBg, VECTOR2(0, 0), size);
    bg->SetLayer(0);
    m_pBg = AddChild(std::move(bg));
    auto fill = std::make_unique<CUIImage>(pFill, VECTOR2(0, 0), size);
    fill->SetLayer(1);
    fill->SetVisible(false);
    m_pFill = AddChild(std::move(fill));
    m_arcParams.startAngle = 0.0f;
    m_arcParams.clockwise = true;
    m_arcParams.innerRadius=0.5f;
}

void CUIRingGauge::SetRatio(float ratio)
{
    m_startRatio = m_currentRatio;
    m_targetRatio = ratio;
    m_t = 0.0f;
}


void CUIRingGauge::SetAnim(const UIAnimationTween& anim)
{
    m_moveSpeed = anim.m_moveSpeed;
    m_easing = anim.m_easing;
}

void CUIRingGauge::SetArcDrawParams(ArcDrawParams params)
{
    m_arcParams = params;
}

void CUIRingGauge::Update()
{
    if (m_t >= 1.0f)return;
    const float delta = SceneManager::DeltaTime() * m_moveSpeed;
    const float easedT = m_easing(m_t);
    m_t = std::clamp(m_t + delta, 0.0f, 1.0f);
    m_currentRatio = Lerp(m_startRatio, m_targetRatio, easedT);

    const float newWidth = m_maxWidth * m_currentRatio;
    m_pFill->SetSrcRect(VECTOR4(0, 0, newWidth, m_size.y));
    m_pFill->SetSize(VECTOR2(newWidth, m_size.y));
    CUIWidget::Update();
}

void CUIRingGauge::Draw(CSprite& sprite)
{
    if (!m_visible) return;
    CUIWidget::Draw(sprite);
    const VECTOR2 worldPos = GetWorldPosition();
    m_arcParams.ratio = m_currentRatio;
    sprite.DrawArc(m_pFill->GetImage(), worldPos.x, worldPos.y,
                   (DWORD)m_srcSize.x, (DWORD)m_srcSize.y, (DWORD)m_srcSize.z, (DWORD)m_srcSize.w,
                   m_arcParams, m_alpha);
}
