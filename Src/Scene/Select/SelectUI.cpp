#include "SelectUI.h"

#include "../../Core/Game/GameMain.h"
#include "../../Framework/sceneManager.h"

namespace
{
    // 難易度画像のパス（ボタンインデックス 0=Tutorial, 1=Easy, 2=Normal に対応）
    const std::vector<std::string> DIFFICULTY_IMAGE_PATHS =
    {
        "data/Select/T.png",
        "data/Select/E.png",
        "data/Select/N.png",
    };

    // 難易度画像の描画位置・サイズ
    constexpr float DIFF_IMG_X = 0.0f;
    constexpr float DIFF_IMG_Y = 0.0f;
    constexpr float DIFF_IMG_W = 1366.0f;
    constexpr float DIFF_IMG_H = 768.0f;
}

CSelectUI::CSelectUI()
    : m_pDifficultyImage(nullptr)
    , m_lastFocusIndex(-1)
{
    m_sceneNames = {SceneName::TITLE, SceneName::TUTORIAL, SceneName::EASY, SceneName::NORMAL};
    BackImage();
    InitDifficultyImage();
    InitButtons();
    UpdateDifficultyImage();
}

void CSelectUI::InitButtons()
{
    const std::string fileName = "data/Select/Button.png";
    auto buttonTexture = ImageRegistry::LoadTexture(fileName);
    //戻るボタン
    VECTOR4 buttonSize = VECTOR4(0, 297.0f, 156.0f, 59.0f);
    VECTOR2 buttonPos = VECTOR2(15.0f, 20.0f);
    m_buttons.AddButton(this, buttonTexture, buttonPos, buttonSize);
    
    //Scene選択のボタン
    {
        //1つ目のbuttonの描画位置
        constexpr float FRIST_BUTTON_Y = 113.0f;
        VECTOR2 pos = VECTOR2(30.0f, FRIST_BUTTON_Y);

        //button画像サイズ
        VECTOR4 imageSize = VECTOR4(0, 0, 612, 99);

        //ボタンを生成
        constexpr int BUTTON_COUNT = 3;
        for (auto i = 0; i != BUTTON_COUNT; ++i)
        {
            constexpr float BUTTON_Y_SPACING = 171.0f;
            //描画位置を更新
            pos.y = FRIST_BUTTON_Y + BUTTON_Y_SPACING * i;
            //On画像サイズ(612x99)、背景なし
            m_buttons.AddButton(this, buttonTexture, pos, imageSize);
            //画像の読み込む位置を更新
            imageSize.y += 99;
        }
        m_buttons.SetAlpha(0.0f);
        m_buttons.SetAnim(UIPreset::FadeIn(0.1f), UIPreset::FadeOut(0.1f));
        m_buttons.SetFocus(0);
        m_buttons.SetLayer(1);
    }
}

void CSelectUI::BackImage()
{
    CSpriteImage* white = ImageRegistry::LoadTexture("data/Select/_0006_Base2.png");
    auto backImage = std::make_unique<CUIImage>(white, VECTOR2(0, 0), VECTOR2(WINDOW_WIDTH,WINDOW_HEIGHT));
    m_canvas.AddWidget(std::move(backImage));
}

void CSelectUI::InitDifficultyImage()
{
    // テクスチャをあらかじめ全部ロードしておく
    for (const auto& path : DIFFICULTY_IMAGE_PATHS)
        m_difficultyTextures.push_back(ImageRegistry::LoadTexture(path));

    // 最初はインデックス0の画像で初期化
    auto img = std::make_unique<CUIImage>(
        m_difficultyTextures[0],
        VECTOR2(DIFF_IMG_X, DIFF_IMG_Y),
        VECTOR2(DIFF_IMG_W, DIFF_IMG_H));
    m_pDifficultyImage = m_canvas.AddWidget(std::move(img));
    m_pDifficultyImage->SetLayer(5);
}

void CSelectUI::UpdateDifficultyImage()
{
    const int focusIndex = m_buttons.GetFocusIndex();
    if (focusIndex == m_lastFocusIndex) return;
    m_lastFocusIndex = focusIndex;

    // インデックス0は戻るボタンなので難易度画像は対象外
    const int diffIndex = focusIndex - 1;
    if (m_pDifficultyImage && diffIndex >= 0 && diffIndex < static_cast<int>(m_difficultyTextures.size()))
        m_pDifficultyImage->SetImage(m_difficultyTextures[diffIndex]);
}

void CSelectUI::Update()
{
    auto input = GameDevice()->m_pDI;
    m_buttons.MoveFocus(input->IsPushDownKey());
    m_buttons.MoveFocus(input->IsPushUpKey());

    UpdateDifficultyImage();

    if (input->IsPushEnter())
    {
        SceneManager::ChangeSceneWithTransition(m_sceneNames[m_buttons.GetFocusIndex()]);
    }

    CUIBase::Update();
}
