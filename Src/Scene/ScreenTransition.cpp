#include "ScreenTransition.h"
#include "../Utils/Sprite3D.h"

CScreenTransition::CScreenTransition()
{
    m_pImage = std::make_unique<CSpriteImage>("data/BlackImage.png");
}

CScreenTransition::~CScreenTransition() = default;

void CScreenTransition::StartFadeOut(const std::function<void()>& onComplete)
{
    m_state = State::FadeOut;
    m_timer = 0.0f;
    m_percent = 0.0f;
    m_onComplete = onComplete;
}

void CScreenTransition::StartFadeIn(const std::function<void()>& onComplete)
{
    m_state = State::FadeIn;
    m_timer = 0.0f;
    m_percent = 1.0f;
    m_onComplete = onComplete;
}

void CScreenTransition::Update(float deltaTime)
{
    if (m_state == State::Idle)
        return;

    m_timer += deltaTime;
   const float progress = m_timer / m_duration;

    if (m_state == State::FadeOut)
    {
        m_percent = progress;
        //割合が100％よりも大きくなったら100％にする
        if (m_percent >= 1.0f)
        {
            m_percent = 1.0f;
            m_state = State::Idle;
            if (m_onComplete)
            {
                m_onComplete();
                m_onComplete = nullptr;
            }
        }
    }
    else if (m_state == State::FadeIn)
    {
        
        //100%から今の％を引いて求める割合を計算する
        m_percent = 1.0f - progress;
        //割合が0％よりも小さくなったら0％にする
        if (m_percent <= 0.0f)
        {
            m_percent = 0.0f;
            m_state = State::Idle;
            if (m_onComplete)
            {
                m_onComplete();
                m_onComplete = nullptr;
            }
        }
    }
}

void CScreenTransition::Draw() 
{
    if (m_state == State::Idle && m_percent <= 0.0f)
        return;

    CSprite spr;
    constexpr float imageWidth = 1366.0f;
    const float width = imageWidth * m_percent;
    spr.Draw(m_pImage.get(), 0, 0, 0, 0, static_cast<int>(width), 768);
}
