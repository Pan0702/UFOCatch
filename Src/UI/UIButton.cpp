#include "UIButton.h"

CUIButton::CUIButton(CSpriteImage* pNormalImage, CSpriteImage* pFocusImage, const VECTOR2& pos, const VECTOR4& size)
    : CUIImage(pNormalImage, pos, size) ,m_pNormalImage(pNormalImage), m_pFocusImage(pFocusImage)
{
}

void CUIButton::SetFocus(bool focused)
{
    m_isFocus = focused;
    SetImage(focused ? m_pFocusImage : m_pNormalImage);
}

bool CUIButton::IsFocus() const
{
    return m_isFocus;
}
