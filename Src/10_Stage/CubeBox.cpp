#include "CubeBox.h"

CCubeBox::CCubeBox(const char* meshPath)
{
    m_pMesh = new CFbxMesh();
    m_pMesh->Load(meshPath);
}

CCubeBox::~CCubeBox()
{
}

void CCubeBox::Draw()
{
    Object3D::Draw();
}
