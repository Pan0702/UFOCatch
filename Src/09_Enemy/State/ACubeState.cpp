#include "ACubeState.h"

#include "../ACube.h"

namespace
{
    constexpr float MAX_MOVE_SPEED = 2.0f;
    constexpr float MIN_MOVE_SPEED = 0.5f;
    constexpr float MAX_MOVE_AMOUNT = 3.5f;
    constexpr float MIN_MOVE_AMOUNT = 1.0f;
    constexpr float TURN_ANGLE = 180.0f;
    std::queue<CACubeState::Type> actionQueue;
}

CACubeState::CACubeState(CACube* cube, Type type)
    : m_pCube(cube), m_type(type)
{
}

void CACubeState::SetNextState()
{
    while (actionQueue.size() <= 3)
    {
        float randomNum = Randomf(0, 1);
        if (randomNum < 0.5f)
        {
            actionQueue.push(Type::Walk);
        }
        else
        {
            actionQueue.push(Type::Idle);
        }
    }
}

CIdleState::CIdleState(CACube* cube)
    : CACubeState(cube, Type::Idle)
      , timerCount(0)
{
}

void CIdleState::Enter()
{
   timerCount = 0;
}

void CIdleState::Update()
{
    timerCount += SceneManager::DeltaTime();
    if (timerCount > 1)
    {
        Type type = actionQueue.front();
        actionQueue.pop();
        SetNextState();
        m_pCube->SetState(type);
    }
    m_pCube->IsSuctionCheck();
}

CWalkState::CWalkState(CACube* cube)
    : CACubeState(cube, Type::Walk)
      , BASE_POS(0, 0, 0)
{
}

void CWalkState::Enter()
{
    bool boundaryFlag = false;
    while (!boundaryFlag)
    {
        m_totalPosZMoveAmount = 0;
        m_moveSpeed = Randomf(MIN_MOVE_SPEED, MAX_MOVE_SPEED);
        m_turnAmount = Randomf(-TURN_ANGLE, TURN_ANGLE) * DegToRad;
        m_moveAmount = Randomf(MIN_MOVE_AMOUNT, MAX_MOVE_AMOUNT);
        m_position = m_pCube->GetTransform().position;
        if (BoundaryCheck(m_pCube->GetMoveAreaSize()))
        {
            boundaryFlag = true;
        }
    }
    m_pCube->SetRotationY(m_turnAmount);
}

bool CWalkState::BoundaryCheck(const VECTOR2& areaSize) const
{
    VECTOR3 tmpPos = m_position + VECTOR3(0, 0, m_moveAmount) * XMMatrixRotationY(m_turnAmount);
    if (tmpPos.x <= areaSize.x && tmpPos.x >= -areaSize.x && tmpPos.z <= areaSize.y && tmpPos.z >= -areaSize.y)
    {
        return true;
    }
    return false;
}

void CWalkState::Update()
{
    m_pCube->AddPos(
        VECTOR3(0, 0, m_moveSpeed * SceneManager::DeltaTime()) * XMMatrixRotationY(m_turnAmount));
    m_totalPosZMoveAmount += m_moveSpeed * SceneManager::DeltaTime();

    if (m_totalPosZMoveAmount > m_moveAmount)
    {
        
        Type type = actionQueue.front();
        actionQueue.pop();
        SetNextState();
        m_pCube->SetState(type);
    }
    m_pCube->IsSuctionCheck();
}

CSuction::CSuction(CACube* cube)
    : CACubeState(cube, Type::Suction)
      , m_pPlayer(ObjectManager::FindGameObject<CPlayer>())
{
    m_distanceFromObjectToUFO = m_pCube->SuctionSpeed();
}

void CSuction::Update()
{
    if (m_pPlayer->GetIsSuckUp())
    {
        if (m_pPlayer->GetPos().y <= m_pCube->GetTransform().position.y)
        {
            m_pCube->SetState(Type::Destroy);
        }
        else
        {
            m_pCube->AddPos(m_distanceFromObjectToUFO);
        }
    }
    else
    {
        Type type = actionQueue.front();
        actionQueue.pop();
        m_pCube->SetState(type);
    }
}

CDestroy::CDestroy(CACube* cube)
    : CACubeState(cube, Type::Destroy)
{
}

void CDestroy::Enter()
{
    m_pCube->DestroyMe();
}
