#pragma once
#include "UIWidget.h"

class CUIImage : public CUIWidget
{
public:
    CUIImage(CSpriteImage* pImage, const VECTOR2& pos, const VECTOR2& size);
    CUIImage(CSpriteImage* pImage, const VECTOR2& pos, const VECTOR4& srcRect);
    void Draw(CSprite& sprite);
    
    void SetImage(CSpriteImage* pImage);
    void SetSrcRect(const VECTOR4& srcRect);

    VECTOR4 GetSrcRect() const;

private:
    CSpriteImage* m_pImage;
    VECTOR4 m_srcRect;
};
