#pragma once
#include "../05_CommonFile/Object3D.h"

class CCubeBox : public Object3D
{
public:
    CCubeBox(const char* meshPath);
private:
    ~CCubeBox();
    void Draw() override;
};
