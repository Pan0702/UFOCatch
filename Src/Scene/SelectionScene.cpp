#include "SelectionScene.h"
#include "../Framework/AudioManager.h"

namespace
{
    constexpr int BUTTON_COUNT = 3;
}

CSelectionScene::CSelectionScene()
{
    m_pImageBackGround = new CSpriteImage("data/Select/_0006_Base.png"); //("data/Select/Select.png");
    m_selectedIndex = 0;
    InitButtons();
    InitScene();
    AudioManager::Load("Select",_T("data/Sound/himitu.wav"));
    AudioManager::Play("Select");
}
//Debug用
// void CSelectionScene::InitButtons()
// {
//     //PlaySceneボタン //
//     {
//         ButtomInfo info{};
//         info.image = new CSpriteImage("data/Select/DOButton.png");
//         info.sceneName = "PlayScene";
//         m_buttons.push_back(info);
//     }
//
//     //Tutorialボタン //
//     {
//         ButtomInfo info{};
//         info.image = new CSpriteImage("data/Select/TutoButton.png");
//         info.sceneName = "TutorialScene";
//         m_buttons.push_back(info);
//     }
// }

void CSelectionScene::InitButtons()
{
    {
        ButtomInfo info{};
        info.image = new CSpriteImage("data/Select/_0000_Title_On.png");
        info.imageSize = VECTOR2(612, 99);
        m_buttons.push_back(info);
    }
    {
        ButtomInfo info{};
        info.image = new CSpriteImage("data/Select/_0005_Play_On.png");
        info.imageSize = VECTOR2(249, 76);
        m_buttons.push_back(info);
    }
    {
        ButtomInfo info{};
        info.image = new CSpriteImage("data/Select/_0003_Modoru_On.png");
        info.imageSize = VECTOR2(156, 58);
        m_buttons.push_back(info);
    }
    {
        ButtomInfo info{};
        info.image = new CSpriteImage("data/Select/_0001_TitLe_off.png");
        info.imageSize = VECTOR2(603, 91);
        m_buttons.push_back(info);
    }
    {
        ButtomInfo info{};
        info.image = new CSpriteImage("data/Select/_0002_Modoru_Off.png");
        info.imageSize = VECTOR2(156, 58);
        m_buttons.push_back(info);
    }
    {
        ButtomInfo info{};
        info.image = new CSpriteImage("data/Select/_0004_Play_Off.png");
        info.imageSize = VECTOR2(249, 76);
        m_buttons.push_back(info);
    }
    {
        ButtomInfo info{};
        info.image = new CSpriteImage("data/Select/String.png");
        info.imageSize = VECTOR2(454, 768);
        m_buttons.push_back(info);
    }
}

void CSelectionScene::InitScene()
{
    m_sceneName = { "Tutorial", "Tutorial", "Normal", "Non" };
}


CSelectionScene::~CSelectionScene()
{
    for (auto& button : m_buttons)
    {
        SAFE_DELETE(button.image);
    }
    AudioManager::Stop(_T("Select"));
}

void CSelectionScene::Update()
{
    if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_RETURN))
    {
        SceneManager::ChangeSceneWithTransition(m_sceneName[m_selectedIndex].c_str());
        AudioManager::Play(_T("Decide"), false);
    }

    PlayButton();
    LevelButton();

    m_wipeAnim.Update(0.05f);
}
void CSelectionScene::PlayButton()
{
    if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_D))
    {
        m_play = (m_play + 1) % 2;
        AudioManager::Play(_T("Select"), false);
    }
    if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_A))
    {
        m_play = (m_play - 1 + 2) % 2;
        AudioManager::Play(_T("Select"), false);
    }
}

void CSelectionScene::LevelButton()
{
    int newIndex = m_selectedIndex;

    int direction = 0;
    if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_S))
    {
        newIndex  = (m_selectedIndex + 1) % BUTTON_COUNT;
        direction = 1;
        AudioManager::Play(_T("Select"), false);
    }
    if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_W))
    {
        newIndex  = (m_selectedIndex - 1 + BUTTON_COUNT) % BUTTON_COUNT;
        direction = -1;
        AudioManager::Play(_T("Select"), false);
    }

    if (newIndex != m_selectedIndex)
    {
        m_wipeAnim.ChangeTo(m_selectedIndex, newIndex, direction);
        m_selectedIndex = newIndex;
    }
}


void CSelectionScene::Draw()
{
    static constexpr float imageWidthMaxSize = 1366;
    static constexpr float imageHeightMaxSize = 768;
    CSprite spr;
    //背景のひょうじ //
    spr.Draw(m_pImageBackGround, 0, 0, 0, 0, imageWidthMaxSize, imageHeightMaxSize);
    ButtonsDraw();

    if (m_play == 1)
    {
        VECTOR2 playSize = m_buttons[1].imageSize;
        spr.Draw(m_buttons[1].image, 1083, 489, 0, 0, playSize.x, playSize.y);
    }
    //文字の表示
    int arryEnd = m_buttons.size() - 1;
    VECTOR2 strSize = m_buttons[arryEnd].imageSize;
    spr.Draw(m_buttons[arryEnd].image, 0, 0, 0, 0, strSize.x, strSize.y);
}

void CSelectionScene::ButtonsDraw()
{
    CSprite spr;
    VECTOR2 offSize = m_buttons[3].imageSize;
    VECTOR2 onSize  = m_buttons[0].imageSize;

    for (int i = 0; i < BUTTON_COUNT; i++)
    {
        float yBase = 113.0f + (float)i * (80.0f + offSize.y);

        // Off画像（常に表示）
        spr.Draw(m_buttons[3].image, 30.0f, yBase, 0, 0, offSize.x, offSize.y);

        // On画像をワイプ描画
        m_wipeAnim.Draw(spr, i, m_buttons[0].image, 30.0f - 9.0f, yBase, onSize.x, onSize.y);
    }

    // Play Off画像
    VECTOR2 playSize = m_buttons[5].imageSize;
    spr.Draw(m_buttons[5].image, 1083, 489, 0, 0, playSize.x, playSize.y);
}
