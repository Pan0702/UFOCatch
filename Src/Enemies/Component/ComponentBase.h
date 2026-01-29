#pragma once
#include "../../Player/Player.h"
#include "../System/EnemyRegistr.h"
#include "../Base/EnemyBase.h"

class CComponentBase
{
public:
    CComponentBase() = default;
    virtual ~CComponentBase()
    {
        // m_pPlayer と m_pOwner は参照用ポインタなので削除しない
        // 実際のオブジェクトは別の場所で管理されている
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
