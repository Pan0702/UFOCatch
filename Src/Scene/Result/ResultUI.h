#pragma once
#include "../../UI/UIBase.h"
#include "../../UI/TextRenderer.h"

class CResultUI : public CUIBase
{
public:
    CResultUI();

private:
    void InitBack();
    void InitRank();
    void InitResultNums();
    void InitDigits(int value, float srcY);
    int CalcRank();

    CSpriteImage* m_pSprite = nullptr;
    CUIText* m_pRank = nullptr;
};
