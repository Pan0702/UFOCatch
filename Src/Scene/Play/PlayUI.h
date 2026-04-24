#pragma once
#include "../../UI/UIBase.h"
#include "../../UI/UIProgressBar.h"
#include "../../UI/UIRingGauge.h"
#include "../../Utils/MyMath.h"

class CUIText;

class CPlayUI : public CUIBase
{
public:
    CPlayUI();
    void Update() override;
    CUIProgressBar* GetBar() const;
    CUIRingGauge* GetGauge() const;
    CUIImage* GetLookImage() const;

private:
    void Gauge();
    void Bar();
    void Image();
    void LookImage();
    void Digits();
    void UpdateTimeDigits();
    void UpdateHPDigits();
    void UpdateLvDigits();
    void UpdateCutIn();
    void UpdateGauge();
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
