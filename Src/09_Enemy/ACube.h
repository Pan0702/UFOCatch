#pragma once
#include <chrono>
#include <iostream>

#include "../05_CommonFile/Object3D.h"
#include "AnimalManager.h"
#include "SpatialGrid.h"
#include "State/ACubeState.h"


class InterfaceACubeState;
class SpatialGrid;
class CACube : public CAnimalManager
{
public:
    CACube(const VECTOR3& iniPos = VECTOR3(0,0,0),const VECTOR2& moveAreaSize = VECTOR2(10,10));
    ~CACube();

    void SetState(CACubeState::Type type);
    void Destroy();
    void IsSuctionCheck();
    VECTOR3 SuctionSpeed();

    VECTOR3 GetPos() { return transform.position; }
    void AddPos(const VECTOR3& pos) { transform.position = transform.position + pos; }
    void SetRotationY(const float& angle) { transform.rotation.y = angle; }
    VECTOR3 ObjectMaxSize(){return m_maxSize;}
    VECTOR2 GetMoveAreaSize(){return m_moveAreaSize;}

private:
    void Update() override;
    void Draw();
    template <class C>
    void DrawObject(C c);
    void HitCheck();
 

private:
    CFbxMesh* m_pRedMesh;
    CFbxMesh* m_pWhiteMesh;
    MeshCollider* m_pRedColl;
    MeshCollider* m_pWhiteColl;
    SpatialGrid* m_pGrid;

    CACubeState* m_pCubeState;
    CACubeState* m_pCube;
    std::unordered_map<CACubeState::Type, CACubeState*> m_cubeStates;
    VECTOR3 m_maxSize;
    bool m_isInConeArea;
    const VECTOR3 m_basePos;
    const VECTOR2 m_moveAreaSize;
};
