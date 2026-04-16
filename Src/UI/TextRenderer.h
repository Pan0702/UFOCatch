#pragma once
#include "UIBase.h"

/// @brief スプライトフォントから1文字分を切り抜いて描画するウィジェット
/// @details フォントシートのUV座標(srcPos)を変えることで表示文字を切り替える
class CUIText : public CUIWidget
{
public:
    /// @param pFont フォントシートのテクスチャ
    /// @param pos 描画位置
    /// @param srcPos テクスチャ上の切り抜き開始座標 (srcX, srcY)
    /// @param size 描画サイズ
    CUIText(CSpriteImage* pFont, const VECTOR2& pos, const VECTOR2& srcPos, const VECTOR2& size);

    /// @brief 表示する文字のテクスチャ座標を変更する
    /// @param Size テクスチャ上の切り抜き開始座標 (srcX, srcY)
    void SetSize(const VECTOR2& Size);
    void Draw(CSprite& sprite) override;

private:
    CSpriteImage* m_pFont;
    VECTOR2 m_srcPos; // フォントシート上の切り抜き開始座標
    VECTOR2 m_srcSize;  
};
