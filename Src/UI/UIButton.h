#pragma once
#include <functional>

#include "UIImage.h"

/// @brief フォーカス状態を持つUIボタン
/// @details フォーカスのON/OFFで画像を切り替え、アニメーションPlayerを通じてアニメも再生できる
class CUIButton : public CUIImage
{
public:
    /// @param pNormalImage 通常時の画像（nullptr可）
    /// @param pFocusImage フォーカス時の画像（nullptr可）
    CUIButton(const VECTOR2& pos, const VECTOR4& size, CSpriteImage* pNormalImage = nullptr, CSpriteImage* pFocusImage = nullptr);

    /// @brief スプライトシート用コンストラクタ（通常・フォーカス時の切り抜き範囲を個別指定）
    /// @param normalSrcRect  通常時の切り抜き範囲 (srcX, srcY, srcW, srcH)
    /// @param focusSrcRect   フォーカス時の切り抜き範囲 (srcX, srcY, srcW, srcH)
    CUIButton(const VECTOR2& pos, CSpriteImage* pImage,
              const VECTOR4& normalSrcRect, const VECTOR4& focusSrcRect);

    /// @brief 背景なしコンストラクタ（フォーカス時のみ画像を表示）
    /// @param focusSrcRect フォーカス時の切り抜き範囲 (srcX, srcY, srcW, srcH)
    CUIButton(const VECTOR2& pos, CSpriteImage* pImage, const VECTOR4& focusSrcRect);

    /// @brief フォーカス状態を設定する
    /// @details フォーカスOFF時はアニメーション完了後に通常画像へ戻る
    void SetFocus(bool focused);
    bool IsFocus() const;

    /// @brief フォーカス状態が変化したときのコールバックを設定する
    /// @param callback 引数はフォーカス後の状態（true=フォーカスON）
    void SetOnFocusChanged(std::function<void(bool)> callback);

private:
    std::function<void(bool)> m_onFocusChanged = nullptr; // フォーカス変化時コールバック

    float m_normalAlpha = 1.0f;
    float m_focusAlpha = 1.0f;
    bool m_isFocus = false;
    bool m_useSpriteSheet = false;          // スプライトシートモードかどうか
    CSpriteImage* m_pNormalImage = nullptr; // 通常時の画像（別ファイルモード）
    CSpriteImage* m_pFocusImage = nullptr;  // フォーカス時の画像（別ファイルモード）
    VECTOR4 m_normalSrcRect = {};           // 通常時の切り抜き範囲（スプライトシートモード）
    VECTOR4 m_focusSrcRect = {};            // フォーカス時の切り抜き範囲（スプライトシートモード）

};
