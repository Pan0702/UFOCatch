#include "Ground.h"
#define NOMINMAX
#include <Windows.h>
#include "../06_GameLib/CsvReader.h"

CGround::CGround(const char* mesh,const VECTOR3& scale)
{
    m_pMesh = new CFbxMesh();
    m_pMeshCol = new MeshCollider();
   // Init(filePath);
    m_pMesh->Load(mesh);
    m_pMeshCol->MakeFromMesh(m_pMesh);
    transform.position = VECTOR3(0, 0, 0);
   transform.scale = scale;

}
// bool CGround::Init(const char* filePath)
// {
//     CsvReader stage(filePath);
//     const std::string stageFilePath = stage.GetString(0,0);
// }

CGround::~CGround() = default;


