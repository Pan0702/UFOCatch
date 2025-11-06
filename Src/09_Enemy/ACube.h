#pragma once
#include <chrono>
#include <iostream>

#include "../05_CommonFile/Object3D.h"
#include "AnimalManager.h"
#include "SpatialGrid.h"
#include "State/ACubeState.h"


class CACube : public CAnimalManager
{
public:
    CACube(const VECTOR3& iniPos = VECTOR3(0,0,0),const VECTOR2& moveAreaSize = VECTOR2(10,10));
    ~CACube();

    void SetState(CACubeState::Type type);
    void IsSuctionCheck();
    VECTOR3 SuctionSpeed() const;
    void SetAnim(const int& animNum) const;
    bool AnimationFinish() const;
    
    void AddPos(const VECTOR3& pos) { transform.position = transform.position + pos; }
    void SetRotationY(const float& angle) ;
    VECTOR2 GetMoveAreaSize() const{return m_moveAreaSize;}

private:
    void Update() override;
    void Draw() override;

private:

    CACubeState* m_pCubeState;
    std::unordered_map<CACubeState::Type, CACubeState*> m_cubeStates;
    bool m_isInConeArea;
    const VECTOR3 m_basePos;
    const VECTOR2 m_moveAreaSize;
};
