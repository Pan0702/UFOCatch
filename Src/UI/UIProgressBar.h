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
    /// @brief シンプルなコンストラクタ（bg/fill共通サイズ）
    CUIProgressBar(CSpriteImage* pBg, CSpriteImage* pFill,
               const VECTOR2& pos, const VECTOR4& imageSize);

    /// @brief bgとfillのテクスチャ座標を個別に指定するコンストラクタ
    /// @param bgImage   背景テクスチャのUV原点 (srcX, srcY)
    /// @param fillImage ゲージテクスチャのUV原点 (srcX, srcY)
    /// @param imageSize ゲージ全体のサイズ (width, height)
    CUIProgressBar(CSpriteImage* pBg, CSpriteImage* pFill,
           const VECTOR2& pos, const VECTOR2& bgImage, const VECTOR2& fillImage, const VECTOR2& imageSize);

    /// @brief ゲージの目標割合を設定してアニメーションを開始する
    /// @param ratio 増加量（0〜1）。EXPバーモード時は1.0超えで折り返しが発生する
    void SetRatio(float ratio);
    void SetAnim(const UIAnimationTween& anim);

    /// @brief EXPバーモードを設定する
    /// @details trueにすると100%到達時にゲージをリセットして残り分から再アニメーションする
    void SetIsEXPBar(bool isEXPBar);
    void Update() override;

private:
    /// @brief EXPバーモード時の更新処理（100%超えで折り返してアニメーション継続）
    void Progress();
    /// @brief 通常ゲージの更新処理
    void Gauge();

private:
    CUIImage* m_pFill        = nullptr;                    // ゲージ塗り部分の画像
    CUIImage* m_pBg          = nullptr;                    // ゲージ背景の画像
    float     m_maxWidth     = 0.0f;                       // ゲージ満タン時の最大幅（ピクセル）
    float     m_startRatio   = 0.0f;                       // アニメーション開始時の割合（0〜1）
    float     m_targetRatio  = 1.0f;                       // アニメーション目標の割合（0〜1）
    float     m_currentRatio = 0.0f;                       // 現在の表示割合（アニメーション中に補間）
    float     m_moveSpeed    = 0.5f;                       // ゲージの進む速さ
    std::function<float(float)> m_easing = EaseOutQuint;   // イージング関数
    float     m_t            = 1.0f;                       // アニメーション進行度（0=開始, 1=完了）
    float     m_duration     = 0.5f;                       // アニメーション時間（秒）
    bool      m_isEXPBar     = false;                      // trueならEXPバーモード（オーバーフロー時に折り返し）
    VECTOR2   m_srcFillImage;                              // fillテクスチャのUV原点 (srcX, srcY)
};
