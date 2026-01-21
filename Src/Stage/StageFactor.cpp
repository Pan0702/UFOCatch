#include "StageFactor.h"

#include "CubeBox.h"
#include "Ground.h"
#include "StageObject.h"

CStageFactor::CStageFactor()
{
    new CGround("data/Ground/Prefabs/MapPlane001.mesh",VECTOR3(3.0f,3.0f,3.0f));
    new CCubeBox("data/Ground/CubeBoxSky.mesh");
    new CStageObject("data/Ground/Prefabs/Tree1a.mesh",VECTOR3(1.0f,0.0f,1.0f),2);
}

void CStageFactor::SpawnObjects(float sizeX,float sizez)
{
    for (int i = 0; i < 30; ++i)
    {
       int  randomX = Randomf(-sizeX, sizeX);
        int randomZ = Randomf(-sizez, sizez);
        new CStageObject("data/Ground/Prefabs/Tree1a.mesh",VECTOR3(randomX,0.0f,randomZ),2);
    }
}
