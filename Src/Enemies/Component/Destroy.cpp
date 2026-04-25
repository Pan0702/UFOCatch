#include "Destroy.h"
#include "../Base/EnemyBase.h"
#include "../../System/GameInstance.h"
#include "../../Player/PlayerLevel.h"
#include "../System/Flog.h"

CDestroy::CDestroy(CEnemyBase* e, int score, float exp)
    : m_score(score), m_exp(exp)
{
    m_pOwner = e;
}


void CDestroy::Enter()
{
    CSheep* owner = dynamic_cast<CSheep*>(m_pOwner);
    if (owner != nullptr)
    {
        owner->SetFlog(nullptr);
    }
    CGameInstance::Get()->AddScore(m_score);
    CGameInstance::Get()->AddCapture(1);
    ObjectManager::FindGameObject<CPlayerLevel>()->AddExp(m_exp);
    m_pOwner->DestroyMe();
}
