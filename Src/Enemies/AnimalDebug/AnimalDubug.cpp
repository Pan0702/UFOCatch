#include "AnimalDubug.h"

#include "../../Player/Player.h"
#include "../../Stage/Ground.h"
#include "../Component/Idle.h"
#include "../Base/StateBase.h"
#include "../System/EnemyRegistr.h"

CADebug::CADebug(const VECTOR3& iniPos, const VECTOR2& moveAreaSize)
    : m_basePos(iniPos)
{
    m_pMesh = ObjectManager::FindGameObject<CAnimalManager>()->MeshList("Dog");
    m_pAnimator = new Animator();
    m_pAnimator->SetModel(m_pMesh);
    m_pAnimator->Play(A_WALK);

    m_areaSize = moveAreaSize;
    transform.position = iniPos;
    m_pPlayer = ObjectManager::FindGameObject<CPlayer>();
    m_pGround = ObjectManager::FindGameObject<CGround>();

    m_states[CBaseState::Type::IDLE] = new CCubeIdleState(this);
    m_states[CBaseState::Type::WALK] = new CCubeWalkState(this);
    m_states[CBaseState::Type::SUCTION] = new CCubeSuction(this);
    m_states[CBaseState::Type::DESTROY] = new CCubeDestroy(this);
    
    m_components[CBaseState::Type::IDLE] = new CIdle();
    m_pState = m_states[CBaseState::Type::WALK];
    m_pState->Enter(TODO);
    m_pBBox = CreateBBox();
}

CADebug::~CADebug()
{

}


void CADebug::Update()
{
    m_pPlayer = ObjectManager::FindGameObject<CPlayer>();
    if (m_pPlayer != nullptr)
    {
        m_isInConeArea = m_pPlayer->IsWithSuctionCone(transform.position);
    }

    CEnemyBase::Update();

    // 削除フラグが立っている（CEnemyBase::Updateで処理がスキップされた）場合は、
    // これ以上の処理（衝突判定など）を行わない
    if (m_pState != nullptr && m_pState == m_states[CBaseState::Type::DESTROY])
    {
        return;
    }

    m_pAnimator->Update();
    ResolveOBBCollisions();
    UpdateBBox();

    // ステージオブジェクトとの衝突判定と押し戻し（最後に実行）
    ResolveStageCollisions();
}

void CADebug::Draw()
{
    m_pMesh->Render(m_pAnimator, transform.matrix());
}

void CADebug::IsSuctionCheck()
{
    if (m_pPlayer == nullptr)return;
    if (m_pPlayer->IsWithSuctionCone(transform.position /* + VECTOR3(0, m_maxSize.y, 0)*/) && m_pPlayer->GetIsSuckUp())
    {
        SetState(CBaseState::Type::SUCTION);
    }
}

const VECTOR3& CADebug::SuctionSpeed() const
{
    return m_pPlayer->
        CalcSuctionDisplacement(1, transform.position);
}

bool CADebug::ShouldApplyGravity() const
{
    return m_pComponent != m_components.at(CBaseState::Type::SUCTION);
}
