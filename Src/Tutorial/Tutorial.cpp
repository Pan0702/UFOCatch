#include "Tutorial.h"

#include "TutorialAnimal.h"
#include "TutorialHuman.h"
#include "../System/GameInstance.h"
#include "../Enemies/Human/Human.h"
#include "../Enemies/System/EnemyManager.h"
#include "../System/Timer.h"

namespace
{
    // 蜍慕黄縺ｮ繧ｹ繝昴・繝ｳ遽・峇 //
    constexpr float SPAWN_RANGE_MIN = -10.0f;
    constexpr float SPAWN_RANGE_MAX = 10.0f;
}

CTutorial::CTutorial()
{
    // 蜷・せ繝・・繝医ｒ逕滓・縺励※繝槭ャ繝励↓逋ｻ骭ｲ //
    m_states[CTutorialState::State::Move] = std::make_unique< CMoveState>(this);
    m_states[CTutorialState::State::Expands] = std::make_unique< CExpands>(this);
    m_states[CTutorialState::State::Discovery] = std::make_unique< CDiscoveryState>(this);
    m_states[CTutorialState::State::Suction] = std::make_unique< CSuctionState>(this);
    m_states[CTutorialState::State::Play] = std::make_unique< CPlayState>(this);
    m_pCurrentState = m_states[CTutorialState::State::Move].get();

    // 譛蛻昴・繝√Η繝ｼ繝医Μ繧｢繝ｫ逕ｨ蜍慕黄繧堤函謌・//
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
// 繝√Η繝ｼ繝医Μ繧｢繝ｫ縺ｮ繧ｹ繝・・繝医ｒ螟画峩縺吶ｋ
// @param state 螟画峩蜈医・繧ｹ繝・・繝・
////////////////////
void CTutorial::SetState(CTutorialState::State state)
{
    m_pCurrentState->Exit();
    m_pCurrentState = m_states[state].get();
    m_pCurrentState->Enter();
}

////////////////////
// 繝√Η繝ｼ繝医Μ繧｢繝ｫ逕ｨ縺ｮ蜍慕黄繧定､・焚菴鍋函謌舌☆繧・
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
// 繝√Η繝ｼ繝医Μ繧｢繝ｫ逕ｨ縺ｮ莠ｺ髢薙→蜍慕黄繧堤函謌舌☆繧・
////////////////////
void CTutorial::SpawnHuman()
{
    static constexpr float HUMAN_SPAWN_Z = 6.0f;
    static constexpr float ANIMAL_SPAWN_Z = 5.0f;
    new CTutorialHuman(VECTOR3(0, 0, HUMAN_SPAWN_Z));
    new CTutorialAnimal(VECTOR3(0, 0, ANIMAL_SPAWN_Z));
}

////////////////////
// 螳溯ｷｵ蠖｢蠑上・繝ｬ繝・せ繝ｳ繧帝幕蟋九☆繧・
////////////////////
void CTutorial::Lesson()
{
    new CTimer(20);
    static constexpr int LESSON_TIME_SECONDS = 30;
    static constexpr int LESSON_ANIMAL_COUNT = 5;

    new CEnemyManager();
    new CTimer(LESSON_TIME_SECONDS);

    // 莠ｺ髢薙ｒ1菴鍋函謌・//
    float randomX = Randomf(SPAWN_RANGE_MIN, SPAWN_RANGE_MAX);
    float randomZ = Randomf(SPAWN_RANGE_MIN, SPAWN_RANGE_MAX);
    new CHuman(VECTOR3(randomX, 0, randomZ));

    // 蜍慕黄繧定､・焚菴鍋函謌・//
    for (int i = 0; i < LESSON_ANIMAL_COUNT; i++)
    {
        randomX = Randomf(SPAWN_RANGE_MIN, SPAWN_RANGE_MAX);
        randomZ = Randomf(SPAWN_RANGE_MIN, SPAWN_RANGE_MAX);
        new CTutorialAnimal(VECTOR3(randomX, 0, randomZ));
    }
}

