#include "AnimalFactor.h"

#include "../Dog/ACube.h"
#include "../Human/Human.h"

CAnimalFactor::CAnimalFactor()
{
    for (int i = 0; i < 10; i++)
    {
        static constexpr int MAX_POS = 20;
        static constexpr int MIX_POS = -20;
        float f1 = Randomf(MIX_POS,MAX_POS);
        float f2 = Randomf(MIX_POS,MAX_POS);
        new CACube(VECTOR3(0, 0, 0), VECTOR2(10, 10));
    }
    new CHuman(VECTOR3(0, 0, 0), VECTOR2(10, 10));
}

CAnimalFactor::~CAnimalFactor()
{
}