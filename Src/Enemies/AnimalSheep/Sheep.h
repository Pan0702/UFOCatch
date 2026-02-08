#pragma once
#include "../Base/EnemyBase.h"

class CSheep : public CEnemyBase
{
public:
    CSheep(const VECTOR3& iniPos = VECTOR3(0, 0, 0));
    ~CSheep();

    const VECTOR3& SuctionSpeed() const;

private:
    void InitStates();
    void Update() override;
    bool ShouldApplyGravity() const override;

private:
    CPlayer* m_pPlayer;
    bool m_wasSuctioned;  // 前フレームで吸い込まれていたか
};
