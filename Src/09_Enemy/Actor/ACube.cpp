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

    m_cubeStates[CBaseState::Type::IDLE] = new CCubeIdleState(this);
    m_cubeStates[CBaseState::Type::WALK] = new CCubeWalkState(this);
    m_cubeStates[CBaseState::Type::SUCTION] = new CCubeSuction(this);
    m_cubeStates[CBaseState::Type::DESTROY] = new CCubeDestroy(this);
    m_pCurrentState = m_cubeStates[CBaseState::Type::IDLE];
    m_pCurrentState->Enter();
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
    m_pPlayer = ObjectManager::FindGameObject<CPlayer>();
    if (m_pPlayer != nullptr)
    {
        m_isInConeArea = m_pPlayer->IsWithSuctionCone(transform.position);
    }
    m_pAnimator->Update();
    if (m_pCurrentState)
    {
        m_pCurrentState->Update();
    }
}


//Stateをここでセット
void CACube::SetState(CBaseState::Type type)
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
    if (m_pPlayer == nullptr)return;
    if (m_pPlayer->IsWithSuctionCone(transform.position /* + VECTOR3(0, m_maxSize.y, 0)*/) && m_pPlayer->GetIsSuckUp())
    {
        SetState(CBaseState::Type::SUCTION);
    }
}

VECTOR3 CACube::SuctionSpeed() const
{
    return m_pPlayer->
        CalcSuctionDisplacement(1, transform.position);
}



