#pragma once
#include <cstdint>
#include <vector>

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

struct KeyFrame
{
    float time;
    float value;

    KeyFrame(float time, float value) : time(time), value(value)
    {
    }
};

class CUIAnimationTrack
{
public:
    CUIAnimationTrack(AnimatedProperty property);

    void AddKeyFrame(float time, float value);

    float Evaluate(float time) const;

    AnimatedProperty GetPopety() const;

    size_t GetKeyFrameCount() const;

private:
    AnimatedProperty m_property;
    std::vector<KeyFrame> m_keyFrames;
};
