#include "StateBase.h"
#include "EnemyBase.h"
#include "../../Utils/MyMath.h"
#include "../AnimalSheep/Sheep.h"
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
        // デバッグ用に型名と数値を表示
        std::string str = "State Component is Null: Type=" + std::to_string(num);
        MessageBox(nullptr, str.c_str(), "Error", MB_OK);
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
    if (m_pComponent)
    {
        m_pComponent->Exit();
    }
    m_pComponent = nullptr;
}

CBaseState::State CBaseState::NextStatePop()
{
    // Suctionコンポーネントが完了している場合はDestroyに切り替える
    CSuction* suctionComponent = dynamic_cast<CSuction*>(m_pEnemy->GetComponent(State::SUCTION));
    if (suctionComponent != nullptr && suctionComponent->IsFinishSuction())
    {
        return State::DESTROY;
    }
    // 吸い込みが中断（IsFinish）した場合はWALKに戻る
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
    // キューが空きすぎないように補充する
    constexpr int NEXT_STATE_MAX_SIZE = 3;
    while (actionQueue.size() <= NEXT_STATE_MAX_SIZE)
    {
        float randomNum = Randomf(0, 1);
        CSheep* s = dynamic_cast<CSheep*>(m_pEnemy);
        if (s)
        {
            // 羊（CSheep）の場合の確率
            if (randomNum > 0.7f)
            {
                actionQueue.push(State::WALK);
            }
            else
            {
                actionQueue.push(State::IDLE);
            }
        }
        else
        {
            // それ以外（通常エネミー）の場合の確率
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
}
