#pragma once
#include "../../UI/UIBase.h"
#include "../../UI/UIButtons.h"

class CSelectUI : public CUIBase
{
public:
    CSelectUI();
    
private:
    void InitButtons();
    void BackImage();
    void Update() override;
    CUIButtons m_buttons;
    int m_selectIndex;
    std::vector<std::string> m_sceneNames;
};
