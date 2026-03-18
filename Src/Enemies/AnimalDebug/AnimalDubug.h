#pragma once
#include "../Base/EnemyBase.h"
class CPlayer;

class CADebug : public CEnemyBase
{
public:
    CADebug();
    CADebug(const VECTOR3& iniPos, const VECTOR2& moveAreaSize);
    ~CADebug();
private:
    ///
    ///蜷ｸ縺・ｾｼ縺ｿ遽・峇縺ｫ繧ｪ繝悶ず繧ｧ繧ｯ繝医′縺・※縲∝精縺・ｾｼ縺ｿ繝懊ち繝ｳ縺梧款縺輔ｌ縺ｦ縺・◆繧唄tate繧担uction縺ｫ縺九∴繧・
    ///
    void IsSuctionCheck();

    VECTOR3 SuctionSpeed() const override;

    void AddPos(const VECTOR3& pos) { transform.position = transform.position + pos; }


private:
    void Update() override;
    void Draw() override;
    bool ShouldApplyGravity() const override;

private:
    bool m_isInConeArea;
    const VECTOR3 m_basePos;
    CPlayer* m_pPlayer;

};

