#include "SelectionScene.h"
#include "../Framework/AudioManager.h"
CSelectionScene::CSelectionScene()
{   
    m_pImageBackGround = new CSpriteImage("data/Select/Select.png");
    m_selectedIndex = 0;
    InitButtons();
    AudioManager::Load("Select",_T("data/Sound/himitu.wav"));
    AudioManager::Play("Select");
}

void CSelectionScene::InitButtons()
{
    //PlaySceneボタン //
    {
        ButtomInfo info{};
        info.image = new CSpriteImage("data/Select/DOButton.png");
        info.sceneName = "PlayScene";
        m_buttons.push_back(info);
    }
    
    //Tutorialボタン //
    {
        ButtomInfo info{};
        info.image = new CSpriteImage("data/Select/TutoButton.png");
        info.sceneName = "TutorialScene";
        m_buttons.push_back(info);
    }
}

CSelectionScene::~CSelectionScene()
{
    for (auto& button : m_buttons)
    {
        SAFE_DELETE(button.image);
    }
}

void CSelectionScene::Update()
{
    if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_RETURN))
    {
        SceneManager::ChangeSceneWithTransition(m_buttons[m_selectedIndex].sceneName.c_str());
        AudioManager::Play(_T("Decide"), false);
    }

    if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_D))
    {
        m_selectedIndex = (m_selectedIndex + 1) % 2;
        AudioManager::Play(_T("Select"), false);
    }
    if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_A))
    {
        m_selectedIndex = (m_selectedIndex - 1 + 2) % 2;
        AudioManager::Play(_T("Select"), false);
    }
    
}

void CSelectionScene::Draw()
{
    static constexpr float imageWidthMaxSize = 1366;
    static constexpr float imageHeightMaxSize = 768;
    CSprite spr;
    //背景のひょうじ //
    spr.Draw(m_pImageBackGround, 0, 0, 0, 0, imageWidthMaxSize, imageHeightMaxSize);
    //選択中のボタンの表示//
    spr.Draw(m_buttons[m_selectedIndex].image, 0, 0, 0, 0, imageWidthMaxSize, imageHeightMaxSize);
}

