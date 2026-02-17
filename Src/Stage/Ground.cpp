#include "Ground.h"
#define NOMINMAX
#include <Windows.h>
#include "../Utils/CsvReader.h"

////////////////////
// .meshのPathとmeshの大きさを渡す
// 初期状態は10×10、中心点は5,0,5
// @param mesh メッシュファイルのパス
// @param scale メッシュのスケール //
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


