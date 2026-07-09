#include "TutorialScene.h"
#include "../../Player/PCamera.h"
#include "../../Player/Player.h"
#include "../../Enemies/AnimalDog/Dog.h"
#include "../../Enemies/System/EnemyManager.h"
#include "../../Stage/StageFactor.h"
#include "../../System/DisplayInfo.h"
#include "../../System/VisionSystem.h"
#include "../../Tutorial/Tutorial.h"
#include "../../Tutorial/TutorialDisplayInfo.h"
#include "../../System/GameInstance.h"
#include "../../Stage/SkyBox.h"
using namespace Constants;

CTutorialScene::CTutorialScene()
{
    Instantiate<CStageFactor>("data/Stage/tutorial.json");
    Instantiate<CSkyBox>("data/Ground/SkyBox.mesh");
    SingleInstantiate<CEnemyManager>()->BuildStaticTree();
    Instantiate<CPlayerCamera>();
    Instantiate<CPlayer>(10);
    Instantiate<CVisionSystem>();
    Instantiate<CTutorial>();
    // CDisplayInfo と CTutorialDisplayInfo は同じ描画順(-100)なので登録順で前後が決まる。
    // 疑惑ゲージ(CDisplayInfo)を最後に登録して最前面に描画する。
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
