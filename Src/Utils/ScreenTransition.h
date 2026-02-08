#pragma once
#include <memory>
#include <functional>

class CSprite;
class CSpriteImage;

class CScreenTransition
{
public:
    enum class State
    {
        Idle,       // 何もしていない
        FadeOut,    // 左→右に黒い幕が出てくる
        Hold,       // 全画面黒で一時停止
        FadeIn      // 左→右に黒い幕がなくなる
    };

    CScreenTransition();
    ~CScreenTransition();

    // フェードアウト開始（画面を覆う）
    void StartFadeOut(const std::function<void()>& onComplete = nullptr);

    // フェードイン開始（画面を開く）
    void StartFadeIn(const std::function<void()>& onComplete = nullptr);

    void Update(float deltaTime);
    void Draw();

    // 現在の状態が遷移中かどうかを取得する
    bool IsTransitioning() const { return m_state != State::Idle; }
    //現在のStateを取得する
    State GetState() const { return m_state; }

private:
    std::unique_ptr<CSpriteImage> m_pImage;

    State m_state = State::Idle;
    float m_duration = 0.2f;    // アニメーション時間（秒）秒/
    float m_timer = 0.0f;
    float m_percent = 0.0f;     // 0.0〜1.0

    std::function<void()> m_onComplete;
};
