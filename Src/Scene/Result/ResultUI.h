#pragma once
#include "../../UI/UIBase.h"
#include "../../UI/TextRenderer.h"

class CResultUI : public CUIBase
{
public:
    CResultUI();
private:
    void InitBack();
    void ScoreDraw();
    void RankDraw();
    
    CUIText* m_pScore;
    CUIText* m_pRank;
};
