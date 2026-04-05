#pragma once
#include "UIButton.h"
#include "UICanvas.h"

class CUIBase
{
public:
    CUICanvas& GetCanvas();
protected:
    CUICanvas m_canvas;
};
