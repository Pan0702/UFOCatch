#pragma once
#include "UIBase.h"
#include "UIButton.h"
#include "UIImage.h"

/// @brief 複数のCUIButtonをまとめて管理し、フォーカス移動を制御するクラス
class CUIButtons
{
public:
    CUIButtons();

    /// @brief ボタンを追加してキャンバスに登録する（別ファイルモード）
    /// @param ui 追加先のUIBase
    /// @param name ImageRegistryに登録済みのテクスチャ名
    /// @param pos 配置位置
    void AddButton(CUIBase* ui, const std::string& name, const VECTOR2& pos);

    /// @brief ボタンを追加してキャンバスに登録する（スプライトシートモード）
    /// @param ui 追加先のUIBase
    /// @param pImage スプライトシートのテクスチャ
    /// @param pos 配置位置
    /// @param normalSrcRect 通常時の切り抜き範囲 (srcX, srcY, srcW, srcH)
    /// @param focusSrcRect  フォーカス時の切り抜き範囲 (srcX, srcY, srcW, srcH)
    void AddButton(CUIBase* ui, CSpriteImage* pImage, const VECTOR2& pos,
                   const VECTOR4& normalSrcRect, const VECTOR4& focusSrcRect);

    /// @brief 全ボタンにフォーカス・アンフォーカスアニメーションを設定する
    /// @param Foucus フォーカス時のアニメーション
    /// @param unFocus アンフォーカス時のアニメーション
    void SetAnim(const std::shared_ptr<CUIAnimation>& Foucus, const std::shared_ptr<CUIAnimation>& unFocus) const;

    /// @brief フォーカスをn個分移動する（範囲外はクランプ）
    /// @param n 正で下/右方向、負で上/左方向
    void MoveFocus(int n);

    /// @brief ボタン画像サイズを設定する
    void SetImageSize(const VECTOR4& size);

    /// @brief 指定インデックスのボタンにフォーカスを設定する
    void SetFocus(int index);

private:
    std::vector<CUIButton*> m_buttons; // 管理中のボタン一覧（所有しない）
    int m_focusIndex;                  // 現在フォーカス中のインデックス
    VECTOR4 m_imageSize;               // ボタン画像サイズ (srcX, srcY, srcWidth, srcHeight)
};
