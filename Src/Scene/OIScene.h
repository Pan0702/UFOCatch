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
    std::unique_ptr<CSpriteImage> m_pSpriteImage;
};
