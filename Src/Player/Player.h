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