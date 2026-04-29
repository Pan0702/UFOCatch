#pragma once
#include "../Base/EnemyBase.h"

class CFlock;

class CSheep : public CEnemyBase
{
public:
    CSheep(const VECTOR3& iniPos = VECTOR3(0, 0, 0));
    ~CSheep();

    VECTOR3 SuctionSpeed() const override;
    void SetFlock(CFlock* flock);
    CFlock* GetFlock() const;

private:
    void InitStates();
    void Update() override;
    bool ShouldApplyGravity() const override;

private:
    CPlayer* m_pPlayer;
    bool m_isInConeArea = false;
    CFlock* m_pFlock = nullptr;
    bool m_wasOutside = false;
};
