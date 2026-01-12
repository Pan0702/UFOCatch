#include "GameInstance.h"

CGameInstance::CGameInstance() : m_score(0)
{
    ObjectManager::DontDestroy(this);		// DataCarrierは消されない
    ObjectManager::SetVisible(this, false);// DataCarrierは表示しない
    m_score = 0;
    m_maxScore = 0;
    m_discovery = 0;
    m_saw = 0;
    m_capture = 0;
}

void CGameInstance::Init(int max)
{
    m_maxScore = max;
    m_score = 0;
    m_discovery = 0;
    m_saw = 0;
    m_capture = 0;
}



