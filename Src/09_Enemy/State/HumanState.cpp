#include "HumanState.h"

CHumanIdleState::CHumanIdleState(CHuman* human)
: CBaseState(human, Type::Idle)
{
}

void CHumanIdleState::Enter()
{
   
}

void CHumanIdleState::Update()
{
   
}

CHumanWalkState::CHumanWalkState(CHuman* human)
: CBaseState(human, Type::Idle)
{
}

void CHumanWalkState::Enter()
{
   
}

void CHumanWalkState::Update()
{
   
}

CHumanDestroy::CHumanDestroy(CHuman* human)
: CBaseState(human, Type::Idle)
{
    
}

void CHumanDestroy::Enter()
{
    m_pOwner->DestroyMe();
}

