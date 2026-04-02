#pragma once
#include "UIImage.h"
class CUIButton : public CUIImage
{
public:
    CUIButton(CSpriteImage* pNormalImage, CSpriteImage* pFocusImage, const VECTOR2& pos, const VECTOR4& size);
    void SetFocus(bool focused);
    bool IsFocus() const;
    
private:
    float m_normalAlpha = 1.0f;
    float m_focusAlpha = 1.0f;
    bool m_isFocus = false;
    CSpriteImage* m_pNormalImage = nullptr;
    CSpriteImage* m_pFocusImage = nullptr;
    
};
