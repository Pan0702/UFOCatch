#include "AnimalChicken.h"

#include "../../Player/Player.h"
#include "../../Stage/Ground.h"
#include <queue>
#include <thread>

#include "../System/EnemyRegistr.h"
#include "../Base/StateBase.h"
#include "State/ChickenState.h"
#include "../../Utils/BBox.h"

CAnimalChicken::CAnimalChicken(const VECTOR3& iniPos, const VECTOR2& moveAreaSize)
    : m_basePos(iniPos), m_areaSize(moveAreaSize)
{
    m_pMesh = ObjectManager::FindGameObject<CAnimalManager>()->MeshList("Chicken");
    m_pAnimator = new Animator();
    m_pAnimator->SetModel(m_pMesh);
    m_pAnimator->Play(A_WALK);

    transform.position = iniPos;
    m_pPlayer = ObjectManager::FindGameObject<CPlayer>();
    m_pGround = ObjectManager::FindGameObject<CGround>();

    m_cubeStates[CBaseState::Type::IDLE] = new CChickenIdleState(this);
    m_cubeStates[CBaseState::Type::WALK] = new CChickenWalkState(this);
    m_cubeStates[CBaseState::Type::SUCTION] = new CChickenSuction(this);
    m_cubeStates[CBaseState::Type::DESTROY] = new CChickenDestroy(this);
    m_pCurrentState = m_cubeStates[CBaseState::Type::WALK];
    m_pCurrentState->Enter();
    m_pBBox = CreateBBox();
}

CAnimalChicken::~CAnimalChicken()
{
    for (auto& state : m_cubeStates)
    {
        SAFE_DELETE(state.second);
    }
}


void CAnimalChicken::Update()
{
    m_pPlayer = ObjectManager::FindGameObject<CPlayer>();
    if (m_pPlayer != nullptr)
    {
        m_isInConeArea = m_pPlayer->IsWithSuctionCone(transform.position);
    }

    CEnemyBase::Update();

    if (m_pCurrentState != nullptr && m_pCurrentState == m_cubeStates[CBaseState::Type::DESTROY])
    {
        return;
    }

    m_pAnimator->Update();
    ResolveOBBCollisions();
    UpdateBBox();

    ResolveStageCollisions();
}

void CAnimalChicken::Draw()
{
    m_pMesh->Render(m_pAnimator, transform.matrix());
}

void CAnimalChicken::IsSuctionCheck()
{
    if (m_pPlayer == nullptr)return;
    if (m_pPlayer->IsWithSuctionCone(transform.position) && m_pPlayer->GetIsSuckUp())
    {
        SetState(CBaseState::Type::SUCTION);
    }
}

VECTOR3 CAnimalChicken::SuctionSpeed() const
{
    return m_pPlayer->
        CalcSuctionDisplacement(1, transform.position);
}

bool CAnimalChicken::ShouldApplyGravity() const
{
    return m_pCurrentState != m_cubeStates.at(CBaseState::Type::SUCTION);
}