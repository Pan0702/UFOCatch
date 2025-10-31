#pragma once
#include <queue>

#include "../../08_Player/Player.h"

class CACube;

class CACubeState
{
public:
    enum class Type
    {
        Idle,
        Walk,
        Suction,
        Destroy,
    };

    virtual ~CACubeState()
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
    CACubeState(CACube* cube, Type type);
    CACube* m_pCube;
    const Type m_type;
    
};

class CIdleState : public CACubeState
{
public:
    CIdleState(CACube* cube);
    void Update() override;

private:
    int timerCount;
};

class CWalkState : public CACubeState
{
public:
    CWalkState(CACube* cube);
    void Enter() override;
    void Update() override;

private:
    bool BoundaryCheck(const VECTOR2& areaSize) const;
    VECTOR3 BASE_POS;
    float m_moveSpeed;
    float m_turnAmount;
    float m_moveAmount;
    float m_totalPosZMoveAmount;
    VECTOR3 m_position;
};

class CSuction : public CACubeState
{
public:
    CSuction(CACube* cube);
    void Update() override;

private:
    CPlayer* m_pPlayer;
    VECTOR3 m_distanceFromObjectToUFO;
};

class CDestroy : public CACubeState
{
public:
    CDestroy(CACube* cube);
    void Enter() override;
};
