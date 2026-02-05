#include "TutorialScene.h"
#include "../Enemies/System/EnemyManager.h"
#include "../Player/PCamera.h"
#include "../Player/Player.h"
#include "../Enemies/AnimalDog/Dog.h"
#include "../Stage/Ground.h"
#include "../Stage/StageFactor.h"
#include "../System/DisplayInfo.h"
#include "../System/Timer.h"
#include "../System/VisionSystem.h"
#include "../Tutorial/Tutorial.h"
#include "../Tutorial/TutorialDisplayInfo.h"
#include "../System/GameInstance.h"

CTutorialScene::CTutorialScene()
{
    Instantiate<CStageFactor>();
    new CGround("data/Ground/Prefabs/Objectes001.mesh", VECTOR3(1.0f, 1.0f, 1.0f));
    Instantiate<CPlayerCamera>();
    new CPlayer(10);
    Instantiate<CVisionSystem>();
    Instantiate<CDisplayInfo>();
    Instantiate<CTutorial>();
    Instantiate<CTutorialDisplayInfo>();
    Instantiate<CDisplayInfo>();
    ObjectManager::FindGameObject<CGameInstance>()->Init(1000);
}

CTutorialScene::~CTutorialScene()
{
}




