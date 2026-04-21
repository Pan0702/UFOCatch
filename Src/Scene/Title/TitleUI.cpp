#include "TitleUI.h"

#include "../../Core/Game/GameMain.h"
#include "../../Framework/sceneManager.h"

CTitleUI::CTitleUI()
{
    m_sceneNames = {SceneName::SELECT,SceneName::OI};
    InitBackground();
    InitButtons();
}

void CTitleUI::InitButtons()
{
    //Load
    const std::string fileName = "data/Title/Buttons.png";
    auto buttonTexture = ImageRegistry::LoadTexture(fileName);
    
    //ButtonのPos
    std::vector<VECTOR2> pos = 
        {VECTOR2(42, 300), VECTOR2(100, 510)};
    //SpriteからのButtonの読み込む大きさ。
    std::vector<VECTOR4> imageSize = 
        {VECTOR4(0, 0, 505, 260), VECTOR4(0, 260, 462, 227)};
    //ボタンを生成
    constexpr int BUTTON_COUNT = 2;
    for (auto i = 0; i != BUTTON_COUNT; ++i)
    {
        m_buttons.AddButton(this, buttonTexture, pos[i], imageSize[i]);
    }
    m_buttons.SetAlpha(0.0f);
    m_buttons.SetAnim(UIPreset::FadeIn(0.2f), UIPreset::FadeOut(0.2f));
    m_buttons.SetFocus(0);
    m_buttons.SetLayer(1);
}

void CTitleUI::InitBackground()
{
    CSpriteImage* white = ImageRegistry::LoadTexture("data/Title/Title.jpg");
    auto backImage = std::make_unique<CUIImage>(white, VECTOR2(0, 0), VECTOR2(WINDOW_WIDTH,WINDOW_HEIGHT));
    m_canvas.AddWidget(std::move(backImage));
}

void CTitleUI::Update()
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
