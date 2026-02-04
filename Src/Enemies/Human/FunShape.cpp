#include "FunShape.h"

CFunShape::CFunShape()
{
    m_pMesh = new CFbxMesh();
    m_pMesh->Load("data/NewAnimal/Human/AtkArea.mesh");
    transform.scale = VECTOR3(1.0f, 1.0f, 1.0f);
}

CFunShape::~CFunShape()
{
}
void CFunShape::Draw()
{
    Object3D::Draw();
}
