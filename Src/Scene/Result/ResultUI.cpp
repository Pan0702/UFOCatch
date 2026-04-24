#include "ResultUI.h"

#include "../../System/GameInstance.h"
#include "../../Utils/MyMath.h"

CResultUI::CResultUI()
{
    m_pSprite = ImageRegistry::LoadTexture(SpriteName::UI_SPRITE, FileName::UI_SPRITE);
    InitBack();
    InitRank();
    InitResultNums();
}

void CResultUI::InitBack()
{
    CSpriteImage* image = ImageRegistry::LoadTexture(SceneName::RESULT, "data/Result/ScoreBG.jpg");
    if (image == nullptr)
        assert(false);
    auto widget = std::make_unique<CUIImage>(image, VECTOR2(0, 0),
                                             VECTOR2(WINDOW_WIDTH, WINDOW_HEIGHT));
    m_canvas.AddWidget(std::move(widget));
}

int CResultUI::CalcRank()
{
    CGameInstance* pGI = CGameInstance::Get();
    int score = pGI->GetScore() - (pGI->GetDiscovery() * 3) - pGI->GetSaw();
    float ratio = avoidZero(static_cast<float>(score) / static_cast<float>(pGI->GetMaxScore()));
    if (ratio < 0.30f) return 3;
    if (ratio < 0.60f) return 2;
    if (ratio < 0.90f) return 1;
    return 0;
}

void CResultUI::InitRank()
{
    int rankNum = CalcRank();
    auto rank = std::make_unique<CUIText>(m_pSprite, VECTOR2(270, 340),
                                          VECTOR2(180.0f * rankNum, 230.0f), VECTOR2(179, 308));
    rank->SetLayer(1);
    m_pRank = m_canvas.AddWidget(std::move(rank));
}

void CResultUI::InitResultNums()
{
    CGameInstance* pGI = CGameInstance::Get();
    InitDigits(pGI->GetDiscovery(), 60.0f);
    InitDigits(pGI->GetSaw(), 217.0f);
    InitDigits(pGI->GetCapture(), 387.0f);
}

void CResultUI::InitDigits(int value, float srcY)
{
    int count = 0, tmp = value;
    while (tmp > 0)
    {
        tmp /= 10;
        count++;
    }
    if (count == 0) count = 1;

    for (int i = 0; i < count; i++)
    {
        int divisor = static_cast<int>(Pow(10, count - 1 - i));
        int num = (value / divisor) % 10;
        auto d = std::make_unique<CUIText>(m_pSprite,
                                           VECTOR2(1000.0f + i * 73.0f, srcY),
                                           VECTOR2(68.0f * num, 540.0f),
                                           VECTOR2(68, 103));
        d->SetLayer(1);
        m_canvas.AddWidget(std::move(d));
    }
}
