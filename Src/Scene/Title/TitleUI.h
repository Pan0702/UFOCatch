#pragma once
#include "../../UI/UIBase.h"
#include "../../UI/UIButtons.h"

/// <summary>シーンで使う Title UI の情報と処理をまとめる型</summary>
class CTitleUI : public CUIBase
{
public:
    /// CTitleUI を初期化する
    CTitleUI();

private:
    /// Buttons を初期化する
    void InitButtons();
    /// Background を初期化する
    void InitBackground();
    /// 毎フレームの状態を更新する
    void Update() override;
    CUIButtons m_buttons;
    std::vector<std::string> m_sceneNames;
};
