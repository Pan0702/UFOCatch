#pragma once
#include "UIButton.h"
#include "UICanvas.h"

class CUIBase
{
public:
    CUICanvas& GetCanvas();
    virtual void Update();
    virtual void Draw();
protected:
    CUICanvas m_canvas;
};
