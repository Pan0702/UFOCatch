#pragma once
#include <cmath>

inline float EaseOutQuint(const float& t)
{
    return 1 - Pow2(Pow2(1 -t)) * (1 - t);
}
inline float Lerp(float start, float end, float t) {
    return start + (end - start) * t;
}