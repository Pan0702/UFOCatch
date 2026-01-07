#pragma once
#include "../05_CommonFile/Object3D.h"

class CTimer : public Object3D
{
public:
    CTimer(float maxTimeSec);
    ~CTimer();

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
