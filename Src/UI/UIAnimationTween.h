#pragma once
#include <functional>

/// @brief ゲージ等のTweenアニメーション設定をまとめた構造体
/// @details UIProgressBarやUIRingGaugeに渡して、移動速度とイージングを一括指定する
struct UIAnimationTween
{
    /// UIAnimationTween を初期化する
    /// @param moveSpeed moveSpeed に渡す値
    /// @param easing easing に渡す値
    UIAnimationTween(float moveSpeed, std::function<float(float)> easing)
        : m_moveSpeed(moveSpeed), m_easing(std::move(easing))
    {
    }

    float m_moveSpeed; // アニメーションの進む速さ（1秒あたりのt増加量）
    /// function<float を返す
    std::function<float(float)> m_easing;
};
