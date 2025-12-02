#pragma once
#include "../05_CommonFile/Object3D.h"
#include "../06_GameLib/Sprite3D.h"
#include "../06_GameLib/LerpValue.h"

class CDisplayInfo : public Object3D
{
public:
    CDisplayInfo();
    
private:
    ~CDisplayInfo();
    void Update() override;
    void Draw() override;
    void GiwakuDraw() const;
    void ExpDraw();
    
    CSprite* m_sprite;
    CSpriteImage* m_playUIImage;
    CSpriteImage* m_expImage;

    LerpValue m_xpWeightLerp;
    float m_prevProportion;
    float m_currentWidth;
    
};