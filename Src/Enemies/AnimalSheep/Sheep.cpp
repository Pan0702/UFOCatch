#include "Sheep.h"

CSheep::CSheep(CAShepherdDog* shepherdDog,const VECTOR3& iniPos, const VECTOR2& moveAreaSize)
    :m_basePos(iniPos), m_areaSize(moveAreaSize), m_pShepherdDog(shepherdDog)
{
    InitStates();
    transform.position = iniPos;
}
void CSheep::InitStates()
{
    m_components[CBaseState::State::IDLE]; 
    m_components[CBaseState::State::HERDED]; 
    m_components[CBaseState::State::PANIC]; 
    m_components[CBaseState::State::SUCTION]; 
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
    CEnemyBase::Update();
}

bool CSheep::ShouldApplyGravity() const
{
    return m_pComponent != m_components.at(CBaseState::State::SUCTION);
}
