#include "Find.h"
#include "../../Player/PlayerHP.h"
#include "../Human/Human.h"
CFind::CFind(CHuman* human)
{
    m_pOwner = human;
}

void CFind::Enter()
{
    m_isFinish = true;
    ObjectManager::FindGameObject<CPlayerHP>()->SubHP();
    m_pOwner->GetAnimator()->MergePlay(A_IDEL);
}

void CFind::Update()
{
    if (not m_pOwner->GetInSight())
    {
        m_pOwner->SetAngle(0);
        m_isFinish = true;
    }
}