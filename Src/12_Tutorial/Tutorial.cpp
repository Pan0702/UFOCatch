#include "Tutorial.h"

#include "TutorialAnimal.h"
#include "TutorialHuman.h"
#include "../GameInstance.h"
#include "../08_Player/Player.h"
#include "../09_Enemy/Human/Human.h"
#include "../09_Enemy/System/AnimalManager.h"
#include "../11_GameSystem/Timer.h"

namespace
{
    // 動物のスポーン範囲 //
    constexpr float SPAWN_RANGE_MIN = -10.0f;
    constexpr float SPAWN_RANGE_MAX = 10.0f;
}

CTutorial::CTutorial()
{
    // 各ステートを生成してマップに登録 //
    m_states[CTutorialState::State::Move] = new CMoveState(this);
    m_states[CTutorialState::State::Expands] = new CExpands(this);
    m_states[CTutorialState::State::Discovery] = new CDiscoveryState(this);
    m_states[CTutorialState::State::Suction] = new CSuctionState(this);
    m_states[CTutorialState::State::Play] = new CPlayState(this);
    m_pCurrentState = m_states[CTutorialState::State::Move];

    // 最初のチュートリアル用動物を生成 //
    static constexpr float INITIAL_ANIMAL_Z = 5.0f;
    new CTutorialAnimal(VECTOR3(0, 0, INITIAL_ANIMAL_Z));
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
    static constexpr int SPAWN_COUNT = 4;
    for (int i = 0; i < SPAWN_COUNT; i++)
    {
        float randomX = Randomf(SPAWN_RANGE_MIN, SPAWN_RANGE_MAX);
        float randomZ = Randomf(SPAWN_RANGE_MIN, SPAWN_RANGE_MAX);
        new CTutorialAnimal(VECTOR3(randomX, 0, randomZ));
    }
}

////////////////////
// チュートリアル用の人間と動物を生成する //
////////////////////
void CTutorial::SpawnHuman()
{
    static constexpr float HUMAN_SPAWN_Z = 6.0f;
    static constexpr float ANIMAL_SPAWN_Z = 5.0f;
    new CTutorialHuman(VECTOR3(0, 0, HUMAN_SPAWN_Z));
    new CTutorialAnimal(VECTOR3(0, 0, ANIMAL_SPAWN_Z));
}

////////////////////
// 実践形式のレッスンを開始する //
////////////////////
void CTutorial::Lesson()
{
    static constexpr int LESSON_TIME_SECONDS = 30;
    static constexpr int LESSON_ANIMAL_COUNT = 5;

    new CAnimalManager();
    new CTimer(LESSON_TIME_SECONDS);

    // 人間を1体生成 //
    float randomX = Randomf(SPAWN_RANGE_MIN, SPAWN_RANGE_MAX);
    float randomZ = Randomf(SPAWN_RANGE_MIN, SPAWN_RANGE_MAX);
    new CHuman(VECTOR3(randomX, 0, randomZ));

    // 動物を複数体生成 //
    for (int i = 0; i < LESSON_ANIMAL_COUNT; i++)
    {
        randomX = Randomf(SPAWN_RANGE_MIN, SPAWN_RANGE_MAX);
        randomZ = Randomf(SPAWN_RANGE_MIN, SPAWN_RANGE_MAX);
        new CTutorialAnimal(VECTOR3(randomX, 0, randomZ));
    }
}
