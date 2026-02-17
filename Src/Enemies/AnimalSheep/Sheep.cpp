#include "Sheep.h"

#include "../Component/Destroy.h"
#include "../Component/Idle.h"
#include "../Component/Suction.h"
#include "../Component/Walk.h"
#include "../../Utils/MyLib.h"

CSheep::CSheep(const VECTOR3& iniPos)
    : m_wasSuctioned(false)
{
    m_pMesh = ObjectManager::FindGameObject<CEnemyManager>()->MeshList("Sheep");
    m_pAnimator = new Animator();
    m_pAnimator->SetModel(m_pMesh);
    m_pAnimator->Play(A_IDEL);
    m_pGround = ObjectManager::FindGameObject<CGround>();
    m_pPlayer = ObjectManager::FindGameObject<CPlayer>();
    InitStates();
    transform.position = iniPos;
}
void CSheep::InitStates()
{
    //アニメーションの最終フレーム : 120.0f //
    m_components[CBaseState::State::IDLE] = new CIdle(this, 120.0f);
    //移動スピード : 1.2f//
    m_components[CBaseState::State::WALK] = new CWalk(this, 1.3f);
    m_components[CBaseState::State::SUCTION] = new CSuction(this);
    //Score : 100 ,Exp : 1.0f //
    m_components[CBaseState::State::DESTROY] = new CDestroy(this, 80, 0.8f);

    // 最初はIDLE状態から開始
    m_pComponent = m_components[CBaseState::State::IDLE];
    m_pState = new CBaseState(this);
    m_pState->Enter(CBaseState::State::IDLE);
    m_pBBox = CreateBBox();
}

CSheep::~CSheep()
{
    for (auto& c:m_components)
    {
        SAFE_DELETE(c.second);
    }
}

const VECTOR3& CSheep::SuctionSpeed() const
{
    CPlayerLevel* level = ObjectManager::FindGameObject<CPlayerLevel>();
    return CalcSuctionDisplacement(1, transform.position, m_pPlayer->GetPos(), m_pPlayer->GetIsSuckUp() ? level->GetConeTopPos() : 0.0f);
}



void CSheep::Update()
{
    // SUCTION状態から別の状態に遷移したかチェック
    bool isSuctionedNow = (m_pComponent == m_components.at(CBaseState::State::SUCTION));

    // 吸い込まれていた状態から解放された時、IDLE状態に戻る
    if (m_wasSuctioned && !isSuctionedNow)
    {
        // 完全に吸われてDESTROYになった場合を除く
        if (m_pComponent != m_components.at(CBaseState::State::DESTROY))
        {
            SetState(CBaseState::State::IDLE);
        }
    }

    m_wasSuctioned = isSuctionedNow;

    CEnemyBase::Update();
}

bool CSheep::ShouldApplyGravity() const
{
    return m_pComponent != m_components.at(CBaseState::State::SUCTION);
}
