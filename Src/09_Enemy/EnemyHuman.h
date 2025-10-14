#pragma once
#include "AnimalManager.h"
#include "../05_CommonFile/Object3D.h"

class CEnemyHuman : public CAnimalManager
{
public:
    CEnemyHuman();
    ~CEnemyHuman();

private:
    void Update() override;
    void Draw() override;
    void DrawDirectionLine();
    void FanShape();
};
