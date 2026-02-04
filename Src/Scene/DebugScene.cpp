#include "DebugScene.h"
#include <cassert>

#include "../System/GameInstance.h"
#include "../Enemies/System//EnemyManager.h"
#include "../Player/PCamera.h"
#include "../Player/Player.h"
#include "../Enemies/AnimalChicken/Chicken.h"
#include "../Enemies/AnimalDog//Dog.h"
#include "../Enemies/System//AnimalFactor.h"
#include "../Enemies/System/Flog.h"
#include "../System/DisplayInfo.h"
#include "../System/VisionSystem.h"
#include "../Stage/StageFactor.h"
#include "../System/Timer.h"

CDebugScene::CDebugScene()
{
    Instantiate<CStageFactor>();
    Instantiate<CPlayerCamera>();
    SingleInstantiate<CEnemyManager>();
    new CTimer(300);
    //new CAnimalFactor(20, 20);
   // new CAnimalChicken();
    new CPlayer(50);
    Instantiate<CVisionSystem>();
    Instantiate<CDisplayInfo>();
    Instantiate<CFlog>();
    ObjectManager::FindGameObject<CGameInstance>()->Init(1000);
}

CDebugScene::~CDebugScene()
{
}

void CDebugScene::ChangeResultScene()
{
    SceneManager::ChangeScene("ResultScene");
}

void CDebugScene::Update()
{
}

void CDebugScene::Draw()
{
}
