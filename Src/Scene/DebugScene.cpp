#include "DebugScene.h"
#include <cassert>

#include "../System/GameInstance.h"
#include "../Enemies/System//EnemyRegistr.h"
#include "../Player/PCamera.h"
#include "../Player/Player.h"
#include "../Enemies/AnimalChicken/Chicken.h"
#include "../Enemies/AnimalDog//Dog.h"
#include "../Enemies/System//AnimalFactor.h"
#include "../System/DisplayInfo.h"
#include "../System/VisionSystem.h"
#include "../Stage/StageFactor.h"
#include "../System/Timer.h"

CDebugScene::CDebugScene()
{
    Instantiate<CStageFactor>();
    Instantiate<CPlayerCamera>();
    new CAnimalManager(0);
    new CTimer(30);
    Instantiate<CAnimalFactor>();
    new CAnimalChicken();
    new CPlayer(20);
    Instantiate<CVisionSystem>();
    Instantiate<CDisplayInfo>();
    
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
