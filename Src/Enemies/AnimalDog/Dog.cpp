#include "Dog.h"

#include "../../Player/Player.h"
#include "../../Stage/Ground.h"
#include <queue>
#include <thread>

#include "../System/EnemyRegistr.h"
#include "../Base/StateBase.h"
#include "State/DogState.h"
#include "../../Utils/BBox.h"
#include "../Component/Idle.h"
#include "../Component/Walk.h"
#include "../Component/Suction.h"

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
    
    m_components[CBaseState::Type::IDLE] = new CIdle(this,570.0f);
    m_components[CBaseState::Type::WALK] = new CWalk(this, 2.0f);
    m_components[CBaseState::Type::SUCTION] = new CSuction(this);
    m_pState->Enter(CBaseState::Type::WALK);
    m_pBBox = CreateBBox();
}

CADog::~CADog()
{
    for (auto& com : m_components)
    {
        SAFE_DELETE(com.second);
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
    if (m_pState != nullptr && m_pComponent == m_components[CBaseState::Type::DESTROY])
    {
        return;
    }

    m_pAnimator->Update();
    m_pState->Update();
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
    return m_pComponent != m_components.at(CBaseState::Type::SUCTION);
}
