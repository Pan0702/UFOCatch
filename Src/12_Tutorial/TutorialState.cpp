#include "TutorialState.h"
#include "Tutorial.h"

#include "TutorialAnimal.h"
#include "TutorialDisplayInfo.h"
#include "TutorialHuman.h"
#include "../GameInstance.h"
#include "../08_Player/Player.h"
#include "../08_Player/PHP.h"
#include "../11_GameSystem/Timer.h"

namespace
{
    // 表示タイプ定数 //
    constexpr int DISPLAY_TYPE_SUCTION = 1;
    constexpr int DISPLAY_TYPE_EXPANDS = 2;
    constexpr int DISPLAY_TYPE_DISCOVERY = 3;
    constexpr int DISPLAY_TYPE_PLAY = 4;

    // クエスト番号定数 //
    constexpr int QUEST_SUCTION = 1;
    constexpr int QUEST_EXPANDS = 2;

    // 拡大ステートから次へ進むのに必要な捕獲数 //
    constexpr int REQUIRED_CAPTURE_COUNT = 3;
}

CTutorialState::CTutorialState(CTutorial* pT)
    : m_pTutorial(pT)
{
}

////////////////////
// プレイヤーに捕獲された動物を削除する //
////////////////////
void CTutorialState::RemoveCaughtAnimals()
{
    std::list<CTutorialAnimal*> animals = ObjectManager::FindGameObjects<CTutorialAnimal>();
    for (CTutorialAnimal* animal : animals)
    {
        CPlayer* pPl = ObjectManager::FindGameObject<CPlayer>();
        // プレイヤーのY座標が動物の上端以下なら捕獲とみなす //
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
    // チュートリアル用人間を削除 //
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
