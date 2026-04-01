#pragma once
#include "ComponentBase.h"

class CIdle : public CComponentBase
{
public:
    CIdle(CEnemyBase* e, float endFrame);
    void Enter() override;
    void Update() override;
    
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
    bool m_isHuman = false;
    const float m_endFrame;
};
