#pragma once
#include "../../UI/UIBase.h"
#include "../../UI/UIProgressBar.h"
#include "../../UI/UIRingGauge.h"

class PlayUI : public CUIBase
{
public:
    PlayUI();
    ~PlayUI();

private:
    void InitButton();
    void InitImage();
    void InitBar();

private:
    CUIProgressBar* m_pBar;
    CUIRingGauge* m_pRingGauge;
    CSpriteImage* m_pImage;
};
