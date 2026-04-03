#include "DebugUI.h"

#include "../../Core/Game/GameMain.h"
#include "../../UI/ImageRegistry.h"
#include "../../UI/UIImage.h"
#include "../../UI/UIPreset.h"
DebugUI::DebugUI()
{
    CSpriteImage* image = ImageRegistry::LoadTexture("test", "data/debug.png");
    if (image == nullptr)
        assert(false);
    auto widget = std::make_unique<CUIImage>(image, VECTOR2(0, 0),
                                             VECTOR2(WINDOW_WIDTH,WINDOW_HEIGHT));
    widget->SetVisible(true);
    widget->SetAlpha(1.0f);
    m_canvas.AddWidget(std::move(widget));
    CSpriteImage* imageButton = ImageRegistry::LoadTexture("Black", "data/BlackImage.png");
    if (image == nullptr)
        assert(false);
    auto btn = std::make_unique<CUIButton>(VECTOR2(200, 300), VECTOR4(0, 0, 128, 64), nullptr, imageButton);
    btn->SetVisible(true);
    btn->SetAlpha(1.0f);

    m_pButton = m_canvas.AddWidget(std::move(btn));
    m_pButton->SetOnFocusChanged([this](bool isFocus)
    {
        m_pButton->GetAnimationPlayer().Play(isFocus ? "Focus" : "Normal",false);
    });
    m_pButton->GetAnimationPlayer().AddAnimation("OnFocus",UIPreset::PopIn(0.15f));
    m_pButton->GetAnimationPlayer().AddAnimation("OnUnfocus",UIPreset::ScaleIn(0.1f, 1.1f, 1.0f));
}

void DebugUI::Update()
{
    static bool isPressed = false;
    m_canvas.Update();
    if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_P))
    {
        isPressed = !isPressed;
        m_pButton->SetFocus(isPressed);
    }
}

void DebugUI::Draw()
{
    m_canvas.Draw();
}
