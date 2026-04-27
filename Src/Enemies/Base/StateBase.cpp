#include "StateBase.h"
#include "EnemyBase.h"
#include "../../Utils/MyMath.h"
#include "../Component/Suction.h"
# define STR(var) #var

CBaseState::CBaseState(CEnemyBase* e)
{
    m_pEnemy = e;
}

void CBaseState::Enter(State type)
{
    m_kType = type;
    m_pComponent = m_pEnemy->GetComponent(type);
    if (m_pComponent == nullptr)
    {
        int num = static_cast<int>(m_kType);
        std::string str = STR(m_kType) + std::to_string(num) +
            " Component is Null";
        MessageBox(nullptr, str.c_str(), _T(str.c_str()), MB_OK);
    }
    m_pComponent->Enter();
}

void CBaseState::Update()
{
    if (m_pComponent == nullptr) return;

    m_pComponent->Update();
    if (m_pComponent->IsFinish())
    {
        m_pEnemy->ChangeState(NextStatePop());
    }
}

void CBaseState::Exit()
{
    m_pComponent->Exit();
    m_pComponent = nullptr;
}

CBaseState::State CBaseState::NextStatePop()
{
    // Suction繧ｳ繝ｳ繝昴・繝阪Φ繝医′螳御ｺ・＠縺ｦ縺・ｋ蝣ｴ蜷医・Destroy縺ｫ蛻・ｊ譖ｿ縺医ｋ
    CSuction* suctionComponent = dynamic_cast<CSuction*>(m_pEnemy->GetComponent(State::SUCTION));
    if (suctionComponent != nullptr && suctionComponent->IsFinishSuction())
    {
        return State::DESTROY;
    }
    if (suctionComponent != nullptr && suctionComponent->IsFinish())
    {
        return State::WALK;
    }

    SetNextState();
    State type = actionQueue.front();
    actionQueue.pop();
    return type;
}

float CBaseState::ClampRotateY(const float& angle)
{
    float degAngle = angle * RadToDeg;
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
            actionQueue.push(State::WALK);
        }
        else
        {
            actionQueue.push(State::IDLE);
        }
    }
}
