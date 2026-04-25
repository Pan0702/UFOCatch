#pragma once
#include "DebugUI.h"
#include "../../Framework/SceneBase.h"
#include "../Play/PlayUI.h"
#include "../../Enemies/AnimalDog/ShepherdDog.h"

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

    void DrawFlogCircles() const;
    void UpdateImguiPanel();

    DebugUI* m_pDebugUI;
    CPlayUI* m_pPlayUI;
    CAShepherdDog* m_pDog;

    // ImGui の Flog 追加フォーム入力値
    float m_newFlogCenterX = 20.0f;
    float m_newFlogCenterZ = 0.0f;
    float m_newFlogRadius = 5.0f;
    int m_newFlogSheepCount = 5;
};
