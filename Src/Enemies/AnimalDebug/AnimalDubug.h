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
    ///吸い込み範囲にオブジェクトがいて、吸い込みボタンが押されていたらStateをSuctionにかえる
    ///
    void IsSuctionCheck();

    const VECTOR3& SuctionSpeed() const override;

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
