#pragma once
#include <unordered_map>

#include "StateBase.h"
#include "../../05_CommonFile/Object3D.h"

class CGround;

class CEnemyBase : public Object3D
{
public:
    CEnemyBase();
    virtual void SetState(CBaseState::Type type);

    ~CEnemyBase();
    void Update() override;

protected:
    virtual void ApplyGravity();
    virtual bool IsGrounded() const;
    virtual bool ShouldApplyGravity() const { return true; }

    CBaseState* m_pCurrentState;
    std::unordered_map<CBaseState::Type, CBaseState*> m_cubeStates;

    CGround* m_pGround;
    float m_velocityY;
    
};
