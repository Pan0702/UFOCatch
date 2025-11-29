#pragma once
#include "../05_CommonFile/Object3D.h"
class CGround : public Object3D
{
public:
    CGround();
private:
    ~CGround();
    void Update();
    void Draw() override;
    VECTOR3 max;
    VECTOR3 min;
};
