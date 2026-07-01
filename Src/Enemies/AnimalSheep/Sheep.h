#pragma once
#include "../Base/EnemyBase.h"

/// <summary>敵AIで使う Flock の情報と処理をまとめる型</summary>
class CFlock;

/// <summary>敵AIで使う Sheep の情報と処理をまとめる型</summary>
class CSheep : public CEnemyBase
{
public:
    /// CSheep を初期化する
    /// @param iniPos 初期座標
    CSheep(const VECTOR3& iniPos = VECTOR3(0, 0, 0));
    /// CSheep の終了処理を行う
    ~CSheep();

    /// Suction Speed を返す
    /// @return 3次元ベクトル
    VECTOR3 SuctionSpeed() const override;
    /// Flock を設定する
    /// @param flock flock に渡す値
    void SetFlock(CFlock* flock);
    /// Flock を取得する
    /// @return 対象のポインタ
    CFlock* GetFlock() const;

private:
    /// States を初期化する
    void InitStates();

    /// 毎フレームの状態を更新する
    void Update() override;

    /// Should Apply Gravity を返す
    /// @return 成功または条件を満たす場合 true
    bool ShouldApplyGravity() const override;

private:
    CPlayer* m_pPlayer;
    bool m_isInConeArea = false;
    CFlock* m_pFlock = nullptr;
    bool m_wasOutside = false;
};
