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
    num = 0;
    timereset = false;
}

CACube::~CACube()
{
    if (m_pWhiteMesh != nullptr)
    {
        delete m_pWhiteMesh;
        m_pWhiteMesh = nullptr;
    }
    if (m_pWhiteColl != nullptr)
    {
        delete m_pWhiteColl;
        m_pWhiteColl = nullptr;
    }
}

void CACube::Update()
{
    m_isInConeArea = m_pPlayer->IsWithSuctionCone(transform.position + VECTOR3(0,m_maxSize.y,0));
    
    if (m_isInConeArea && m_pPlayer->GetIsSuckUp())
    {
        m_distanceFromObjectToUFO = m_pPlayer->CalcSuctionVelocity(100, transform.position + VECTOR3(0,m_maxSize.y,0));
        MoveForUFO(transform.position,m_distanceFromObjectToUFO, 1);
    }
    ImGui::Begin("ACube");
    ImGui::Text("transform.position.z:%lf", transform.position.z);
    ImGui::Text("transform.Rotate.y:%lf", transform.rotation.y * RadToDeg);
    ImGui::Text("timer:%lf", time);
    ImGui::End();
    EmemyMove();
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

void CACube::EmemyMove()
{
    
    switch (num)
    {
    case 0:
        {
            if (!timereset)
            {
                 time = 0.0f;
                timereset = true;
            }
            MATRIX4X4 mat = XMMatrixRotationY(transform.rotation.y);
            transform.position = transform.position + VECTOR3(0, 0, 0.01f) * mat;
            time += 0.01f;
            if (time >= 5.0f)
            {
                num = 1;
                timereset = false;
            }
            break;
        }
    case 1:
        {
            if (!timereset)
            {
                 time = 0.0f;
                timereset = true;
            }
            transform.rotation.y += 1.0f * DegToRad;
            time += 1.0f;
            if (time >= 90)
            {
                num = 0;
                timereset = false;
            }  
            break;
        }
    }
    
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
