#include "ConeDraw.h"

CConeDraw::CConeDraw(float coneTopPos)
{
    m_pMesh = new CFbxMesh();
    m_pMesh->Load("data/Player/Cone.mesh");
    transform.position = VECTOR3(0, 0, 0);
    m_pPlayer = ObjectManager::FindGameObject<CPlayer>();
    transform.scale.y  = coneTopPos / 5;
}

CConeDraw::~CConeDraw()
{
    SAFE_DELETE(m_pMesh);
}

void CConeDraw::Update()
{
    Object3D::Update();
    transform.scale = VECTOR3(m_pPlayer->GetRadius() / 2 , transform.scale.y , m_pPlayer->GetRadius() / 2);
    transform.position = VECTOR3(m_pPlayer->GetTransform().position.x, 0, m_pPlayer->GetTransform().position.z);
}

void CConeDraw::Draw()
{
    if (m_pPlayer->GetIsSuckUp())Object3D::Draw();
}
