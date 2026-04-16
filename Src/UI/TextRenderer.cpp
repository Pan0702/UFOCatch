#include "TextRenderer.h"

CUIText::CUIText(CSpriteImage* pFont, const VECTOR2& pos,const VECTOR2& srcPos,const VECTOR2& size)
    :m_pFont(pFont),m_srcPos(srcPos)
{
    m_position = pos;
    m_srcSize = size;
}
void CUIText::SetSize(const VECTOR2& size)
{
    m_size = size;
}

void CUIText::Draw(CSprite& sprite)
{
    VECTOR2 worldPos = GetWorldPosition();
    sprite.Draw(m_pFont,worldPos.x,worldPos.y,
                static_cast<DWORD>(m_srcPos.x), static_cast<DWORD>(m_srcPos.y),
                static_cast<DWORD>(m_srcSize.x), static_cast<DWORD>(m_srcSize.y));
    CUIWidget::Draw(sprite);
}
