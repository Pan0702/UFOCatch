#pragma once
#include "../../UI/UICanvas.h"
#include "../../UI/UIButton.h"

class DebugUI
{
public:
    DebugUI();
    void Update();
    void Draw();

private:
    CUICanvas m_canvas;
    CUIButton* m_pButton;
};
