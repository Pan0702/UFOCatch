#include "UIProgressBar.h"
#include "../Utils/Lerp.h"
#include "../Framework/sceneManager.h"

CUIProgressBar::CUIProgressBar(CSpriteImage* pBg, CSpriteImage* pFill, const VECTOR2& pos, const VECTOR4& size)
    : m_maxWidth(size.z), m_t(0.0f)

{
    m_position = pos;
    m_size = VECTOR2(size.z, size.w);
    m_visible = true;
    auto bg = std::make_unique<CUIImage>(pBg, VECTOR2(0, 0),size);
    bg->SetLayer(0);
    bg->SetVisible(true);
    m_pBg = AddChild(std::move(bg));
    auto fill = std::make_unique<CUIImage>(pFill, VECTOR2(0, 0),size);
    fill->SetLayer(1);
    fill->SetVisible(true);
    m_pFill = AddChild(std::move(fill));
}

void CUIProgressBar::SetRatio(float ratio)
{
    m_startRatio = m_currentRatio;  
    m_targetRatio = std::clamp(ratio, 0.0f, 1.0f);
    m_t = 0.0f;  
}


void CUIProgressBar::Update()
{
    const float delta = SceneManager::DeltaTime();
    if (m_t < 1.0f)
    {
        m_t = std::clamp(m_t + delta / m_duration, 0.0f, 1.0f);
        float easedT = EaseOutQuint(m_t);
        m_currentRatio = Lerp(m_startRatio, m_targetRatio,easedT);

        float newWidth = m_maxWidth * m_currentRatio;
        m_pFill->SetSrcRect(VECTOR4(0, 0, newWidth, m_size.y));
        m_pFill->SetSize(VECTOR2(newWidth, m_size.y));
    }
    CUIWidget::Update();
}
