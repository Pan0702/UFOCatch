#include "Sheep.h"

#include "../Component/ComponentFwd.h"
#include "../../Common/ShadowObject.h"
#include "../System/Flog.h"
#include "../Component/SheepComp.h"

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
    m_components[CBaseState::State::IDLE] = std::make_unique<CIdle>(this, 120.0f);
    //移動スピード : 1.2f//
    m_components[CBaseState::State::WALK] = std::make_unique<CWalk>(this, 1.3f);
    m_components[CBaseState::State::SUCTION] = std::make_unique<CSuction>(this);
    //Score : 100 ,Exp : 1.0f //
    m_components[CBaseState::State::DESTROY] = std::make_unique<CDestroy>(this, 80, 0.8f);
    m_components[CBaseState::State::HERDED] = std::make_unique<CHerded>(this);
    m_components[CBaseState::State::PANIC] = std::make_unique<CPanic>(this);

    // 最初はIDLE状態から開始
    m_pComponent = m_components[CBaseState::State::IDLE].get();
    m_pState = std::make_unique<CBaseState>(this);
    m_pState->Enter(CBaseState::State::IDLE);
    constexpr float SHRINK = 0.6f;
    m_pBBox = CreateBBox(SHRINK);
}

CSheep::~CSheep()
{
    if (m_pFlog != nullptr)
    {
        m_pFlog->RemoveSheep(this);
        m_pFlog = nullptr;
    }
}

VECTOR3 CSheep::SuctionSpeed() const
{
    constexpr float suctionTime = 1.0f;
    return m_pPlayer->CalcSuctionDisplacement(suctionTime, transform.position);
}

void CSheep::SetFlog(CFlog* flog)
{
    if (flog == m_pFlog) return;
    if (m_pFlog != nullptr)
    {
        m_pFlog->RemoveSheep(this);
    }
    m_pFlog = flog;
    if (m_pFlog != nullptr)
    {
        m_pFlog->AddSheep(this);
    }
}

CFlog* CSheep::GetFlog() const
{
    return m_pFlog;
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
