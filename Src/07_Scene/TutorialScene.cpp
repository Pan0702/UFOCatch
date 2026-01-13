#include "TutorialScene.h"
#include "../09_Enemy/System//AnimalManager.h"
#include "../08_Player/PCamera.h"
#include "../08_Player/Player.h"
#include "../09_Enemy/Dog//ACube.h"
#include "../10_Stage/Ground.h"
#include "../10_Stage/StageFactor.h"
#include "../11_GameSystem/DisplayInfo.h"
#include "../11_GameSystem/Timer.h"
#include "../11_GameSystem/VisionSystem.h"
#include "../12_Tutorial/Tutorial.h"
#include "../12_Tutorial/TutorialDisplayInfo.h"
#include "../GameInstance.h"

CTutorialScene::CTutorialScene()
{
    Instantiate<CStageFactor>();
    new CGround("data/Ground/Prefabs/Objectes001.mesh", VECTOR3(1.0f, 1.0f, 1.0f));
    Instantiate<CPlayerCamera>();
    new CTimer(20);
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




