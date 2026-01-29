#include "HumanState.h"
#include "../../../Utils/MyMath.h"
#include "../../../Utils/Lerp.h"
#include "../../../Player/PlayerHP.h"
#include "../Human.h"
#include "../../System/EnemyRegistr.h"


CHumanBase::CHumanBase(CHuman* human, State type)
    : CBaseState(human), m_pOwner(human), m_kType(type)
{
}

CHumanBase::~CHumanBase()
{
    if (m_pOwner != nullptr)
    {
        m_pOwner = nullptr;
    }
}

void CHumanBase::NextState()
{
    m_pOwner->SetState(NextStatePop());
}
CHumanIdleState::CHumanIdleState(CHuman* human)
    : CHumanBase(human, State::IDLE)
{
}

void CHumanIdleState::Enter(State type)
{

}

void CHumanIdleState::Update()
{
    switch (stateIdle)
    {
    case 0:
        LookAround();
        break;
    case 1:
        Idle();
        break;
    default:
        assert("error:cubeState");
        break;
    }
}

void CHumanIdleState::LookAround()
{
    RotationAngle();

}

void CHumanIdleState::RotationAngle()
{

}


void CHumanIdleState::Idle()
{
    if (m_pOwner->GetAnimator()->Finished())
    {
        m_pOwner->SetState(NextStatePop());
    }
}



CHumanWalkState::CHumanWalkState(CHuman* human)
    : CHumanBase(human, State::IDLE)
{
}

void CHumanWalkState::Enter(State type)
{
    bool boundaryFlag = false;
    int retryCount = 0;
    static constexpr int MAX_RETRY = 50;
    static constexpr float TURN_ANGLE = 180.0f;        // 方向転換時の最大回転角度（度数法）
    while (!boundaryFlag && retryCount < MAX_RETRY)
    {
        m_totalPosZMoveAmount = 0;

        m_turnAmount = Randomf(-TURN_ANGLE, TURN_ANGLE) * DegToRad;
        static constexpr float MAX_MOVE_AMOUNT = 3.5f;     // 1回の移動で進む最大距離
        static constexpr float MIN_MOVE_AMOUNT = 1.0f;     // 1回の移動で進む最小距離
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
}

bool CHumanWalkState::BoundaryCheck(const VECTOR2& areaSize) const
{
    VECTOR3 NextPos = m_pOwner->GetTransform().position + VECTOR3(0, 0, m_moveAmount) * XMMatrixRotationY(m_turnAmount);
    return  IsInsideAreaXZ(NextPos,areaSize);
}

void CHumanWalkState::Update()
{
    if (m_isRotation)
    {
        static constexpr float ROTATION_LERP_SPEED = 10.0f;    // 回転補間の速度係数（値が大きいほど素早く回転）
        float t = ROTATION_LERP_SPEED * SceneManager::DeltaTime();
        m_currentRotation = m_currentRotation + (m_targetRotation - m_currentRotation) * t;
        if (abs(m_targetRotation - m_currentRotation) < 0.01f)
        {
            m_currentRotation = m_targetRotation;
            m_isRotation = false;
        }
        m_pOwner->SetRotateY(ClampRotateY(m_currentRotation));  
    }
    static constexpr float MOVE_SPEED = 1.2f;    // Humanの移動速度（m/s）
    float moveAmount = MOVE_SPEED * SceneManager::DeltaTime();
    m_pOwner->AddPos(
        VECTOR3(0, 0, moveAmount) * XMMatrixRotationY(m_currentRotation));
    m_totalPosZMoveAmount += moveAmount;

    if (m_totalPosZMoveAmount > m_moveAmount)
    {
        m_pOwner->SetState(NextStatePop());
    }
}


