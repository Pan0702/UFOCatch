#pragma once
#include "../../Framework/SceneBase.h"
#include "../../Utils/WipeAnimator.h"

class CSelectionScene : public SceneBase
{
public:
    CSelectionScene();
    ~CSelectionScene();

private:
    void Update() override;
    void Draw() override;
};
