#pragma once
#include <functional>

/// @brief ゲージ等のTweenアニメーション設定をまとめた構造体
/// @details UIProgressBarやUIRingGaugeに渡して、移動速度とイージングを一括指定する
struct UIAnimationTween
{
    UIAnimationTween(float moveSpeed, std::function<float(float)> easing)
        : m_moveSpeed(moveSpeed), m_easing(std::move(easing)) {}

    float m_moveSpeed;                       // アニメーションの進む速さ（1秒あたりのt増加量）
    std::function<float(float)> m_easing;    // イージング関数（引数・戻り値ともに0〜1）
};
