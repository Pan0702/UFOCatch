#pragma once

#include "../../Common/Object3D.h"
#include "../Base/EnemyBase.h"
#include "../../Player/Player.h"
#include "../Component/ComponentBase.h"


class CCubeBase;

class CADog : public CEnemyBase
{
public:
    CADog(const VECTOR3& iniPos = VECTOR3(0, 0, 0), const VECTOR2& moveAreaSize = VECTOR2(10, 10));
    ~CADog();

    ///
    ///蜷ｸ縺・ｾｼ縺ｿ遽・峇縺ｫ繧ｪ繝悶ず繧ｧ繧ｯ繝医′縺・※縲∝精縺・ｾｼ縺ｿ繝懊ち繝ｳ縺梧款縺輔ｌ縺ｦ縺・◆繧唄tate繧担uction縺ｫ縺九∴繧・
    ///
    // void IsSuctionCheck();

    VECTOR3 SuctionSpeed() const override;

private:
    void InitStates();
    void Update() override;
    bool ShouldApplyGravity() const override;


protected:
    CPlayer* m_pPlayer;
private:
    bool m_isInConeArea;
    const VECTOR3 m_basePos;
    const VECTOR2 m_areaSize;

};

