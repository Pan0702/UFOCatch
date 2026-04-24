#include "PlayUI.h"

#include "../../UI/ImageRegistry.h"
#include "../../UI/TextRenderer.h"
#include "../../Common/Constants.h"
#include "../../Core/Game/GameMain.h"
#include "../../System/Timer.h"
#include "../../Player/PlayerHP.h"
#include "../../Player/PlayerLevel.h"

CPlayUI::CPlayUI()
{
    m_pUI = ImageRegistry::LoadTexture(SceneName::PLAY, "data/PlayUIParts.png");
    m_pLogo = ImageRegistry::LoadTexture("Logo", "data/Logo.png");
    Image();
    Gauge();
    Bar();
    LookImage();
    Digits();
}

void CPlayUI::Gauge()
{
    auto circle = std::make_unique<CUIRingGauge>(nullptr, m_pUI, VECTOR2(1123, 466), VECTOR4(965, 226, 229, 232));
    m_pGauge = m_canvas.AddWidget(std::move(circle));
    m_pGauge->SetAnim(UIPreset::Transition(16.0f, EaseLinear));
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
    m_pBar->SetAnim(UIPreset::Transition(1.5f));
    m_pBar->SetIsEXPBar(true);
    m_pBar->SetRatio(0.0f);
}

void CPlayUI::Image()
{
    // CSpriteImage* white = ImageRegistry::LoadTexture("white", "data/white.png");
    // auto i = std::make_unique<CUIImage>(white, VECTOR2(0, 0), VECTOR2(WINDOW_WIDTH,WINDOW_HEIGHT));
    // i->SetLayer(0);
    // m_canvas.AddWidget(std::move(i));

    auto UI = std::make_unique<CUIImage>(m_pUI, VECTOR2(144.0f, 711.0f),
                                         VECTOR4(0, 163.0f, 1254.0f, 57.0f));
    UI->SetLayer(1);
    m_canvas.AddWidget(std::move(UI));

    CSpriteImage* backUI = ImageRegistry::LoadTexture("PlayUIBack", "data/PlayUI.png");
    auto backImage = std::make_unique<CUIImage>(backUI, VECTOR2(0, 0), VECTOR2(WINDOW_WIDTH, WINDOW_HEIGHT));
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

void CPlayUI::Digits()
{
    // Time digits (最大TIME_MAX_DIGITS桁、位置固定)
    for (int i = 0; i < TIME_MAX_DIGITS; i++)
    {
        auto d = std::make_unique<CUIText>(m_pUI, VECTOR2(230.0f + i * 58, 640), VECTOR2(0, 540), VECTOR2(68, 103));
        d->SetSize(VECTOR2(55, 67));
        d->SetLayer(7);
        d->SetVisible(false);
        m_pTimeDigits[i] = m_canvas.AddWidget(std::move(d));
    }

    // HP current
    auto hpCur = std::make_unique<CUIText>(m_pUI, VECTOR2(1195, 530), VECTOR2(0, 540), VECTOR2(68, 103));
    hpCur->SetSize(VECTOR2(32, 54));
    hpCur->SetLayer(7);
    m_pHpCurrent = m_canvas.AddWidget(std::move(hpCur));

    // HP max
    auto hpMax = std::make_unique<CUIText>(m_pUI, VECTOR2(1247, 530), VECTOR2(0, 540), VECTOR2(68, 103));
    hpMax->SetSize(VECTOR2(32, 54));
    hpMax->SetLayer(7);
    m_pHpMax = m_canvas.AddWidget(std::move(hpMax));

    // LV digits (位置はUpdate時に計算)
    for (int i = 0; i < LV_MAX_DIGITS; i++)
    {
        auto d = std::make_unique<CUIText>(m_pUI, VECTOR2(0, 666), VECTOR2(0, 540), VECTOR2(68, 103));
        d->SetSize(VECTOR2(64, 80));
        d->SetLayer(7);
        d->SetVisible(false);
        m_pLvDigits[i] = m_canvas.AddWidget(std::move(d));
    }

    // CutIn
    auto cutIn = std::make_unique<CUIImage>(m_pLogo, VECTOR2(400, 185), VECTOR4(0, 0, 605, 141));
    cutIn->SetLayer(8);
    cutIn->SetVisible(false);
    m_pCutIn = m_canvas.AddWidget(std::move(cutIn));
}

void CPlayUI::Update()
{
    CUIBase::Update();
    UpdateTimeDigits();
    UpdateHPDigits();
    UpdateLvDigits();
    UpdateCutIn();
    UpdateGauge();
    UpdateBar();

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

void CPlayUI::UpdateTimeDigits()
{
    CTimer* pTimer = ObjectManager::FindGameObject<CTimer>();
    if (!pTimer)
    {
        for (auto d : m_pTimeDigits) d->SetVisible(false);
        return;
    }

    int time = static_cast<int>(pTimer->GetTime());
    int count = 0, tmp = time;
    while (tmp > 0)
    {
        tmp /= 10;
        count++;
    }
    count = (std::max)(count, 2);

    for (int i = 0; i < TIME_MAX_DIGITS; i++)
    {
        if (i >= count)
        {
            m_pTimeDigits[i]->SetVisible(false);
            continue;
        }
        int divisor = static_cast<int>(Pow(10, count - 1 - i));
        int num = (time / divisor) % 10;
        m_pTimeDigits[i]->SetVisible(true);
        m_pTimeDigits[i]->SetSrcPos(VECTOR2(68.0f * num, 540));
    }
}

void CPlayUI::UpdateHPDigits()
{
    CPlayerHP* pHp = ObjectManager::FindGameObject<CPlayerHP>();
    if (!pHp) return;
    m_pHpCurrent->SetSrcPos(VECTOR2(68.0f * pHp->GetHP(), 540));
    m_pHpMax->SetSrcPos(VECTOR2(68.0f * pHp->GetMaxHP(), 540));
}

void CPlayUI::UpdateLvDigits()
{
    CPlayerLevel* pl = ObjectManager::FindGameObject<CPlayerLevel>();
    if (!pl)
    {
        for (auto d : m_pLvDigits) d->SetVisible(false);
        return;
    }

    int lv = pl->GetLv();
    int count = 0, tmp = lv;
    while (tmp > 0)
    {
        tmp /= 10;
        count++;
    }
    count = (std::max)(count, 1);

    float halfWidth = count * 33.0f;
    for (int i = 0; i < LV_MAX_DIGITS; i++)
    {
        if (i >= count)
        {
            m_pLvDigits[i]->SetVisible(false);
            continue;
        }
        int divisor = static_cast<int>(Pow(10, count - 1 - i));
        int num = (lv / divisor) % 10;
        m_pLvDigits[i]->SetVisible(true);
        m_pLvDigits[i]->SetPosition(VECTOR2((83 - halfWidth) + i * 64, 666));
        m_pLvDigits[i]->SetSrcPos(VECTOR2(68.0f * num, 540));
    }
}

void CPlayUI::UpdateBar()
{
    CPlayerLevel* pl = ObjectManager::FindGameObject<CPlayerLevel>();
    if (!pl) return;
    float raito = pl->GetRaito();
    float delta = raito - m_prevRaito;
    if (delta > 0.0f)
        m_pBar->SetRatio(delta);
    m_prevRaito = raito;
}

void CPlayUI::UpdateGauge()
{
    CPlayerHP* pHp = ObjectManager::FindGameObject<CPlayerHP>();
    if (!pHp) return;
    if (pHp->GetFoundFlag())
        m_pGauge->SetRatio(1.0f);
    else
        m_pGauge->SetRatio(pHp->GetFindCount() / pHp->GetMaxFindCount());
}

void CPlayUI::UpdateCutIn()
{
    CTimer* pTimer = ObjectManager::FindGameObject<CTimer>();
    if (!pTimer)
    {
        m_pCutIn->SetVisible(false);
        return;
    }
    bool visible = pTimer->IsCutInVisible();
    m_pCutIn->SetVisible(visible);
    if (visible)
        m_pCutIn->SetSrcRect(VECTOR4(0, 141.0f * pTimer->GetCutInNum(), 605, 141));
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
