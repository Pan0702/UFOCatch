#pragma once
#include "../Common/Object3D.h"
#include "../Utils/Sprite3D.h"
#include "../Utils/LerpValue.h"

// ゲーム中のUIを表示するクラス //
class CDisplayInfo : public Object3D
{
public:
    CDisplayInfo();
    ~CDisplayInfo();
private:

    void Update() override;
    void Draw() override;
    void GiwakuDraw();
    void ExpDraw();
    void TimeDraw();
    void HPDraw();
    void LvDraw();
    void CutIn(int num);

    CSprite* m_pSprite;
    CSpriteImage* m_playUIImage;
    CSpriteImage* m_expImage;
    CSpriteImage* m_giwakuImage;
    CSpriteImage* m_pLogo;

    // 経験値ゲージ用
    LerpValue m_xpWeightLerp;
    float m_prevProportion;
    float m_currentWidth;

    // 疑惑ゲージ用（円形）
    LerpValue m_giwakuAngleLerp;
    float m_prevGiwakuProportion;
    float m_currentAngle;
    
    int m_cutInCnt;
    bool m_isCutInDraw;
};