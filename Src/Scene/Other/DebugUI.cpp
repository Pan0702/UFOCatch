#include "DebugUI.h"

#include "../../Common/Constants.h"
#include "../../Core/Game/GameMain.h"
#include "../../UI/ImageRegistry.h"
#include "../../UI/UIImage.h"
#include "../../UI/UIPreset.h"
#include "../../UI/UIProgressBar.h"

DebugUI::DebugUI()
{
    ImageRegistry::LoadTexture("Button1", "data/BlackImage.png");
    ImageRegistry::LoadTexture("Button2", "data/BlackImage.png");
    ImageRegistry::LoadTexture("Button3", "data/BlackImage.png");
    AddImage();
    AddButton();
    AddButtons();
    AddBar();
    AddCircle();
}

void DebugUI::AddImage()
{
    //Canvasに画像追加
    CSpriteImage* image = ImageRegistry::LoadTexture("test", "data/debug.png");
    if (image == nullptr)
        assert(false);
    auto widget = std::make_unique<CUIImage>(image, VECTOR2(0, 0),
                                             VECTOR2(WINDOW_WIDTH,WINDOW_HEIGHT));
    widget->SetVisible(true);
    widget->SetAlpha(1.0f);
    m_canvas.AddWidget(std::move(widget));
}

void DebugUI::AddButton()
{
    //Canvasにボタン追加
    CSpriteImage* imageButton = ImageRegistry::LoadTexture("Black", "data/BlackImage.png");
    auto btn = std::make_unique<CUIButton>(VECTOR2(200, 300), VECTOR4(0, 0, 128, 64), nullptr, imageButton);
    btn->SetVisible(true);
    btn->SetAlpha(1.0f);
    m_pButton = m_canvas.AddWidget(std::move(btn));
    m_pButton->SetOnFocusChanged([this](bool isFocus)
    {
        m_pButton->GetAnimationPlayer().Play(isFocus ? "OnFocus" : "OnUnfocus",false);
    });
    //buttonのアニメーションを指定
    m_pButton->GetAnimationPlayer().AddAnimation("OnFocus",UIPreset::PopIn(1.0f));
    m_pButton->GetAnimationPlayer().AddAnimation("OnUnfocus",UIPreset::FadeOut(1.0f));
}

void DebugUI::AddButtons()
{
    m_buttons.SetImageSize(VECTOR4(0, 0, 128, 128));
    std::vector<std::string> buttonNames = {"Button1", "Button2", "Button3"};
    VECTOR2 size = VECTOR2(128, 128);
  
    for (const auto& name : buttonNames)
    {
        m_buttons.AddButton(this,name,size);
        size.y = size.y + 128;
    }
    m_buttons.SetAnim(UIPreset::FadeIn(0.3f),UIPreset::FadeOut(0.3f));
    m_buttons.SetFocus(0);
}

void DebugUI::AddCircle()
{
    CSpriteImage* image = ImageRegistry::GetTexture("Button3");
    auto circle = std::make_unique<CUIRingGauge>(nullptr, image,VECTOR2(500, 50), VECTOR4(0, 0, 100, 100));
    m_pRingGauge = m_canvas.AddWidget(std::move(circle));
    m_pRingGauge->SetAnim(UIPreset::Transition(0.3));
}

void DebugUI::AddBar()
{
    CSpriteImage* image = ImageRegistry::GetTexture("Button3");
    auto bar = std::make_unique<CUIProgressBar>(nullptr, image,VECTOR2(50, 20), VECTOR4(0, 0, 300, 30));
    m_pBar = m_canvas.AddWidget(std::move(bar));
    m_pBar->SetIsEXPBar(true);
    m_pBar->SetAnim(UIPreset::Transition(0.3));
}
void DebugUI::Update()
{
    static bool isPressed = false;

    if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_P))
    {
        isPressed = !isPressed;
        m_pButton->SetFocus(isPressed);
    }
    if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_W))
    {
        m_buttons.MoveFocus(-1);
    }
    if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_S))
    {
        m_buttons.MoveFocus(1);
    }
    if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_R))
    {
        m_pBar->SetRatio(1.5f);
    }
    if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_F))
    {
        float r = Randomf(0.1f, 1.0f);
        m_pBar->SetRatio(r);
    }
    if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_G))
    {
        float r = Randomf(1.1f, 3.0f);
        m_pBar->SetRatio(r);
    }
    if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_H))
    {
        m_pRingGauge->SetRatio(1.0f);
    }
    if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_J))
    {
        m_pRingGauge->SetRatio(0.1f);
    }
    m_canvas.Update();
}

void DebugUI::Draw()
{
    m_canvas.Draw();
}


