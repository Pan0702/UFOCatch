#include "EnemyBase.h"

CEnemyBase::CEnemyBase()
{
    m_pCurrentState = nullptr;
}

void CEnemyBase::SetState(CBaseState::Type type)
{
    m_pCurrentState->Exit();
    m_pCurrentState = m_cubeStates[type];
    m_pCurrentState->Enter();
}

CEnemyBase::~CEnemyBase()
{
}

void CEnemyBase::Update()
{
    if (m_pCurrentState)
    {
        m_pCurrentState->Update();
    }
}
