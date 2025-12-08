#pragma once
#include <unordered_map>

#include "StateBase.h"
#include "../../05_CommonFile/Object3D.h"

class CEnemyBase : public Object3D
{
public:
    CEnemyBase();
    virtual void SetState(CBaseState::Type type);


    ~CEnemyBase();
    void Update() override;

protected:
    
    
    CBaseState* m_pCurrentState;
    std::unordered_map<CBaseState::Type, CBaseState*> m_cubeStates;
};
