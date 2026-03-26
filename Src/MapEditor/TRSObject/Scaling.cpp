#include "Scaling.h"

CScaling::CScaling()
{
    InitMeshes();
    SetDrawOrder(10);
}

// X/Y/Z軸のスケールギズモメッシュとコライダーを初期化する
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



