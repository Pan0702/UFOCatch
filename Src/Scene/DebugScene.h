#pragma once
#include "../Framework/SceneBase.h"

class CDebugScene : public SceneBase
{
public:
    CDebugScene();
    ~CDebugScene();

    // リザルトシーンへ遷移する //
    void ChangeResultScene();
private:
    void Update() override;
    void Draw() override;
};


