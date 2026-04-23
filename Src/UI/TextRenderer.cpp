#include "TextRenderer.h"

CUIText::CUIText(CSpriteImage* pFont, const VECTOR2& pos, const VECTOR2& srcPos, const VECTOR2& srcSize)
    : m_pFont(pFont), m_srcPos(srcPos), m_srcSize(srcSize)
{
    m_position = pos;
    m_size = srcSize;
}

void CUIText::SetSrcPos(const VECTOR2& srcPos)
{
    m_srcPos = srcPos;
}

void CUIText::Draw(CSprite& sprite)
{
    if (!m_visible || !m_pFont) return;
    VECTOR2 worldPos = GetWorldPosition();
    VECTOR2 scaledSize = m_size * m_scale;
    sprite.Draw(m_pFont, worldPos.x, worldPos.y,
                static_cast<DWORD>(m_srcPos.x), static_cast<DWORD>(m_srcPos.y),
                static_cast<DWORD>(m_srcSize.x), static_cast<DWORD>(m_srcSize.y),
                static_cast<DWORD>(scaledSize.x), static_cast<DWORD>(scaledSize.y),
                m_alpha);
    CUIWidget::Draw(sprite);
}
