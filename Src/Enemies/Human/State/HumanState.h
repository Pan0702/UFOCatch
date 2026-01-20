#pragma once
#include "../../Base/StateBase.h"
#include "../../../Common/Object3D.h"
class CHuman;

class CHumanBase : public CBaseState
{
public:
    CHumanBase(CHuman* cube, Type type);
    ~CHumanBase();
    CHuman* m_pOwner;
    const Type m_kType;

protected:
    void NextState();
};

class CHumanIdleState : public CHumanBase
{
public:
    CHumanIdleState(CHuman* human);
    void Enter() override;
    void Update() override;

private:
    void LookAround();
    void Idle();
    void RotationAngle();
    float GetCurrentFrame() const;

private:
    int stateIdle;
    int frameCount;
    float currentAngle;
    float animationTime;
};

class CHumanWalkState : public CHumanBase
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

class CHumanFindPlayer : public CHumanBase
{
public:
    CHumanFindPlayer(CHuman* human);
    void Enter() override;
    void Update() override;
};
