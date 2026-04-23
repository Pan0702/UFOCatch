#pragma once
#include <functional>
#include <memory>
#include "UIAnimation.h"
#include "UIAnimationTween.h"
#include "../Utils/Lerp.h"

/// @brief よく使うUIアニメーションをワンライナーで生成するファクトリ関数群
namespace UIPreset
{
    /// @brief UIAnimationTweenをワンライナーで生成するヘルパー
    UIAnimationTween Transition(float speed, std::function<float(float)> easing = EaseOutQuint);

    // フェード
    /// @brief 指定秒数でアルファをfromからtoへ変化させるアニメーションを生成する
    std::unique_ptr<CUIAnimation> FadeIn (float duration, float from = 0.0f, float to = 1.0f);
    std::unique_ptr<CUIAnimation> FadeOut(float duration, float from = 1.0f, float to = 0.0f);

    // スケール
    std::unique_ptr<CUIAnimation> ScaleIn (float duration, float from = 0.0f, float to = 1.0f);
    /// @brief スケールが 1→peak→1 と膨らんで戻るポップアニメーションを生成する
    std::unique_ptr<CUIAnimation> ScalePop(float duration, float peak = 1.2f);

    // スライド（親からの相対ピクセル移動）
    std::unique_ptr<CUIAnimation> SlideInX(float duration, float fromOffset, float toOffset = 0.0f);
    std::unique_ptr<CUIAnimation> SlideInY(float duration, float fromOffset, float toOffset = 0.0f);

    /// @brief FadeIn + ScaleIn を1つのアニメーションにまとめたポップイン演出を生成する
    std::unique_ptr<CUIAnimation> PopIn(float duration, float fromScale = 0.8f, float fromAlpha = 0.0f);
};
