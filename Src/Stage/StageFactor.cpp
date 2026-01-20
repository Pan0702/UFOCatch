#include "StageFactor.h"

#include "CubeBox.h"
#include "Ground.h"
#include "StageObject.h"

CStageFactor::CStageFactor()
{
    new CGround("data/Ground/Prefabs/MapPlane001.mesh",VECTOR3(2.0f,2.0f,2.0f));
    new CCubeBox("data/Ground/CubeBoxSky.mesh");
    new CStageObject("data/Ground/Prefabs/Tree1a.mesh",VECTOR3(1.0f,0.0f,1.0f),2);
}

void CStageFactor::SpawnObjects()
{
    constexpr float SPAWN_RANGE_MIN = -20.0f;
    constexpr float SPAWN_RANGE_MAX = 20.0f;
    for (int i = 0; i < 10; ++i)
    {
       int  randomX = Randomf(SPAWN_RANGE_MIN, SPAWN_RANGE_MAX);
        int randomZ = Randomf(SPAWN_RANGE_MIN, SPAWN_RANGE_MAX);
        new CStageObject("data/Ground/Prefabs/Tree1a.mesh",VECTOR3(randomX,0.0f,randomZ),2);
    }
}
