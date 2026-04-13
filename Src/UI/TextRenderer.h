#pragma once
#include "UIBase.h"

/// @brief スプライトフォントから1文字分を切り抜いて描画するウィジェット
/// @details フォントシートのUV座標(srcPos)を変えることで表示文字を切り替える
class CUIText : public CUIWidget
{
public:
    /// @param pFont フォントシートのテクスチャ
    /// @param srcPos テクスチャ上の切り抜き開始座標 (srcX, srcY)
    CUIText(CSpriteImage* pFont, const VECTOR2& pos, const VECTOR2& size, const VECTOR2& srcPos);

    /// @brief 表示する文字のテクスチャ座標を変更する
    /// @param srcPos テクスチャ上の切り抜き開始座標 (srcX, srcY)
    void SetSrcPos(const VECTOR2& srcPos);
    void Draw(CSprite& sprite) override;

private:
    CSpriteImage* m_pFont;
    VECTOR2 m_srcPos; // フォントシート上の切り抜き開始座標
};
