#include "DebugScene.h"
#include <cassert>

#include "../../System/GameInstance.h"
#include "../../Enemies/System/EnemyManager.h"
#include "../../Player/PCamera.h"
#include "../../Player/Player.h"
#include "../../Enemies/AnimalChicken/Chicken.h"
#include "../../Enemies/AnimalDog/Dog.h"
#include "../../Enemies/System/Flog.h"
#include "../../System/DisplayInfo.h"
#include "../../System/VisionSystem.h"
#include "../../Stage/StageFactor.h"
#include "../../System/Timer.h"

CDebugScene::CDebugScene()
{
   //  Instantiate<CStageFactor>();
   //  Instantiate<CPlayerCamera>();
   //  SingleInstantiate<CEnemyManager>();
   //  Instantiate<CTimer>(300);
   //  //new CAnimalFactor(20, 20);
   // // new CAnimalChicken();
   //  Instantiate<CPlayer>(50);
   //  Instantiate<CVisionSystem>();
   //  Instantiate<CDisplayInfo>();
   //  Instantiate<CFlog>();
   //  CGameInstance::Get()->Init(1000);
    
    m_pDebugUI = new DebugUI();
    m_pUI = new CPlayUI();
}

CDebugScene::~CDebugScene()
{
    SAFE_DELETE(m_pDebugUI);
}

void CDebugScene::ChangeResultScene()
{
    SceneManager::ChangeScene("ResultScene");
}

void CDebugScene::Update()
{
    m_pDebugUI->Update();
    //m_pUI->Update();
}

void CDebugScene::Draw()
{
    
    m_pDebugUI->Draw();
    //m_pUI->Draw();
}
