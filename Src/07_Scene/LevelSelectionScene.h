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
    

private:
    std::vector<CSpriteImage*> m_pLevelImages;
    int m_selectedIndex;
    float m_animationOffset; // アニメーション用オフセット
    float m_targetOffset;

};
