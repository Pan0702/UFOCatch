#pragma once
#include "../05_CommonFile/Object3D.h"
#include "../06_GameLib/Sprite3D.h"
#include "../06_GameLib/LerpValue.h"

// ゲーム中のUIを表示するクラス //
class CDisplayInfo : public Object3D
{
public:
    CDisplayInfo();

private:
    ~CDisplayInfo();
    void Update() override;
    void Draw() override;
    void GiwakuDraw();
    void ExpDraw();
    void TimeDraw();
    void HPDraw();

    CSprite* m_pSprite;
    CSpriteImage* m_playUIImage;
    CSpriteImage* m_expImage;
    CSpriteImage* m_giwakuImage;

    // 経験値ゲージ用
    LerpValue m_xpWeightLerp;
    float m_prevProportion;
    float m_currentWidth;

    // 疑惑ゲージ用（円形）
    LerpValue m_giwakuAngleLerp;
    float m_prevGiwakuProportion;
    float m_currentAngle;
    
    float tmp;
    float tmp2;
    VECTOR2 tmp3;
};