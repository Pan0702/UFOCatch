#include "ACube.h"

#include "../../08_Player/Player.h"
#include <queue>
#include <thread>

#include "AnimalManager.h"
#include "../SpatialGrid.h"
#include "../State/BaseState.h"
#include "../State/CubeState.h"

CACube::CACube(const VECTOR3& iniPos, const VECTOR2& moveAreaSize)
    : m_basePos(iniPos), m_moveAreaSize(moveAreaSize)
{
    m_pMesh = new CFbxMesh();
    m_pAnimator = new Animator();
    m_pMesh->Load("data/NewAnimal/Dog/Dog.mesh");
    m_pAnimator->SetModel(m_pMesh);
    m_pMesh->LoadAnimation(AnimationType::A_IDEL, "data/NewAnimal/Dog/Dog_Idle.anmx", false);
    m_pMesh->LoadAnimation(AnimationType::A_WALK, "data/NewAnimal/Dog/Dog_Walk.anmx", true);
    m_pMesh->LoadAnimation(AnimationType::A_RUN, "data/NewAnimal/Dog/Dog_Idle.anmx", true);

    m_pAnimator->Play(A_WALK);

    transform.position = iniPos;
    m_pPlayer = ObjectManager::FindGameObject<CPlayer>();

    m_cubeStates[CBaseState<CACube>::Type::Idle] = new CCubeIdleState(this);
    m_cubeStates[CBaseState<CACube>::Type::Walk] = new CCubeWalkState(this);
    m_cubeStates[CBaseState<CACube>::Type::Suction] = new CCubeSuction(this);
    m_cubeStates[CBaseState<CACube>::Type::Destroy] = new CCubeDestroy(this);
    m_pCurrentState = m_cubeStates[CBaseState<CACube>::Type::Idle];
    m_pCurrentState->SetNextState();
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
    if (m_pCurrentState)
    {
        m_pCurrentState->Update();
    }
    ImGui::Begin("begin");
    ImGui::Text("CurrentAnim: %lf", transform.rotation.y * RadToDeg);
    ImGui::End();
}


//Stateをここでセット
void CACube::SetState(CBaseState<CACube>::Type type)
{
    m_pCurrentState->Exit();
    m_pCurrentState = m_cubeStates[type];
    m_pCurrentState->Enter();
}

void CACube::Draw()
{
    m_pMesh->Render(m_pAnimator, transform.matrix());
}

void CACube::IsSuctionCheck()
{
    if (m_pPlayer->IsWithSuctionCone(transform.position /* + VECTOR3(0, m_maxSize.y, 0)*/) && m_pPlayer->GetIsSuckUp())
    {
        SetState(CBaseState<CACube>::Type::Suction);
    }
}

VECTOR3 CACube::SuctionSpeed() const
{
    return m_pPlayer->
        CalcSuctionVelocity(100, transform.position);
}



