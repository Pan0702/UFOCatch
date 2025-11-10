#pragma once
#include <cmath>

inline float EaseOutQuint(const float& t)
{
    return 1 - std::pow(1 - t, 5);
}
inline float Lerp(float start, float end, float t) {
    return start + (end - start) * t;
}