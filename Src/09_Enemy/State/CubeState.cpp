#include "CubeState.h"
#include "../Actor/ACube.h"
namespace
{
    constexpr float MOVE_SPEED = 1.2f;
    constexpr float MAX_MOVE_AMOUNT = 3.5f;
    constexpr float MIN_MOVE_AMOUNT = 1.0f;
    constexpr float TURN_ANGLE = 180.0f;
    constexpr float ROTATION_LERP_SPEED = 10.0f;

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///Idle
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CIdleState::CIdleState(CACube* cube)
    : CBaseState(cube, Type::Idle)
      , timerCount(0)
{
}
void CIdleState::Enter()
{
    stateWait = static_cast<int>(round(Randomf(0, 1)));
    if (stateWait == 1)
    {
        m_pCube->GetAnimator()->MergePlay(A_IDEL);
        m_pCube->GetAnimator()->SetPlaySpeed(1.0f);
    }else
    {
        m_pCube->GetAnimator()->Stop();
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
        default:
        assert("error:cubeState");
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
    if (AnimationFinish())
    {
        Next();
    }
}

bool CIdleState::AnimationFinish()
{
    if (m_pCube->GetAnimator()->CurrentFrame() >= 570.0f)
    {
        return true;
    }
    return false;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CWalkState::CWalkState(CACube* cube)
    : CBaseState(cube, Type::Walk)
      , BASE_POS(0, 0, 0)
{
}

void CWalkState::Enter()
{
    bool boundaryFlag = false;
    int retryCount = 0;
    constexpr int MAX_RETRY = 50; 

    while (!boundaryFlag && retryCount < MAX_RETRY)
    {
        m_totalPosZMoveAmount = 0;
        m_turnAmount = Randomf(-TURN_ANGLE, TURN_ANGLE) * DegToRad;
        m_moveAmount = Randomf(MIN_MOVE_AMOUNT, MAX_MOVE_AMOUNT);
        m_position = m_pCube->GetTransform().position;
        if (BoundaryCheck(m_pCube->GetMoveAreaSize()))
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
    m_currentRotation = m_pCube->GetTransform().rotation.y;
    m_targetRotation = m_currentRotation + m_turnAmount;
    m_rotation = true;
    m_pCube->GetAnimator()->MergePlay(A_WALK);
    m_pCube->GetAnimator()->SetPlaySpeed(1.0f);
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CSuction::CSuction(CACube* cube)
    : CBaseState(cube, Type::Suction)
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
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CDestroy::CDestroy(CACube* cube)
    : CBaseState(cube, Type::Destroy)
{
}

void CDestroy::Enter()
{
    m_pCube->DestroyMe();
}
