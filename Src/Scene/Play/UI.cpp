#include "UI.h"

#include "../../UI/ImageRegistry.h"
#include "../../Common/Constants.h"
#include "../../Core/Game/GameMain.h"

PlayUI::PlayUI()
{
    
    m_pUI = ImageRegistry::LoadTexture(Constants::SceneName::PLAY, "data/PlayUI.png");
    Image();
    Gauge();
    Bar();
    LookImage();

}

void PlayUI::Gauge()
{
    auto circle = std::make_unique<CUIRingGauge>(nullptr, m_pUI, VECTOR2(1123, 469), VECTOR4(965, 227, 226, 230));
    m_pGauge = m_canvas.AddWidget(std::move(circle));
    m_pGauge->SetAnim(UIPreset::Transition(0.5f));
    m_pGauge->SetRatio(0.0f);
}

void PlayUI::Bar()
{
    auto bar = std::make_unique<CUIProgressBar>(m_pUI, m_pUI,
                                              VECTOR2(144, 713), VECTOR2(0, 157), 
                                              VECTOR2(0, 100), VECTOR2(1254, 57));
    m_pBar = m_canvas.AddWidget(std::move(bar));
    m_pBar->SetAnim(UIPreset::Transition(5.0f));
    m_pBar->SetRatio(0.0f);
}

void PlayUI::Image()
{
    CSpriteImage* white = ImageRegistry::LoadTexture("white","data/white.png");
    auto backImage = std::make_unique<CUIImage>(white,VECTOR2(0,0),VECTOR2(WINDOW_WIDTH,WINDOW_HEIGHT));
    m_canvas.AddWidget(std::move(backImage));
    constexpr float IMAGE_HEIGHT = 987 - 656;
    auto backUI = std::make_unique<CUIImage>(m_pUI, VECTOR2(0,WINDOW_HEIGHT - IMAGE_HEIGHT),
                                             VECTOR4(0, 656.0f,WINDOW_WIDTH, 987));
    m_canvas.AddWidget(std::move(backUI));
}

void PlayUI::LookImage()
{
    auto look = std::make_unique<CUIImage>(m_pUI, VECTOR2(1192,588),VECTOR2(97,73));
    m_pLookImage = m_canvas.AddWidget(std::move(look));
    m_pLookImage->SetVisible(flag);
}

void PlayUI::Update()
{
#if DEBUG 
    
    if (GameDevice()->m_pDI->CheckKey(KD_TRG,DIK_P))
    {
        m_pBar->SetRatio(1.0f);
        m_pGauge->SetRatio(1.0f);
        flag = !flag;
        m_pLookImage->SetVisible(flag);
    }
    CUIBase::Update();
#endif
    
}

CUIProgressBar* PlayUI::GetBar() const
{
    return m_pBar;
}

CUIRingGauge* PlayUI::GetGauge() const
{
    return m_pGauge;
}

CUIImage* PlayUI::GetLookImage() const
{
    return m_pLookImage;
}
