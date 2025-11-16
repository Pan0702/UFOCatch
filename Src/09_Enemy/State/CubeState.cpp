#include "CubeState.h"

#include "../../GameInstance.h"
#include "../Actor/ACube.h"

namespace
{
    constexpr float MOVE_SPEED = 1.2f;
    constexpr float MAX_MOVE_AMOUNT = 3.5f;
    constexpr float MIN_MOVE_AMOUNT = 1.0f;
    constexpr float TURN_ANGLE = 180.0f;
    constexpr float ROTATION_LERP_SPEED = 10.0f;
}

CCubeBase::CCubeBase(CACube* cube, Type type)
    :m_pOwner(cube), m_kType(type)
{
}

CCubeBase::~CCubeBase()
{
    if (m_pOwner != nullptr)
    {
        m_pOwner = nullptr;
    }
}

void CCubeBase::NextState()
{
    m_pOwner->SetState(NextStatePop());
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///Idle
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CCubeIdleState::CCubeIdleState(CACube* cube)
    : CCubeBase(cube, Type::IDLE)
      , timerCount(0)
,stateIdle(0)
{
}

void CCubeIdleState::Enter()
{
    stateIdle = static_cast<int>(round(Randomf(0, 1)));
    if (stateIdle)
    {
        m_pOwner->GetAnimator()->MergePlay(A_IDEL);
        m_pOwner->GetAnimator()->SetPlaySpeed(1.0f);
    }
    else
    {
        m_pOwner->GetAnimator()->Stop();
    }
    timerCount = 0;
}

void CCubeIdleState::Update()
{
    switch (stateIdle)
    {
    case 0:

        Idle();
        break;
    case 1:
        IdleAnim();
        break;
    default:
        assert("error:cubeState");
        break;
    }
    m_pOwner->IsSuctionCheck();
}

void CCubeIdleState::Idle()
{
    timerCount += SceneManager::DeltaTime();
    if (timerCount > 1)
    {
        NextStatePop();
    }
}

void CCubeIdleState::IdleAnim()
{
    if (AnimationFinish())
    {
        NextStatePop();
    }
}

bool CCubeIdleState::AnimationFinish() const
{
    if (m_pOwner->GetAnimator()->CurrentFrame() >= 570.0f)
    {
        return true;
    }
    return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CCubeWalkState::CCubeWalkState(CACube* cube)
    : CCubeBase(cube, Type::WALK)
      , BASE_POS(0, 0, 0)
{
}

void CCubeWalkState::Enter()
{
    bool boundaryFlag = false;
    int retryCount = 0;
    constexpr int MAX_RETRY = 50;

    while (!boundaryFlag && retryCount < MAX_RETRY)
    {
        m_totalPosZMoveAmount = 0;
        m_turnAmount = Randomf(-TURN_ANGLE, TURN_ANGLE) * DegToRad;
        m_moveAmount = Randomf(MIN_MOVE_AMOUNT, MAX_MOVE_AMOUNT);
        m_position = m_pOwner->GetTransform().position;
        if (BoundaryCheck(m_pOwner->GetMoveAreaSize()))
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
    m_rotation = true;
    m_pOwner->GetAnimator()->MergePlay(A_WALK);
    m_pOwner->GetAnimator()->SetPlaySpeed(1.0f);
}

bool CCubeWalkState::BoundaryCheck(const VECTOR2& areaSize) const
{
    VECTOR3 tmpPos = m_position + VECTOR3(0, 0, m_moveAmount) * XMMatrixRotationY(m_turnAmount);
    if (tmpPos.x <= areaSize.x && tmpPos.x >= -areaSize.x && tmpPos.z <= areaSize.y && tmpPos.z >= -areaSize.y)
    {
        return true;
    }
    return false;
}

void CCubeWalkState::Update()
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
        m_pOwner->SetRotationY(m_currentRotation);
    }
    m_pOwner->AddPos(
        VECTOR3(0, 0, MOVE_SPEED * SceneManager::DeltaTime()) * XMMatrixRotationY(m_currentRotation));
    m_totalPosZMoveAmount += MOVE_SPEED * SceneManager::DeltaTime();

    if (m_totalPosZMoveAmount > m_moveAmount)
    {
        NextStatePop();
    }
    m_pOwner->IsSuctionCheck();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CCubeSuction::CCubeSuction(CACube* cube)
    : CCubeBase(cube, Type::SUCTION)
      , m_pPlayer(ObjectManager::FindGameObject<CPlayer>())
      ,m_distanceFromObjectToUFO(VECTOR3(0,0,0))
{
}

void CCubeSuction::Update()
{
    m_distanceFromObjectToUFO = m_pOwner->SuctionSpeed();
    if (m_pPlayer->GetIsSuckUp())
    {
        if (m_pPlayer->GetPos().y <= m_pOwner->GetTransform().position.y)
        {
            m_pOwner->SetState(Type::DESTROY);
        }
        else
        {
            m_pOwner->AddPos(m_distanceFromObjectToUFO);
        }
    }
    else
    {
        NextStatePop();
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CCubeDestroy::CCubeDestroy(CACube* cube)
    : CCubeBase(cube, Type::DESTROY)
{
}

void CCubeDestroy::Enter()
{
    ObjectManager::FindGameObject<CGameInstance>()->AddScore(100);
    m_pOwner->DestroyMe();
}
