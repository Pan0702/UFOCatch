#pragma once
#include "UIWidget.h"

/// @brief テクスチャを描画するウィジェット
/// @details スプライトシートの切り抜き範囲（srcRect）を指定して描画できる
class CUIImage : public CUIWidget
{
public:
    /// CUIImage を初期化する
    /// @param pImage pImage に渡す値
    /// @param pos 座標
    /// @param size サイズ
    CUIImage(CSpriteImage* pImage, const VECTOR2& pos, const VECTOR2& size);

    /// CUIImage を初期化する
    /// @param pImage pImage に渡す値
    /// @param pos 座標
    /// @param srcRect srcRect に渡す値
    CUIImage(CSpriteImage* pImage, const VECTOR2& pos, const VECTOR4& srcRect);

    /// 描画する
    /// @param sprite sprite に渡す値
    void Draw(CSprite& sprite);

    /// Image を設定する
    /// @param pImage pImage に渡す値
    void SetImage(CSpriteImage* pImage);
    /// Src Rect を設定する
    /// @param srcRect srcRect に渡す値
    void SetSrcRect(const VECTOR4& srcRect);
    /// Image を取得する
    /// @return 対象のポインタ
    CSpriteImage* GetImage();
    /// Src Rect を取得する
    /// @return 4次元ベクトル
    VECTOR4 GetSrcRect() const;

private:
    CSpriteImage* m_pImage;
    VECTOR4 m_srcRect; // テクスチャの切り抜き範囲 (srcX, srcY, srcWidth, srcHeight)
};
