#include "TitleScene.h"
#include "../03_GameMain/GameMain.h"

TitleScene::TitleScene()
{
    m_imageInfos.push_back(ImageInfo(
        VECTOR2(42, 301),
        VECTOR4(0, 0, 505, 260),
        new CSpriteImage("data/Title/start.png")));

    m_imageInfos.push_back(ImageInfo(
        VECTOR2(100, 511),
        VECTOR4(0, 0, 465, 230),
        new CSpriteImage("data/Title/help.png")));

    m_imageInfos.push_back(ImageInfo(
        VECTOR2(0, 0),
        VECTOR4(0, 0, 1366, 768),
        new CSpriteImage("data/Title/Title.jpg")));

    m_text.push_back("PlayScene");
    m_text.push_back("SelectScene");
    m_selectedIndex = 0;
}

TitleScene::~TitleScene()
{
    for (auto& info : m_imageInfos)
    {
        SAFE_DELETE(info.pImage);
        info.pImage = nullptr;
    }
}

void TitleScene::Update()
{
    m_selectedIndex = GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_W) ? (m_selectedIndex - 1 + 2) % 2 : m_selectedIndex;
    m_selectedIndex = GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_S) ? (m_selectedIndex + 1) % 2 : m_selectedIndex;
    if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_RETURN))
    {
        SceneManager::ChangeScene(m_text[m_selectedIndex].c_str());
    }
    if (GameDevice()->m_pDI->
                      CheckKey(KD_TRG, DIK_RETURN))
    {
        SceneManager::ChangeScene("SelectScene");
    }
    //     SceneManager::ChangeScene("PlayScene");
    // }
    // if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_1))
    // {
    //     SceneManager::ChangeScene("ResultScene");
    // }
    // if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_2))
    // {
    //     SceneManager::ChangeScene("SelectScene");
    // }
    if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_3))
    {
        SceneManager::ChangeScene("TutorialScene");
    }
}

void TitleScene::Draw()
{
    GameDevice()->m_pFont->Draw(
        20, 20, "TitleScene", 16, RGB(255, 0, 0));
    CSprite spr;

    // 背景画像を描画//
    const auto& bgInfo = m_imageInfos[2];
    spr.Draw(bgInfo.pImage, bgInfo.pos.x, bgInfo.pos.y,
             bgInfo.imageSize.x, bgInfo.imageSize.y, bgInfo.imageSize.z, bgInfo.imageSize.w);

    //ボタンの描画//
    spr.Draw(m_imageInfos[m_selectedIndex].pImage,
             m_imageInfos[m_selectedIndex].pos.x, m_imageInfos[m_selectedIndex].pos.y, //描画したいいちの最小Pos//
             m_imageInfos[m_selectedIndex].imageSize.x, m_imageInfos[m_selectedIndex].imageSize.y, //読み込む画像の最小Pos//
             m_imageInfos[m_selectedIndex].imageSize.z, m_imageInfos[m_selectedIndex].imageSize.w); //読み込む画像の最大Pos//
}
