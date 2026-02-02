#include "Sheep.h"
#include "../Component/Idle.h"
#include "../Component/Suction.h"
CSheep::CSheep(CAShepherdDog* shepherdDog,const VECTOR3& iniPos, const VECTOR2& moveAreaSize)
    :m_basePos(iniPos), m_areaSize(moveAreaSize), m_pShepherdDog(shepherdDog), m_wasSuctioned(false)
{
    InitStates();
    transform.position = iniPos;
}
void CSheep::InitStates()
{
    m_components[CBaseState::State::IDLE] = new CIdle(this,1.2f);
    m_components[CBaseState::State::HERDED] = new CHerded(this);
    m_components[CBaseState::State::PANIC] = new CPanic(this);
    m_components[CBaseState::State::SUCTION] = new CSuction(this);
    m_pComponent = m_components[CBaseState::State::IDLE];
    m_pState = new CBaseState(this);
    m_pState->Enter(CBaseState::State::IDLE);
}

CSheep::~CSheep()
{
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
