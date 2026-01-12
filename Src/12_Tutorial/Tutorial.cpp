#include "Tutorial.h"

#include "TutorialAnimal.h"
#include "TutorialHuman.h"
#include "../GameInstance.h"
#include "../08_Player/Player.h"
#include "../09_Enemy/Human/Human.h"
#include "../09_Enemy/System/AnimalManager.h"
#include "../11_GameSystem/Timer.h"

CTutorial::CTutorial()
{
    m_states[CTutorialState::State::Move] = new CMoveState(this);
    m_states[CTutorialState::State::Expands] = new CExpands(this);
    m_states[CTutorialState::State::Discovery] = new CDiscoveryState(this);
    m_states[CTutorialState::State::Suction] = new CSuctionState(this);
    m_pCurrentState = m_states[CTutorialState::State::Move];
    
    new CTutorialAnimal(VECTOR3(0,0,5.0f));
}

CTutorial::~CTutorial()
{
    for (auto state : m_states)
    {
        SAFE_DELETE(state.second);
    }
}

void CTutorial::Update()
{
    if (m_pCurrentState != nullptr)
    {
        m_pCurrentState->Update();
    }
}

////////////////////
// チュートリアルのステートを変更する
// @param state 変更先のステート //
////////////////////
void CTutorial::SetState(CTutorialState::State state)
{
    m_pCurrentState->Exit();
    m_pCurrentState = m_states[state];
    m_pCurrentState->Enter();
}

////////////////////
// チュートリアル用の動物を複数体生成する //
////////////////////
void CTutorial::SpawnAnimal()
{
    for (int i = 0; i < 8; i++)
    {
        int randomX = Randomf(-10.0f,10.0f);
        int randomZ = Randomf(-10.0f, 10.0f);
        new CTutorialAnimal(VECTOR3(randomX,0,randomZ));
    }
}

////////////////////
// チュートリアル用の人間と動物を生成する //
////////////////////
void CTutorial::SpawnHuman()
{
    new CTutorialHuman(VECTOR3(0,0,6.0f));
    new CTutorialAnimal(VECTOR3(0.0f,0,5.0f));
}

////////////////////
// 実践形式のレッスンを開始する //
////////////////////
void CTutorial::Lesson()
{
    new CAnimalManager();
    new CTimer(30);
    int randomX = Randomf(-10.0f,10.0f);
    int randomZ = Randomf(-10.0f, 10.0f);
    new CHuman(VECTOR3(randomX,0,randomZ));
    for (int i = 0; i < 6; i++)
    {
        randomX = Randomf(-10.0f,10.0f);
        randomZ = Randomf(-10.0f, 10.0f);
        new CTutorialAnimal(VECTOR3(randomX,0,randomZ));
    }

}
