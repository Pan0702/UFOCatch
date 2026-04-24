#pragma once
#include "../../UI/UIBase.h"
#include "../../UI/UIButtons.h"

class CSelectUI : public CUIBase
{
public:
    CSelectUI();
    
private:
    /// @brief buttonの初期化処理
    void InitButtons();
    
    /// @brief バックイメージの初期化処理
    void BackImage();
    void Update() override;
    
    CUIButtons m_buttons;
    std::vector<std::string> m_sceneNames;
};
