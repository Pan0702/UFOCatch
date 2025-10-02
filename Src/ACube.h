#pragma once
#include <chrono>

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

    CFbxMesh* m_pRedMesh;
    CFbxMesh* m_pWhiteMesh;
    MeshCollider* m_pRedColl;
    MeshCollider* m_pWhiteColl;

    VECTOR3 m_maxSize;
    bool m_isInConeArea;
    bool m_pushButton;
    VECTOR3 m_distanceFromObjectToUFO;
    bool m_isMovingToUFO = false;
    std::chrono::steady_clock::time_point m_moveStartTime;

};
