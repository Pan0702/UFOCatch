#include "ConeDraw.h"

CConeDraw::CConeDraw()
{
    m_pMesh = new CFbxMesh();
    m_pMesh->Load("data/Player/Cone.mesh");
    transform.position = VECTOR3(0,0,0);
    m_pPlayer = ObjectManager::FindGameObject<CPlayer>();
}

CConeDraw::~CConeDraw() = default;

void CConeDraw::Update()
{
    Object3D::Update();
}

void CConeDraw::Draw()
{
    if (m_pPlayer->GetIsSuckUp())Object3D::Draw();
}
