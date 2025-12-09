#include "AnimalManager.h"

#include "ACube.h"
#include "../../06_GameLib/BBox.h""
#include "../../08_Player/Player.h"
#include <thread>

#include "Human.h"

namespace
{
    constexpr float HALF_ROTATION_DEG = 180.0f;
    constexpr float FULL_ROTATION_DEG = 360.0f;
}
CAnimalManager::CAnimalManager()
{
    ObjectManager::DontDestroy(this);		            // 自体は消されない
    ObjectManager::SetVisible(this, false);		// 自体は表示しない
    
    meshstruct ms = {};
    m_meshList.push_back(ms);
    m_meshList.back().name = "Dog";
    m_meshList.back().mesh = new CFbxMesh();
    m_meshList.back().mesh->Load("data/NewAnimal/Dog/Dog.mesh");;
    m_meshList.back().mesh->LoadAnimation(A_IDEL, "data/NewAnimal/Dog/Dog_Idle.anmx", false);
    m_meshList.back().mesh->LoadAnimation(A_RUN, "data/NewAnimal/Dog/Dog_Walk.anmx", true);
    m_meshList.back().mesh->LoadAnimation(A_WALK, "data/NewAnimal/Dog/Dog_Idle.anmx", true);
    
    m_meshList.push_back(ms);
    m_meshList.back().name = "Human";
    m_meshList.back().mesh = new CFbxMesh();
    m_meshList.back().mesh->Load("data/NewAnimal/Human/Human.mesh");
    m_meshList.back().mesh->LoadAnimation(A_IDEL, "data/NewAnimal/Human/Human_Idle.anmx", false);
    m_meshList.back().mesh->LoadAnimation(A_WALK, "data/NewAnimal/Human/Human_Walk.anmx", true);
    m_meshList.back().mesh->LoadAnimation(A_SEACH, "data/NewAnimal/Human/Human_Find.anmx", false);
    
    
} 



CAnimalManager::~CAnimalManager()
{
    for (meshstruct &ms : m_meshList)
    {
        SAFE_DELETE(ms.mesh);
    }
}

void CAnimalManager::Update()
{

}




VECTOR3 CAnimalManager::GetObjectSize(MeshCollider* meshColl) const
{
    return meshColl->bBox.max;
}
void CAnimalManager::SetRotationY(const float& angle)
{
    float degAngle = angle * RadToDeg;

    while (degAngle > HALF_ROTATION_DEG)
    {
        degAngle -= FULL_ROTATION_DEG;
    }
    while (degAngle < -HALF_ROTATION_DEG)
    {
        degAngle += FULL_ROTATION_DEG;
    }

    transform.rotation.y = degAngle * DegToRad;
}

CFbxMesh* CAnimalManager::MeshList(const std::string& str)
{
    for (meshstruct& ms : m_meshList)
    {
        if (str == ms.name) return ms.mesh;
    }
    MessageBox(nullptr, "EnemyManager::MeshList()", _T("■□■ 指定のメッシュ名のメッシュはメッシュリストにありません ■□■"), MB_OK);
    return nullptr;
}


