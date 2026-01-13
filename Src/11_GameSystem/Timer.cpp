#include "Timer.h"

#include "../07_Scene/PlayScene.h"


////////////////////
// タイマーの最大時間を指定してインスタンスを生成する
// @param maxTimeSec 最大時間（秒） //
////////////////////
CTimer::CTimer(const float maxTimeSec)
    : m_currentTime(maxTimeSec), m_maxTime(maxTimeSec)
{
}


CTimer::~CTimer() = default;

void CTimer::Update()
{
    if (m_stareFlag)
    {
        m_currentTime -= SceneManager::DeltaTime();
    }
     if (m_currentTime <= 0)
     {
    m_currentTime =0;
         TimeOut();
     }
}

////////////////////
// タイムアウト時の処理 //
////////////////////
void CTimer::TimeOut()
{
    SceneManager::ChangeScene("ResultScene");
}
