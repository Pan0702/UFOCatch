#include "TutorialState.h"
#include "Tutorial.h"

#include "TutorialAnimal.h"
#include "../GameInstance.h"
#include "../08_Player/Player.h"
#include "../08_Player/PHP.h"

CTutorialState::CTutorialState(CTutorial* pT)
    : m_pTutorial(pT)
{
}

void CTutorialState::RemoveCaughtAnimals()
{
    std::list<CTutorialAnimal*> animals = ObjectManager::FindGameObjects<CTutorialAnimal>();
    for (CTutorialAnimal* animal : animals)
    {
        CPlayer* pPl = ObjectManager::FindGameObject<CPlayer>();
        if (pPl->GetTransform().position.y - 0.25f <= animal->GetTransform().position.y)
        {
            animal->Destroy();
        }
    }
}

CMoveState::CMoveState(CTutorial* pT)
    : CTutorialState(pT)
{
}

void CMoveState::Update()
{
    CPlayer* pPl = ObjectManager::FindGameObject<CPlayer>();
    CTutorialAnimal* pTA = ObjectManager::FindGameObject<CTutorialAnimal>();
    if (pPl->IsWithSuctionCone(pTA->GetTransform().position))
    {
        m_pTutorial->SetState(State::Suction);
    }
}

CSuctionState::CSuctionState(CTutorial* pT)
    : CTutorialState(pT)
{
}

void CSuctionState::Update()
{
    CPlayer* pPl = ObjectManager::FindGameObject<CPlayer>();
    CTutorialAnimal* pTA = ObjectManager::FindGameObject<CTutorialAnimal>();
    if (pPl->GetTransform().position.y - 0.25f <= pTA->GetTransform().position.y)
    {
        pTA->Destroy();
        m_pTutorial->SetState(State::Expands);
    }
}

CExpands::CExpands(CTutorial* pT)
    : CTutorialState(pT)
{
}

void CExpands::Enter()
{
    m_pTutorial->SpawnAnimal();
}

void CExpands::Update()
{
    RemoveCaughtAnimals();
    int score = ObjectManager::FindGameObject<CGameInstance>()->GetScore();
    if (score > 5)
    {
        m_pTutorial->SetState(State::Discovery);
    }
}

CDiscoveryState::CDiscoveryState(CTutorial* pT)
    : CTutorialState(pT)
{
}

void CDiscoveryState::Enter()
{
    m_pTutorial->SpawnHuman();
}

void CDiscoveryState::Update()
{
    RemoveCaughtAnimals();
    CPlayerHP* pHP = ObjectManager::FindGameObject<CPlayerHP>();
    if (pHP->GetHP() < pHP->GetMaxHP())
    {
        
    }
}

CPlayState::CPlayState(CTutorial* pT)
    : CTutorialState(pT)
{
}

void CPlayState::Enter()
{
    ObjectManager::FindGameObject<CTutorial>()->
}

void CPlayState::Update()
{
    RemoveCaughtAnimals();
}
