#include "TutorialState.h"
#include "Tutorial.h"

#include "TutorialAnimal.h"
#include "TutorialDisplayInfo.h"
#include "TutorialHuman.h"
#include "../System/GameInstance.h"
#include "../Player/Player.h"
#include "../Player/PlayerHP.h"
#include "../System/Timer.h"

namespace
{
    // 陦ｨ遉ｺ繧ｿ繧､繝怜ｮ壽焚 //
    constexpr int DISPLAY_TYPE_SUCTION = 1;
    constexpr int DISPLAY_TYPE_EXPANDS = 2;
    constexpr int DISPLAY_TYPE_DISCOVERY = 3;
    constexpr int DISPLAY_TYPE_PLAY = 4;

    // 繧ｯ繧ｨ繧ｹ繝育分蜿ｷ螳壽焚 //
    constexpr int QUEST_SUCTION = 1;
    constexpr int QUEST_EXPANDS = 2;

    // 諡｡螟ｧ繧ｹ繝・・繝医°繧画ｬ｡縺ｸ騾ｲ繧縺ｮ縺ｫ蠢・ｦ√↑謐慕佐謨ｰ //
    constexpr int REQUIRED_CAPTURE_COUNT = 3;
}

CTutorialState::CTutorialState(CTutorial* pT)
    : m_pTutorial(pT)
{
}

////////////////////
// 繝励Ξ繧､繝､繝ｼ縺ｫ謐慕佐縺輔ｌ縺溷虚迚ｩ繧貞炎髯､縺吶ｋ 
////////////////////
void CTutorialState::RemoveCaughtAnimals()
{
    std::list<CTutorialAnimal*> animals = ObjectManager::FindGameObjects<CTutorialAnimal>();
    for (CTutorialAnimal* animal : animals)
    {
        CPlayer* pPl = ObjectManager::FindGameObject<CPlayer>();
        // 繝励Ξ繧､繝､繝ｼ縺ｮY蠎ｧ讓吶′蜍慕黄縺ｮ荳顔ｫｯ莉･荳九↑繧画黒迯ｲ縺ｨ縺ｿ縺ｪ縺・//
        if (pPl->GetTransform().position.y <= animal->GetTransform().position.y + animal->GetMesh()->m_vMax.y)
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

void CMoveState::Exit()
{
    CTutorialDisplayInfo* pTI = ObjectManager::FindGameObject<CTutorialDisplayInfo>();
    pTI->SetDisplayType(DISPLAY_TYPE_SUCTION);
    pTI->SetQuest(QUEST_SUCTION);
}

CSuctionState::CSuctionState(CTutorial* pT)
    : CTutorialState(pT)
{
}

void CSuctionState::Update()
{
    CPlayer* pPl = ObjectManager::FindGameObject<CPlayer>();
    CTutorialAnimal* pTA = ObjectManager::FindGameObject<CTutorialAnimal>();
    if (pPl->GetTransform().position.y <= pTA->GetTransform().position.y + pTA->GetMesh()->m_vMax.y)
    {
        pTA->Destroy();
        m_pTutorial->SetState(State::Expands);
    }
}

void CSuctionState::Exit()
{
    CTutorialDisplayInfo* pTI = ObjectManager::FindGameObject<CTutorialDisplayInfo>();
    pTI->SetDisplayType(DISPLAY_TYPE_EXPANDS);
    pTI->SetQuest(QUEST_EXPANDS);
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
    int captureCount = ObjectManager::FindGameObject<CGameInstance>()->GetCapture();
    if (captureCount > REQUIRED_CAPTURE_COUNT)
    {
        m_pTutorial->SetState(State::Discovery);
    }
}

void CExpands::Exit()
{
    CTutorialDisplayInfo* pTI = ObjectManager::FindGameObject<CTutorialDisplayInfo>();
    pTI->SetDisplayType(DISPLAY_TYPE_DISCOVERY);
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
        m_pTutorial->SetState(State::Play);
    }
}

void CDiscoveryState::Exit()
{
    CTutorialDisplayInfo* pTI = ObjectManager::FindGameObject<CTutorialDisplayInfo>();
    pTI->SetDisplayType(DISPLAY_TYPE_PLAY);
    // 繝√Η繝ｼ繝医Μ繧｢繝ｫ逕ｨ莠ｺ髢薙ｒ蜑企勁 //
    ObjectManager::FindGameObject<CTutorialHuman>()->DestroyMe();
}

CPlayState::CPlayState(CTutorial* pT)
    : CTutorialState(pT)
{
}

void CPlayState::Enter()
{
    ObjectManager::FindGameObject<CTutorial>()->Lesson();
    ObjectManager::FindGameObject<CTimer>()->SetStareFlag(true);
}

void CPlayState::Update()
{
    RemoveCaughtAnimals();
}
