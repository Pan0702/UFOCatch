#pragma once
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
};
