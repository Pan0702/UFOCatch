#pragma once
#include <queue>

#include "../../08_Player/Player.h"
template<class T>
class CBaseState
{
public:
    enum class Type
    {
        Idle,
        Walk,
        Suction,
        Destroy,
        LookAround,
        Escape
    };

    virtual ~CBaseState(){}
    virtual void Enter(){}
    virtual void Update(){}
    virtual void Exit(){}
    
    void SetNextState()
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

protected:
    CBaseState(T* owner, Type type)
    : m_pOwner(owner), m_type(type)
    {
    }
    void Next()
    {
        Type type = actionQueue.front();
        actionQueue.pop();
        SetNextState();
        m_pOwner->SetState(type);
    }
    T* m_pOwner;
private:
    const int NEXT_STATE_MAX_SIZE = 3;
    const Type m_type;
    std::queue<CBaseState::Type> actionQueue;
    
};
