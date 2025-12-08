#include "AnimalFactor.h"

#include "../Dog/ACube.h"
#include "../Human/Human.h"

CAnimalFactor::CAnimalFactor()
{
    for (int i = 0; i < 10; i++)
    {
        new CACube(VECTOR3(0, 0, 0), VECTOR2(10, 10));
    }
    new CHuman(VECTOR3(0, 0, 0), VECTOR2(10, 10));
}

CAnimalFactor::~CAnimalFactor()
{
}