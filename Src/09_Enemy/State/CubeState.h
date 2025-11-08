#include "BaseState.h"

class CIdleState : public CBaseState
{
public:
    CIdleState(CACube* cube);
    void Enter() override;
    void Update() override;

private:
    ///
    ///その場で止まるだけ
    ///
    void Stop();
    ///
    ///animationを再生
    ///
    void Idle();
    bool AnimationFinish();
private:
    float timerCount;
    int stateWait;
};

class CWalkState : public CBaseState
{
public:
    CWalkState(CACube* cube);
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

class CSuction : public CBaseState
{
public:
    CSuction(CACube* cube);
    void Update() override;

private:
    CPlayer* m_pPlayer;
    VECTOR3 m_distanceFromObjectToUFO;
};

class CDestroy : public CBaseState
{
public:
    CDestroy(CACube* cube);
    void Enter() override;
};