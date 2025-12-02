#include "PlayScene.h"
#include <fstream>
#include "../06_GameLib/CsvReader.h"
#include <assert.h>
#include "../09_Enemy/Actor/AnimalManager.h"
#include "../08_Player/PCamera.h"
#include "../08_Player/Player.h"
#include "../09_Enemy/Actor/ACube.h"
#include "../09_Enemy/Actor/AnimalFactor.h"
#include "../09_Enemy/Actor/Human.h"
#include "../10_Stage/Ground.h"
#include "../11_GameSystem/DisplayInfo.h"
#include "../11_GameSystem/TimeLimit.h"
#include "../11_GameSystem/VisionSystem.h"


PlayScene::PlayScene()
{
    
    Instantiate<CPlayerCamera>();
    new CAnimalManager();
   Instantiate<CGround>();
    //new CTimeLimit(30);
    Instantiate<CAnimalFactor>();
    Instantiate<CPlayer>();
    Instantiate<CVisionSystem>();
    Instantiate<CDisplayInfo>();

}

PlayScene::~PlayScene()
{
}

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

void PlayScene::ChangeResultScene()
{
    SceneManager::ChangeScene("ResultScene");
}
