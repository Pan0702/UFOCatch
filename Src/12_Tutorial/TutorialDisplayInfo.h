#pragma once
#include "../05_CommonFile/Object3D.h"

class CTutorialDisplayInfo : public Object3D
{
public:
    CTutorialDisplayInfo();
    void SetDisplayType(int type){ m_nDisplayType = type;}
    void SetQuest(int quest){ m_quest = quest;}

private:
    ~CTutorialDisplayInfo();
    void Draw() override;
    void OperationDraw();
    void TargetDraw();
    
    enum
    {
        Move = 0,
        Suction = 1,
    };
    
    int m_nDisplayType;
    int m_quest;
    CSprite* m_pSprite;
    CSpriteImage* m_pImage;
};
