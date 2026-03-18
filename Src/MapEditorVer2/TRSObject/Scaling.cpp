#include "Scaling.h"

CScaling::CScaling()
{
    InitMeshes();
    SetDrawOrder(10);
}

// X/Y/Z霆ｸ縺ｮ繧ｹ繧ｱ繝ｼ繝ｫ繧ｮ繧ｺ繝｢繝｡繝・す繝･縺ｨ繧ｳ繝ｩ繧､繝繝ｼ繧貞・譛溷喧縺吶ｋ
void CScaling::InitMeshes()
{
    auto InitMesh = [](Info& info, const char* path)
    {
        info.mesh = std::make_unique<CFbxMesh>();
        info.mesh->Load(path);
        info.coll = std::make_unique<MeshCollider>();
        info.coll->MakeFromMesh(info.mesh.get());
    };
    InitMesh(xInfo,"data/Editor/Scaling/X.mesh");
    InitMesh(yInfo,"data/Editor/Scaling/Y.mesh");
    InitMesh(zInfo,"data/Editor/Scaling/Z.mesh");
}


CScaling::~CScaling()
{
}


