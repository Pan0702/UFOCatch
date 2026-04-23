#pragma once
#include "UIBase.h"
#include "../Utils/MyMath.h"

/// @brief スプライトフォントから1文字分を切り抜いて描画するウィジェット
/// @details SetSrcPos()でフォントシート上の座標を変えることで表示文字を切り替える。
///          SetSize()で描画サイズを指定するとスケーリングされる（未指定時はsrcSizeと同じ）//
class CUIText : public CUIWidget
{
public:
    /// @param pFont    フォントシートのテクスチャ
    /// @param pos      描画位置
    /// @param srcPos   フォントシート上の切り抜き開始座標 (srcX, srcY)
    /// @param srcSize  フォントシート上の1文字のサイズ (srcW, srcH)//
    CUIText(CSpriteImage* pFont, const VECTOR2& pos, const VECTOR2& srcPos, const VECTOR2& srcSize);

    /// @brief 表示する文字のフォントシート座標を変更する//
    void SetSrcPos(const VECTOR2& srcPos);

    void Draw(CSprite& sprite) override;

private:
    CSpriteImage* m_pFont;
    VECTOR2 m_srcPos; // フォントシート上の切り抜き開始座標//
    VECTOR2 m_srcSize; // フォントシート上の1文字サイズ//
};
