#pragma once
#include <cmath>

inline float EaseOutQuint(const float& t)
{
    return 1 - Pow2(Pow2(1 -t)) * (1 - t);
}

/**
 * 前半減速、後半加速のイージング関数
 * 0.0～0.5: 減速（ゆっくり）
 * 0.5～1.0: 加速
 */
inline float EaseSlowToFast(float t)
{
    if (t < 0.5f)
    {
        // 前半: EaseOut的な動き（減速）
        float t2 = t * 2.0f; // 0.0～1.0にスケール
        return (1.0f - (1.0f - t2) * (1.0f - t2)) * 0.5f; // 結果を0.0～0.5にスケール
    }
    else
    {
        // 後半: EaseIn的な動き（加速）
        float t2 = (t - 0.5f) * 2.0f; // 0.0～1.0にスケール
        return 0.5f + (t2 * t2) * 0.5f; // 結果を0.5～1.0にスケール
    }
}

inline float Lerp(float start, float end, float t) {
    return start + (end - start) * t;
}