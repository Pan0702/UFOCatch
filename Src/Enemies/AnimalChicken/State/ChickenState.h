#include "../../Base/StateBase.h"
#include "../../../Common/Object3D.h"
#include "../../../Player/Player.h"
class CAnimalChicken;

class CChickenBase : public CBaseState
{
public:
    CChickenBase(CAnimalChicken* chicken, Type type);
    ~CChickenBase();
    CAnimalChicken* m_pOwner;
    const Type m_kType;

protected:
    void NextState();
};

class CChickenIdleState : public CChickenBase
{
public:
    CChickenIdleState(CAnimalChicken* chicken);
    void Enter() override;
    void Update() override;

private:
    void Idle();
    void IdleAnim();
    bool AnimationFinish() const;

private:
    float timerCount;
    int stateIdle;
};

class CChickenWalkState : public CChickenBase
{
public:
    CChickenWalkState(CAnimalChicken* chicken);
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

class CChickenSuction : public CChickenBase
{
public:
    CChickenSuction(CAnimalChicken* chicken);
    void Update() override;

private:
    CPlayer* m_pPlayer;
    VECTOR3 m_distanceFromObjectToUFO;
};

class CChickenDestroy : public CChickenBase
{
public:
    CChickenDestroy(CAnimalChicken* chicken);
    void Enter() override;
};
