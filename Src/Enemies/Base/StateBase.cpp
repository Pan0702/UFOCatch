#include "StateBase.h"
#include "../../Utils/MyMath.h"

CBaseState::CBaseState(CEnemyBase* enemy, Type type)
    : m_pEnemy(enemy), m_kType(type)
{
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

