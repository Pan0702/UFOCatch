#pragma once
#include "UIBase.h"
#include "UIButton.h"
#include "UIImage.h"

/// @brief 複数のCUIButtonをまとめて管理し、フォーカス移動を制御するクラス
class CUIButtons
{
public:
    /// CUIButtons を初期化する
    CUIButtons();

    /// Button を追加する
    /// @param ui ui に渡す値
    /// @param name 名前
    /// @param pos 座標
    void AddButton(CUIBase* ui, const std::string& name, const VECTOR2& pos);

    /// Button を追加する
    /// @param ui ui に渡す値
    /// @param pImage pImage に渡す値
    /// @param pos 座標
    /// @param normalSrcRect normalSrcRect に渡す値
    /// @param focusSrcRect focusSrcRect に渡す値
    void AddButton(CUIBase* ui, CSpriteImage* pImage, const VECTOR2& pos,
                   const VECTOR4& normalSrcRect, const VECTOR4& focusSrcRect);

    /// Button を追加する
    /// @param ui ui に渡す値
    /// @param pImage pImage に渡す値
    /// @param pos 座標
    /// @param focusSrcRect focusSrcRect に渡す値
    void AddButton(CUIBase* ui, CSpriteImage* pImage, const VECTOR2& pos,
                   const VECTOR4& focusSrcRect);

    /// Anim を設定する
    /// @param Foucus Foucus に渡す値
    /// @param unFocus unFocus に渡す値
    void SetAnim(const std::shared_ptr<CUIAnimation>& Foucus, const std::shared_ptr<CUIAnimation>& unFocus) const;

    /// Focus を移動する
    /// @param n n に渡す値
    void MoveFocus(int n);

    /// Image Size を設定する
    /// @param size サイズ
    void SetImageSize(const VECTOR4& size);

    /// Focus を設定する
    /// @param index インデックス
    void SetFocus(int index);

    /// Layer を設定する
    /// @param layer layer に渡す値
    void SetLayer(int layer);

    /// Alpha を設定する
    /// @param alpha 透明度
    void SetAlpha(float alpha);

    /// Focus Index を取得する
    /// @return 処理結果の数値
    int GetFocusIndex() const;

private:
    std::vector<CUIButton*> m_buttons; // 管理中のボタン一覧（所有しない）
    int m_focusIndex; // 現在フォーカス中のインデックス
    VECTOR4 m_imageSize; // ボタン画像サイズ (srcX, srcY, srcWidth, srcHeight)
};
