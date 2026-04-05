#include "UIButton.h"

CUIButton::CUIButton(const VECTOR2& pos, const VECTOR4& size, CSpriteImage* pNormalImage, CSpriteImage* pFocusImage)
    : CUIImage(pNormalImage, pos, size), m_pNormalImage(pNormalImage), m_pFocusImage(pFocusImage)
{
}

void CUIButton::SetFocus(bool focused)
{
    m_isFocus = focused;
    if (focused)
    {
        SetImage(m_pFocusImage);
        if (m_onFocusChanged) m_onFocusChanged(true);
    }
    else
    {
        if (m_onFocusChanged) m_onFocusChanged(false);
        GetAnimationPlayer().SetOnComplete([this]
        {
            SetImage(m_pNormalImage);
        });
    }
}

bool CUIButton::IsFocus() const
{
    return m_isFocus;
}

void CUIButton::SetOnFocusChanged(std::function<void(bool)> callback)
{
    m_onFocusChanged = std::move(callback);   
}
