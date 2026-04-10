#pragma once
#include "UIBase.h"

class CTextRenderer : public CUIBase
{
public:
    CTextRenderer(CSpriteImage* pFont);
private:
    CSpriteImage* m_pFont;
};
