#include "SelectionScene.h"

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
}

void CSelectionScene::Update()
{
    if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_RETURN))
    if (m_selectedIndex == 0)
    {
        SceneManager::ChangeScene("PlayScene");
        m_pBGM->Stop();
    }else if (m_selectedIndex == 1)
    {
        SceneManager::ChangeScene("TutorialScene");
        m_pBGM->Stop();
    }

    if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_D))
    {
        m_selectedIndex = (m_selectedIndex + 1) % 2;
    }
    if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_A))
    {
        m_selectedIndex = (m_selectedIndex - 1 + 2) % 2;
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
