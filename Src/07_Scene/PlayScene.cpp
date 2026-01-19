#include "PlayScene.h"
#include <cassert>

#include "../11_GameSystem/GameInstance.h"
#include "../09_Enemy/System//AnimalManager.h"
#include "../08_Player/PCamera.h"
#include "../08_Player/Player.h"
#include "../09_Enemy/AnimalDog//ACube.h"
#include "../09_Enemy/System//AnimalFactor.h"
#include "../11_GameSystem/DisplayInfo.h"
#include "../11_GameSystem/VisionSystem.h"
#include "../10_Stage/StageFactor.h"
#include "../11_GameSystem/Timer.h"

PlayScene::PlayScene()
{
    Instantiate<CStageFactor>();
    Instantiate<CPlayerCamera>();
    SingleInstantiate<CAnimalManager>();
    new CTimer(30);
    Instantiate<CAnimalFactor>();
    new CPlayer(20);
    Instantiate<CVisionSystem>();
    Instantiate<CDisplayInfo>();
    ObjectManager::FindGameObject<CGameInstance>()->Init(1000);

}

PlayScene::~PlayScene() = default;

void PlayScene::Update()
{
    if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_T))
    {
        SceneManager::ChangeScene("TitleScene");
    }
    if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_1))
    {
        SceneManager::ChangeScene("ResultScene");
    }
    if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_2))
    {
        SceneManager::ChangeScene("SelectScene");
    }
    if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_0))
    {
        SceneManager::ChangeScene("Debug");
    }
}

void PlayScene::Draw()
{
    GameDevice()->m_pFont->Draw(
        20, 20, "PlayScene", 16, RGB(255, 255, 0));
}

////////////////////
// リザルトシーンへ遷移する //
////////////////////
void PlayScene::ChangeResultScene()
{
    SceneManager::ChangeScene("ResultScene");
}
