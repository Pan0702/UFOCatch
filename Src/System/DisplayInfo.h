#pragma once
#include "../Common/Object3D.h"
#include "../Utils/Sprite3D.h"
#include "../Utils/LerpValue.h"

// 繧ｲ繝ｼ繝荳ｭ縺ｮUI繧定｡ｨ遉ｺ縺吶ｋ繧ｯ繝ｩ繧ｹ //
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

    // 邨碁ｨ灘､繧ｲ繝ｼ繧ｸ逕ｨ
    LerpValue m_xpWeightLerp;
    float m_prevProportion;
    float m_currentWidth;

    // 逍第ヱ繧ｲ繝ｼ繧ｸ逕ｨ・亥・蠖｢・・
    LerpValue m_giwakuAngleLerp;
    float m_prevGiwakuProportion;
    float m_currentAngle;
    
    int m_cutInCnt;
    bool m_isCutInDraw;
};