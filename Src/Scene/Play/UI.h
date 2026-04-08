#pragma once
#include "../../UI/UIBase.h"
#include "../../UI/UIProgressBar.h"
#include "../../UI/UIRingGauge.h"
#include "../../Utils/MyMath.h"

class PlayUI : public CUIBase
{
public:
    PlayUI();
    void Update() override;
    CUIProgressBar* GetBar() const;
    CUIRingGauge* GetGauge() const;
    CUIImage* GetLookImage() const;
private:
    void Gauge();
    void Bar();
    void Image();
    void LookImage();


    CSpriteImage* m_pUI;
    CUIProgressBar* m_pBar;
    CUIRingGauge* m_pGauge;
    CUIImage* m_pLookImage;
    bool flag = false;
};
