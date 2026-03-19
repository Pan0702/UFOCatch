#pragma once
#include "../Common/Object3D.h"
#include "PlayerLevel.h"

// プレイヤーのUFOを制御するクラス //
class CPlayer : public Object3D
{
public:
    // 原点から移動できる距離
    // 例えば-20~20なら20と入力 //
    CPlayer(float moveRange);
    ~CPlayer();

    ///
    /// 指定した座標が、吸い込みコーンの範囲内にあるか判定する
    /// @param targetPos 判定したい対象の座標
    /// @return true: コーン内 / false: コーン外
    ///
    bool IsWithSuctionCone(const VECTOR3& targetPos) const;

    ///
    /// 経過時間と対象位置をもとに、吸い込みによって移動する量を計算する
    /// @param moveTimeSecond 経過時間[秒]
    /// @param animalPos 対象オブジェクトの座標
    /// @return 吸い込み移動量
    ///
    VECTOR3 CalcSuctionDisplacement(float moveTimeSecond, const VECTOR3& animalPos) const;

    ///
    /// 指定した座標が、吸い込み円の範囲内にあるか判定する
    /// @param targetPos 判定したい対象の座標
    /// @return true: 円内 / false: 円外
    ///
    bool IsInsideSuctionCircle(const VECTOR3& targetPos) const;

    const VECTOR3& GetPos() const { return transform.position; }
    bool GetIsSuckUp()      const { return m_SuctionActive; }

private:
    void Update() override;
    void Draw()   override;

    void HandleMovementInput();
    void UpdateCameraPos();


    bool m_SuctionActive;
    bool m_prevSuctionActive;
    bool m_zoomUp;
    bool m_draw;
    float m_moveRange;

    CPlayerLevel* m_pLevel;
};