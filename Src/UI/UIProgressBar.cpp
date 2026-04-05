#include "UIProgressBar.h"

#include "UIAnimationTween.h"
#include "../Framework/sceneManager.h"

CUIProgressBar::CUIProgressBar(CSpriteImage* pBg, CSpriteImage* pFill, const VECTOR2& pos, const VECTOR4& size)
    : m_maxWidth(size.z), m_t(0.0f)

{
    m_position = pos;
    m_size = VECTOR2(size.z, size.w);
    m_visible = true;
    auto bg = std::make_unique<CUIImage>(pBg, VECTOR2(0, 0), size);
    bg->SetLayer(0);
    bg->SetVisible(true);
    m_pBg = AddChild(std::move(bg));
    auto fill = std::make_unique<CUIImage>(pFill, VECTOR2(0, 0), size);
    fill->SetLayer(1);
    fill->SetVisible(true);
    m_pFill = AddChild(std::move(fill));
}

void CUIProgressBar::SetAnim(const UIAnimationTween& anim)
{
    m_moveSpeed = anim.m_moveSpeed;
    m_easing = anim.m_easing;
}

void CUIProgressBar::SetIsEXPBar(bool isEXPBar)
{
    m_isEXPBar = isEXPBar;
}

void CUIProgressBar::SetRatio(float ratio)
{
    m_startRatio    = m_currentRatio;
    m_targetRatio   = ratio + m_currentRatio;
    m_t             = 0.0f;
}


void CUIProgressBar::Update()
{
    if (m_isEXPBar)
    {
        Progress();
    }
    else
    {
        Gauge();
    }
    CUIWidget::Update();
}

void CUIProgressBar::Progress()
{
    //ゲージが100%になったので初期化
    if (m_currentRatio >= 1.0f)
    {
        m_currentRatio = 0.0f;
        m_targetRatio -= 1.0f;
        m_startRatio = 0.0f;
        m_t = 0.0f;
    }
    if (m_t >= 1.0f)
    {
        return;
    }
    
    //移動速度
    const float delta = SceneManager::DeltaTime() * m_moveSpeed;
    //

    m_t = std::clamp(m_t + delta, 0.0f, 1.0f);
    const float easedT = m_easing(m_t);
    //easeTを使って補間
    m_currentRatio = Lerp(m_startRatio, m_targetRatio,easedT);

    //長さ
    const float newWidth = m_maxWidth * m_currentRatio;
    m_pFill->SetSrcRect(VECTOR4(0, 0, newWidth, m_size.y));
    m_pFill->SetSize(VECTOR2(newWidth, m_size.y));
}

void CUIProgressBar::Gauge()
{
    if (m_t >= 1.0f)return;
    const float delta = SceneManager::DeltaTime() * m_moveSpeed;
    const float easedT = m_easing(m_t);
    m_t = std::clamp(m_t + delta, 0.0f, 1.0f);
    m_currentRatio = Lerp(m_startRatio, m_targetRatio, easedT);

    const float newWidth = m_maxWidth * m_currentRatio;
    m_pFill->SetSrcRect(VECTOR4(0, 0, newWidth, m_size.y));
    m_pFill->SetSize(VECTOR2(newWidth, m_size.y));
}
