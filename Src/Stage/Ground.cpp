#include "Ground.h"
#define NOMINMAX
#include <Windows.h>
#include "../Utils/CsvReader.h"

////////////////////
// .mesh縺ｮPath縺ｨmesh縺ｮ螟ｧ縺阪＆繧呈ｸ｡縺・
// 蛻晄悄迥ｶ諷九・10ﾃ・0縲∽ｸｭ蠢・せ縺ｯ5,0,5
// @param mesh 繝｡繝・す繝･繝輔ぃ繧､繝ｫ縺ｮ繝代せ
// @param scale 繝｡繝・す繝･縺ｮ繧ｹ繧ｱ繝ｼ繝ｫ //
////////////////////
CGround::CGround(const char* mesh,const VECTOR3& scale)
{
    m_pMesh = new CFbxMesh();
    m_pMeshCol = new MeshCollider();
   // Init(filePath);   
    m_pMesh->Load(mesh);
    m_pMeshCol->MakeFromMesh(m_pMesh);
    transform.position = VECTOR3(0, 0, 0);
   transform.scale = scale;
    constexpr int drawOrder = 100;
    SetDrawOrder(drawOrder);
}
// bool CGround::Init(const char* filePath)
// {
//     CsvReader stage(filePath);
//     const std::string stageFilePath = stage.GetString(0,0);
// }

CGround::~CGround() = default;



