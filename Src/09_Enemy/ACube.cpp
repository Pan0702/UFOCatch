#include "ACube.h"

#include "../08_Player/Player.h"
#include <chrono>
#include <thread>

CACube::CACube()
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
    m_maxSize = CAnimalManager::GetObjectSize(m_pRedColl);
    m_isMovingToUFO = false;
    m_isDestroyMe = false;
    m_pPlayer = ObjectManager::FindGameObject<CPlayer>();
}

CACube::~CACube()
{
}

void CACube::Update()
{
    m_isInConeArea = m_pPlayer->IsWithSuctionCone(transform.position + VECTOR3(0,m_maxSize.y,0));
    
    if (m_isInConeArea && m_pPlayer->GetIsSuckUp())
    {
        m_distanceFromObjectToUFO = m_pPlayer->SuckUpAnimal(100, transform.position + VECTOR3(0,m_maxSize.y,0));
        MoveForUFO(transform.position,m_distanceFromObjectToUFO, 1);
    }
    
}


void CACube::Draw()
{
    if (m_isInConeArea)
    {
        RedDraw();
    }else
    {
        WhiteDraw();       
    }
}

void CACube::WhiteDraw()
{
    m_pWhiteMesh->Render(transform.matrix());
}

void CACube::RedDraw()
{
    m_pRedMesh->Render(transform.matrix());
}
void CACube::MoveForUFO(const VECTOR3& animalPos,const VECTOR3& distanceFromObjectToUFO, const int& exp)
{
    if (m_pPlayer->GetPos().y <= animalPos.y)  
    {
        m_isDestroyMe = true;
        m_pPlayer->AddExp(exp);
        DestroyMe();
    }
    else
    {
        transform.position += distanceFromObjectToUFO;
    }
}
