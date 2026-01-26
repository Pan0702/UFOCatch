#include "Dog.h"

#include "../../Player/Player.h"
#include "../../Stage/Ground.h"
#include <queue>
#include <thread>

#include "../System/EnemyRegistr.h"
#include "../Base/StateBase.h"
#include "State/DogState.h"
#include "../../Utils/BBox.h"

CADog::CADog(const VECTOR3& iniPos, const VECTOR2& moveAreaSize)
    : m_basePos(iniPos), m_areaSize(moveAreaSize)
{
    m_pMesh = ObjectManager::FindGameObject<CAnimalManager>()->MeshList("Dog");
    m_pAnimator = new Animator();
    m_pAnimator->SetModel(m_pMesh);
    m_pAnimator->Play(A_WALK);

    transform.position = iniPos;
    m_pPlayer = ObjectManager::FindGameObject<CPlayer>();
    m_pGround = ObjectManager::FindGameObject<CGround>();

    m_states[CBaseState::Type::IDLE] = new CCubeIdleState(this);
    m_states[CBaseState::Type::WALK] = new CCubeWalkState(this);
    m_states[CBaseState::Type::SUCTION] = new CCubeSuction(this);
    m_states[CBaseState::Type::DESTROY] = new CCubeDestroy(this);
    m_pCurrentState = m_states[CBaseState::Type::WALK];
    m_pCurrentState->Enter();
    m_pBBox = CreateBBox();
}

CADog::~CADog()
{
    for (auto& state : m_states)
    {
        SAFE_DELETE(state.second);
    }
}


void CADog::Update()
{
    m_pPlayer = ObjectManager::FindGameObject<CPlayer>();
    if (m_pPlayer != nullptr)
    {
        m_isInConeArea = m_pPlayer->IsWithSuctionCone(transform.position);
    }

    CEnemyBase::Update();

    // 削除フラグが立っている（CEnemyBase::Updateで処理がスキップされた）場合は、
    // これ以上の処理（衝突判定など）を行わない
    if (m_pCurrentState != nullptr && m_pCurrentState == m_states[CBaseState::Type::DESTROY])
    {
        return;
    }

    m_pAnimator->Update();
    ResolveOBBCollisions();
    UpdateBBox();

    // ステージオブジェクトとの衝突判定と押し戻し（最後に実行）
    ResolveStageCollisions();
}

void CADog::Draw()
{
    m_pMesh->Render(m_pAnimator, transform.matrix());
}

void CADog::IsSuctionCheck()
{
    if (m_pPlayer == nullptr)return;
    if (m_pPlayer->IsWithSuctionCone(transform.position /* + VECTOR3(0, m_maxSize.y, 0)*/) && m_pPlayer->GetIsSuckUp())
    {
        SetState(CBaseState::Type::SUCTION);
    }
}

const VECTOR3& CADog::SuctionSpeed() const
{
    return m_pPlayer->
        CalcSuctionDisplacement(1, transform.position);
}

bool CADog::ShouldApplyGravity() const
{
    return m_pCurrentState != m_states.at(CBaseState::Type::SUCTION);
}
