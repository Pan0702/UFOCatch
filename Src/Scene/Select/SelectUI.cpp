#include "SelectUI.h"
#include "../../UI/UIPreset.h"

CSelectUI::CSelectUI()
{
    InitButtons();
}

void CSelectUI::InitButtons()
{
    m_buttons.SetImageSize(VECTOR4(0, 0, 128, 128));
    std::vector<std::string> buttonNames =
        {SceneName::TUTORIAL, SceneName::EASY, SceneName::NORMAL};
    constexpr float FRIST_BUTTON_Y = 113.0f;
    VECTOR2 size = VECTOR2(30.0f, FRIST_BUTTON_Y);

    for (auto it = buttonNames.begin(); it != buttonNames.end(); ++it)
    {
        m_buttons.AddButton(this, *it, size);
        int i = std::distance(buttonNames.begin(), it);
        constexpr float BUTTON_Y_SPACING = 58.0f;
        size.y = FRIST_BUTTON_Y * i + BUTTON_Y_SPACING * (i - 1);
    }
    m_buttons.SetAnim(UIPreset::FadeIn(0.3f), UIPreset::FadeOut(0.3f));
    m_buttons.SetFocus(0);
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
