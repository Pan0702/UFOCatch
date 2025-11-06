#include "ACubeState.h"

#include "../ACube.h"

namespace
{
    constexpr float MOVE_SPEED = 1.2f;
    constexpr float MAX_MOVE_AMOUNT = 3.5f;
    constexpr float MIN_MOVE_AMOUNT = 1.0f;
    constexpr float TURN_ANGLE = 180.0f;
    constexpr int MAX_SIZE = 3;
    constexpr float ROTATION_LERP_SPEED = 10.0f;
    std::queue<CACubeState::Type> actionQueue;
}

CACubeState::CACubeState(CACube* cube, Type type)
    : m_pCube(cube), m_type(type)
{
}

void CACubeState::Next()
{
    Type type = actionQueue.front();
    actionQueue.pop();
    SetNextState();
    m_pCube->SetState(type);
}

void CACubeState::SetNextState()
{
    while (actionQueue.size() <= MAX_SIZE)
    {
        float randomNum = Randomf(0, 1);
        if (randomNum > 0.3f)
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
    stateWait = static_cast<int>(round(Randomf(0, 1)));
    if (stateWait == 1)
    {
        m_pCube->SetAnim(A_RUN);
    }
    timerCount = 0;
}

void CIdleState::Update()
{
    switch (stateWait)
    {
    case 0:
        Stop();
        break;
    case 1:
        Idle();
        break;
    }
    m_pCube->IsSuctionCheck();
}

void CIdleState::Stop()
{
    timerCount += SceneManager::DeltaTime();
    if (timerCount > 1)
    {
        Next();
    }
}

void CIdleState::Idle()
{
    if (m_pCube->AnimationFinish())
    {
        Next();
    }
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
        m_turnAmount = Randomf(-TURN_ANGLE, TURN_ANGLE) * DegToRad;
        m_moveAmount = Randomf(MIN_MOVE_AMOUNT, MAX_MOVE_AMOUNT);
        m_position = m_pCube->GetTransform().position;
        if (BoundaryCheck(m_pCube->GetMoveAreaSize()))
        {
            boundaryFlag = true;
        }
    }
    m_currentRotation = m_pCube->GetTransform().rotation.y;
    m_targetRotation = m_currentRotation + m_turnAmount;
    m_rotation = true;
    m_pCube->SetAnim(A_WALK);
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
    if (m_rotation)
    {
        float t = ROTATION_LERP_SPEED * SceneManager::DeltaTime();
        m_currentRotation = m_currentRotation + (m_targetRotation - m_currentRotation) * t;
        if (abs(m_targetRotation - m_currentRotation) < 0.01f)
        {
            m_currentRotation = m_targetRotation;
            m_rotation = false;
        }
        m_pCube->SetRotationY(m_currentRotation);
    }
    m_pCube->AddPos(
        VECTOR3(0, 0, MOVE_SPEED * SceneManager::DeltaTime()) * XMMatrixRotationY(m_currentRotation));
    m_totalPosZMoveAmount += MOVE_SPEED * SceneManager::DeltaTime();

    if (m_totalPosZMoveAmount > m_moveAmount)
    {
        Next();
    }
    m_pCube->IsSuctionCheck();
}

CSuction::CSuction(CACube* cube)
    : CACubeState(cube, Type::Suction)
      , m_pPlayer(ObjectManager::FindGameObject<CPlayer>())
{
}

void CSuction::Update()
{
    m_distanceFromObjectToUFO = m_pCube->SuctionSpeed();
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
        Next();
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
