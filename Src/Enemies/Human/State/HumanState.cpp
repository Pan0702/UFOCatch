#include "HumanState.h"
#include "../../../Utils/MyMath.h"
#include "../../../Utils/Lerp.h"
#include "../../../Player/PolayerHP.h"
#include "../Human.h"
#include "../../System/AnimalManager.h"
namespace
{
    //////////////////////////////////////////////
    ///Idle
    /// //////////////////////////////////////////
     constexpr float ANIMATION_FPS = 30.0f;          // アニメーションのフレームレート（1秒あたりのフレーム数） 
     constexpr float TOTAL_FRAMES = 100.0f;          // アニメーション全体の総フレーム数 
     constexpr float NECK_ANIMATION_END = 86.0f;     // 首の回転アニメーションが終了するフレーム番号 
     constexpr float WAIT_START_FRAME = 6.0f;        // 首の回転が開始されるまでの待機フレーム数 
     constexpr float CYCLE_DURATION = 81.0f;         // 左右を見回す1サイクルの合計フレーム数 
     constexpr float HALF_CYCLE_DURATION = 41.0f;    // 半サイクル（片側を見る）のフレーム数 
     constexpr float CHANGE_DURATION = 15.0f;        // 首を回転させる動作にかかるフレーム数 
     constexpr float HOLD_END_FRAME = 27.0f;         // 首を回転させた状態を保持する終了フレーム 
     constexpr float RETURN_START_FRAME = 27.0f;     // 首を元の位置に戻し始めるフレーム
     constexpr float RETURN_END_FRAME = 41.0f;       // 首を元の位置に戻し終わるフレーム
     constexpr float CHANGE_DIVISOR = 14.0f;         // 首の回転補間計算用の除数（CHANGE_DURATION - 1） 
     constexpr float RETURN_DIVISOR = 13.0f;         // 首の復帰補間計算用の除数（RETURN_END_FRAME - RETURN_START_FRAME - 1） 
     constexpr float ANGLE = 50.0f;                  // 首を左右に回転させる角度（度数法） 
}


CHumanBase::CHumanBase(CHuman* human, Type type)
    :m_pOwner(human), m_kType(type)
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
    : CHumanBase(human, Type::IDLE)
{
}

void CHumanIdleState::Enter()
{
    frameCount = 0;
    currentAngle = 0;
    animationTime = 0;
    stateIdle = static_cast<int>(std::round(Randomf(0, 1)));
    if (stateIdle)
    {
        m_pOwner->GetAnimator()->MergePlay(A_IDEL);
    }
    else
    {
        m_pOwner->GetAnimator()->MergePlay(A_SEACH);
    }
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
    if (m_pOwner->GetAnimator()->Finished())
    {
        m_pOwner->SetAngle(0);
        m_pOwner->SetState(NextStatePop());
    }
}

void CHumanIdleState::RotationAngle()
{
    animationTime += SceneManager::DeltaTime();
        
    float currentFrame = GetCurrentFrame();
    
    if (currentFrame >= TOTAL_FRAMES) { 
        animationTime = 0.0f;
        currentAngle = 0.0f;
        return;
    }
    
    if (currentFrame > NECK_ANIMATION_END) {  
        return;
    }
        
    if (currentFrame <= WAIT_START_FRAME) {
        currentAngle = 0.0f;
    }
    else {
        float cycleFrame = currentFrame - WAIT_START_FRAME;
            
        if (cycleFrame <= CYCLE_DURATION) {
            int halfCycle = static_cast<int>((cycleFrame - 1.0f) / HALF_CYCLE_DURATION);
            float localFrame = fmodf(cycleFrame - 1.0f, HALF_CYCLE_DURATION);
                
            float targetAngle = (halfCycle == 0) ? ANGLE : -ANGLE;
                
            if (localFrame < CHANGE_DURATION) {
                float t = localFrame / CHANGE_DIVISOR;
                currentAngle = Lerp(0.0f, targetAngle, t);
            }
            else if (localFrame < HOLD_END_FRAME) {
                currentAngle = targetAngle;
            }
            else if (localFrame < RETURN_END_FRAME) {
                float t = (localFrame - RETURN_START_FRAME) / RETURN_DIVISOR;
                currentAngle = Lerp(targetAngle, 0.0f, t);
            }
        }
    }
    m_pOwner->SetAngle(currentAngle * DegToRad);
}

float CHumanIdleState::GetCurrentFrame() const {
    return animationTime * ANIMATION_FPS;
}
void CHumanIdleState::Idle()
{
    if (m_pOwner->GetAnimator()->Finished())
    {
        m_pOwner->SetState(NextStatePop());
    }
}



CHumanWalkState::CHumanWalkState(CHuman* human)
    : CHumanBase(human, Type::IDLE)
{
}

void CHumanWalkState::Enter()
{
    bool boundaryFlag = false;
    int retryCount = 0;
    static constexpr int MAX_RETRY = 50;
    static constexpr float TURN_ANGLE = 180.0f;        // 方向転換時の最大回転角度（度数法）
    while (!boundaryFlag)
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
        if (retryCount < MAX_RETRY)
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

CHumanFindPlayer::CHumanFindPlayer(CHuman* human)
    : CHumanBase(human, Type::FIND_PLAYER)
{
}

void CHumanFindPlayer::Enter()
{
    ObjectManager::FindGameObject<CPlayerHP>()->SubHP();
    m_pOwner->GetAnimator()->MergePlay(A_IDEL);
}

void CHumanFindPlayer::Update()
{
   if (not m_pOwner->GetInSight())
   {
       m_pOwner->SetAngle(0);
       m_pOwner->SetState(Type::WALK);
   }
    
}
