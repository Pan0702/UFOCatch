#pragma once
#include "../Common/Object3D.h"
#include "Player.h"
#include "PlayerLevel.h"

class CSpriteImage;

class CConeDraw : public Object3D
{
public:
    CConeDraw(float coneTopPos);
    ~CConeDraw() override;

private:
    void Update() override;
    void Draw()   override;

    void DrawSuctionCircle() const;

    CPlayer*      m_pPlayer;
    CPlayerLevel* m_pLevel;
    CSpriteImage* m_pCircleImage;
};