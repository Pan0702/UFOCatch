#include "DebugScene.h"
#include <cassert>

#include "../../Debug/DebugCamera.h"
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
    Instantiate<CStageFactor>();
    SingleInstantiate<CEnemyManager>();
    Instantiate<CDebugCamera>();
    auto* flog = Instantiate<CFlog>();
    auto* dog = Instantiate<CAShepherdDog>();
    CGameInstance::Get()->Init(1000);
    for (CSheep* s : flog->GetAllSheeps())
    {
        dog->AddSheep(s);
    }
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
    // m_pDebugUI->Update();
    // m_pUI->Update();
}

void CDebugScene::Draw()
{
    //m_pDebugUI->Draw();
    //m_pUI->Draw();
}
