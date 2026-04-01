#pragma once
#include "../Base/EnemyBase.h"

class CSheep : public CEnemyBase
{
public:
    CSheep(const VECTOR3& iniPos = VECTOR3(0, 0, 0));
    ~CSheep();

    VECTOR3 SuctionSpeed() const override;

private:
    void InitStates();
    void Update() override;
    bool ShouldApplyGravity() const override;

private:
    CPlayer* m_pPlayer;
    bool m_isInConeArea = false;
};
