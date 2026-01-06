#pragma once
#include <chrono>
#include <iostream>
#include <unordered_map>

#include "../../05_CommonFile/Object3D.h"
#include "../Base/EnemyBase.h"
#include "../Base/StateBase.h"


class CCubeBase;

class CACube : public CEnemyBase
{
public:
    CACube(const VECTOR3& iniPos = VECTOR3(0, 0, 0), const VECTOR2& moveAreaSize = VECTOR2(10, 10));
    ~CACube();
    
    ///
    ///吸い込み範囲にオブジェクトがいて、吸い込みボタンが押されていたらStateをSuctionにかえる
    ///
    void IsSuctionCheck();

    VECTOR3 SuctionSpeed() const;
    void DestroyCube();

    void AddPos(const VECTOR3& pos) { transform.position = transform.position + pos; }
    VECTOR2 GetAreaSize() const { return m_areaSize; }
    void SetRotateY(float r){transform.rotation.y = r;}

private:
    void Update() override;
    void Draw() override;
    void CollisionCheck();

    bool ShouldApplyGravity() const override;

private:
    bool m_isInConeArea;
    const VECTOR3 m_basePos;
    const VECTOR2 m_areaSize;
    CPlayer* m_pPlayer;
};
