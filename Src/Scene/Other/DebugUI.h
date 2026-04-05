#pragma once
#include "../../UI/UIBase.h"
#include "../../UI/UIButton.h"
#include "../../UI/UIButtons.h"
#include "../../UI/UIProgressBar.h"

class DebugUI : public CUIBase
{
public:
    void AddBar();
    DebugUI();
    void Update();
    void Draw();
private:
    void AddImage();
    void AddButton();
    void AddButtons();
private:
    CUIButton* m_pButton;
    CUIButtons m_buttons;
    CUIProgressBar* m_pBar;
};
