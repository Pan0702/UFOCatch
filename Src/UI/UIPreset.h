#pragma once
#include <functional>
#include <memory>
#include "UIAnimation.h"
#include "UIAnimationTween.h"
#include "../Utils/Lerp.h"

/// @brief よく使うUIアニメーションをワンライナーで生成するファクトリ関数群
namespace UIPreset
{
    /// Transition を返す
    /// @param speed speed に渡す値
    /// @param easing easing に渡す値
    /// @return 処理結果
    UIAnimationTween Transition(float speed, std::function<float(float)> easing = EaseOutQuint);

    // フェード
    /// Fade In を返す
    /// @param duration duration に渡す値
    /// @param from from に渡す値
    /// @param to to に渡す値
    /// @return 処理結果
    std::unique_ptr<CUIAnimation> FadeIn(float duration, float from = 0.0f, float to = 1.0f);
    /// Fade Out を返す
    /// @param duration duration に渡す値
    /// @param from from に渡す値
    /// @param to to に渡す値
    /// @return 処理結果
    std::unique_ptr<CUIAnimation> FadeOut(float duration, float from = 1.0f, float to = 0.0f);

    // スケール
    /// In を拡大縮小する
    /// @param duration duration に渡す値
    /// @param from from に渡す値
    /// @param to to に渡す値
    /// @return 処理結果
    std::unique_ptr<CUIAnimation> ScaleIn(float duration, float from = 0.0f, float to = 1.0f);
    /// Pop を拡大縮小する
    /// @param duration duration に渡す値
    /// @param peak peak に渡す値
    /// @return 処理結果
    std::unique_ptr<CUIAnimation> ScalePop(float duration, float peak = 1.2f);

    // スライド（親からの相対ピクセル移動）
    /// Slide In X を返す
    /// @param duration duration に渡す値
    /// @param fromOffset fromOffset に渡す値
    /// @param toOffset toOffset に渡す値
    /// @return 処理結果
    std::unique_ptr<CUIAnimation> SlideInX(float duration, float fromOffset, float toOffset = 0.0f);
    /// Slide In Y を返す
    /// @param duration duration に渡す値
    /// @param fromOffset fromOffset に渡す値
    /// @param toOffset toOffset に渡す値
    /// @return 処理結果
    std::unique_ptr<CUIAnimation> SlideInY(float duration, float fromOffset, float toOffset = 0.0f);

    /// In を取り出す
    /// @param duration duration に渡す値
    /// @param fromScale スケール
    /// @param fromAlpha 透明度
    /// @return 処理結果
    std::unique_ptr<CUIAnimation> PopIn(float duration, float fromScale = 0.8f, float fromAlpha = 0.0f);
};
