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

    if (m_state == State::FadeOut)
    {
        const float progress = m_timer / m_duration;
        m_percent = progress;
        //割合が100％よりも大きくなったら100％にしてホールドへ移行
        if (m_percent >= 1.0f)
        {
            m_percent = 1.0f;
            m_state = State::Hold;
            m_timer = 0.0f;
        }
    }
    else if (m_state == State::Hold)
    {
        constexpr float m_holdDuration = 0.15f; // ホールド時間（秒）
        //ホールド時間が経過したらコールバックを呼ぶ
        if (m_timer >= m_holdDuration && m_onComplete)
        {
            m_onComplete();
            m_onComplete = nullptr;
        }
    }
    else if (m_state == State::FadeIn)
    {
        const float progress = m_timer / m_duration;
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
    if (m_state == State::Idle)
        return;

    CSprite spr;
    constexpr float imageWidth = 1366.0f;
    constexpr float imageHeight = 768.0f;
    if (m_state == State::FadeOut)
    {
        // 左→右に黒い幕が出てくる：幅が0→imageWidthに増える
        float width = imageWidth * m_percent;
        spr.Draw(m_pImage.get(), 0, 0, 0, 0, static_cast<int>(width), static_cast<int>(imageHeight));
    }
    else if (m_state == State::Hold)
    {
        // 全画面黒
        spr.Draw(m_pImage.get(), 0, 0, 0, 0, static_cast<int>(imageWidth), static_cast<int>(imageHeight));
    }
    else if (m_state == State::FadeIn)
    {
        // 左→右に黒い幕がなくなる：左端が右に移動していく
        float x = imageWidth * (1.0f - m_percent);
        float width = imageWidth * m_percent;
        spr.Draw(m_pImage.get(), x, 0, 0, 0, static_cast<int>(width), static_cast<int>(imageHeight));
    }
}
