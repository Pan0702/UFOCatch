#include "SelectionScene.h"
#include "../Framework/AudioManager.h"
CSelectionScene::CSelectionScene()
{
    m_pSprite = new CSprite();
    m_pImageBackGround = new CSpriteImage("data/Select/Select.png");
    m_pImageDoButton = new CSpriteImage("data/Select/DOButton.png");
    m_pImageTutorialButton = new CSpriteImage("data/Select/TutoButton.png");
    m_selectedIndex = 0;
    m_pBGM = new CXAudioSource(_T("data/Sound/himitu.wav"));
    m_pBGM->Play(1);
}

CSelectionScene::~CSelectionScene()
{
    SAFE_DELETE(m_pBGM);
}

void CSelectionScene::Update()
{
    if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_RETURN))
    if (m_selectedIndex == 0)
    {
        SceneManager::ChangeSceneWithTransition("PlayScene");
        AudioManager::Play(_T("Decide"), false);
    }else if (m_selectedIndex == 1)
    {
        SceneManager::ChangeSceneWithTransition("TutorialScene");
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
    m_pSprite->Draw(m_pImageBackGround, 0, 0, 0, 0, 1366, 768);
    switch (m_selectedIndex)
    {
        case 0:
            m_pSprite->Draw(m_pImageDoButton, 0, 0, 0, 0, 1366, 768);
            break;
        case 1:
            m_pSprite->Draw(m_pImageTutorialButton, 0, 0, 0, 0, 1366, 768);
            break;
    }
 
}
