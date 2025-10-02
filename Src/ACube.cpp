#include "ACube.h"

#include "Player.h"

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
    m_pushButton = true;
}

CACube::~CACube()
{
}

void CACube::Update()
{
    m_isInConeArea = ObjectManager::FindGameObject<CPlayer>()->IsInConeArea(transform.position + VECTOR3(0,m_maxSize.y,0));

    if (m_isInConeArea && m_pushButton)
    {
        m_distanceFromObjectToUFO = ObjectManager::FindGameObject<CPlayer>()->SuckUpAnimal(transform.position + VECTOR3(0,m_maxSize.y,0));
        MoveForUFO();
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

void CACube::MoveForUFO()
{
    transform.posm_distanceFromObjectToUFO
}
