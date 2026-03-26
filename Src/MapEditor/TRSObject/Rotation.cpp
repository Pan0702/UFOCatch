#include "Rotation.h"


CRotation::CRotation()
{
    InitMeshes();
    SetDrawOrder(10);
    transform.scale = VECTOR3{2.0f, 2.0f, 2.0f};
}

// X/Y/Z軸の回転ギズモメッシュとコライダーを初期化する
void CRotation::InitMeshes()
{
    auto InitMesh = [](Info& info, const char* path)
    {
        info.mesh = std::make_unique<CFbxMesh>();
        info.mesh->Load(path);
        info.coll = std::make_unique<MeshCollider>();
        info.coll->MakeFromMesh(info.mesh.get());
    };
    InitMesh(xInfo,"data/Editor/Rotation/X.mesh");
    InitMesh(yInfo,"data/Editor/Rotation/Y.mesh");
    InitMesh(zInfo,"data/Editor/Rotation/Z.mesh");
}


CRotation::~CRotation() = default;



