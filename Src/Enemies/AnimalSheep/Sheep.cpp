#include "Sheep.h"

#include "../Component/Destroy.h"
#include "../Component/Idle.h"
#include "../Component/Suction.h"
#include "../Component/SheepComp.h"
#include "../Component/Walk.h"

CSheep::CSheep(CAShepherdDog* shepherdDog,const VECTOR3& iniPos, const VECTOR2& moveAreaSize)
    :m_pShepherdDog(shepherdDog), m_wasSuctioned(false)
{
    m_pMesh = ObjectManager::FindGameObject<CEnemyManager>()->MeshList("Sheep");
    m_pAnimator = new Animator();
    m_pAnimator->SetModel(m_pMesh);
    m_pAnimator->Play(A_WALK);
    m_pGround = ObjectManager::FindGameObject<CGround>();
    m_pPlayer = ObjectManager::FindGameObject<CPlayer>();
    InitStates();
    transform.position = iniPos;
}
void CSheep::InitStates()
{
    //アニメーションの最終フレーム : 120.0f //
    m_components[CBaseState::State::IDLE] = new CIdle(this,120.0f);
    //移動スピード : 1.2f//
    m_components[CBaseState::State::WALK] = new CWalk(this,1.2f);
    m_components[CBaseState::State::SUCTION] = new CSuction(this);
    m_components[CBaseState::State::HERDED] = new CHerded(this);
    m_components[CBaseState::State::PANIC] = new CPanic(this);
    //Score : 100 ,Exp : 1.0f //
    m_components[CBaseState::State::DESTROY] = new CDestroy(this,100,1.0f);
    // 最初からHERDED状態でBoidsで動く
    m_pComponent = m_components[CBaseState::State::HERDED];
    m_pState = new CBaseState(this);
    m_pState->Enter(CBaseState::State::HERDED);
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
    return m_pPlayer->
       CalcSuctionDisplacement(1, transform.position);
}



void CSheep::Update()
{

    // SUCTION状態から別の状態に遷移したかチェック
    bool isSuctionedNow = (m_pComponent == m_components.at(CBaseState::State::SUCTION));

    if (isSuctionedNow)
    {
        OnSuction();
    }
    // 吸い込まれていた状態から解放された時
    if (m_wasSuctioned && !isSuctionedNow)
    {
        // 完全に吸われてDESTROYになった場合は救助不要
        if (m_pComponent != m_components.at(CBaseState::State::DESTROY))
        {
            OnSuctionReleased();
        }
    }

    
    m_wasSuctioned = isSuctionedNow;

    CEnemyBase::Update();
}

bool CSheep::ShouldApplyGravity() const
{
    return m_pComponent != m_components.at(CBaseState::State::SUCTION);
}

void CSheep::OnSuction()
{
    if (m_pShepherdDog != nullptr)
    {
        m_pShepherdDog->ChangeStateHerded(this);  // 犬に報告
        m_pShepherdDog->StartHerding();  // 犬に群れ制御開始を指示
        m_pShepherdDog->RescueSheep(this);  // Queue rescue while suctioning
    }
}

void CSheep::OnSuctionReleased()
{
    // 吸い込みから解放された（地面に戻った）時の処理
    if (m_pShepherdDog != nullptr)
    {
        m_pShepherdDog->RescueSheep(this);  // 犬に救助を要請
    }
}
