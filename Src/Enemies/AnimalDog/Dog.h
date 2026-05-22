#include "../Component/ComponentBase.h"
#pragma once

#include "../../Common/Object3D.h"
#include "../Base/EnemyBase.h"
#include "../../Player/Player.h"


/// <summary>敵AIで使う Cube Base の情報と処理をまとめる型</summary>
class CCubeBase;

/// <summary>敵AIで使う ADog の情報と処理をまとめる型</summary>
class CADog : public CEnemyBase
{
public:
    /// CADog を初期化する
    /// @param iniPos 初期座標
    /// @param moveAreaSize サイズ
    CADog(const VECTOR3& iniPos = VECTOR3(0, 0, 0), const VECTOR2& moveAreaSize = VECTOR2(10, 10));
    /// CADog の終了処理を行う
    ~CADog();

    ///
    ///
    // void IsSuctionCheck();

    /// Suction Speed を返す
    /// @return 3次元ベクトル
    VECTOR3 SuctionSpeed() const override;

private:
    /// States を初期化する
    void InitStates();
    /// 毎フレームの状態を更新する
    void Update() override;
    /// Should Apply Gravity を返す
    /// @return 成功または条件を満たす場合 true
    bool ShouldApplyGravity() const override;

protected:
    CPlayer* m_pPlayer;

private:
    bool m_isInConeArea;
    const VECTOR3 m_basePos;
    const VECTOR2 m_areaSize;
};
