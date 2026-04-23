#include "UIButton.h"

CUIButton::CUIButton(const VECTOR2& pos, const VECTOR4& size, CSpriteImage* pNormalImage, CSpriteImage* pFocusImage)
    : CUIImage(pNormalImage, pos, size), m_pNormalImage(pNormalImage), m_pFocusImage(pFocusImage)
{
}

CUIButton::CUIButton(const VECTOR2& pos, CSpriteImage* pImage,
                     const VECTOR4& normalSrcRect, const VECTOR4& focusSrcRect)
    : CUIImage(pImage, pos, normalSrcRect)
    , m_useSpriteSheet(true)
    , m_normalSrcRect(normalSrcRect)
    , m_focusSrcRect(focusSrcRect)
{
}

CUIButton::CUIButton(const VECTOR2& pos, CSpriteImage* pImage, const VECTOR4& focusSrcRect)
    : CUIImage(pImage, pos, focusSrcRect)
    , m_useSpriteSheet(true)
    , m_normalSrcRect(VECTOR4(0, 0, 0, 0))
    , m_focusSrcRect(focusSrcRect)
{
    SetVisible(false);
}

void CUIButton::SetFocus(bool focused)
{
    m_isFocus = focused;
    if (focused)
    {
        if (m_useSpriteSheet)
        {
            SetSrcRect(m_focusSrcRect);
            SetVisible(true);
        }
        else
            SetImage(m_pFocusImage);

        if (m_onFocusChanged) m_onFocusChanged(true);
    }
    else
    {
        if (m_onFocusChanged) m_onFocusChanged(false);
        if (m_useSpriteSheet)
        {
            GetAnimationPlayer().SetOnComplete([this]
            {
                SetSrcRect(m_normalSrcRect);
                if (m_normalSrcRect.z == 0 && m_normalSrcRect.w == 0)
                    SetVisible(false);
            });
        }
        else
        {
            GetAnimationPlayer().SetOnComplete([this]
            {
                SetImage(m_pNormalImage);
            });
        }
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
