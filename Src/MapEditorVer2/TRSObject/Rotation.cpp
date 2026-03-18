#include "Rotation.h"


CRotation::CRotation()
{
    InitMeshes();
    SetDrawOrder(10);
    transform.scale = VECTOR3{2.0f, 2.0f, 2.0f};
}

// X/Y/Z霆ｸ縺ｮ蝗櫁ｻ｢繧ｮ繧ｺ繝｢繝｡繝・す繝･縺ｨ繧ｳ繝ｩ繧､繝繝ｼ繧貞・譛溷喧縺吶ｋ
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



