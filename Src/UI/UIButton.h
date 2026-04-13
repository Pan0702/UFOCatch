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
    CSpriteImage* m_pNormalImage = nullptr; // 通常時の画像
    CSpriteImage* m_pFocusImage = nullptr;  // フォーカス時の画像

};
