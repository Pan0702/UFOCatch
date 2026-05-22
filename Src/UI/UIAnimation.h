#pragma once
#include <memory>
#include <string>
#include <unordered_map>

#include "UIAnimationTrack.h"

/// @brief 1つのUIアニメーションデータ
/// @details 複数のトラック（プロパティごと）を持ち、指定時刻の全プロパティ値を一括評価できる
class CUIAnimation
{
public:
    /// CUIAnimation を初期化する
    /// @param name 名前
    CUIAnimation(std::string name = "");

    /// Track を追加する
    /// @param property property に渡す値
    /// @return 対象のポインタ
    CUIAnimationTrack* AddTrack(AnimatedProperty property);

    /// Evaluate の処理を行う
    /// @param time time に渡す値
    /// @param outValues outValues に渡す値
    void Evaluate(float time, std::unordered_map<AnimatedProperty, float>& outValues) const;

    /// Name を取得する
    /// @return 取得した文字列
    const std::string& GetName() const;
    /// Name を設定する
    /// @param name 名前
    void SetName(const std::string& name);

    /// Duration を取得する
    /// @return 計算結果の値
    float GetDuration() const;
    /// Duration を設定する
    /// @param duration duration に渡す値
    void SetDuration(float duration);

    /// Track Count を取得する
    /// @return 処理結果
    size_t GetTrackCount() const;

private:
    std::string m_name;
    std::vector<std::unique_ptr<CUIAnimationTrack>> m_tracks;
    float m_duration = 0.0f; // アニメーションの総再生時間（秒）
};
