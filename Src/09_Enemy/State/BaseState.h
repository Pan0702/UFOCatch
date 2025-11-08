#pragma once
#include <queue>

#include "../../08_Player/Player.h"

class CACube;

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

    virtual ~CBaseState()
    {
    }

    virtual void Enter()
    {
    }

    virtual void Update()
    {
    }

    virtual void Exit()
    {
    }

    virtual void SetNextState();

protected:
    CBaseState(CACube* cube, Type type);
    void Next();
    CACube* m_pCube;
    const Type m_type;
    std::queue<CBaseState::Type> actionQueue;
};
