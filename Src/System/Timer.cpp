#include "Timer.h"
#include "../Scene/Play/PlayScene.h"

using namespace Constants;

////////////////////
// タイマーの最大時間を指定してインスタンスを生成する
// @param maxTimeSec 最大時間（秒）
////////////////////
CTimer::CTimer(const float maxTimeSec)
    : m_currentTime(maxTimeSec), m_maxTime(maxTimeSec)
{
    m_cutInCnt = 0;
    m_stareFlag = false;
    m_finishFlag = false;

    // カットイン初期化：Readyから開始
    m_cutInNum = 0;
    m_isCutInVisible = true;
    m_cutInTimer = 0.0f;
}

CTimer::~CTimer() = default;

void CTimer::Update()
{
    m_cutInTimer += SceneManager::DeltaTime();

    constexpr float GO_DURATION = 1.0f;     // Go表示時間
    constexpr float READY_DURATION = 1.5f;  // Ready表示時間

    // カットインの状態遷移
    if (m_cutInNum == 0 && m_cutInTimer >= READY_DURATION)
    {
        // Ready -> Go
        m_cutInNum = 1;
        m_cutInTimer = 0.0f;
        m_stareFlag = true;  // タイマー開始
    }
    else if (m_cutInNum == 1 && m_cutInTimer >= GO_DURATION)
    {
        // Go非表示
        m_isCutInVisible = false;
    }

    // タイマーカウントダウン
    if (m_stareFlag && !m_finishFlag)
    {
        m_currentTime -= SceneManager::DeltaTime();

        if (m_currentTime <= 0)
        {
            m_currentTime = 0;
            m_finishFlag = true;
            m_cutInNum = 2; // Finish
            m_isCutInVisible = true;
            m_cutInTimer = 0.0f;
        }
    }

    constexpr float FINISH_DURATION = 2.0f; // Finish表示時間
    // Finish表示後にシーン遷移
    if (m_finishFlag && m_cutInTimer >= FINISH_DURATION)
    {
        TimeOut();
    }
}

////////////////////
// タイムアウト時の処理
////////////////////
void CTimer::TimeOut()
{
    SceneManager::ChangeSceneWithTransition(SceneName::RESULT);
}