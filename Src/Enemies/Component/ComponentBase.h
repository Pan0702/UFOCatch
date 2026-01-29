#pragma once
#include "../../Player/Player.h"
#include "../Base/EnemyBase.h"
#include "../System/EnemyRegistr.h"
class CEnemyBase;

class CComponentBase
{
public:
    CComponentBase();
    virtual ~CComponentBase();
    
    virtual void Enter();
    virtual void Update();
    virtual void Exit();
    
    bool IsFinish() const;
    
protected:
    CPlayer* m_pPlayer;
    CEnemyBase* m_pOwner;
    bool m_isFinish;

};
