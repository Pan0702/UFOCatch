#include "DogState.h"

#include "../../../System/GameInstance.h"
#include "../Dog.h"
#include "../../System/EnemyRegistr.h"



CCubeBase::~CCubeBase()
{
    if (m_pOwner != nullptr)
    {
        m_pOwner = nullptr;
    }
}

void CCubeBase::NextState()
{
    m_pOwner->SetState(NextStatePop());
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///Idle
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CCubeSuction::CCubeSuction(CADog* cube)
    : CCubeBase(cube, Type::SUCTION)
      , m_distanceFromObjectToUFO(VECTOR3(0, 0, 0))
{
}

void CCubeSuction::Update()
{
    m_distanceFromObjectToUFO = m_pOwner->SuctionSpeed();
    m_pPlayer = ObjectManager::FindGameObject<CPlayer>();
    if (m_pPlayer != nullptr){
        if (m_pPlayer->GetIsSuckUp())
        {
            if (m_pPlayer->GetTransform().position.y - 0.15f <= m_pOwner->GetTransform().position.y )
            {
                m_pOwner->SetState(Type::DESTROY);
            }
            else
            {
                m_pOwner->AddPos(m_distanceFromObjectToUFO);
            }
        }
        else
        {
            m_pOwner->SetState(NextStatePop());
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CCubeDestroy::CCubeDestroy(CADog* cube)
    : CCubeBase(cube, Type::DESTROY)
{
}

void CCubeDestroy::Enter(Type type)
{
    ObjectManager::FindGameObject<CGameInstance>()->AddScore(1000);
    ObjectManager::FindGameObject<CGameInstance>()->AddCapture(1);
    ObjectManager::FindGameObject<CPlayer>()->AddExp(1);
    m_pOwner->DestroyMe();
}
