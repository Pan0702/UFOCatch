#pragma once
#include "../../UI/UIBase.h"
#include "../../UI/UIButtons.h"

class CTitleUI : public CUIBase
{
public:
    CTitleUI();
    
private:
    void InitButtons();
    void InitBackground();
    void Update() override;
    CUIButtons m_buttons;
    std::vector<std::string> m_sceneNames;
};
