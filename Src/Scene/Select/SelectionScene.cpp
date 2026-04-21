#include "SelectionScene.h"

#include "SelectUI.h"
#include "../../Common/Constants.h"
#include "../../Framework/AudioManager.h"
#include "../../Framework/GameObject.h"

namespace
{
    constexpr int BUTTON_COUNT = 3;
}

using namespace Constants;

CSelectionScene::CSelectionScene()
{
    m_pUI = new CSelectUI();
    AudioManager::Load(Sound::Key::SELECT_BGM,_T(Sound::Path::SELECT_BGM));
    AudioManager::Play(Sound::Key::SELECT_BGM);
}


CSelectionScene::~CSelectionScene()
{
    AudioManager::Stop(_T(Sound::Key::SELECT_BGM));
}

void CSelectionScene::Update()
{
    m_pUI->Update();
}


void CSelectionScene::Draw()
{
    m_pUI->Draw();
}
