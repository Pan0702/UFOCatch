#pragma once
#include "../Base/EnemyBase.h"
#include "../AnimalDog/ShepherdDog.h"
class CSheep : public CEnemyBase
{
public:

    CSheep(CAShepherdDog* shepherdDog,const VECTOR3& iniPos = VECTOR3(0, 0, 0), const VECTOR2& moveAreaSize = VECTOR2(10, 10));
    ~CSheep();

    const VECTOR3& SuctionSpeed() const;
    void OnSuctionReleased();
    CAShepherdDog* GetMaster() const { return m_pShepherdDog; }

private:
    void InitStates();
    void Update() override;
    bool ShouldApplyGravity() const override;
    void OnSuction() const;

private:
    bool m_isInConeArea;
    CPlayer* m_pPlayer;
    CAShepherdDog* m_pShepherdDog;
    bool m_wasSuctioned;  // 前フレームで吸い込まれていたか
};
