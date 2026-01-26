#include "State.h"

#include "EnemyBase.h"

CWalk::CWalk(CEnemyBase* e)
    : CBaseState(e, Type::WALK)
{
}

void CWalk::Enter()
{
}

void CWalk::Update()
{
    CBaseState::Update();
}

void CWalk::Exit()
{
    CBaseState::Exit();
}

CIdle::CIdle(CEnemyBase* e)
    : CBaseState(e, Type::IDLE)
{
}

void CIdle::Enter()
{
    CBaseState::Enter();
}

void CIdle::Update()
{
    m_pEnemy->GetComponent();

}

void CIdle::Exit()
{
    CBaseState::Exit();
}

CSuction::CSuction(CEnemyBase* e)
    : CBaseState(e, Type::SUCTION)
{
}

void CSuction::Enter()
{
    CBaseState::Enter();
}

void CSuction::Update()
{
    CBaseState::Update();
}

void CSuction::Exit()
{
    CBaseState::Exit();
}

CUniqueAction::CUniqueAction(CEnemyBase* e)
    : CBaseState(e, Type::UNIQUEACTION)
{
}

void CUniqueAction::Enter()
{
    CBaseState::Enter();
}

void CUniqueAction::Update()
{
    CBaseState::Update();
}

void CUniqueAction::Exit()
{
    CBaseState::Exit();
}
