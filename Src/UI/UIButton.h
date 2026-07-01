#pragma once
#include <functional>

#include "UIImage.h"

/// @brief フォーカス状態を持つUIボタン
/// @details フォーカスのON/OFFで画像を切り替え、アニメーションPlayerを通じてアニメも再生できる
class CUIButton : public CUIImage
{
public:
    /// CUIButton を初期化する
    /// @param pos 座標
    /// @param size サイズ
    /// @param pNormalImage pNormalImage に渡す値
    /// @param pFocusImage pFocusImage に渡す値
    CUIButton(const VECTOR2& pos, const VECTOR4& size, CSpriteImage* pNormalImage = nullptr,
              CSpriteImage* pFocusImage = nullptr);

    /// CUIButton を初期化する
    /// @param pos 座標
    /// @param pImage pImage に渡す値
    /// @param normalSrcRect normalSrcRect に渡す値
    /// @param focusSrcRect focusSrcRect に渡す値
    CUIButton(const VECTOR2& pos, CSpriteImage* pImage,
              const VECTOR4& normalSrcRect, const VECTOR4& focusSrcRect);

    /// CUIButton を初期化する
    /// @param pos 座標
    /// @param pImage pImage に渡す値
    /// @param focusSrcRect focusSrcRect に渡す値
    CUIButton(const VECTOR2& pos, CSpriteImage* pImage, const VECTOR4& focusSrcRect);

    /// Focus を設定する
    /// @param focused focused に渡す値
    void SetFocus(bool focused);
    /// Focus を判定する
    /// @return 成功または条件を満たす場合 true
    bool IsFocus() const;

    /// On Focus Changed を設定する
    /// @param callback callback に渡す値
    void SetOnFocusChanged(std::function<void(bool)> callback);

private:
    /// function<void を返す
    std::function<void(bool)> m_onFocusChanged = nullptr; // フォーカス変化時コールバック

    float m_normalAlpha = 1.0f;
    float m_focusAlpha = 1.0f;
    bool m_isFocus = false;
    bool m_useSpriteSheet = false; // スプライトシートモードかどうか
    CSpriteImage* m_pNormalImage = nullptr; // 通常時の画像（別ファイルモード）
    CSpriteImage* m_pFocusImage = nullptr; // フォーカス時の画像（別ファイルモード）
    VECTOR4 m_normalSrcRect = {}; // 通常時の切り抜き範囲（スプライトシートモード）
    VECTOR4 m_focusSrcRect = {}; // フォーカス時の切り抜き範囲（スプライトシートモード）
};
