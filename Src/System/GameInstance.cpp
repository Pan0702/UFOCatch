#include "GameInstance.h"

CGameInstance::CGameInstance() : m_score(0)
{
    ObjectManager::DontDestroy(this);      // GameInstanceは破棄されない
    ObjectManager::SetVisible(this, false);// GameInstanceは表示しない
    m_score = 0;
    m_maxScore = 0;
    m_discovery = 0;
    m_saw = 0;
    m_capture = 0;
    m_mapSize = VECTOR4(-51.4f, -51.4f, 51.4f, 51.4f);
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