#include "TitleScene.h"

#include "TitleUI.h"
#include "../../Core/Game/GameMain.h"
#include "../../Framework/AudioManager.h"
#include "../../Enemies/System/EnemyManager.h"
#include "../../Framework/ResourceManager.h"
#include "../../MapEditor/Import.h"
using namespace Constants;

TitleScene::TitleScene()
{
    std::vector<std::string> modelNames = Import::ModelPath(FileName::MODEL_LIST);
    ResourceManager::LoadFbx(modelNames);

    AudioManager::Load(Sound::Key::TITLE_BGM,_T(Sound::Path::TITLE_BGM));
    AudioManager::Play(_T(Sound::Key::TITLE_BGM), true);
    AudioManager::Load(Sound::Key::DECIDE_SE,_T(Sound::Path::DECIDE_SE));
    AudioManager::Load(Sound::Key::SELECT_SE,_T(Sound::Path::SELECT_SE));
    m_pUI = new CTitleUI();
}

TitleScene::~TitleScene()
{
    AudioManager::Stop(_T(Sound::Key::TITLE_BGM));
}

void TitleScene::Update()
{
    m_pUI->Update();
}

void TitleScene::Draw()
{
    m_pUI->Draw();
}
