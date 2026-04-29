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
    void DrawHalfFlockCircles() const;

    void DrawFlockCircles() const;
    void DrawMaxFlockCircles() const;
    void UpdateImguiPanel();

    DebugUI* m_pDebugUI;
    CPlayUI* m_pPlayUI;
    CAShepherdDog* m_pDog;

    // ImGui の Flock 追加フォーム入力値
    float m_newFlockCenterX = 20.0f;
    float m_newFlockCenterZ = 0.0f;
    float m_newFlockRadius = 5.0f;
    int m_newFlockSheepCount = 5;
};
