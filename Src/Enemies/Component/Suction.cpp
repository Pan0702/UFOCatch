#include "Suction.h"

Suction::Suction(CEnemyBase* e)
{
    m_pOwner = e;
}


void Suction::Enter()
{
    m_isFinish = false;
}

void Suction::Update()
{
    VECTOR3 distanceFromObjectToUFO = m_pOwner->S;
    m_pPlayer = ObjectManager::FindGameObject<CPlayer>();
    if (m_pPlayer == nullptr) return;
    if (m_pPlayer->GetIsSuckUp())
    {
        float maxPos = m_pOwner->GetTransform().position.y + m_pOwner->GetMesh()->m_vMax.y
        if (m_pPlayer->GetTransform().position.y <= maxPos)
        {
            m_isFinish = true;
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


void Suction::Exit()
{
    CComponentBase::Exit();
}
