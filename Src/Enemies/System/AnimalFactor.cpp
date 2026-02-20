#include "AnimalFactor.h"

#include "../../Stage/StageFactor.h"
#include "../AnimalDog/Dog.h"
#include "../Human/Human.h"

///動物を生成
/// @param sizeX 生成する動物のXの範囲 
/// @param sizeZ 生成する動物のZの範囲 //
CAnimalFactor::CAnimalFactor(float sizeX,float sizeZ)
{
    float f1 ;
    float f2;
    ObjectManager::FindGameObject<CStageFactor>()->SpawnObjects(sizeX, sizeZ);
    for (int i = 0; i < 50; i++)
    {
         f1 = Randomf(-sizeX,sizeX);
         f2 = Randomf(-sizeZ,sizeZ);
        new CADog(VECTOR3(f1, 0, f2), VECTOR2(sizeX, sizeZ));
    }
    f1 = Randomf(-sizeX,sizeX);
    f2 = Randomf(-sizeZ,sizeZ);
    new CHuman(VECTOR3(f1, 0, f2), VECTOR2(sizeX, sizeZ));
}

CAnimalFactor::~CAnimalFactor() = default; 