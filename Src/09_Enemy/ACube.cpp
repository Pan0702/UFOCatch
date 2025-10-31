#include "ACube.h"

#include "../08_Player/Player.h"
#include <queue>
#include <thread>

#include "SpatialGrid.h"
#include "State/ACubeState.h"

namespace
{
    std::queue<InterfaceACubeState*> actionQueue;
}

CACube::CACube(const VECTOR3& iniPos, const VECTOR2& moveAreaSize)
    : m_basePos(iniPos), m_moveAreaSize(moveAreaSize)
{
    m_pWhiteMesh = new CFbxMesh();
    m_pWhiteColl = new MeshCollider();
    m_pWhiteMesh->Load("data/LowPoly/white.mesh");
    m_pWhiteColl->MakeFromMesh(m_pWhiteMesh);

    m_pRedMesh = new CFbxMesh();
    m_pRedColl = new MeshCollider();
    m_pRedMesh->Load("data/LowPoly/Red1.mesh");
    m_pRedColl->MakeFromMesh(m_pRedMesh);

    transform.position = iniPos;
    m_maxSize = m_pRedColl->bBox.max;
    m_pPlayer = ObjectManager::FindGameObject<CPlayer>();
    m_pGrid = ObjectManager::FindGameObject<SpatialGrid>();

    m_cubeStates[CACubeState::Type::Idle] = new CIdleState(this);
    m_cubeStates[CACubeState::Type::Walk] = new CWalkState(this);
    m_cubeStates[CACubeState::Type::Suction] = new CSuction(this);
    m_cubeStates[CACubeState::Type::Destroy] = new CDestroy(this);
    m_pCubeState = m_cubeStates[CACubeState::Type::Idle];
    m_pCubeState->SetNextState();
}

CACube::~CACube()
{
    SAFE_DELETE(m_pWhiteMesh);
    SAFE_DELETE(m_pRedMesh);
    SAFE_DELETE(m_pWhiteColl);
    SAFE_DELETE(m_pRedColl);
    for (auto& state : m_cubeStates)
    {
        SAFE_DELETE(state.second);
    }
}


void CACube::Update()
{
    m_isInConeArea = m_pPlayer->IsWithSuctionCone(transform.position + VECTOR3(0, m_maxSize.y, 0));

    ImGui::Begin("ACube");
    ImGui::Text("transform.position.z:%lf", transform.position.z);
    ImGui::Text("transform.Rotate.y:%lf", transform.rotation.y * RadToDeg);
    ImGui::Text("timer:%lf", time);
    ImGui::End();
    if (m_pCubeState)
    {
        m_pCubeState->Update();
    }
   // m_pGrid->Insert(this);
    
}


void CACube::Draw()
{
    if (m_isInConeArea)
    {
        DrawObject(m_pRedMesh);
    }
    else
    {
        DrawObject(m_pWhiteMesh);
    }
}


template <class C>
void CACube::DrawObject(C c)
{
    c->Render(transform.matrix());
}

//Stateをここでセット
void CACube::SetState(CACubeState::Type type)
{
    m_pCubeState->Exit();
    m_pCubeState = m_cubeStates[type];
    m_pCubeState->Enter();
}

void CACube::HitCheck()
{
    std::vector<CACube*> nearby = m_pGrid->CheckNearby(this);
    for (auto* cube : nearby)
    {
        if (cube == this)continue;
        //当たり判定
    }
}

void CACube::IsSuctionCheck()
{
    if (m_pPlayer->IsWithSuctionCone(transform.position + VECTOR3(0, m_maxSize.y, 0)) && m_pPlayer->GetIsSuckUp())
    {
        SetState(CACubeState::Type::Suction);
    }
}

VECTOR3 CACube::SuctionSpeed()
{
    return m_pPlayer->
        CalcSuctionVelocity(100, transform.position + VECTOR3(0, m_maxSize.y, 0));
}

