#include "Dog.h"

#include "../../Player/Player.h"
#include "../../Stage/Ground.h"

#include "../System/EnemyManager.h"
#include "../Base/StateBase.h"
#include "../../Utils/BBox.h"
#include "../Component/Idle.h"
#include "../Component/Walk.h"
#include "../Component/Suction.h"
#include "../Component/Destroy.h"
#include "../../Utils/MyLib.h"

CADog::CADog(const VECTOR3& iniPos, const VECTOR2& moveAreaSize)
    : m_basePos(iniPos), m_areaSize(moveAreaSize)
{
    m_pMesh = ObjectManager::FindGameObject<CEnemyManager>()->MeshList("Dog");
    m_pAnimator = new Animator();
    m_pAnimator->SetModel(m_pMesh);
    m_pAnimator->Play(A_WALK);

    transform.position = iniPos;
    m_pPlayer = ObjectManager::FindGameObject<CPlayer>();
    m_pGround = ObjectManager::FindGameObject<CGround>();
    InitStates();
    m_pBBox = CreateBBox();
}

void CADog::InitStates()
{
    //アニメーションの最終フレーム : 570.0f //
    m_components[CBaseState::State::IDLE] = new CIdle(this, 570.0f);
    //移動スピード : 1.2f//
    m_components[CBaseState::State::WALK] = new CWalk(this, 1.2f);
    m_components[CBaseState::State::SUCTION] = new CSuction(this);
    //Score : 100 ,Exp : 1.0f //
    m_components[CBaseState::State::DESTROY] = new CDestroy(this, 100, 1.0f);
    m_pComponent = m_components[CBaseState::State::IDLE];
    m_pState = new CBaseState(this);
    m_pState->Enter(CBaseState::State::IDLE);
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
    
    //地面との当たり判定と、ComponentのUpdateを呼び出してる
    CEnemyBase::Update();

    // 削除フラグが立っている（CEnemyBase::Updateで処理がスキップされた）場合は、
    // これ以上の処理（衝突判定など）を行わない
    if (m_pState != nullptr && m_pComponent == m_components[CBaseState::State::DESTROY])
    {
        return;
    }

}

// void CADog::IsSuctionCheck()
// {
//     if (m_pPlayer == nullptr)return;
//     if (m_pPlayer->IsWithSuctionCone(transform.position /* + VECTOR3(0, m_maxSize.y, 0)*/) && m_pPlayer->GetIsSuckUp())
//     {
//         SetState(CBaseState::State::SUCTION);
//     }
// }

VECTOR3 CADog::SuctionSpeed() const
{
    CPlayerLevel * level = ObjectManager::FindGameObject<CPlayerLevel>();
    //1秒で吸い込む
    return CalcSuctionDisplacement(1, transform.position, m_pPlayer->GetPos(),m_pPlayer->GetTransform().position.y);
}


bool CADog::ShouldApplyGravity() const
{
  return m_pComponent != m_components.at(CBaseState::State::SUCTION);
}
