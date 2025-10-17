#include "LevelSelectionScene.h"

CLevelSelectionScene::CLevelSelectionScene()
{
    InitImage();
}

void CLevelSelectionScene::InitImage()
{
    m_pLevelImages.reserve(3);
    m_pLevelImages.emplace_back(new CSpriteImage("data/Button1.png"));
    m_pLevelImages.emplace_back(new CSpriteImage("data/Button2.png"));
    m_pLevelImages.emplace_back(new CSpriteImage("data/Button3.png"));
}

CLevelSelectionScene::~CLevelSelectionScene()
{
    for (auto& p : m_pLevelImages)
    {
        delete p;
        p = nullptr;
    }
}

void CLevelSelectionScene::Update()
{
    CDirectInput* pDI = GameDevice()->m_pDI;
    if (pDI->CheckKey(KD_TRG, DIK_W) || pDI->CheckKey(KD_TRG,DIK_UP))
    {
            MoveToTop();
    }
    if (pDI->CheckKey(KD_TRG, DIK_S) || pDI->CheckKey(KD_TRG, DIK_DOWN))
    {
            MoveToEnd();
    }
    if (pDI->CheckKey(KD_TRG, DIK_RETURN))
    {
        SceneManager::ChangeScene("SelectScene");
    }
}

void CLevelSelectionScene::Draw()
{
    CSprite spr;
    for (int i = 0; i < m_pLevelImages.size(); i++)
    {
        if (m_pLevelImages[i] != nullptr) // nullptrチェック
        {
            spr.Draw(m_pLevelImages[i], 100, i * 250, 0, 0, 670, 190);
        }
    }
    GameDevice()->m_pFont->Draw(
    20, 20, "LevelSelect", 16, RGB(255, 0, 0));
    
}

void CLevelSelectionScene::MoveToTop()
{
    CSpriteImage* tmpImage = m_pLevelImages.back();
    m_pLevelImages.pop_back();
    m_pLevelImages.insert(m_pLevelImages.begin(), tmpImage);
}

void CLevelSelectionScene::MoveToEnd()
{
    CSpriteImage* tmpImage = m_pLevelImages.front();
    m_pLevelImages.erase(m_pLevelImages.begin());
    m_pLevelImages.push_back(tmpImage);
}
