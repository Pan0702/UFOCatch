#pragma once
#include "DebugUI.h"
#include "../../Framework/SceneBase.h"
#include "../Play/PlayUI.h"
#include "../../Enemies/AnimalDog/ShepherdDog.h"

/// <summary>シーンで使う Debug Scene の情報と処理をまとめる型</summary>
class CDebugScene : public SceneBase
{
public:
    /// CDebugScene を初期化する
    CDebugScene();
    /// CDebugScene の終了処理を行う
    ~CDebugScene();

    // リザルトシーンへ遷移する //
    /// Result Scene を切り替える
    void ChangeResultScene();

private:
    /// 毎フレームの状態を更新する
    void Update() override;
    /// 描画する
    void Draw() override;

    /// Flock Circles を描画する
    void DrawFlockCircles() const;
    /// Max Flock Circles を描画する
    void DrawMaxFlockCircles() const;
    /// Imgui Panel を毎フレームの状態を更新する
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
