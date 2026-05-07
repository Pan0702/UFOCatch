#pragma once
#include "../Common/Object3D.h"
#include "Player.h"
#include "PlayerLevel.h"

class CSpriteImage;

class CConeDraw : public Object3D
{
public:
    CConeDraw(float coneTopPos);
    CConeDraw(float coneTopPos, CPlayer* p);
    ~CConeDraw() override;

private:
    void Start() override;
    void Update() override;
    void Draw() override;

    CPlayer* m_pPlayer;
    CPlayerLevel* m_pLevel;
};

class CCircleDraw : public Object3D
{
public:
    CCircleDraw();
    CCircleDraw(CPlayer* p);
    ~CCircleDraw() override;

private:
    void Start() override;
    void Update() override;
    void Draw() override;

    CPlayer* m_pPlayer;
    std::unique_ptr<CSpriteImage> m_pCircleImage;
    CPlayerLevel* m_pLevel;

    float m_radius;
};
