#pragma once
#include "ComponentBase.h"

class CIdle : public CComponentBase
{
public:
    CIdle();
    ~CIdle();

    CIdle(CEnemyBase* e);
    void Enter() override;
    void Update() override;

    void Exit() override;
    
private:
    void IdleStop();
    void IdleAnim();
    bool AnimationFinish() const;
    
    enum
    {
        ANIMATION,
        STOP,
    };
    
    int m_idleState = 0;
    float m_timerCnt = 0;
};
