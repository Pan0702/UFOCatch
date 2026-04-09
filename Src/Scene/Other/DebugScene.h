#pragma once
#include "DebugUI.h"
#include "../../Framework/SceneBase.h"
#include "../Play/PlayUI.h"
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
    DebugUI* m_pDebugUI;
    CPlayUI* m_pPlayUI;
};


