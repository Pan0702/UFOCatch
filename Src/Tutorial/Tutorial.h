#pragma once
#include <unordered_map>
#include <memory> // unique_ptr のために必要

#include "TutorialState.h"
#include "../Common/Object3D.h"

// チュートリアルの進行を管理するクラス //
class CTutorial : public Object3D
{
public:
    /// CTutorial を初期化する
    CTutorial();

    // チュートリアルのステートを変更する
    // @param state 変更先のステート
    /// State を設定する
    /// @param state 状態
    void SetState(CTutorialState::State state);

    // チュートリアル用の動物を複数体生成する //
    /// Spawn Animal の処理を行う
    void SpawnAnimal();

    // チュートリアル用の人間と動物を生成する //
    /// Spawn Human の処理を行う
    void SpawnHuman();

    // 実践形式のレッスンを開始する //
    /// Lesson の処理を行う
    void Lesson();

private:
    /// 毎フレームの状態を更新する
    void Update() override;

    CTutorialState* m_pCurrentState;
    std::unordered_map<CTutorialState::State, std::unique_ptr<CTutorialState>> m_states;
};
