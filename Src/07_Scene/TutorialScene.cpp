#include "TutorialScene.h"
#include "../09_Enemy/System//AnimalManager.h"
#include "../08_Player/PCamera.h"
#include "../08_Player/Player.h"
#include "../09_Enemy/Dog//ACube.h"
#include "../10_Stage/CubeBox.h"
#include "../10_Stage/Ground.h"
#include "../11_GameSystem/DisplayInfo.h"
#include "../11_GameSystem/VisionSystem.h"
#include "../12_Tutorial/Tutorial.h"

CTutorialScene::CTutorialScene()
{
    new CGround("data/Ground/Prefabs/MapPlane001.mesh",VECTOR3(2.0f,2.0f,2.0f));
    new CGround("data/Ground/Prefabs/Objectes001.mesh", VECTOR3(1.0f, 1.0f, 1.0f));
    new CCubeBox("data/Ground/CubeBoxSky.mesh");
    Instantiate<CPlayerCamera>();
    //new CTimeLimit(30);
    //Instantiate<CAnimalFactor>();
    new CPlayer(10);
    Instantiate<CVisionSystem>();
    Instantiate<CDisplayInfo>();
    Instantiate<CTutorial>();
}

CTutorialScene::~CTutorialScene()
{
}




