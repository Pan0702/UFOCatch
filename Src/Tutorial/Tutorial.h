#pragma once
#include <unordered_map>

#include "TutorialState.h"
#include "../Common/Object3D.h"

// チュートリアルの進行を管理するクラス //
class CTutorial : public Object3D
{
public:
    CTutorial();

    // チュートリアルのステートを変更する
    // @param state 変更先のステート //
    void SetState(CTutorialState::State state);

    // チュートリアル用の動物を複数体生成する //
    void SpawnAnimal();

    // チュートリアル用の人間と動物を生成する //
    void SpawnHuman();

    // 実践形式のレッスンを開始する //
    void Lesson();

private:
    ~CTutorial();
    void Update() override;

    CTutorialState* m_pCurrentState;
    std::unordered_map<CTutorialState::State, CTutorialState*> m_states;
};
