#pragma once

class CPlayer;
class CEnemyBase;
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

