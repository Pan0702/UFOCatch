#pragma once
#include "../../UI/UIBase.h"
#include "../../UI/UIProgressBar.h"
#include "../../UI/UIRingGauge.h"
#include "../../Utils/MyMath.h"

/// <summary>シーンで使う UIText の情報と処理をまとめる型</summary>
class CUIText;

/// <summary>シーンで使う Play UI の情報と処理をまとめる型</summary>
class CPlayUI : public CUIBase
{
public:
    /// CPlayUI を初期化する
    CPlayUI();
    /// 毎フレームの状態を更新する
    void Update() override;
    /// Bar を取得する
    /// @return 対象のポインタ
    CUIProgressBar* GetBar() const;
    /// Gauge を取得する
    /// @return 対象のポインタ
    CUIRingGauge* GetGauge() const;
    /// Look Image を取得する
    /// @return 対象のポインタ
    CUIImage* GetLookImage() const;

private:
    /// Gauge の処理を行う
    void Gauge();
    /// Bar の処理を行う
    void Bar();
    /// Image の処理を行う
    void Image();
    /// Look Image の処理を行う
    void LookImage();
    /// Digits の処理を行う
    void Digits();
    /// Time Digits を毎フレームの状態を更新する
    void UpdateTimeDigits();
    /// HPDigits を毎フレームの状態を更新する
    void UpdateHPDigits();
    /// Lv Digits を毎フレームの状態を更新する
    void UpdateLvDigits();
    /// Cut In を毎フレームの状態を更新する
    void UpdateCutIn();
    /// Gauge を毎フレームの状態を更新する
    void UpdateGauge();
    /// Bar を毎フレームの状態を更新する
    void UpdateBar();

    static constexpr int TIME_MAX_DIGITS = 4;
    static constexpr int LV_MAX_DIGITS = 3;

    CSpriteImage* m_pUI;
    CSpriteImage* m_pLogo;
    CUIProgressBar* m_pBar = nullptr;
    CUIRingGauge* m_pGauge = nullptr;
    CUIImage* m_pLookImage = nullptr;
    CUIImage* m_pCutIn = nullptr;
    CUIText* m_pTimeDigits[TIME_MAX_DIGITS] = {};
    CUIText* m_pHpCurrent = nullptr;
    CUIText* m_pHpMax = nullptr;
    CUIText* m_pLvDigits[LV_MAX_DIGITS] = {};
    bool flag = false;
    bool m_prevFound = false;
    float m_prevRaito = 0.0f;
};
