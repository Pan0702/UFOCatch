#include "TutorialScene.h"
#include "../09_Enemy/System//AnimalManager.h"
#include "../08_Player/PCamera.h"
#include "../08_Player/Player.h"
#include "../09_Enemy/Dog//ACube.h"
#include "../09_Enemy/System//AnimalFactor.h"
#include "../10_Stage/Ground.h"
#include "../11_GameSystem/DisplayInfo.h"
#include "../11_GameSystem/VisionSystem.h"
#include "../12_Tutorial/Tutorial.h"

CTutorialScene::CTutorialScene()
{
    new CGround("data/Ground/tesg20x20m.mesh");
    Instantiate<CPlayerCamera>();
    //new CTimeLimit(30);
    //Instantiate<CAnimalFactor>();
    Instantiate<CPlayer>();
    Instantiate<CVisionSystem>();
    Instantiate<CDisplayInfo>();
    Instantiate<CTutorial>();
}

CTutorialScene::~CTutorialScene()
{
}




