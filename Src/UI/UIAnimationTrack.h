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
    float time; // キーフレームの時刻（秒）
    float value; // その時刻での値

    /// KeyFrame を初期化する
    /// @param time time に渡す値
    /// @param value value に渡す値
    KeyFrame(float time, float value) : time(time), value(value)
    {
    }
};

/// @brief 1つのプロパティに対するアニメーショントラック
/// @details キーフレームを時系列で保持し、任意の時刻の値を線形補間で返す
class CUIAnimationTrack
{
public:
    /// CUIAnimationTrack を初期化する
    /// @param property property に渡す値
    CUIAnimationTrack(AnimatedProperty property);

    /// Key Frame を追加する
    /// @param time time に渡す値
    /// @param value value に渡す値
    void AddKeyFrame(float time, float value);

    /// Evaluate を返す
    /// @param time time に渡す値
    /// @return 計算結果の値
    float Evaluate(float time) const;

    /// Property を取得する
    /// @return 処理結果
    AnimatedProperty GetProperty() const;

    /// Key Frame Count を取得する
    /// @return 処理結果
    size_t GetKeyFrameCount() const;

private:
    AnimatedProperty m_property;
    std::vector<KeyFrame> m_keyFrames; // 時刻順にソートされたキーフレーム列
};
