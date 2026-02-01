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
    ///吸い込み範囲にオブジェクトがいて、吸い込みボタンが押されていたらStateをSuctionにかえる
    ///
    // void IsSuctionCheck();

    const VECTOR3& SuctionSpeed() const;

private:
    void InitStates();
    void Update() override;
    bool ShouldApplyGravity() const override;


private:
    bool m_isInConeArea;
    const VECTOR3 m_basePos;
    const VECTOR2 m_areaSize;
    CPlayer* m_pPlayer;
};
