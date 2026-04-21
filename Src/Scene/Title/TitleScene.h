#pragma once
#include "../../Framework/SceneBase.h"
#include "../../Utils/WipeAnimator.h"

// 繧ｲ繝ｼ繝襍ｷ蜍墓凾縺ｮ繧ｿ繧､繝医Ν繧ｷ繝ｼ繝ｳ //
class TitleScene : public SceneBase
{
public:
    TitleScene();
    ~TitleScene();

private:
    void Update() override;
    void Draw() override;
};
