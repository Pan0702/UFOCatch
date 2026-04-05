#pragma once
#include <functional>

struct UIAnimationTween
{
    UIAnimationTween(float moveSpeed,std::function<float(float)> easing)
        : m_moveSpeed(moveSpeed), m_easing(std::move(easing)){}

    float m_moveSpeed;
    std::function<float(float)> m_easing;
};
