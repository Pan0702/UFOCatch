#include "ACube.h"

#include "../../08_Player/Player.h"
#include "../../10_Stage/Ground.h"
#include <queue>
#include <thread>

#include "../System/AnimalManager.h"
#include "../Base/StateBase.h"
#include "State/CubeState.h"

CACube::CACube(const VECTOR3& iniPos, const VECTOR2& moveAreaSize)
    : m_basePos(iniPos), m_moveAreaSize(moveAreaSize)
{
    m_pMesh = ObjectManager::FindGameObject<CAnimalManager>()->MeshList("Dog");
    m_pAnimator = new Animator();
    m_pAnimator->SetModel(m_pMesh);
    m_pAnimator->Play(A_WALK);

    transform.position = iniPos;
    m_pPlayer = ObjectManager::FindGameObject<CPlayer>();
    m_pGround = ObjectManager::FindGameObject<CGround>();

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

    CEnemyBase::Update();

    m_pAnimator->Update();
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
void CACube::DestroyCube()
{
    DestroyMe();
}

bool CACube::ShouldApplyGravity() const
{
    return m_pCurrentState != m_cubeStates.at(CBaseState::Type::SUCTION);
}
