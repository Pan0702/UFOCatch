#include "HumanState.h"

namespace
{
    constexpr float MOVE_SPEED = 1.2f;
    constexpr float MAX_MOVE_AMOUNT = 3.5f;
    constexpr float MIN_MOVE_AMOUNT = 1.0f;
    constexpr float TURN_ANGLE = 180.0f;
    constexpr float ROTATION_LERP_SPEED = 10.0f;
}
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
    bool boundaryFlag = false;
    int retryCount = 0;
    constexpr int MAX_RETRY = 50;

    while (!boundaryFlag && retryCount < MAX_RETRY)
    {
        m_totalPosZMoveAmount = 0;
        m_turnAmount = Randomf(-TURN_ANGLE, TURN_ANGLE) * DegToRad;
        m_moveAmount = Randomf(MIN_MOVE_AMOUNT, MAX_MOVE_AMOUNT);
        if (BoundaryCheck(m_pOwner->GetAreaSize()))
        {
            boundaryFlag = true;
        }
        retryCount++;
    }

    if (!boundaryFlag)
    {
        m_moveAmount = 0.0f;
        m_turnAmount = Randomf(-TURN_ANGLE, TURN_ANGLE) * DegToRad;
    }
    m_currentRotation = m_pOwner->GetTransform().rotation.y;
    m_targetRotation = m_currentRotation + m_turnAmount;
    m_isRotation = true;
    m_pOwner->GetAnimator()->MergePlay(A_WALK);
    m_pOwner->GetAnimator()->SetPlaySpeed(1.0f);
}

bool CHumanWalkState::BoundaryCheck(const VECTOR2& areaSize) const
{
    VECTOR3 tmpPos = m_pOwner->GetTransform().position + VECTOR3(0, 0, m_moveAmount) * XMMatrixRotationY(m_turnAmount);
    if (tmpPos.x <= areaSize.x && tmpPos.x >= -areaSize.x && tmpPos.z <= areaSize.y && tmpPos.z >= -areaSize.y)
    {
        return true;
    }
    return false;
}

void CHumanWalkState::Update()
{
    if (m_isRotation)
    {
        float t = ROTATION_LERP_SPEED * SceneManager::DeltaTime();
        m_currentRotation = m_currentRotation + (m_targetRotation - m_currentRotation) * t;
        if (abs(m_targetRotation - m_currentRotation) < 0.01f)
        {
            m_currentRotation = m_targetRotation;
            m_isRotation = false;
        }
        m_pOwner->SetRotationY(m_currentRotation);
    }
    float moveAmount = MOVE_SPEED * SceneManager::DeltaTime();
    m_pOwner->AddPos(
        VECTOR3(0, 0, moveAmount) * XMMatrixRotationY(m_currentRotation));
    m_totalPosZMoveAmount += moveAmount;

    if (m_totalPosZMoveAmount > m_moveAmount)
    {
        Next();
    }
}

CHumanDestroy::CHumanDestroy(CHuman* human)
    : CBaseState(human, Type::Idle)
{
}

void CHumanDestroy::Enter()
{
    m_pOwner->DestroyMe();
}
