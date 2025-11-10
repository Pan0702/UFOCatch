#include "BaseState.h"

class CACube;

class CCubeIdleState : public CBaseState<CACube>
{
public:
    CCubeIdleState(CACube* cube);
    void Enter() override;
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

class CCubeWalkState : public CBaseState<CACube>
{
public:
    CCubeWalkState(CACube* cube);
    void Enter() override;
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

class CCubeSuction : public CBaseState<CACube>
{
public:
    CCubeSuction(CACube* cube);
    void Update() override;

private:
    CPlayer* m_pPlayer;
    VECTOR3 m_distanceFromObjectToUFO;
};

class CCubeDestroy : public CBaseState<CACube>
{
public:
    CCubeDestroy(CACube* cube);
    void Enter() override;
};