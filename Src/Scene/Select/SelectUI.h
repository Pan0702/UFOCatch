#pragma once
#include "../../UI/UIBase.h"
#include "../../UI/UIButtons.h"

class CSelectUI : public CUIBase
{
public:
    CSelectUI();
    
private:
    void InitButtons();
    CUIButtons m_buttons;
};
