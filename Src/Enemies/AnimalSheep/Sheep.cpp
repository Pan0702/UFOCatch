#include "Sheep.h"

#include "../Component/Destroy.h"
#include "../Component/Idle.h"
#include "../Component/Suction.h"
#include "../Component/Walk.h"
#include "../../Utils/MyLib.h"
#include "../../Common/ShadowObject.h"

CSheep::CSheep(const VECTOR3& iniPos)
{
    m_pMesh = ObjectManager::FindGameObject<CEnemyManager>()->MeshList("Sheep");
    m_pAnimator = std::make_unique<Animator>();     
    m_pAnimator->SetModel(m_pMesh);
    m_pAnimator->Play(A_IDEL);
    m_pGround = ObjectManager::FindGameObject<CGround>();
    m_pPlayer = ObjectManager::FindGameObject<CPlayer>();
    InitStates();
    transform.position = iniPos;
    Instantiate<CShadowObject>(this, TEXT("data/CircleSuction.png"));
}
void CSheep::InitStates()
{
    //アニメーションの最終フレーム : 120.0f //
    m_components[CBaseState::State::IDLE] = std::make_unique< CIdle>(this, 120.0f);
    //移動スピード : 1.2f//
    m_components[CBaseState::State::WALK] = std::make_unique< CWalk>(this, 1.3f);
    m_components[CBaseState::State::SUCTION] = std::make_unique< CSuction>(this);
    //Score : 100 ,Exp : 1.0f //
    m_components[CBaseState::State::DESTROY] = std::make_unique<CDestroy>(this, 80, 0.8f);

    // 最初はIDLE状態から開始
    m_pComponent = m_components[CBaseState::State::IDLE].get();
    m_pState = std::make_unique<CBaseState>(this);
    m_pState->Enter(CBaseState::State::IDLE);
    m_pBBox = CreateBBox();
}

CSheep::~CSheep()
{
}

VECTOR3 CSheep::SuctionSpeed() const
{
    constexpr float suctionTime = 1.0f;
    return m_pPlayer->CalcSuctionDisplacement(suctionTime,transform.position);
}



void CSheep::Update()
{
    m_pPlayer = ObjectManager::FindGameObject<CPlayer>();
    if (m_pPlayer != nullptr)
    {
        m_isInConeArea = m_pPlayer->IsWithSuctionCone(transform.position);
    }
    CEnemyBase::Update();
}

bool CSheep::ShouldApplyGravity() const
{
    return m_pComponent != m_components.at(CBaseState::State::SUCTION).get();
}
