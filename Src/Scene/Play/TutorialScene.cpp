#include "TutorialScene.h"
#include "../../Player/PCamera.h"
#include "../../Player/Player.h"
#include "../../Enemies/AnimalDog/Dog.h"
#include "../../Stage/Ground.h"
#include "../../Stage/StageFactor.h"
#include "../../System/DisplayInfo.h"
#include "../../System/VisionSystem.h"
#include "../../Tutorial/Tutorial.h"
#include "../../Tutorial/TutorialDisplayInfo.h"
#include "../../System/GameInstance.h"
using namespace Constants;
CTutorialScene::CTutorialScene()
{
    Instantiate<CStageFactor>();
    Instantiate<CGround>("data/Ground/Prefabs/Objectes001.mesh", VECTOR3(1.0f, 1.0f, 1.0f));
    Instantiate<CPlayerCamera>();
    Instantiate<CPlayer>(10);
    Instantiate<CVisionSystem>();
    Instantiate<CDisplayInfo>();
    Instantiate<CTutorial>();
    Instantiate<CTutorialDisplayInfo>();
    Instantiate<CDisplayInfo>();
    AudioManager::Load(_T(Sound::Key::PLAY_BGM), _T(Sound::Path::PLAY_BGM));
    AudioManager::Play(_T(Sound::Key::PLAY_BGM), true);
    CGameInstance::Get()->Init(1000);
}

CTutorialScene::~CTutorialScene()
{
    AudioManager::Stop(_T(Sound::Key::PLAY_BGM));
}

