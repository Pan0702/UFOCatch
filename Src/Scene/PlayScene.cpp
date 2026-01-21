#include "PlayScene.h"
#include <cassert>

#include "../System/GameInstance.h"
#include "../Enemies/System/EnemyRegistr.h"
#include "../Player/PCamera.h"
#include "../Player/Player.h"
#include "../Enemies/AnimalDog/Dog.h"
#include "../Enemies/System/AnimalFactor.h"
#include "../System/DisplayInfo.h"
#include "../System/VisionSystem.h"
#include "../Stage/StageFactor.h"
#include "../System/Timer.h"

PlayScene::PlayScene()
{
    Instantiate<CStageFactor>();
    Instantiate<CPlayerCamera>();
    new CAnimalManager(2);
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
