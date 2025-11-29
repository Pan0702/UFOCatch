#include "Ground.h"

void CGround::Draw()
{
    Object3D::Draw();
}

CGround::CGround()
{
    m_pMesh = new CFbxMesh();
    m_pMeshCol = new MeshCollider();
    //10cm~10cm‚Ì”Â//
    m_pMesh->Load("data/Ground/Ground.mesh");
    m_pMeshCol->MakeFromMesh(m_pMesh);
    transform.position = VECTOR3(0, 0, 0);
    transform.scale = VECTOR3(1000, 10, 1000);

}

CGround::~CGround() = default;

void CGround::Update()
{
    Object3D::Update();
}
