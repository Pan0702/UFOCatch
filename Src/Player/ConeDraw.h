#pragma once
#include "Player.h"
#include "../Common/Object3D.h"
class CConeDraw : public Object3D
{
public:
    CConeDraw();
    ~CConeDraw();
private:
    void Update() override;
    void Draw() override;
    CPlayer* m_pPlayer;
};
