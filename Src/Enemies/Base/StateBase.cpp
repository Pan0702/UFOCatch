#include "StateBase.h"
#include "../../Utils/MyMath.h"
# define STR(var) #var	

CBaseState::CBaseState(CEnemyBase* e)
{
    m_pEnemy = e;
}

CBaseState::~CBaseState()
{
    SAFE_DELETE(m_pComponent);
    SAFE_DELETE(m_pEnemy)
}

void CBaseState::Enter(Type type)
{
    m_pComponent = m_pEnemy->GetComponent(type);
    if (m_pComponent == nullptr)
    {
        MessageBox(nullptr, STR(m_kType), _T(STR(m_kType)"ComponentがNullです"), MB_OK);
    }
    m_pComponent->Enter();
}

void CBaseState::Update()
{
    m_pComponent->Update();
    if (m_pComponent->IsFinish())
    {
        m_pEnemy->SetState(NextStatePop());
    }
}

void CBaseState::Exit()
{
    m_pComponent->Exit();
    m_pComponent = nullptr;
}

CBaseState::Type CBaseState::NextStatePop()
{
    SetNextState();
    Type type = actionQueue.front();
    actionQueue.pop();
    return type;
}

float CBaseState::ClampRotateY(const float& angle)
{    float degAngle = angle * RadToDeg;
    constexpr float HALF_ROTATION_DEG = 180.0f;
    constexpr float FULL_ROTATION_DEG = 360.0f;
    while (degAngle > HALF_ROTATION_DEG)
    {
        degAngle -= FULL_ROTATION_DEG;
    }
    while (degAngle < -HALF_ROTATION_DEG)
    {
        degAngle += FULL_ROTATION_DEG;
    }

    return degAngle * DegToRad;
}



void CBaseState::SetNextState()
{
    constexpr int NEXT_STATE_MAX_SIZE = 3;
    while (actionQueue.size() <= NEXT_STATE_MAX_SIZE)
    {
        float randomNum = Randomf(0, 1);
        if (randomNum > 0.3f)
        {
            actionQueue.push(Type::WALK);
        }
        else
        {
            actionQueue.push(Type::IDLE);
        }
    }
}

