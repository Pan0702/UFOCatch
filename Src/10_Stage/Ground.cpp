#include "Ground.h"
#define NOMINMAX
#include <Windows.h>
#include "../06_GameLib/CsvReader.h"

CGround::CGround(const char* mesh)
{
    m_pMesh = new CFbxMesh();
    m_pMeshCol = new MeshCollider();
   // Init(filePath);
    m_pMesh->Load(mesh);
    m_pMeshCol->MakeFromMesh(m_pMesh);
    transform.position = VECTOR3(0, 0, 0);
   // transform.scale = VECTOR3(1000, 10, 1000);

}
// bool CGround::Init(const char* filePath)
// {
//     CsvReader stage(filePath);
//     const std::string stageFilePath = stage.GetString(0,0);
// }

CGround::~CGround() = default;

void CGround::Update()
{
    Object3D::Update();
}
void CGround::Draw()
{
    m_pMesh->Render(m_pAnimator, transform.matrix());
}

