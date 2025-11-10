#include "BaseState.h"
#include "../Actor/ACube.h"
namespace
{
    constexpr int NEXT_STATE_MAX_SIZE = 3;
}


CBaseState::Type CBaseState::NextStatePop()
{
    SetNextState();
    Type type = actionQueue.front();
    actionQueue.pop();
    return type;
}

void CBaseState::SetNextState()
{
    while (actionQueue.size() <= NEXT_STATE_MAX_SIZE)
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

