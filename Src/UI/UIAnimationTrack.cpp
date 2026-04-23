#include "UIAnimationTrack.h"

#include <algorithm>

CUIAnimationTrack::CUIAnimationTrack(AnimatedProperty property)
    : m_property(property)
{
}

void CUIAnimationTrack::AddKeyFrame(float time, float value)
{
    // 時刻順を保つため、挿入位置を二分探索で求めてから挿入する
    auto it = std::ranges::lower_bound(m_keyFrames, time, {}, &KeyFrame::time);
    m_keyFrames.insert(it, KeyFrame(time, value));
}

float CUIAnimationTrack::Evaluate(float time) const
{
    if (m_keyFrames.empty()) return 0.0f;
    // 範囲外は両端の値をそのまま返す（クランプ動作）
    if (time < m_keyFrames.front().time) return m_keyFrames.front().value;
    if (time > m_keyFrames.back().time) return m_keyFrames.back().value;

    // timeが含まれる区間を探して線形補間する
    for (size_t i = 0; i < m_keyFrames.size() - 1; i++)
    {
        const KeyFrame& k1 = m_keyFrames[i];
        const KeyFrame& k2 = m_keyFrames[i + 1];
        if (time >= k1.time && time <= k2.time)
        {
            // 区間内でのtを0〜1に正規化して補間
            float t = (time - k1.time) / (k2.time - k1.time);
            return k1.value + (k2.value - k1.value) * t;
        }
    }
    return 0.0f;
}


AnimatedProperty CUIAnimationTrack::GetProperty() const
{
    return m_property;
}

size_t CUIAnimationTrack::GetKeyFrameCount() const
{
    return m_keyFrames.size();
}
