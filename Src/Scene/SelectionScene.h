#pragma once
#include "../Framework/SceneBase.h"

class CSelectionScene : public SceneBase
{
public:
    CSelectionScene();
    ~CSelectionScene();

private:
    void Update() override;
    void Draw() override;
    void InitButtons();

    //Buttonに関する情報
    struct ButtomInfo
    {
        CSpriteImage* image;
        std::string sceneName;
    };
    std::vector<ButtomInfo> m_buttons;
    CSpriteImage* m_pImageBackGround;
    int m_selectedIndex;
};
