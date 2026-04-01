#include "Destroy.h"
#include "../Base/EnemyBase.h"
#include "../../System/GameInstance.h"
#include "../../Player/PlayerLevel.h"
CDestroy::CDestroy(CEnemyBase* e,int score,float exp)
    :m_score(score),m_exp(exp)
{
    m_pOwner = e;
}


void CDestroy::Enter()
{
    CGameInstance::Get()->AddScore(m_score);
    CGameInstance::Get()->AddCapture(1);
    ObjectManager::FindGameObject<CPlayerLevel>()->AddExp(m_exp);
    m_pOwner->DestroyMe();
}

