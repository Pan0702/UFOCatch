#pragma once
#include "../../Player/Player.h"
#include "../System/EnemyManager.h"
#include "../Base/EnemyBase.h"

class CComponentBase
{
public:
    CComponentBase() = default;
    virtual ~CComponentBase()
    {
    }
    
    virtual void Enter(){}
    virtual void Update(){}
    virtual void Exit()
    {
        m_isFinish = false;
    }
    
    bool IsFinish() const{return m_isFinish;}
    
protected:
    CPlayer* m_pPlayer;
    CEnemyBase* m_pOwner;
    bool m_isFinish;

};

