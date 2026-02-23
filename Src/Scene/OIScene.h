#pragma once
#include "../Framework/SceneBase.h"
#include "../Utils/Sprite3D.h"

class OIScene : public SceneBase
{
public:
    OIScene();

    
private:
    void Draw() override;
    void Update() override;
    CSpriteImage* m_pSpriteImage;
};
