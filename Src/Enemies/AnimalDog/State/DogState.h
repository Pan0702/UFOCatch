#pragma once
#include "../../Base/StateBase.h"
#include "./../../../Common/Object3D.h"
#include "./../../../Player/Player.h"
class CADog;

class CCubeBase : public CBaseState
{
public:
    CCubeBase(CADog* cube, Type type);
    ~CCubeBase();
    CADog* m_pOwner;
    const Type m_kType;

protected:
    void NextState();
};

class CCubeIdleState : public CCubeBase
{
public:
    CCubeIdleState(CADog* cube);
    void Enter(Type type) override;
    void Update() override;

private:
    ///
    ///その場で止まるだけ///
    void Idle();
    ///
    ///animationを再生
    ///
    void IdleAnim();
    bool AnimationFinish() const;

private:
    float timerCount;
    int stateIdle;
};

class CCubeWalkState : public CCubeBase
{
public:
    CCubeWalkState(CADog* cube);
    void Enter(Type type) override;
    void Update() override;

private:
    bool BoundaryCheck(const VECTOR2& areaSize) const;
    VECTOR3 BASE_POS;
    float m_turnAmount;
    float m_moveAmount;
    float m_totalPosZMoveAmount;
    VECTOR3 m_position;
    bool m_rotation;
    float m_currentRotation;
    float m_targetRotation;
};

class CCubeSuction : public CCubeBase
{
public:
    CCubeSuction(CADog* cube);
    void Update() override;

private:
    CPlayer* m_pPlayer;
    VECTOR3 m_distanceFromObjectToUFO;
};

class CCubeDestroy : public CCubeBase
{
public:
    CCubeDestroy(CADog* cube);
    void Enter(Type type) override;
};
