#pragma once
#include "../../Framework/SceneBase.h"

class CNormalScene : public SceneBase   
{
public:
    CNormalScene();
    ~CNormalScene();
    void Update() override;
    void Draw() override;

private:
};
