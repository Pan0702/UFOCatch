#include "NormalScene.h"

#include "../../System/GameInstance.h"
#include "../../Enemies/System/EnemyManager.h"
#include "../../Player/PCamera.h"
#include "../../Player/Player.h"
#include "../../Enemies/AnimalDog/Dog.h"
#include "../../Enemies/System/AnimalFactor.h"
#include "../../Enemies/System/Flog.h"
#include "../../System/DisplayInfo.h"
#include "../../System/VisionSystem.h"
#include "../../Stage/StageFactor.h"
#include "../../System/Timer.h"
using namespace Constants;
CNormalScene::CNormalScene()
{
    Instantiate<CStageFactor>()->SpawnObjects(40, 40,90);
    Instantiate<CPlayerCamera>();
    SingleInstantiate<CEnemyManager>()->BuildStaticTree();
    Instantiate<CTimer>(70);
    Instantiate<CPlayer>(45);             
    Instantiate<CAnimalFactor>(30,30,1);
     Instantiate<CFlog>();           
    Instantiate<CVisionSystem>();
    Instantiate<CDisplayInfo>();
    CGameInstance::Get()->Init(6000 );
    AudioManager::Load(Sound::Path::PLAY_BGM,_T(Sound::Key::PLAY_BGM));
    AudioManager::Play(_T(Sound::Key::PLAY_BGM));
}

CNormalScene::~CNormalScene()
{
    AudioManager::Stop(_T(Sound::Key::PLAY_BGM));
}

void CNormalScene::Update()
{

}

void CNormalScene::Draw()
{
    SceneBase::Draw();
}
