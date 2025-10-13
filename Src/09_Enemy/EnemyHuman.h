#pragma once
#include "../05_CommonFile/Object3D.h"

class CEnemyHuman : public Object3D
{
public:
    CEnemyHuman();
    ~CEnemyHuman();

private:
    void Update() override;
};
