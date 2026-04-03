#include "UIButtons.h"

UIButtons::UIButtons()
    : m_focusIndex(0)
{
}

void UIButtons::Add(CUIButton* b, std::function<void()> onConfirm)
{
    bool isFirst = m_buttons.empty();
    m_buttons.push_back({ b, std::move(onConfirm) });
    if (isFirst)
        b->SetFocus(true);
}

void UIButtons::SetAnim(std::unique_ptr<CUIAnimation> Foucus, std::unique_ptr<CUIAnimation> unFocus) const
{
    for (auto& b : m_buttons)
    {
        b.pButton->GetAnimationPlayer().AddAnimation("Focus", std::move(Foucus));
        b.pButton->GetAnimationPlayer().AddAnimation("UnFocus", std::move(unFocus));
    }
}

void UIButtons::MoveFocus(int delta)
{
    if (m_buttons.empty()) return;

    m_buttons[m_focusIndex].pButton->SetFocus(false);

    int count = static_cast<int>(m_buttons.size());
    m_focusIndex = (m_focusIndex + delta % count + count) % count;

    m_buttons[m_focusIndex].pButton->SetFocus(true);
}

void UIButtons::Confirm() const
{
    if (m_buttons.empty()) return;
    if (m_buttons[m_focusIndex].onConfirm)
        m_buttons[m_focusIndex].onConfirm();
}

CUIButton* UIButtons::GetFocus() const
{
    if (m_buttons.empty()) return nullptr;
    return m_buttons[m_focusIndex].pButton;
}