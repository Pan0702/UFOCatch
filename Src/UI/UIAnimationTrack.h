#pragma once
#include <cstdint>
#include <vector>

/// @brief アニメーションで操作できるプロパティの種類
enum class AnimatedProperty : uint8_t
{
    PositionX,
    PositionY,
    ScaleX,
    ScaleY,
    Rotation,
    Alpha,
    ColorR,
    ColorG,
    ColorB,
};

/// @brief アニメーションの1つのキーフレーム（時刻と値のペア）
struct KeyFrame
{
    float time;   // キーフレームの時刻（秒）
    float value;  // その時刻での値

    KeyFrame(float time, float value) : time(time), value(value)
    {
    }
};

/// @brief 1つのプロパティに対するアニメーショントラック
/// @details キーフレームを時系列で保持し、任意の時刻の値を線形補間で返す
class CUIAnimationTrack
{
public:
    /// @brief コンストラクタ
    /// @param property このトラックが操作するプロパティ
    CUIAnimationTrack(AnimatedProperty property);

    /// @brief キーフレームを追加する（時刻順に自動挿入される）
    /// @param time キーフレームの時刻（秒）
    /// @param value その時刻での値
    void AddKeyFrame(float time, float value);

    /// @brief 指定時刻の値を線形補間で取得する
    /// @param time 評価する時刻（秒）
    /// @return 補間された値
    float Evaluate(float time) const;

    /// @brief このトラックが操作するプロパティを返す
    AnimatedProperty GetProperty() const;

    size_t GetKeyFrameCount() const;

private:
    AnimatedProperty m_property;
    std::vector<KeyFrame> m_keyFrames; // 時刻順にソートされたキーフレーム列
};
