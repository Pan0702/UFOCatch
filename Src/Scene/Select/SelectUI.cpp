#include "SelectUI.h"

#include "../../Core/Game/GameMain.h"
#include "../../Framework/sceneManager.h"
#include "../../UI/UIPreset.h"

CSelectUI::CSelectUI()
{
    
    m_selectIndex = 0;
    m_sceneNames = { SceneName::TUTORIAL, SceneName::EASY, SceneName::NORMAL };
    BackImage();
    InitButtons();

}

void CSelectUI::InitButtons()
{
    const std::string fileName = "data/Select/String.png";
    auto buttonTexture = ImageRegistry::LoadTexture(fileName);
    for (int i = 0; i < static_cast<int>(m_sceneNames.size()); ++i)
    {
        constexpr float FIRST_BUTTON_Y  = 113.0f;
        constexpr float OFF_IMAGE_HEIGHT = 91.0f;
        constexpr float BUTTON_SPACING   = 80.0f;
        constexpr float BUTTON_X         = 30.0f;
        constexpr float SRC_HEIGHT       = 99.0f;
        VECTOR2 pos     = VECTOR2(BUTTON_X, FIRST_BUTTON_Y + i * (OFF_IMAGE_HEIGHT + BUTTON_SPACING));
        VECTOR4 srcRect = VECTOR4(0, i * SRC_HEIGHT, 612, SRC_HEIGHT);
        m_buttons.AddButton(this, buttonTexture, pos, srcRect, srcRect);
    }
    m_buttons.SetAlpha(0.0f);
    m_buttons.SetAnim(UIPreset::FadeIn(0.3f), UIPreset::FadeOut(0.3f));
    m_buttons.SetFocus(0);
    m_buttons.SetLayer(1);
}

void CSelectUI::BackImage()
{
    CSpriteImage* white = ImageRegistry::LoadTexture("data/Select/_0006_Base.png");
    auto backImage = std::make_unique<CUIImage>(white,VECTOR2(0,0),VECTOR2(WINDOW_WIDTH,WINDOW_HEIGHT));
    backImage->SetLayer(0);
    m_canvas.AddWidget(std::move(backImage));
}

void CSelectUI::Update()
{
    auto input = GameDevice()->m_pDI;
    if (input->IsPushUpKey())
    {
        m_buttons.MoveFocus(-1);
        m_selectIndex = (std::max)(m_selectIndex - 1, 0);
    }
    if (input->IsPushDownKey())
    {
        m_buttons.MoveFocus(1);
        m_selectIndex = (std::min)(m_selectIndex + 1, static_cast<int>(m_sceneNames.size()) - 1);
    }
    if (input->IsPushEnter())
    {
        SceneManager::ChangeSceneWithTransition(m_sceneNames[m_selectIndex]);
    }
    ImGui::Begin("Select");
    ImGui::Text("%d", m_selectIndex);
    ImGui::End();
        CUIBase::Update();
}

// ● レベル選択ボタンの位置・大きさまとめ           
//                                             
//   サイズ                                        
//          
//   ┌──────────────────────┬────────────────────┐
//   │         種類         │ サイズ（幅×高さ）  │  
//   ├──────────────────────┼────────────────────┤
//   │ On画像（Title_On）   │ 612 × 99           │  
//   ├──────────────────────┼────────────────────┤
//   │ Off画像（Title_Off） │ 603 × 91 ←         │
//   │                      │ 描画に使われる高さ │
//   └──────────────────────┴────────────────────┘
//
//   ---
//   描画位置（ButtonsDraw内のループ）
//
//   float yBase = 113.0f + (float)i * (80.0f +
//   offSize.y);
// //                                          ↑
// 91
//
// ┌───────────────┬─────────────┬────────────┐
// │   ボタン i    │    X座標    │   Y座標    │
// ├───────────────┼─────────────┼────────────┤
// │ 0（Tutorial） │ Off: 30.0 / │ 113.0      │
// │               │  On: 21.0   │            │
// ├───────────────┼─────────────┼────────────┤
// │ 1（Easy）     │ Off: 30.0 / │ 284.0 (113 │
// │               │  On: 21.0   │  + 171)    │
// ├───────────────┼─────────────┼────────────┤
// │ 2（Normal）   │ Off: 30.0 / │ 455.0 (113 │
// │               │  On: 21.0   │  + 342)    │
// └───────────────┴─────────────┴────────────┘
//
// ボタン間の間隔内訳：
// 171 = offSize.y(91) + 間隔(80)
//
// ---
// 図解
//
// Y=113  [Tutorial  Off/On]  X=30/21
//          ↕ 80px 余白
//          ↕ 91px 画像高さ
// Y=284  [Easy      Off/On]  X=30/21
//          ↕ 80px 余白
//          ↕ 91px 画像高さ
// Y=455  [Normal    Off/On]  X=30/21
