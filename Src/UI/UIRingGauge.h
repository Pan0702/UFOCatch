#pragma once
#include "UIAnimationTween.h"
#include "UIImage.h"
#include "UIWidget.h"
#include "../UI/UIPreset.h"

/// @brief 円弧状に描画するリングゲージウィジェット
/// @details SetRatio()で目標割合を設定してアニメーションし、DrawArcで扇形に描画する。
class CUIRingGauge : public CUIWidget
{
public:
    /// CUIRingGauge を初期化する
    /// @param pBg pBg に渡す値
    /// @param pFill pFill に渡す値
    /// @param pos 座標
    /// @param size サイズ
    CUIRingGauge(CSpriteImage* pBg, CSpriteImage* pFill,
                 const VECTOR2& pos, const VECTOR4& size);

    /// Ratio を設定する
    /// @param ratio ratio に渡す値
    void SetRatio(float ratio);
    /// Anim を設定する
    /// @param anim anim に渡す値
    void SetAnim(const UIAnimationTween& anim);

    /// Arc Draw Params を設定する
    /// @param params params に渡す値
    void SetArcDrawParams(ArcDrawParams params);

    /// 毎フレームの状態を更新する
    void Update() override;

    /// 描画する
    /// @param sprite sprite に渡す値
    void Draw(CSprite& sprite) override;

private:
    VECTOR4 m_srcSize; // テクスチャのソース矩形 (srcX, srcY, srcW, srcH)
    CUIImage* m_pFill = nullptr; // ゲージ塗り部分の画像
    CUIImage* m_pBg = nullptr; // ゲージ背景の画像
    /// function<float を返す
    std::function<float(float)> m_easing = EaseOutQuint; // イージング関数
    ArcDrawParams m_arcParams; // DrawArcに渡す円弧描画パラメータ
    float m_maxWidth = 0.0f; // ゲージ満タン時の最大幅（ピクセル）
    float m_startRatio = 0.0f; // アニメーション開始時の割合（0〜1）
    float m_targetRatio = 1.0f; // アニメーション目標の割合（0〜1）
    float m_currentRatio = 0.0f; // 現在の表示割合（アニメーション中に補間）
    float m_moveSpeed = 0.5f; // ゲージの進む速さ
    float m_t = 0.0f; // アニメーション進行度（0=開始, 1=完了）
};
