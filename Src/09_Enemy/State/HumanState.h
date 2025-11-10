#pragma once
#include "BaseState.h"
#include "../Actor/Human.h"

class CHumanIdleState : public CBaseState<CHuman>
{
public:
    CHumanIdleState(CHuman* human);
    void Enter() override;
    void Update() override;

private:
    void LookAround();
    void Idel();
    void RotationANgle();
    float GetCurrentFrame() const;

private:
    int stateIdle;
    int frameCount;
    float currentAngle;
    float animationTime;
    
};

class CHumanWalkState : public CBaseState<CHuman>
{
public:
    CHumanWalkState(CHuman* human);
    void Enter() override;
    void Update() override;

private:
    bool BoundaryCheck(const VECTOR2& areaSize) const;
    float m_turnAmount;
    float m_moveAmount;
    float m_totalPosZMoveAmount;
    bool m_isRotation;
    float m_currentRotation;
    float m_targetRotation;
};

class CHumanDestroy : public CBaseState<CHuman>
{
public:
    CHumanDestroy(CHuman* human);
    void Enter() override;
};
