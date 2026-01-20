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
    CSprite* m_pSprite;
    CSpriteImage* m_pImageBackGround;
    CSpriteImage* m_pImageDoButton;
    CSpriteImage* m_pImageTutorialButton;
    int m_selectedIndex;
};
