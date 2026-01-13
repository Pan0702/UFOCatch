#include "FunShape.h"

CFunShape::CFunShape()
{
    m_pMesh = new CFbxMesh();
    m_pMesh->Load("data/NewAnimal/Human/AtkArea.mesh");
    transform.scale = VECTOR3(0.07f, 0.1f, 0.07f);
}

CFunShape::~CFunShape()
{
}
void CFunShape::Draw()
{
    Object3D::Draw();
}
