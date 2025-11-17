#pragma once
#include "../05_CommonFile/Object3D.h"
class CGround : public Object3D
{
public:
    CGround();
    ~CGround();
    void Update();
    void Draw() override;
};
