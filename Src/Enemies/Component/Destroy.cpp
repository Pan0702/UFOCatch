#include "Destroy.h"
#include "../../System/GameInstance.h"
CDestroy::CDestroy(CEnemyBase* e,int score,float exp)
    :m_score(score),m_exp(exp)
{
    m_pOwner = e;
}


void CDestroy::Enter()
{
    ObjectManager::FindGameObject<CGameInstance>()->AddScore(m_score);
    ObjectManager::FindGameObject<CGameInstance>()->AddCapture(1);
    ObjectManager::FindGameObject<CPlayerLevel>()->AddExp(m_exp);
    m_pOwner->DestroyMe();
}

