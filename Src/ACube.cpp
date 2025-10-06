#include "ACube.h"

#include "Player.h"
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
}

CACube::~CACube()
{
}

void CACube::Update()
{
    m_isInConeArea = ObjectManager::FindGameObject<CPlayer>()->IsInConeArea(transform.position + VECTOR3(0,m_maxSize.y,0));

    bool inputKey = GameDevice()->m_pDI->CheckKey(KD_DAT,DIK_1);
    if (m_isInConeArea && inputKey && !m_isMovingToUFO)
    {
        m_isMovingToUFO = true;
    }
    
    if (m_isMovingToUFO && inputKey)
    {
        m_distanceFromObjectToUFO = ObjectManager::FindGameObject<CPlayer>()->SuckUpAnimal(300, transform.position + VECTOR3(0,m_maxSize.y,0));
        
        CAnimalManager::MoveForUFO(transform.position,m_distanceFromObjectToUFO);
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
