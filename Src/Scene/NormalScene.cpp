#include "NormalScene.h"

#include "../System/GameInstance.h"
#include "../Enemies/System/EnemyManager.h"
#include "../Player/PCamera.h"
#include "../Player/Player.h"
#include "../Enemies/AnimalDog/Dog.h"
#include "../Enemies/System/AnimalFactor.h"
#include "../Enemies/System/Flog.h"
#include "../System/DisplayInfo.h"
#include "../System/VisionSystem.h"
#include "../Stage/StageFactor.h"
#include "../System/Timer.h"
CNormalScene::CNormalScene()
{
    Instantiate<CStageFactor>();
    ObjectManager::FindGameObject<CStageFactor>()->SpawnObjects(40, 40,90);
    Instantiate<CPlayerCamera>();
    SingleInstantiate<CEnemyManager>();
    ObjectManager::FindGameObject<CEnemyManager>()->BuildStaticTree();
    Instantiate<CTimer>(70);
    Instantiate<CPlayer>(45);             
    Instantiate<CAnimalFactor>(30,30,1);
     Instantiate<CFlog>();           
    Instantiate<CVisionSystem>();
    Instantiate<CDisplayInfo>();
     CGameInstance::Get()->Init(6000 );

  AudioManager::Play(_T("Play"));
}

CNormalScene::~CNormalScene()
{
    AudioManager::Stop(_T("Play"));
}

void CNormalScene::Update()
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

void CNormalScene::Draw()
{
    SceneBase::Draw();
}
