#include "StageFactor.h"

#include "CubeBox.h"
#include "Ground.h"

CStageFactor::CStageFactor()
{
    new CGround("data/Ground/Prefabs/MapPlane001.mesh",VECTOR3(2.0f,2.0f,2.0f));
    new CCubeBox("data/Ground/CubeBoxSky.mesh");
}
