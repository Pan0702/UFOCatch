#include "Timer.h"

#include "../07_Scene/PlayScene.h"


CTimer::CTimer(const float maxTimeSec)
    : m_CurrentTime(maxTimeSec), m_maxTime(maxTimeSec)
{
    m_pSprite = new CSprite();
    m_pSpriteImage = new CSpriteImage();
    m_pSpriteImage->Load("data/Vector.png");
}


CTimer::~CTimer() = default;

void CTimer::Update()
{
    m_CurrentTime -= SceneManager::DeltaTime();
    // if (m_CurrentTime <= 0)
    // {
    //     TimeOut();
    // }
}

void CTimer::TimeOut()
{
    SceneManager::ChangeScene("ResultScene");
}

void CTimer::Draw()
{
    int num = static_cast<int>(m_CurrentTime);
    DrawTime(1000, 20, num / 10);
    DrawTime(1050, 20, num % 10);
    
}

void CTimer::DrawTime(const float& posX, const float& posY,const float& num) const
{
   m_pSprite->Draw(m_pSpriteImage,posX,posY,45 * num,0 ,45 , 50);
}
