#pragma once
#include "UIWidget.h"

/// @brief テクスチャを描画するウィジェット
/// @details スプライトシートの切り抜き範囲（srcRect）を指定して描画できる
class CUIImage : public CUIWidget
{
public:
    /// @brief サイズ指定コンストラクタ（srcRectは(0,0,size.x,size.y)になる）
    CUIImage(CSpriteImage* pImage, const VECTOR2& pos, const VECTOR2& size);

    /// @brief 切り抜き範囲指定コンストラクタ
    /// @param srcRect (srcX, srcY, srcWidth, srcHeight) テクスチャの切り抜き範囲
    CUIImage(CSpriteImage* pImage, const VECTOR2& pos, const VECTOR4& srcRect);

    void Draw(CSprite& sprite);

    void SetImage(CSpriteImage* pImage);
    /// @brief テクスチャの切り抜き範囲を設定する
    /// @param srcRect (srcX, srcY, srcWidth, srcHeight)
    void SetSrcRect(const VECTOR4& srcRect);
    CSpriteImage* GetImage();
    VECTOR4 GetSrcRect() const;

private:
    CSpriteImage* m_pImage;
    VECTOR4 m_srcRect; // テクスチャの切り抜き範囲 (srcX, srcY, srcWidth, srcHeight)
};
