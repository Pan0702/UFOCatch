#include "Suction.h"
#include "../Base/EnemyBase.h"


CSuction::CSuction(CEnemyBase* e)
{
    m_pOwner = e;
    m_isFinishSuction = false;
}


void CSuction::Enter()
{
    m_isFinish = false;
    m_isFinishSuction = false;
    m_pOwner->GetAnimator()->MergePlay(AnimationType::A_WALK);
    m_pOwner->GetAnimator()->SetPlaySpeed(6.0f);
}

void CSuction::Update()
{
    VECTOR3 distanceFromObjectToUFO = m_pOwner->SuctionSpeed();
    m_pPlayer = ObjectManager::FindGameObject<CPlayer>();
    if (m_pPlayer == nullptr) return;
    if (m_pPlayer->GetIsSuckUp())
    {
        float centerPos = m_pOwner->GetTransform().position.y + (m_pOwner->GetMesh()->m_vMax.y / 2);
        if (m_pPlayer->GetTransform().position.y <= centerPos)
        {
            m_isFinish = true;
            m_isFinishSuction = true;
        }
        else
        {
            m_pOwner->AddPosition(distanceFromObjectToUFO);
        }
    }
    else
    {
        m_isFinish = true;
    }
}

