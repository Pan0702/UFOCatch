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
    /// @brief 羊の各状態コンポーネントを生成し、初期状態をIDLEに設定する
    void InitStates();

    /// @brief 吸い込みコーン内判定を更新してから敵共通の更新を行う
    void Update() override;

    /// @brief 吸い込み中以外は重力を適用するか判定する
    /// @return SUCTION状態でなければtrue
    bool ShouldApplyGravity() const override;

private:
    CPlayer* m_pPlayer;
    bool m_isInConeArea = false;
    CFlock* m_pFlock = nullptr;
    bool m_wasOutside = false;
};
