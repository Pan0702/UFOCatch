#include "FunShape.h"

CFunShape::CFunShape()
{
    m_pMesh = new CFbxMesh();
    m_pMesh->Load("data/NewAnimal/Human/AtkArea.mesh");
    m_pMesh->m_vDiffuse = VECTOR4(1.0f, 1.0f, 1.0f, 0.5f);
    transform.scale = VECTOR3(1.0f, 1.0f, 1.0f);
}

CFunShape::~CFunShape()
{
    SAFE_DELETE(m_pMesh);
}

void CFunShape::PosSet(const VECTOR3& pos, const float angle)
{
    transform.position = VECTOR3(pos.x,0,pos.z);
    transform.rotation.y = angle;
}
