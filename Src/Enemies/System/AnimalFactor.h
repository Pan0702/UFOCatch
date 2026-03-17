#pragma once
#include "../../Common/Object3D.h"

class CAnimalFactor : public Object3D
{
public:

    CAnimalFactor(float sizeX, float sizeZ, int type);
    ~CAnimalFactor();
private:
    static void Normal(float sizeX, float sizeZ);
};
