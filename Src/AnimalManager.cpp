#include "AnimalManager.h"

#include "BBox.h"
#include "Player.h"

CAnimalManager::CAnimalManager()
{
    m_pWhiteMesh = new CFbxMesh();
    m_pWhiteColl = new MeshCollider();
    m_pWhiteMesh->Load("data/LowPoly/white.mesh");
    m_pWhiteColl->MakeFromMesh(m_pWhiteMesh);
    
    m_pRedMesh = new CFbxMesh();
    m_pRedColl = new MeshCollider();
    m_pRedMesh->Load("data/LowPoly/Red1.mesh");
    m_pRedColl->MakeFromMesh(m_pRedMesh);

    transform.position = VECTOR3(0, 0, 0);
    m_maxSize = GetObjectSize(m_pRedColl);
}

CAnimalManager::~CAnimalManager()
{
}

void CAnimalManager::Update()
{
ImGui::Begin("AnimalManager");
    ImGui::Text("%lf\n:%lf\n:%lf", m_maxSize.x, m_maxSize.y + transform.position.y,m_maxSize.z);
    ImGui::End();
    
}

void CAnimalManager::Draw()
{
    if (ObjectManager::FindGameObject<CPlayer>()->IsInConeArea(transform.position + VECTOR3(0,m_maxSize.y,0)))
    {
        RedDraw();
    }else
    {
        WhiteDraw();       
    }
}

void CAnimalManager::WhiteDraw()
{
    m_pWhiteMesh->Render(transform.matrix());
}

void CAnimalManager::RedDraw()
{
    m_pRedMesh->Render(transform.matrix());
}

VECTOR3 CAnimalManager::GetObjectSize(MeshCollider* meshCollider) const
{
    return meshCollider->bBox.max;
}

