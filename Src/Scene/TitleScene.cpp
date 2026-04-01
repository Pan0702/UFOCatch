#include "TitleScene.h"
#include "../Core/Game/GameMain.h"
#include "../Framework/AudioManager.h"
#include "../Enemies/System/EnemyManager.h"
#include "../Framework/ResourceManager.h"
#include "../MapEditor/Import.h"
using namespace Constants;
TitleScene::TitleScene()
{
    //PlayScene用モデルを起動時に先読みしておく
    SingleInstantiate<CModelRegistry>();

    std::vector<std::string> modelNames = Import::ModelPath(FileName::MODEL_LIST);
    ResourceManager::LoadFbx(modelNames);

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
    AudioManager::Load(Sound::Key::TITLE_BGM,_T(Sound::Path::TITLE_BGM));
    AudioManager::Play(_T(Sound::Key::TITLE_BGM), true);
    AudioManager::Load(Sound::Key::DECIDE_SE,_T(Sound::Path::DECIDE_SE));
    AudioManager::Load(Sound::Key::SELECT_SE,_T(Sound::Path::SELECT_SE));
}

TitleScene::~TitleScene()
{
    for (auto& info : m_imageInfos)
    {
        SAFE_DELETE(info.pImage);
        info.pImage = nullptr;
    }
    AudioManager::Stop(_T(Sound::Key::DECIDE_SE));
}

void TitleScene::Update()
{
    int newIndex  = m_selectedIndex;
    int direction = 0;
    if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_W))
    {
        newIndex  = (m_selectedIndex - 1 + 2) % 2;
        direction = -1;
        AudioManager::Play(_T(Sound::Key::SELECT_SE), false);
    }
    if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_S))
    {
        newIndex  = (m_selectedIndex + 1) % 2;
        direction = 1;
        AudioManager::Play(_T(Sound::Key::SELECT_SE), false);
    }
    if (newIndex != m_selectedIndex)
    {
        m_wipeAnim.ChangeTo(m_selectedIndex, newIndex, direction);
        m_selectedIndex = newIndex;
    }

    m_wipeAnim.Update();

    if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_RETURN))
    {
        AudioManager::Play(_T(Sound::Key::DECIDE_SE), false);
        SceneManager::ChangeSceneWithTransition(m_text[m_selectedIndex].c_str());
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

