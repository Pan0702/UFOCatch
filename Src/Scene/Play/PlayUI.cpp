#include "PlayUI.h"

#include "../../UI/ImageRegistry.h"
#include "../../Common/Constants.h"
#include "../../Core/Game/GameMain.h"

CPlayUI::CPlayUI()
{
    m_pUI = ImageRegistry::LoadTexture(SceneName::PLAY, "data/PlayUIParts.png");
    Image();
    Gauge();
    Bar();
    LookImage();
}

void CPlayUI::Gauge()
{
    auto circle = std::make_unique<CUIRingGauge>(nullptr, m_pUI, VECTOR2(1123, 466), VECTOR4(965, 226, 229, 232));
    m_pGauge = m_canvas.AddWidget(std::move(circle));
    m_pGauge->SetAnim(UIPreset::Transition(1.0f, EaseLinear));
    m_pGauge->SetRatio(0.0f);
    m_pGauge->SetLayer(6);
}

void CPlayUI::Bar()
{
    auto bar = std::make_unique<CUIProgressBar>(nullptr, m_pUI,
                                                VECTOR2(144, 719), VECTOR2(0, 157),
                                                VECTOR2(0, 100), VECTOR2(1254, 57));
    bar->SetLayer(2);
    m_pBar = m_canvas.AddWidget(std::move(bar));
    m_pBar->SetAnim(UIPreset::Transition(1.0f));
    m_pBar->SetIsEXPBar(true);
    m_pBar->SetRatio(0.0f);
}

void CPlayUI::Image()
{
    CSpriteImage* white = ImageRegistry::LoadTexture("white", "data/white.png");
    auto i = std::make_unique<CUIImage>(white, VECTOR2(0, 0), VECTOR2(WINDOW_WIDTH,WINDOW_HEIGHT));
    i->SetLayer(0);
    m_canvas.AddWidget(std::move(i));

    auto UI = std::make_unique<CUIImage>(m_pUI, VECTOR2(144.0f, 711.0f),
                                         VECTOR4(0, 163.0f, 1254.0f, 57.0f));
    UI->SetLayer(1);
    m_canvas.AddWidget(std::move(UI));

    CSpriteImage* backUI = ImageRegistry::LoadTexture("PlayUIBack", "data/PlayUI.png");
    auto backImage = std::make_unique<CUIImage>(backUI, VECTOR2(0, 0), VECTOR2(WINDOW_WIDTH,WINDOW_HEIGHT));
    backImage->SetLayer(5);
    m_canvas.AddWidget(std::move(backImage));
}

void CPlayUI::LookImage()
{
    auto look = std::make_unique<CUIImage>(m_pUI, VECTOR2(1192, 588), VECTOR2(97, 73));
    m_pLookImage = m_canvas.AddWidget(std::move(look));
    m_pLookImage->SetVisible(flag);
    m_pLookImage->SetLayer(6);
}

void CPlayUI::Update()
{
    CUIBase::Update();
#if DEBUG

    if (GameDevice()->m_pDI->CheckKey(KD_TRG, DIK_P))
    {
        m_pBar->SetRatio(1.0f);
        m_pGauge->SetRatio(1.0f);
        flag = !flag;
        m_pLookImage->SetVisible(flag);
    }

#endif
}

CUIProgressBar* CPlayUI::GetBar() const
{
    return m_pBar;
}

CUIRingGauge* CPlayUI::GetGauge() const
{
    return m_pGauge;
}

CUIImage* CPlayUI::GetLookImage() const
{
    return m_pLookImage;
}
