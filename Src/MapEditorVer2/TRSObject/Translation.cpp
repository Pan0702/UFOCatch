#include "Translation.h"


CTranslation::CTranslation()
{
    InitMeshes();
    transform.scale = VECTOR3(5,5,5);
    SetDrawOrder(10);
}
// X/Y/Z霆ｸ縺ｮ遘ｻ蜍輔ぐ繧ｺ繝｢繝｡繝・す繝･縺ｨ繧ｳ繝ｩ繧､繝繝ｼ繧貞・譛溷喧縺吶ｋ
void CTranslation::InitMeshes()
{
    auto InitMesh = [](Info& info, const char* path)
    {
        info.mesh = std::make_unique<CFbxMesh>();
        info.mesh->Load(path);
        info.coll = std::make_unique<MeshCollider>();
        info.coll->MakeFromMesh(info.mesh.get());
    };
    InitMesh(xInfo,"data/Editor/Translate/X.mesh");
    InitMesh(yInfo,"data/Editor/Translate/Y.mesh");
    InitMesh(zInfo,"data/Editor/Translate/Z.mesh");
}



