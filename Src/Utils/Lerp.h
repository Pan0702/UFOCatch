#pragma once
#include "../Utils/MyMath.h"
#include <cmath>
#include "MyMath.h"
/// Ease Out Quint を返す
/// @param t t に渡す値
/// @return 計算結果の値
inline float EaseOutQuint(const float& t)
{
    return 1 - Pow2(Pow2(1 - t)) * (1 - t);
}

/// Ease In Quint を返す
/// @param t t に渡す値
/// @return 計算結果の値
inline float EaseInQuint(const float& t)
{
    return Pow2(Pow2(t)) * t;
}

/// Ease Linear を返す
/// @param t t に渡す値
/// @return 計算結果の値
inline float EaseLinear(float t)
{
    return t;
}

/**
 * 前半減速、後半加速のイージング関数
 * 0.0～0.5: 減速（ゆっくり）
 * 0.5～1.0: 加速
 */
/// Ease Slow To Fast を返す
/// @param t t に渡す値
/// @return 計算結果の値
inline float EaseSlowToFast(float t)
{
    if (t < 0.5f)
    {
        // 前半: 2次ベジエのEaseOut風（0.0で傾き2, 0.5で傾き0）
        // 1 - (1-2t)^2 の半分
        float t2 = t * 2.0f;
        return (1.0f - (1.0f - t2) * (1.0f - t2)) * 0.5f;
    }
    else
    {
        // 後半: 2次ベジエのEaseIn風（0.5で傾き0, 1.0で傾き2）
        // (2t-1)^2 の半分 + 0.5
        float t2 = (t - 0.5f) * 2.0f;
        return 0.5f + (t2 * t2) * 0.5f;
    }
}

/// Lerp を返す
/// @param start start に渡す値
/// @param end end に渡す値
/// @param t t に渡す値
/// @return 計算結果の値
inline float Lerp(float start, float end, float t)
{
    return start + (end - start) * t;
}
