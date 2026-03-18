#pragma once
#include "../Common/Object3D.h"
#include "PlayerLevel.h"

// 繝励Ξ繧､繝､繝ｼ縺ｮUFO繧貞宛蠕｡縺吶ｋ繧ｯ繝ｩ繧ｹ //
class CPlayer : public Object3D
{
public:
    // 蜴溽せ縺九ｉ遘ｻ蜍輔〒縺阪ｋ霍晞屬
    // 萓九∴縺ｰ-20~20縺ｪ繧・0縺ｨ蜈･蜉・/
    CPlayer(float moveRange);
    ~CPlayer();

    ///
    /// 繧ｪ繝悶ず繧ｧ繧ｯ繝医・蝣ｴ謇繧呈ｸ｡縺励√◎繧後′繧ｳ繝ｼ繝ｳ縺ｮ遽・峇蜀・↑繧液rue,螟悶↑繧映alse
    /// @param targetPos
    /// @return bool
    ///
    bool IsWithSuctionCone(const VECTOR3& targetPos) const;
    VECTOR3 CalcSuctionDisplacement(float moveTimeSecond, const VECTOR3& animalPos) const;
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
