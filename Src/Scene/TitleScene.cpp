#include "TitleScene.h"
#include "../Core/Game/GameMain.h"
#include "../Framework/AudioManager.h"
#include "../Enemies/System/EnemyManager.h"

TitleScene::TitleScene()
{
    // PlayScene逕ｨ繝｢繝・Ν繧定ｵｷ蜍墓凾縺ｫ蜈郁ｪｭ縺ｿ縺励※縺翫￥
    SingleInstantiate<CEnemyManager>();

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

    m_text.push_back("SelectScene");
    m_text.push_back("OI");
    m_selectedIndex = 0;
    AudioManager::Load("TitleBGM",_T("data/Sound/Sunny_day.wav"));
    AudioManager::Play(_T("TitleBGM"), true);
    AudioManager::Load("Decide",_T("data/Sound/decide.wav"));
    AudioManager::Load("Select",_T("data/Sound/select_002.wav"));
}

TitleScene::~TitleScene()
{
    for (auto& info : m_imageInfos)
    {
        SAFE_DELETE(info.pImage);
        info.pImage = nullptr;
    }
    AudioManager::Stop(_T("TitleBGM"));
}

void TitleScene::Update()
{
    int newIndex  = m_selectedIndex;
    int direction = 0;
    if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_W))
    {
        newIndex  = (m_selectedIndex - 1 + 2) % 2;
        direction = -1;
        AudioManager::Play(_T("Select"), false);
    }
    if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_S))
    {
        newIndex  = (m_selectedIndex + 1) % 2;
        direction = 1;
        AudioManager::Play(_T("Select"), false);
    }
    if (newIndex != m_selectedIndex)
    {
        m_wipeAnim.ChangeTo(m_selectedIndex, newIndex, direction);
        m_selectedIndex = newIndex;
    }

    m_wipeAnim.Update();

    if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_RETURN))
    {
        AudioManager::Play(_T("Decide"), false);
        SceneManager::ChangeSceneWithTransition(m_text[m_selectedIndex].c_str());
    }
    //Debug
    if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_1))
    {
        SceneManager::ChangeSceneWithTransition("ResultScene");
    }
    if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_2))
    {
        SceneManager::ChangeSceneWithTransition("SelectScene");
    }
    if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_3))
    {
        SceneManager::ChangeSceneWithTransition("TutorialScene");
    }
    if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_0))
    {
        SceneManager::ChangeSceneWithTransition("Debug");
    }
    if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_4))
    {
        SceneManager::ChangeSceneWithTransition("PlayScene");
    }
}

void TitleScene::Draw()
{
    GameDevice()->m_pFont->Draw(
        20, 20, "TitleScene", 16, RGB(255, 0, 0));
    CSprite spr;

    // 閭梧勹逕ｻ蜒上ｒ謠冗判//
    const auto& bgInfo = m_imageInfos[2];
    spr.Draw(bgInfo.pImage, bgInfo.pos.x, bgInfo.pos.y,
             bgInfo.imageSize.x, bgInfo.imageSize.y, bgInfo.imageSize.z, bgInfo.imageSize.w);

    //繝懊ち繝ｳ縺ｮ繝ｯ繧､繝玲緒逕ｻ//
    for (int i = 0; i < 2; i++)
    {
        const auto& info = m_imageInfos[i];
        m_wipeAnim.Draw(spr, i, info.pImage, info.pos.x, info.pos.y, info.imageSize.z, info.imageSize.w);
    }
}
