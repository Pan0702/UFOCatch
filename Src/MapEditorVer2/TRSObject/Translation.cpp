#include "Translation.h"


CTranslation::CTranslation()
{
    InitMeshes();
    transform.scale = VECTOR3(5,5,5);
    SetDrawOrder(10);
}
// X/Y/Z軸の移動ギズモメッシュとコライダーを初期化する
void CTranslation::InitMeshes()
{
    auto InitMesh = [](Info& info, const char* path)
    {
        info.mesh_ = std::make_unique<CFbxMesh>();
        info.mesh_->Load(path);
        info.coll_ = std::make_unique<MeshCollider>();
        info.coll_->MakeFromMesh(info.mesh_.get());
    };
    InitMesh(x_info_,"data/Editor/Translate/X.mesh");
    InitMesh(y_info_,"data/Editor/Translate/Y.mesh");
    InitMesh(z_info_,"data/Editor/Translate/Z.mesh");
}


