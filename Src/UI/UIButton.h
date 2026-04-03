#pragma once
#include <functional>

#include "UIImage.h"
class CUIButton : public CUIImage
{
public:
    CUIButton(const VECTOR2& pos, const VECTOR4& size, CSpriteImage* pNormalImage = nullptr, CSpriteImage* pFocusImage = nullptr);
    void SetFocus(bool focused);
    bool IsFocus() const;
    void SetOnFocusChanged(std::function<void(bool)> callback);
private:
    std::function<void(bool)> m_onFocusChanged = nullptr;
    
private:
    float m_normalAlpha = 1.0f;
    float m_focusAlpha = 1.0f;
    bool m_isFocus = false;
    CSpriteImage* m_pNormalImage = nullptr;
    CSpriteImage* m_pFocusImage = nullptr;
    
};
