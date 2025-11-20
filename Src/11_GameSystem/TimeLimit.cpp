#include "TimeLimit.h"

#include "../07_Scene/PlayScene.h"

CTimeLimit::CTimeLimit(const float maxTimeSec)
    : m_CurrentTime(maxTimeSec), m_maxTime(maxTimeSec)
{
    m_pSprite = new CSprite();
    m_pSpriteImage = new CSpriteImage();
    m_pSpriteImage->Load("data/Vector.png");
}

CTimeLimit::~CTimeLimit() = default;

void CTimeLimit::Update()
{
    m_CurrentTime -= SceneManager::DeltaTime();
    if (m_CurrentTime <= 0)
    {
        TimeOut();
    }
}

void CTimeLimit::TimeOut()
{
    //ObjectManager::FindGameObject<PlayScene>()->ChangeResultScene();
}

void CTimeLimit::Draw()
{
    int num = static_cast<int>(m_CurrentTime);
    DrawTime(1000, 20, num / 10);
    DrawTime(1050, 20, num % 10);
    
}

void CTimeLimit::DrawTime(const float& posX, const float& posY,const float& num) const
{
   m_pSprite->Draw(m_pSpriteImage,posX,posY,45 * num,0 ,45 , 50);
}
