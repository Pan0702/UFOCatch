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
        info.mesh_ = std::make_unique<CFbxMesh>();
        info.mesh_->Load(path);
        info.coll_ = std::make_unique<MeshCollider>();
        info.coll_->MakeFromMesh(info.mesh_.get());
    };
    InitMesh(x_info_,"data/Editor/Scaling/X.mesh");
    InitMesh(y_info_,"data/Editor/Scaling/Y.mesh");
    InitMesh(z_info_,"data/Editor/Scaling/Z.mesh");
}


CScaling::~CScaling()
{
}

