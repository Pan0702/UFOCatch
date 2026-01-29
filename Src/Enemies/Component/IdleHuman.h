#pragma once
#include "ComponentBase.h"
#include "../Human/Human.h"

class CIdleHuman : public CComponentBase
{
public:
    CIdleHuman(CHuman* e);
    void Enter() override;
    void Update() override;

    void Exit() override;
    
private:
    void IdleAnim();
    void IdleSearch();
    void LookAroundAnim();
    float GetCurrentFrame() const;
    bool AnimationFinish() const;

    enum
    {
        ANIMATION,
        SEARCH,
    };

    int m_idleState = 0;
    float m_timerCnt = 0;
    bool m_isHuman = false;

    int frameCnt = 0;
    float currentAngle = 0;
    float animationTime = 0;

    // CIdleHuman専用のオーナー（CHuman型） //
    CHuman* m_pOwner;
};
