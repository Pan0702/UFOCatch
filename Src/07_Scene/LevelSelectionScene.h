#pragma once
#include "../04_FrameWork/SceneBase.h"

class CLevelSelectionScene : public SceneBase
{
public:
    CLevelSelectionScene();
private:
    void InitImage();
    ~CLevelSelectionScene();
    void Update() override;
    void Draw() override;
    void MoveToTop();
    void MoveToEnd();
    

private:
    std::vector<CSpriteImage*> m_pLevelImages;

};
