#pragma once
#include "UIBase.h"

//Spriteの文字を描画するクラス
class CTextRenderer : public CUIWidget
{
public:
    CTextRenderer(CSpriteImage* pFont, const VECTOR2& pos, const VECTOR2& size, const VECTOR2& srcPos);
    void SetSrcPos(const VECTOR2& srcPos);
    void Draw(CSprite& sprite) override;
private:
    CSpriteImage* m_pFont;
    VECTOR2 m_srcPos;
};
