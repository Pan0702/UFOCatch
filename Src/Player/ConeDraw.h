#pragma once
#include "Player.h"
#include "PlayerLevel.h"
#include "../Common/Object3D.h"

class CConeDraw : public Object3D
{
public:
    CConeDraw(float coneTopPos);
    ~CConeDraw();
private:
    void Update() override;
    void Draw()   override;

    void DrawSuctionCircle() const;
    void DrawDebugCone()     const;

    CPlayer*      m_pPlayer;
    CPlayerLevel* m_pLevel;
    CSpriteImage* m_pCircleImage;
};