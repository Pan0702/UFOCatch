#pragma once
#include "UIAnimationTween.h"
#include "UIImage.h"
#include "../UI/UIPreset.h"

class CUIProgressBar : public CUIWidget
{
public:
    CUIProgressBar(CSpriteImage* pBg, CSpriteImage* pFill,
               const VECTOR2& pos, const VECTOR4& imageSize);
    CUIProgressBar(CSpriteImage* pBg, CSpriteImage* pFill,
           const VECTOR2& pos, const VECTOR2& bgImage,const VECTOR2& fillImage, const VECTOR2& imageSize);

    void SetRatio(float ratio);
    void SetAnim(const UIAnimationTween& anim);
    void SetIsEXPBar(bool isEXPBar);
    void Update() override;
private:
    void Progress();
    void Gauge();
private:
    CUIImage* m_pFill        = nullptr;                    // ゲージ塗り部分の画像
    CUIImage* m_pBg          = nullptr;                    // ゲージ背景の画像
    float     m_maxWidth     = 0.0f;                       // ゲージ満タン時の最大幅（ピクセル）
    float     m_startRatio   = 0.0f;                       // アニメーション開始時の割合（0〜1）
    float     m_targetRatio  = 1.0f;                       // アニメーション目標の割合（0〜1）
    float     m_currentRatio = 0.0f;                       // 現在の表示割合（アニメーション中に補間）
    float     m_moveSpeed    = 0.5f;                       //ゲージの進む速さ
    std::function<float(float)> m_easing = EaseOutQuint;   // イージング関数
    float     m_t            = 1.0f;                       // アニメーション進行度（0=開始, 1=完了）
    float     m_duration     = 0.5f;                       // アニメーション時間（秒）
    bool      m_isEXPBar     = false;                      // trueならEXPバーモード（オーバーフロー時に折り返し）
    VECTOR2 m_srcFillImage;
};
