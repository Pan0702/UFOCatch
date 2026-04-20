#include "UIButtons.h"

#include "ImageRegistry.h"
#include "UIBase.h"
#include "../Framework/SceneBase.h"

CUIButtons::CUIButtons()
    : m_focusIndex(0)
{
    m_imageSize = VECTOR4(0, 0, 100, 100);
}

void CUIButtons::AddButton(CUIBase* ui,const std::string& name ,const VECTOR2& pos)
{
    CSpriteImage* imageButton = ImageRegistry::LoadTexture(name);
    auto btn = std::make_unique<CUIButton>(pos, m_imageSize, nullptr, imageButton);
    CUIButton* widget = ui->GetCanvas().AddWidget(std::move(btn));
    widget->SetOnFocusChanged([widget](bool isFocus)
    {
        widget->GetAnimationPlayer().Play(isFocus ? "Focus" : "UnFocus", false);
    });
    m_buttons.push_back(widget);
}

void CUIButtons::SetAnim(const std::shared_ptr<CUIAnimation>& Foucus, const std::shared_ptr<CUIAnimation>& unFocus) const
{
    for (auto& b : m_buttons)
    {
        b->GetAnimationPlayer().AddAnimation("Focus", Foucus);
        b->GetAnimationPlayer().AddAnimation("UnFocus", unFocus);
    }
}


void CUIButtons::AddButton(CUIBase* ui, CSpriteImage* pImage, const VECTOR2& pos,
                           const VECTOR4& normalSrcRect, const VECTOR4& focusSrcRect)
{
    auto btn = std::make_unique<CUIButton>(pos, pImage, normalSrcRect, focusSrcRect);
    CUIButton* widget = ui->GetCanvas().AddWidget(std::move(btn));
    widget->SetOnFocusChanged([widget](bool isFocus)
    {
        widget->GetAnimationPlayer().Play(isFocus ? "Focus" : "UnFocus", false);
    });
    widget->SetFocus(false);
    m_buttons.push_back(widget);
}

void CUIButtons::SetImageSize(const VECTOR4& size)
{
    m_imageSize = size;
}

void CUIButtons::SetFocus(int index)
{
    m_focusIndex = index;
    m_buttons[m_focusIndex]->SetFocus(true);
}

void CUIButtons::SetLayer(int layer) const
{
    for (auto& b : m_buttons)
        b->SetLayer(layer);
}

void CUIButtons::SetAlpha(float alpha) const
{
    for (auto& b : m_buttons)
        b->SetAlpha(alpha);
}

void CUIButtons::MoveFocus(int n)
{
    if (m_buttons.empty()) return;
    int newIndex = std::clamp(m_focusIndex + n, 0, static_cast<int>(m_buttons.size()) - 1);
    if (newIndex == m_focusIndex) return;
    m_buttons[m_focusIndex]->SetFocus(false);
    m_focusIndex = newIndex;
    m_buttons[m_focusIndex]->SetFocus(true);
}

