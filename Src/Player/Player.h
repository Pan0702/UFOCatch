#pragma once
#include "../Common/Object3D.h"
#include "PlayerLevel.h"

// プレイヤーのUFOを制御するクラス //
class CPlayer : public Object3D
{
public:
    // 原点から移動できる距離
    // 例えば-20~20なら20と入力//
    CPlayer(float moveRange);
    ~CPlayer();

    ///
    /// オブジェクトの場所を渡し、それがコーンの範囲内ならtrue,外ならfalse
    /// @param targetPos
    /// @return bool
    ///
    bool IsWithSuctionCone(const VECTOR3& targetPos) const;
    bool IsInsideSuctionCircle(const VECTOR3& targetPos) const;

    // 引き寄せるための移動量を計算する
    // 高さの差が大きいほど遅く、近いほど速く吸い込む
    // @param moveTimeSecond  移動にかける時間
    // @param animalPos  動物の位置
    // @return 1回当たりの移動量 //
    VECTOR3 CalcSuctionDisplacement(const float& moveTimeSecond, const VECTOR3& animalPos) const;

    ///
    /// レイを飛ばしてオブジェクトとの交差判定を行う
    /// @param rayStart レイの始点
    /// @param rayEnd レイの終点
    /// @param hitPosition 衝突位置を格納する変数（Out）
    /// @param hitNormal 衝突面の法線を格納する変数（Out）
    /// @return 衝突していたらtrue
    ///
    bool CastRay(const VECTOR3& rayStart, const VECTOR3& rayEnd, MeshCollider::CollInfo*& coll) const;

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