#pragma once
#include "UIAnimationTween.h"
#include "UIImage.h"
#include "UIWidget.h"
#include "../UI/UIPreset.h"

class CUIRingGauge : public CUIWidget
{
public:
    CUIRingGauge(CSpriteImage* pBg, CSpriteImage* pFill,
           const VECTOR2& pos, const VECTOR4& size);
    void SetRatio(float ratio);
    void SetAnim(const UIAnimationTween& anim);
    void SetArcDrawParams(ArcDrawParams params);
    void Update() override;
    void Draw(CSprite& sprite) override;
private:
    CUIImage* m_pFill        = nullptr;                    // ゲージ塗り部分の画像
    CUIImage* m_pBg          = nullptr;                    // ゲージ背景の画像
    std::function<float(float)> m_easing = EaseOutQuint;   // イージング関数
    ArcDrawParams  m_arcParams;
    float     m_maxWidth     = 0.0f;                       // ゲージ満タン時の最大幅（ピクセル）
    float     m_startRatio   = 0.0f;                       // アニメーション開始時の割合（0〜1）
    float     m_targetRatio  = 1.0f;                       // アニメーション目標の割合（0〜1）
    float     m_currentRatio = 0.0f;                       // 現在の表示割合（アニメーション中に補間）
    float     m_moveSpeed    = 0.5f;                       //ゲージの進む速さ
    float     m_t             = 0.0f;
    
};
