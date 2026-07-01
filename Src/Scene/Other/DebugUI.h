#pragma once
#include "../../UI/UIBase.h"
#include "../../UI/UIButton.h"
#include "../../UI/UIButtons.h"
#include "../../UI/UIProgressBar.h"
#include "../../UI/UIRingGauge.h"

/// <summary>シーンで使う Debug UI の情報と処理をまとめる型</summary>
class DebugUI : public CUIBase
{
public:
    /// Bar を追加する
    void AddBar();
    /// DebugUI を初期化する
    DebugUI();
    /// 毎フレームの状態を更新する
    void Update();
    /// 描画する
    void Draw();

private:
    /// Image を追加する
    void AddImage();
    /// Button を追加する
    void AddButton();
    /// Buttons を追加する
    void AddButtons();
    /// Circle を追加する
    void AddCircle();

private:
    CUIButton* m_pButton;
    CUIButtons m_buttons;
    CUIProgressBar* m_pBar;
    CUIRingGauge* m_pRingGauge;
};
