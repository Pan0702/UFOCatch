#include "Ground.h"

CGround::CGround()
{
    m_pMesh = new CFbxMesh();
    m_pMeshCol = new MeshCollider();
    m_pMesh->Load("data/Ground/Ground.mesh");
    m_pMeshCol->MakeFromMesh(m_pMesh);
    transform.position = VECTOR3(0, 0, 0);
    transform.scale = VECTOR3(200, 10, 200);
}

CGround::~CGround()
{
}

void CGround::Update()
{
    Object3D::Update();
}
