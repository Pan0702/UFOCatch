#include "BackGraund.h"

BackGraund::BackGraund()
{
    mesh = new CFbxMesh();
    mesh->Load("data/CubeBoxSky.mesh");
}

