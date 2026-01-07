#pragma once
#include <unordered_map>

#include "TutorialState.h"
#include "../05_CommonFile/Object3D.h"

class CTutorial : public Object3D
{
public:
    CTutorial();
    void SetState(CTutorialState::State state);
    void SpawnAnimal();
    void SpawnHuman();
    void Lesson();
private:
    ~CTutorial();
    void Update() override;
   

    CTutorialState* m_pCurrentState;
    std::unordered_map<CTutorialState::State, CTutorialState*> m_states;
};
