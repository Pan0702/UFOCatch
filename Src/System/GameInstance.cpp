#include "GameInstance.h"

CGameInstance::CGameInstance() : m_score(0)
{
    ObjectManager::DontDestroy(this);		// DataCarrier縺ｯ豸医＆繧後↑縺・
    ObjectManager::SetVisible(this, false);// DataCarrier縺ｯ陦ｨ遉ｺ縺励↑縺・
    m_score = 0;
    m_maxScore = 0;
    m_discovery = 0;
    m_saw = 0;
    m_capture = 0;
}

void CGameInstance::Update()
{

}

void CGameInstance::Init(int max)
{
    m_maxScore = max;
    m_score = 0;
    m_discovery = 0;
    m_saw = 0;
    m_capture = 0;
}




