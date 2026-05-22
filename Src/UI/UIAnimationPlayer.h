#pragma once
#include <functional>
#include <memory>
#include <string>

#include "UIAnimation.h"

/// @brief UIアニメーションの再生を管理するクラス
///          毎フレームUpdate()を呼ぶことで現在のプロパティ値が更新される。
class CUIAnimationPlayer
{
public:
    /// CUIAnimationPlayer を初期化する
    CUIAnimationPlayer();

    /// Animation を追加する
    /// @param name 名前
    /// @param anim anim に渡す値
    void AddAnimation(const std::string& name, std::shared_ptr<CUIAnimation> anim);

    /// 再生する
    /// @param name 名前
    /// @param loop ループ再生するか
    void Play(const std::string& name, bool loop);

    /// 停止する
    void Stop();

    /// On Complete を設定する
    /// @param callback callback に渡す値
    void SetOnComplete(std::function<void()> callback);

    /// Pause の処理を行う
    void Pause();
    /// Resume の処理を行う
    void Resume();

    /// 毎フレームの状態を更新する
    /// @param deltaTime 経過時間[秒]
    void Update(float deltaTime);

    /// Current Values を取得する
    /// @return 処理結果
    const std::unordered_map<AnimatedProperty, float>& GetCurrentValues() const;
    /// Playing を判定する
    /// @return 成功または条件を満たす場合 true
    bool IsPlaying() const;
    /// Current Anim Name を取得する
    /// @return 取得した文字列
    const std::string& GetCurrentAnimName() const;

private:
    std::unordered_map<AnimatedProperty, float> m_currentValues; // 現在フレームのプロパティ値キャッシュ
    std::unordered_map<std::string, std::shared_ptr<CUIAnimation>> m_animations; // 登録済みアニメーション一覧
    CUIAnimation* m_pCurrentAnim = nullptr; // 再生中のアニメーション（所有しない）
    float m_currentTime; // 再生中の現在時刻（秒）
    bool m_isPlaying = false;
    bool m_isLoop = false;
    bool m_isPaused = false;
    /// function<void を返す
    std::function<void()> m_onComplete = nullptr; // 再生完了時コールバック（1度呼んだら破棄）
};
