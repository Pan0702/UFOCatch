#include "AnimalManager.h"

#include "../../06_GameLib/BBox.h""
#include "../../08_Player/Player.h"
#include <thread>
#include "../Dog/ACube.h"
#include "../HUman/Human.h"

namespace
{
    constexpr float HALF_ROTATION_DEG = 180.0f;
    constexpr float FULL_ROTATION_DEG = 360.0f;
}

CAnimalManager::CAnimalManager()
{
    ObjectManager::DontDestroy(this); // 削除しない
    ObjectManager::SetVisible(this, false); // 表示しない

    m_pMesh = nullptr;
    m_pMeshCol = nullptr;

    meshstruct ms = {};
    m_meshList.push_back(ms);
    m_meshList.back().name = "Dog";
    m_meshList.back().mesh = new CFbxMesh();
    m_meshList.back().mesh->Load("data/NewAnimal/Dog/Dog.mesh");
    m_meshList.back().mesh->LoadAnimation(A_IDEL, "data/NewAnimal/Dog/Dog_Idle.anmx", false);
    m_meshList.back().mesh->LoadAnimation(A_RUN, "data/NewAnimal/Dog/Dog_Walk.anmx", true);
    m_meshList.back().mesh->LoadAnimation(A_WALK, "data/NewAnimal/Dog/Dog_Walk.anmx", true);
    m_meshList.push_back(ms);
    m_meshList.back().name = "Human";
    m_meshList.back().mesh = new CFbxMesh();
    m_meshList.back().mesh->Load("data/NewAnimal/Human/Human.mesh");
    m_meshList.back().mesh->LoadAnimation(A_IDEL, "data/NewAnimal/Human/Human_Idle.anmx", false);
    m_meshList.back().mesh->LoadAnimation(A_WALK, "data/NewAnimal/Human/Human_Walk.anmx", true);
    m_meshList.back().mesh->LoadAnimation(A_SEACH, "data/NewAnimal/Human/Human_Find.anmx", false);

    // 4分木の初期化（レベル3、範囲-20〜20）
    m_pTree = new CLiner4Tree<CEnemyBase>(3, VECTOR4(-20, -20, 20, 20));
}


CAnimalManager::~CAnimalManager()
{
    for (meshstruct& ms : m_meshList)
    {
        SAFE_DELETE(ms.mesh);
    }
    SAFE_DELETE(m_pTree);
}

void CAnimalManager::Update()
{
    m_pTree->AllClear();
    std::list<CEnemyBase*> enemyes = ObjectManager::FindGameObjects<CEnemyBase>();
    for (auto enemy : enemyes)
    {
        VECTOR2 pos, size;
        if (enemy->GetBounds2D(pos, size))
        {
            m_pTree->Register(enemy, pos, size);
        }
    }
}


CFbxMesh* CAnimalManager::MeshList(const std::string& str)
{
    for (meshstruct& ms : m_meshList)
    {
        if (str == ms.name) return ms.mesh;
    }
    MessageBox(nullptr, "EnemyManager::MeshList()", _T("エラー 指定のメッシュ名のメッシュはメッシュリストにありません エラー"),
               MB_OK);
    return nullptr;
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

std::vector<CEnemyBase*> CAnimalManager::GetNearbyEnemies(CEnemyBase* pObj, const VECTOR2& pos, const VECTOR2& size) const
{
    if (m_pTree == nullptr)
    {
        return std::vector<CEnemyBase*>();
    }
    return m_pTree->GetObjects(pObj, pos, size);
}
