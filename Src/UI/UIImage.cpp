#include "UIImage.h"

CUIImage::CUIImage(CSpriteImage* pImage, const VECTOR2& pos, const VECTOR2& size)
    :m_pImage(pImage),m_srcRect(0,0,size.x,size.y)
{
    m_position = pos;
    m_size = size;
}

CUIImage::CUIImage(CSpriteImage* pImage, const VECTOR2& pos, const VECTOR4& srcRect)
    :m_pImage(pImage)
    ,m_srcRect(srcRect)
{
    m_position = pos;
    m_size = VECTOR2(srcRect.z,srcRect.w);
}

void CUIImage::Draw(CSprite& sprite)
{
    if (!m_visible || m_pImage == nullptr) return;
    VECTOR2 worldPos = GetWorldPosition();
    VECTOR2 scaledSize = m_size * m_scale;
    sprite.Draw(m_pImage, worldPos.x, worldPos.y,
        static_cast<DWORD>(m_srcRect.x),static_cast<DWORD>(m_srcRect.y),
        static_cast<DWORD>(scaledSize.x),  static_cast<DWORD>(scaledSize.y),  m_alpha);
    
    CUIWidget::Draw(sprite);
}

void CUIImage::SetImage(CSpriteImage* pImage)
{
    m_pImage = pImage;
}

void CUIImage::SetSrcRect(const VECTOR4& srcRect)
{
    m_srcRect = srcRect;
}

CSpriteImage* CUIImage::GetImage()
{
    return m_pImage;
}

VECTOR4 CUIImage::GetSrcRect() const
{
    return m_srcRect;
}
