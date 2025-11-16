#pragma once
#include "../05_CommonFile/Object3D.h"

class CTimeLimit : public Object3D
{
public:
    CTimeLimit(const float maxTimeSec);
    ~CTimeLimit();

private:
    void Update() override;
    void Draw() override;
    void DrawTime(const float& posX, const float& posY, const float& num) const;
    void TimeOut();

private:
    float m_CurrentTime;
    float m_maxTime;
    CSprite* m_pSprite;
    CSpriteImage* m_pSpriteImage;
};
