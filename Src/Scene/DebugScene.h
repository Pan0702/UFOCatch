#pragma once
#include "../Framework/SceneBase.h"

class CDebugScene : public SceneBase
{
public:
    CDebugScene();
    ~CDebugScene();

    // 繝ｪ繧ｶ繝ｫ繝医す繝ｼ繝ｳ縺ｸ驕ｷ遘ｻ縺吶ｋ //
    void ChangeResultScene();
private:
    void Update() override;
    void Draw() override;
};

