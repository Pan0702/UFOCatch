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
    void IsSuctionCheck();

    VECTOR3 SuctionSpeed() const;
    void DestroyCube();

    void AddPos(const VECTOR3& pos) { transform.position = transform.position + pos; }
    const VECTOR2& GetAreaSize() const { return m_areaSize; }
    void SetRotateY(float r) { transform.rotation.y = r; }
    CComponentBase* GetComponent() const { return m_pComponent; }

private:
    void Update() override;
    void Draw() override;
    bool ShouldApplyGravity() const override;

private:
    bool m_isInConeArea;
    const VECTOR3 m_basePos;
    const VECTOR2 m_areaSize;
    CPlayer* m_pPlayer;
    CComponentBase* m_pComponent;
};
