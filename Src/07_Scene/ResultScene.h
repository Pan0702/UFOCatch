#pragma once
#include "../04_FrameWork/SceneBase.h"
class CResultScene : public SceneBase
{
public:
    CResultScene();
    ~CResultScene();
    private:
    void Draw() override;
};
