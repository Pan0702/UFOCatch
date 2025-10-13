#pragma once
#include <chrono>
#include "../05_CommonFile/Object3D.h"
#include "AnimalManager.h"

class CACube: public CAnimalManager
{
public:
    CACube();
    ~CACube();
private:
    void Update() override;
    void Draw();
    void WhiteDraw();
    void RedDraw();
    void MoveForUFO(const VECTOR3& animalPos, const VECTOR3& distanceFromObjectToUFO, const int& exp);

    CFbxMesh* m_pRedMesh;
    CFbxMesh* m_pWhiteMesh;
    MeshCollider* m_pRedColl;
    MeshCollider* m_pWhiteColl;

    VECTOR3 m_maxSize;
    bool m_isInConeArea;
    bool m_pushButton;
    VECTOR3 m_distanceFromObjectToUFO;
    bool m_isMovingToUFO = false;
    bool m_isDestroyMe;
    std::chrono::steady_clock::time_point m_moveStartTime;

};
