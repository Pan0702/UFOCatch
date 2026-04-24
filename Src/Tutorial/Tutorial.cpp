#include "Tutorial.h"

#include "TutorialAnimal.h"
#include "TutorialHuman.h"
#include "../System/GameInstance.h"
#include "../Enemies/Human/Human.h"
#include "../Enemies/System/EnemyManager.h"
#include "../System/Timer.h"

namespace
{
    // 動物のスポーン範囲 //
    constexpr float SPAWN_RANGE_MIN = -10.0f;
    constexpr float SPAWN_RANGE_MAX = 10.0f;
}

CTutorial::CTutorial()
{
    // 各ステートを生成してマップに登録 //
    m_states[CTutorialState::State::Move] = std::make_unique< CMoveState>(this);
    m_states[CTutorialState::State::Expands] = std::make_unique< CExpands>(this);
    m_states[CTutorialState::State::Discovery] = std::make_unique< CDiscoveryState>(this);
    m_states[CTutorialState::State::Suction] = std::make_unique< CSuctionState>(this);
    m_states[CTutorialState::State::Play] = std::make_unique< CPlayState>(this);
    m_pCurrentState = m_states[CTutorialState::State::Move].get();

    // 最初のチュートリアル用動物を生成 //
    static constexpr float INITIAL_ANIMAL_Z = 5.0f;
    Instantiate< CTutorialAnimal>(VECTOR3(0, 0, INITIAL_ANIMAL_Z));
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
// @param state 変更先のステート
////////////////////
void CTutorial::SetState(CTutorialState::State state)
{
    m_pCurrentState->Exit();
    m_pCurrentState = m_states[state].get();
    m_pCurrentState->Enter();
}

////////////////////
// チュートリアル用の動物を複数体生成する
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
// チュートリアル用の人間と動物を生成する
////////////////////
void CTutorial::SpawnHuman()
{
    static constexpr float HUMAN_SPAWN_Z = 6.0f;
    static constexpr float ANIMAL_SPAWN_Z = 5.0f;
    new CTutorialHuman(VECTOR3(0, 0, HUMAN_SPAWN_Z));
    new CTutorialAnimal(VECTOR3(0, 0, ANIMAL_SPAWN_Z));
}

////////////////////
// 実践形式のレッスンを開始する
////////////////////
void CTutorial::Lesson()
{
    new CTimer(20);
    static constexpr int LESSON_TIME_SECONDS = 30;
    static constexpr int LESSON_ANIMAL_COUNT = 5;

    new CEnemyManager();
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