#include "ResultUI.h"

CResultUI::CResultUI()
{
    InitBack();
    ScoreDraw();
    RankDraw();
}

void CResultUI::InitBack()
{
    CSpriteImage* image = ImageRegistry::LoadTexture(SceneName::RESULT, "data/Result/ScoreBG.jpg");
    if (image == nullptr)
        assert(false);
    auto widget = std::make_unique<CUIImage>(image, VECTOR2(0, 0),
                                             VECTOR2(WINDOW_WIDTH,WINDOW_HEIGHT));
    m_canvas.AddWidget(std::move(widget));
}

void CResultUI::ScoreDraw()
{
    CSpriteImage* image = ImageRegistry::LoadTexture(SpriteName::UI_SPRITE, FileName::UI_SPRITE);    
    auto widget = std::make_unique<CUIText>(image, VECTOR2(0, 0),
                                             VECTOR2(0,0),VECTOR2(68,103));
    m_pScore = m_canvas.AddWidget(std::move(widget));
}

void CResultUI::RankDraw()
{
    CSpriteImage* image = ImageRegistry::LoadTexture("test", "data/white.png");
    auto widget = std::make_unique<CUIText>(image, VECTOR2(0, 0),
                                             VECTOR2(0,0),VECTOR2(100,100));
    m_pRank = m_canvas.AddWidget(std::move(widget));
}
