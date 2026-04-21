#include "SelectUI.h"

#include "../../Core/Game/GameMain.h"
#include "../../Framework/sceneManager.h"
#include "../../UI/UIPreset.h"

CSelectUI::CSelectUI()
{
    m_selectIndex = 0;
    m_sceneNames = {SceneName::TITLE, SceneName::TUTORIAL, SceneName::EASY, SceneName::NORMAL};
    BackImage();
    InitButtons();
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

        for (auto it = m_sceneNames.begin(); it != m_sceneNames.end() - 1; ++it)
        {
            constexpr float BUTTON_Y_SPACING = 171.0f;
            int i = std::distance(m_sceneNames.begin(), it);
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

void CSelectUI::Update()
{
    auto input = GameDevice()->m_pDI;
    m_buttons.MoveFocus(input->IsPushDownKey());
    m_buttons.MoveFocus(input->IsPushUpKey());

    if (input->IsPushEnter())
    {
        SceneManager::ChangeSceneWithTransition(m_sceneNames[m_buttons.GetFocusIndex()]);
    }

    CUIBase::Update();
}
