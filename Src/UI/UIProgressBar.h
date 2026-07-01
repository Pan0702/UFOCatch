#pragma once
#include "UIAnimationTween.h"
#include "UIImage.h"
#include "../UI/UIPreset.h"

/// @brief 横伸びゲージウィジェット
/// @details SetRatio()で目標割合を設定すると、イージングアニメーションで現在値へ補間して描画する。
///          通常ゲージモードとEXPバーモード（100%超えで折り返し）の2つの動作を持つ。
class CUIProgressBar : public CUIWidget
{
public:
    /// CUIProgressBar を初期化する
    /// @param pBg pBg に渡す値
    /// @param pFill pFill に渡す値
    /// @param pos 座標
    /// @param imageSize サイズ
    CUIProgressBar(CSpriteImage* pBg, CSpriteImage* pFill,
                   const VECTOR2& pos, const VECTOR4& imageSize);

    /// CUIProgressBar を初期化する
    /// @param pBg pBg に渡す値
    /// @param pFill pFill に渡す値
    /// @param pos 座標
    /// @param bgImage bgImage に渡す値
    /// @param fillImage fillImage に渡す値
    /// @param imageSize サイズ
    CUIProgressBar(CSpriteImage* pBg, CSpriteImage* pFill,
                   const VECTOR2& pos, const VECTOR2& bgImage, const VECTOR2& fillImage, const VECTOR2& imageSize);

    /// Ratio を設定する
    /// @param ratio ratio に渡す値
    void SetRatio(float ratio);
    /// Anim を設定する
    /// @param anim anim に渡す値
    void SetAnim(const UIAnimationTween& anim);

    /// Is EXPBar を設定する
    /// @param isEXPBar isEXPBar に渡す値
    void SetIsEXPBar(bool isEXPBar);
    /// 毎フレームの状態を更新する
    void Update() override;

private:
    /// Progress の処理を行う
    void Progress();
    /// Gauge の処理を行う
    void Gauge();

private:
    CUIImage* m_pFill = nullptr; // ゲージ塗り部分の画像
    CUIImage* m_pBg = nullptr; // ゲージ背景の画像
    float m_maxWidth = 0.0f; // ゲージ満タン時の最大幅（ピクセル）
    float m_startRatio = 0.0f; // アニメーション開始時の割合（0〜1）
    float m_targetRatio = 1.0f; // アニメーション目標の割合（0〜1）
    float m_currentRatio = 0.0f; // 現在の表示割合（アニメーション中に補間）
    float m_moveSpeed = 0.5f; // ゲージの進む速さ
    /// function<float を返す
    std::function<float(float)> m_easing = EaseOutQuint; // イージング関数
    float m_t = 1.0f; // アニメーション進行度（0=開始, 1=完了）
    float m_duration = 0.5f; // アニメーション時間（秒）
    bool m_isEXPBar = false; // trueならEXPバーモード（オーバーフロー時に折り返し）
    VECTOR2 m_srcFillImage; // fillテクスチャのUV原点 (srcX, srcY)
};
