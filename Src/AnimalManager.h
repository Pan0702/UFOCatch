#pragma once
#include "Object3D.h"

class CAnimalManager:public Object3D
{
public:
    CAnimalManager();
    ~CAnimalManager();
private:
    void Update() override;
    void Draw() override;
    void WhiteDraw();
    void RedDraw();
    VECTOR3 GetObjectSize(MeshCollider* mesh_collider) const;
    
    CFbxMesh* m_pRedMesh;
    CFbxMesh* m_pWhiteMesh;
    MeshCollider* m_pRedColl;
    MeshCollider* m_pWhiteColl;

    VECTOR3 m_maxSize;
};
