#include "AnimalFactor.h"

#include "../AnimalChicken/Chicken.h"
#include "../AnimalDog/Dog.h"
#include "../Human/Human.h"

///動物を生成
/// @param sizeX 生成する動物のXの範囲 
/// @param sizeZ 生成する動物のZの範囲 //
CAnimalFactor::CAnimalFactor(float sizeX, float sizeZ, int type)
{
    float f1;
    float f2;
    for (int i = 0; i < 40; i++)
    {
        f1 = Randomf(-sizeX, sizeX);
        f2 = Randomf(-sizeZ, sizeZ);
        new CADog(VECTOR3(f1, 0, f2), VECTOR2(sizeX, sizeZ));
    }
    f1 = Randomf(-sizeX, sizeX);
    f2 = Randomf(-sizeZ, sizeZ);
    new CHuman(VECTOR3(f1, 0, f2), VECTOR2(sizeX, sizeZ));
    if (type == 1)
    {
        float f1;
        float f2;
        for (int i = 0; i < 20; i++)
        {
            f1 = Randomf(-sizeX, sizeX);
            f2 = Randomf(-sizeZ, sizeZ);
            new CAnimalChicken(VECTOR3(f1, 0, f2), VECTOR2(sizeX, sizeZ));
        }
        for (int i = 0; i < 3; i++)
        {
            f1 = Randomf(-sizeX, sizeX);
            f2 = Randomf(-sizeZ, sizeZ);
            new CHuman(VECTOR3(f1, 0, f2), VECTOR2(sizeX, sizeZ));
        }
    }
}

CAnimalFactor::~CAnimalFactor() = default;

void CAnimalFactor::Normal(float sizeX, float sizeZ)
{

}
