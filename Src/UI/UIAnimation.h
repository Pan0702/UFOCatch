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
    /// @brief コンストラクタ
    /// @param name アニメーションの識別名
    CUIAnimation(std::string name = "");

    /// @brief 指定プロパティのトラックを追加して返す
    /// @param property 追加するプロパティの種類
    /// @return 追加されたトラックのポインタ（所有権はこのクラスが持つ）
    CUIAnimationTrack* AddTrack(AnimatedProperty property);

    /// @brief 指定時刻の全トラックの値を評価してoutValuesに書き込む
    /// @param time 評価する時刻（秒）
    /// @param outValues プロパティ→値のマップ（結果の出力先）
    void Evaluate(float time, std::unordered_map<AnimatedProperty, float>& outValues) const;

    const std::string& GetName() const;
    void SetName(const std::string& name);

    float GetDuration() const;
    void SetDuration(float duration);

    size_t GetTrackCount() const;

private:
    std::string m_name;
    std::vector<std::unique_ptr<CUIAnimationTrack>> m_tracks;
    float m_duration = 0.0f; // アニメーションの総再生時間（秒）

};
