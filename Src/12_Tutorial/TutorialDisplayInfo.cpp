#include "TutorialDisplayInfo.h"

CTutorialDisplayInfo::CTutorialDisplayInfo()
{
    m_pSprite = new CSprite();
    m_pImage = new CSpriteImage();
    m_pSprite->SetImage(m_pImage);
    m_nDisplayType = 0;
    m_quest = 0;
}

CTutorialDisplayInfo::~CTutorialDisplayInfo()
{
    SAFE_DELETE(m_pSprite);
    SAFE_DELETE(m_pImage);
}

void CTutorialDisplayInfo::Draw()
{
    OperationDraw();
    TargetDraw();
}

void CTutorialDisplayInfo::OperationDraw()
{
    switch (m_nDisplayType)
    {
    case Move:
        m_pSprite->Draw(m_pImage, 0, 0, 0, 0, 382, 124);
        break;
    case Suction:
        m_pSprite->Draw(m_pImage, 0, 0, 0, 125, 406, 84);
        break;
    default:
        break;
    }
}

void CTutorialDisplayInfo::TargetDraw()
{
    m_pSprite->Draw(m_pImage, 0, 0, 0, 125, 406, 84);
    
}
