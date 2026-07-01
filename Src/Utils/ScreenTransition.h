#pragma once
#include <memory>
#include <functional>

/// <summary>汎用ユーティリティで使う Sprite の情報と処理をまとめる型</summary>
class CSprite;
/// <summary>汎用ユーティリティで使う Sprite Image の情報と処理をまとめる型</summary>
class CSpriteImage;

/// <summary>汎用ユーティリティで使う Screen Transition の情報と処理をまとめる型</summary>
class CScreenTransition
{
public:
    /// <summary>State で扱う状態や種別を表す列挙型</summary>
    enum class State
    {
        Idle, // 何もしていない
        FadeOut, // 左→右に黒い幕が出てくる
        Hold, // 全画面黒で一時停止
        FadeIn // 左→右に黒い幕がなくなる
    };

    /// CScreenTransition を初期化する
    CScreenTransition();
    /// CScreenTransition の終了処理を行う
    ~CScreenTransition();

    // フェードアウト開始（画面を覆う）
    /// Fade Out を開始する
    /// @param onComplete onComplete に渡す値
    void StartFadeOut(const std::function<void()>& onComplete = nullptr);

    // フェードイン開始（画面を開く）
    /// Fade In を開始する
    /// @param onComplete onComplete に渡す値
    void StartFadeIn(const std::function<void()>& onComplete = nullptr);

    /// 毎フレームの状態を更新する
    /// @param deltaTime 経過時間[秒]
    void Update(float deltaTime);
    /// 描画する
    void Draw();

    // 現在の状態が遷移中かどうかを取得する
    /// Transitioning を判定する
    /// @return 成功または条件を満たす場合 true
    bool IsTransitioning() const { return m_state != State::Idle; }
    //現在のStateを取得する
    /// State を取得する
    /// @return 処理結果
    State GetState() const { return m_state; }

private:
    std::unique_ptr<CSpriteImage> m_pImage;

    State m_state = State::Idle;
    float m_duration = 0.2f; // アニメーション時間（秒）秒/
    float m_timer = 0.0f;
    float m_percent = 0.0f; // 0.0〜1.0

    /// function<void を返す
    std::function<void()> m_onComplete;
};
