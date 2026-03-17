#include "StageFactor.h"

#include "CubeBox.h"
#include "Ground.h"
#include "StageObject.h"

CStageFactor::CStageFactor()
{
    Instantiate<CGround>("data/Ground/Prefabs/MapPlane001.mesh",VECTOR3(5.0f,3.0f,5.0f));
    Instantiate<CCubeBox>("data/Ground/CubeBoxSky.mesh");
   // new CStageObject("data/Ground/Prefabs/Tree1a.mesh",VECTOR3(1.0f,0.0f,1.0f),2);
    
}

void CStageFactor::SpawnObjects(float sizeX,float sizeZ,int num)
{
    for (int i = 0; i < num; ++i)
    {
       float randomX = Randomf(-sizeX, sizeX);
        float randomZ = Randomf(-sizeZ, sizeZ);
        Instantiate< CStageObject>("data/Ground/Prefabs/Tree1a.mesh",VECTOR3(randomX,0.0f,randomZ),2);
    }
}
