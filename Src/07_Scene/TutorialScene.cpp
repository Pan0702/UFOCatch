#include "TutorialScene.h"
#include "../09_Enemy/System//AnimalManager.h"
#include "../08_Player/PCamera.h"
#include "../08_Player/Player.h"
#include "../09_Enemy/Dog//ACube.h"
#include "../09_Enemy/System//AnimalFactor.h"
#include "../10_Stage/Ground.h"
#include "../11_GameSystem/DisplayInfo.h"
#include "../11_GameSystem/VisionSystem.h"
CTutorialScene::CTutorialScene()
{
    new CGround("data/Ground/tesg20x20m.mesh");
    Instantiate<CPlayerCamera>();
    SingleInstantiate<CAnimalManager>();
    //new CTimeLimit(30);
    //Instantiate<CAnimalFactor>();
    Instantiate<CPlayer>();
    Instantiate<CVisionSystem>();
    Instantiate<CDisplayInfo>();
}

void CTutorialScene::ChangeStatate(const char* tutorialName)
{
    if (tutorialName == "Move")
    { 
        new 
    }
}


