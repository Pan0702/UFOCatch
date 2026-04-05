#pragma once
#include <functional>
#include <memory>
#include "UIAnimation.h"
#include "UIAnimationTween.h"
#include "../Utils/Lerp.h"

namespace UIPreset
{

    UIAnimationTween Transition(float speed, std::function<float(float)> easing = EaseOutQuint);

    // フェード//
    std::unique_ptr<CUIAnimation> FadeIn (float duration, float from = 0.0f, float to = 1.0f);
    std::unique_ptr<CUIAnimation> FadeOut(float duration, float from = 1.0f, float to = 0.0f);

    // スケール//
    std::unique_ptr<CUIAnimation> ScaleIn (float duration, float from = 0.0f, float to = 1.0f);
    std::unique_ptr<CUIAnimation> ScalePop(float duration, float peak = 1.2f);   // 0→peak→1

    // スライド（親からの相対ピクセル移動）//
    std::unique_ptr<CUIAnimation> SlideInX(float duration, float fromOffset, float toOffset = 0.0f);
    std::unique_ptr<CUIAnimation> SlideInY(float duration, float fromOffset, float toOffset = 0.0f);

    // 組み合わせ（FadeIn + ScaleIn を1つのAnimationで）//
    std::unique_ptr<CUIAnimation> PopIn(float duration, float fromScale = 0.8f, float fromAlpha = 0.0f);
};
