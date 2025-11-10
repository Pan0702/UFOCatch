#pragma once
#include <chrono>
#include <iostream>
#include <unordered_map>

#include "../../05_CommonFile/Object3D.h"
#include "AnimalManager.h"
#include "../State/BaseState.h"


class CCubeBase;

class CACube : public CAnimalManager
{
public:
    CACube(const VECTOR3& iniPos = VECTOR3(0, 0, 0), const VECTOR2& moveAreaSize = VECTOR2(10, 10));
    ~CACube();

    void SetState(CBaseState::Type type);
    ///
    ///吸い込み範囲にオブジェクトがいて、吸い込みボタンが押されていたらStateをSuctionにかえる
    ///
    void IsSuctionCheck();

    VECTOR3 SuctionSpeed() const;

    void AddPos(const VECTOR3& pos) { transform.position = transform.position + pos; }
    VECTOR2 GetMoveAreaSize() const { return m_moveAreaSize; }

private:
    void Update() override;
    void Draw() override;

private:
    CBaseState* m_pCurrentState;
    //stateを保持//
    std::unordered_map<CBaseState::Type, CCubeBase*> m_cubeStates;
    bool m_isInConeArea;
    const VECTOR3 m_basePos;
    const VECTOR2 m_moveAreaSize;
};
