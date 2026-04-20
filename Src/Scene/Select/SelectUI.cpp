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
    //1つ目のbuttonの描画位置
    constexpr float FRIST_BUTTON_Y = 113.0f;
    VECTOR2 size = VECTOR2(30.0f, FRIST_BUTTON_Y);
    
    //button画像サイズ
    VECTOR4 imageSize = VECTOR4(0, 0, 612, 99);
    
    //画像の読み込み
    const std::string fileName = "data/Select/string.png";
    auto buttonTexture = ImageRegistry::LoadTexture(fileName);
    //ボタンを生成
    for (auto it = m_sceneNames.begin(); it != m_sceneNames.end(); ++it)
    {
        const VECTOR4 NonImage = VECTOR4(0, 0, 0, 0);
        m_buttons.AddButton(this, buttonTexture, size, imageSize, imageSize);
        //itrをintに変換
        int i = std::distance(m_sceneNames.begin(), it);
        //描画位置を更新
        constexpr float BUTTON_Y_SPACING = 58.0f;
        size.y = FRIST_BUTTON_Y * i + BUTTON_Y_SPACING * (i - 1);
        //画像の読み込む位置を更新
        imageSize.y += 99;
    }
    m_buttons.SetAnim(UIPreset::FadeIn(0.3f), UIPreset::FadeOut(0.3f));
    m_buttons.SetFocus(0);
    m_buttons.SetLayer(1);
}

void CSelectUI::BackImage()
{
    CSpriteImage* white = ImageRegistry::LoadTexture("data/Select/_0006_Base.png");
    auto backImage = std::make_unique<CUIImage>(white,VECTOR2(0,0),VECTOR2(WINDOW_WIDTH,WINDOW_HEIGHT));
    m_canvas.AddWidget(std::move(backImage));
}

void CSelectUI::Update()
{
    auto input = GameDevice()->m_pDI;
    if (input->IsPushUpKey())
    {
        m_selectIndex = (m_selectIndex + 1) > -1 ? m_selectIndex - 1 : m_selectIndex; 
        m_buttons.MoveFocus(m_selectIndex); 
    }
    if (input->IsPushDownKey())
    {
        m_selectIndex = (m_selectIndex - 1) < 3 ? m_selectIndex + 1 : m_selectIndex;
        m_buttons.MoveFocus(m_selectIndex); 
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
