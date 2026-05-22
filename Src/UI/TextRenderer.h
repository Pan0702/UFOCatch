#pragma once
#include "UIBase.h"
#include "../Utils/MyMath.h"

/// @brief スプライトフォントから1文字分を切り抜いて描画するウィジェット
/// @details SetSrcPos()でフォントシート上の座標を変えることで表示文字を切り替える。
///          SetSize()で描画サイズを指定するとスケーリングされる（未指定時はsrcSizeと同じ）//
class CUIText : public CUIWidget
{
public:
    /// CUIText を初期化する
    /// @param pFont pFont に渡す値
    /// @param pos 座標
    /// @param srcPos 座標
    /// @param srcSize サイズ
    CUIText(CSpriteImage* pFont, const VECTOR2& pos, const VECTOR2& srcPos, const VECTOR2& srcSize);

    /// Src Pos を設定する
    /// @param srcPos 座標
    void SetSrcPos(const VECTOR2& srcPos);

    /// 描画する
    /// @param sprite sprite に渡す値
    void Draw(CSprite& sprite) override;

private:
    CSpriteImage* m_pFont;
    VECTOR2 m_srcPos; // フォントシート上の切り抜き開始座標//
    VECTOR2 m_srcSize; // フォントシート上の1文字サイズ//
};
