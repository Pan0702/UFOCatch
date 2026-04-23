#pragma once
#include <functional>
#include <memory>
#include <string>

#include "UIAnimation.h"

/// @brief UIアニメーションの再生を管理するクラス
/// @details 複数のアニメーションを名前で登録し、再生・停止・一時停止を制御する。
///          毎フレームUpdate()を呼ぶことで現在のプロパティ値が更新される。
class CUIAnimationPlayer
{
public:
    CUIAnimationPlayer();

    /// @brief アニメーションを名前付きで登録する
    /// @param name 再生時に使う識別名
    /// @param anim アニメーションデータ（shared_ptrで共有可能）
    void AddAnimation(const std::string& name, std::shared_ptr<CUIAnimation> anim);

    /// @brief 指定名のアニメーションを再生する
    /// @param name 再生するアニメーション名
    /// @param loop trueでループ再生
    void Play(const std::string& name, bool loop);

    /// @brief 再生を停止し、完了コールバックも破棄する
    void Stop();

    /// @brief アニメーション完了時に呼ばれるコールバックを設定する
    /// @param callback 完了時に1度だけ呼ばれる関数
    void SetOnComplete(std::function<void()> callback);

    void Pause();
    void Resume();

    /// @brief アニメーションを1フレーム分進める
    /// @param deltaTime 経過時間（秒）
    void Update(float deltaTime);

    /// @brief 現在フレームの全プロパティ値を返す
    const std::unordered_map<AnimatedProperty, float>& GetCurrentValues() const;
    bool IsPlaying() const;
    const std::string& GetCurrentAnimName() const;

private:
    std::unordered_map<AnimatedProperty, float> m_currentValues;                  // 現在フレームのプロパティ値キャッシュ
    std::unordered_map<std::string, std::shared_ptr<CUIAnimation>> m_animations;  // 登録済みアニメーション一覧
    CUIAnimation* m_pCurrentAnim = nullptr;  // 再生中のアニメーション（所有しない）
    float m_currentTime;                     // 再生中の現在時刻（秒）
    bool m_isPlaying = false;
    bool m_isLoop = false;
    bool m_isPaused = false;
    std::function<void()> m_onComplete = nullptr; // 再生完了時コールバック（1度呼んだら破棄）

};
