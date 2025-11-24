#pragma once
#include "../05_CommonFile/Object3D.h"

class CPlayerHP : public Object3D
{
public:
    CPlayerHP(const int& hp);
    ~CPlayerHP();
    void SubHP(const int& hp){m_currentHp -= hp;}

private:
    void Update() override;
    void Draw() override;

    int m_currentHp;
    int m_maxHp;
    CSprite* m_pSprite;
    CSpriteImage* m_pHpImage;
    CSpriteImage* m_pHpDamageImage;
    VECTOR2 m_hpPos;
    VECTOR2 m_imageSize;
    VECTOR2 m_drawImageSize;
    int m_k;

};
