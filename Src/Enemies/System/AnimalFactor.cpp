#include "AnimalFactor.h"

#include "../AnimalChicken/Chicken.h"
#include "../AnimalDog/Dog.h"
#include "../Human/Human.h"

///蜍慕黄繧堤函謌・
/// @param sizeX 逕滓・縺吶ｋ蜍慕黄縺ｮX縺ｮ遽・峇 
/// @param sizeZ 逕滓・縺吶ｋ蜍慕黄縺ｮZ縺ｮ遽・峇 //
CAnimalFactor::CAnimalFactor(float sizeX, float sizeZ, int type)
{
    float f1;
    float f2;
    for (int i = 0; i < 60; i++)
    {
        f1 = Randomf(-sizeX, sizeX);
        f2 = Randomf(-sizeZ, sizeZ);
        Instantiate<CADog>(VECTOR3(f1, 0, f2), VECTOR2(sizeX, sizeZ));
    }
    f1 = Randomf(-sizeX, sizeX);
    f2 = Randomf(-sizeZ, sizeZ);
    Instantiate<CHuman>(VECTOR3(f1, 0, f2), VECTOR2(sizeX, sizeZ));
    if (type == 1)
    {
        float f1;
        float f2;
        for (int i = 0; i < 10; i++)
        {
            f1 = Randomf(-sizeX, sizeX);
            f2 = Randomf(-sizeZ, sizeZ);
            Instantiate<CAnimalChicken>(VECTOR3(f1, 0, f2), VECTOR2(sizeX, sizeZ));
        }
        for (int i = 0; i < 3; i++)
        {
            f1 = Randomf(-sizeX, sizeX);
            f2 = Randomf(-sizeZ, sizeZ);
            Instantiate<CHuman>(VECTOR3(f1, 0, f2), VECTOR2(sizeX, sizeZ));
        }
    }
}

CAnimalFactor::~CAnimalFactor() = default;

void CAnimalFactor::Normal(float sizeX, float sizeZ)
{
}

