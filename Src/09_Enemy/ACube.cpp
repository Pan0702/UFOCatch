#include "ACube.h"

#include "../08_Player/Player.h"
#include <queue>
#include <thread>

#include "AnimalManager.h"
#include "AnimalManager.h"
#include "SpatialGrid.h"
#include "State/ACubeState.h"

CACube::CACube(const VECTOR3& iniPos, const VECTOR2& moveAreaSize)
    : m_basePos(iniPos), m_moveAreaSize(moveAreaSize)
{
    m_pMesh= new CFbxMesh();
    m_pAnimator = new Animator();
    m_pMesh->Load("data/NewAnimal/Dog/Dog.mesh");
    m_pAnimator->SetModel(m_pMesh);
    m_pMesh->LoadAnimation(AnimationType::A_IDEL, "data/NewAnimal/Dog/Dog_Idle.anmx", true);
     m_pMesh->LoadAnimation(AnimationType::A_WALK, "data/NewAnimal/Dog/Dog_Idle.anmx", true);
     m_pMesh->LoadAnimation(AnimationType::A_RUN, "data/NewAnimal/Dog/Dog_Idle.anmx", true);
    
    m_pAnimator->Play(0);

    

    transform.position = iniPos;
    m_pPlayer = ObjectManager::FindGameObject<CPlayer>();
    

    m_cubeStates[CACubeState::Type::Idle] = new CIdleState(this);
    m_cubeStates[CACubeState::Type::Walk] = new CWalkState(this);
    m_cubeStates[CACubeState::Type::Suction] = new CSuction(this);
    m_cubeStates[CACubeState::Type::Destroy] = new CDestroy(this);
    m_pCubeState = m_cubeStates[CACubeState::Type::Idle];
    m_pCubeState->SetNextState();
}

CACube::~CACube()
{
    for (auto& state : m_cubeStates)
    {
        SAFE_DELETE(state.second);
    }
}


void CACube::Update()
{
    m_isInConeArea = m_pPlayer->IsWithSuctionCone(transform.position);
    m_pAnimator->Update();
    ImGui::Begin("ACube");
    ImGui::Text("transform.position.z:%lf", transform.position.z);
    ImGui::Text("transform.Rotate.y:%lf", transform.rotation.y * RadToDeg);
    ImGui::Text("timer:%lf", time);
    ImGui::End();
    if (m_pCubeState)
    {
        m_pCubeState->Update();
    }
    HitCheck();
   // m_pGrid->Insert(this);
    
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
// #if 0
//     std::vector<CACube*> nearby = m_pGrid->CheckNearby(this);
//     for (auto* cube : nearby)
//     {
//         if (cube == this)continue;
//         //当たり判定
//     }
// #endif
    
}

void CACube::Draw()
{
    m_pMesh->Render(m_pAnimator,transform.matrix());
}

void CACube::IsSuctionCheck()
{
    if (m_pPlayer->IsWithSuctionCone(transform.position  /* + VECTOR3(0, m_maxSize.y, 0)*/) && m_pPlayer->GetIsSuckUp())
    {
        SetState(CACubeState::Type::Suction);
    }
}

VECTOR3 CACube::SuctionSpeed()
{
    return m_pPlayer->
        CalcSuctionVelocity(100, transform.position);
}

