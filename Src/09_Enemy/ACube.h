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

    void InitCubes();


    
    std::shared_ptr<CFbxMesh> m_pRedMesh;
    std::shared_ptr<CFbxMesh> m_pWhiteMesh;
    std::shared_ptr<MeshCollider> m_pRedColl;
    std::shared_ptr<MeshCollider> m_pWhiteColl;

    VECTOR3 m_maxSize;
    bool m_isInConeArea;
    bool m_pushButton;
    VECTOR3 m_distanceFromObjectToUFO;
    bool m_isMovingToUFO = false;
    bool m_isDestroyMe;

};
