#pragma once
#include "../Base/EnemyBase.h"

class CFlog;

class CSheep : public CEnemyBase
{
public:
    CSheep(const VECTOR3& iniPos = VECTOR3(0, 0, 0));
    ~CSheep();

    VECTOR3 SuctionSpeed() const override;
    void SetFlog(CFlog* flog);
    CFlog* GetFlog() const;

private:
    void InitStates();
    void Update() override;
    bool ShouldApplyGravity() const override;

private:
    CPlayer* m_pPlayer;
    bool m_isInConeArea = false;
    CFlog* m_pFlog = nullptr;
    bool m_wasOutside = false;
};
