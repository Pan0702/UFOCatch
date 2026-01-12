#include "Timer.h"

#include "../07_Scene/PlayScene.h"


////////////////////
// タイマーの最大時間を指定してインスタンスを生成する
// @param maxTimeSec 最大時間（秒） //
////////////////////
CTimer::CTimer(const float maxTimeSec)
    : m_currentTime(maxTimeSec), m_maxTime(maxTimeSec)
{
    m_pSprite = new CSprite();
    m_pSpriteImage = new CSpriteImage();
    m_pSpriteImage->Load("data/Vector.png");
}


CTimer::~CTimer() = default;

void CTimer::Update()
{
    m_currentTime -= SceneManager::DeltaTime();
    // if (m_currentTime <= 0)
    // {
    //m_currentTime =0;
    //     TimeOut();
    // }
}

////////////////////
// タイムアウト時の処理 //
////////////////////
void CTimer::TimeOut()
{
    SceneManager::ChangeScene("ResultScene");
}

////////////////////
// 時間を描画する
// @param posX X座標
// @param posY Y座標
// @param num 数値 //
////////////////////
void CTimer::DrawTime(const float& posX, const float& posY,const float& num) const
{
   m_pSprite->Draw(m_pSpriteImage,posX,posY,45 * num,0 ,45 , 50);
}
