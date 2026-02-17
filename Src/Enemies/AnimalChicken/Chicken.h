#pragma once

#include "../../Common/Object3D.h"
#include "../Base/EnemyBase.h"
#include "../../Player/Player.h"


class CChickenBase;

class CAnimalChicken : public CEnemyBase
{
public:
    CAnimalChicken(const VECTOR3& iniPos = VECTOR3(0, 0, 0), const VECTOR2& moveAreaSize = VECTOR2(10, 10));
    ~CAnimalChicken();

    void IsSuctionCheck();

    VECTOR3 SuctionSpeed() const;

    void AddPos(const VECTOR3& pos) { transform.position = transform.position + pos; }
    CXAudioSource* GetAudio() const { return m_pCry; }

private:
    void Update() override;
    void Draw() override;
    bool ShouldApplyGravity() const override;

private:
    bool m_isInConeArea;
    const VECTOR3 m_basePos;
    const VECTOR2 m_areaSize;
    CPlayer* m_pPlayer;
    CXAudioSource* m_pCry ;
};