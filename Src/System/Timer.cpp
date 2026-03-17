#include "Timer.h"

#include "../Scene/PlayScene.h"

namespace
{
    constexpr float READY_DURATION = 1.5f;  // Ready陦ｨ遉ｺ譎る俣
    constexpr float GO_DURATION = 1.0f;     // Go陦ｨ遉ｺ譎る俣
    constexpr float FINISH_DURATION = 2.0f; // Finish陦ｨ遉ｺ譎る俣
}

////////////////////
// 繧ｿ繧､繝槭・縺ｮ譛螟ｧ譎る俣繧呈欠螳壹＠縺ｦ繧､繝ｳ繧ｹ繧ｿ繝ｳ繧ｹ繧堤函謌舌☆繧・
// @param maxTimeSec 譛螟ｧ譎る俣・育ｧ抵ｼ・//
////////////////////
CTimer::CTimer(const float maxTimeSec)
    : m_currentTime(maxTimeSec), m_maxTime(maxTimeSec)
{
    m_cutInCnt = 0;
    m_stareFlag = false;
    m_finishFlag = false;

    // 繧ｫ繝・ヨ繧､繝ｳ蛻晄悄蛹厄ｼ・eady縺九ｉ髢句ｧ具ｼ・
    m_cutInNum = 0;
    m_isCutInVisible = true;
    m_cutInTimer = 0.0f;
}


CTimer::~CTimer() = default;

void CTimer::Update()
{
    m_cutInTimer += SceneManager::DeltaTime();

    // 繧ｫ繝・ヨ繧､繝ｳ縺ｮ迥ｶ諷矩・遘ｻ
    if (m_cutInNum == 0 && m_cutInTimer >= READY_DURATION)
    {
        // Ready 竊・Go
        m_cutInNum = 1;
        m_cutInTimer = 0.0f;
        m_stareFlag = true;  // 繧ｿ繧､繝槭・髢句ｧ・
    }
    else if (m_cutInNum == 1 && m_cutInTimer >= GO_DURATION)
    {
        // Go髱櫁｡ｨ遉ｺ
        m_isCutInVisible = false;
    }

    // 繧ｿ繧､繝槭・繧ｫ繧ｦ繝ｳ繝医ム繧ｦ繝ｳ
    if (m_stareFlag && !m_finishFlag)
    {
        m_currentTime -= SceneManager::DeltaTime();

        if (m_currentTime <= 0)
        {
            m_currentTime = 0;
            m_finishFlag = true;
            m_cutInNum = 2;
            m_isCutInVisible = true;
            m_cutInTimer = 0.0f;
        }
    }

    // Finish陦ｨ遉ｺ蠕後↓繧ｷ繝ｼ繝ｳ驕ｷ遘ｻ
    if (m_finishFlag && m_cutInTimer >= FINISH_DURATION)
    {
        TimeOut();
    }
}

////////////////////
// 繧ｿ繧､繝繧｢繧ｦ繝域凾縺ｮ蜃ｦ逅・//
////////////////////
void CTimer::TimeOut()
{
    SceneManager::ChangeSceneWithTransition("ResultScene");
}
