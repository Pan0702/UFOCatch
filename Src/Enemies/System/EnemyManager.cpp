#include "EnemyManager.h"
#include "../../Utils/BBox.h"
#include "../AnimalDog/Dog.h"
#include "../HUman/Human.h"
#include "../../Core/Graphics/XAudio.h"

namespace
{
    constexpr float HALF_ROTATION_DEG = 180.0f;
    constexpr float FULL_ROTATION_DEG = 360.0f;
}

CEnemyManager::CEnemyManager()
    : m_pMesh(nullptr)
    , m_pMeshCol(nullptr)
    , m_pModelRegistry(nullptr)
    , m_pQuadTreeIndex(nullptr)
    , m_pPlayer(nullptr)
{
    ObjectManager::DontDestroy(this);
    ObjectManager::SetVisible(this, false);

    // 各クラスを初期化
    m_pModelRegistry = new CModelRegistry();
    m_pQuadTreeIndex = new CQuadTreeIndex();
}

CEnemyManager::~CEnemyManager()
{
    SAFE_DELETE(m_pModelRegistry);
    m_pModelRegistry = nullptr;
    SAFE_DELETE(m_pQuadTreeIndex);
    m_pQuadTreeIndex = nullptr;
}

void CEnemyManager::Update()
{
    // QuadTreeIndexを更新
    if (m_pQuadTreeIndex)
    {
        m_pQuadTreeIndex->Update();
    }
}

CFbxMesh* CEnemyManager::MeshList(const std::string& str) const
{
    if (m_pModelRegistry)
    {
        return m_pModelRegistry->GetMesh(str);
    }
    return nullptr;
}

void CEnemyManager::SetRotationY(const float& angle)
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

std::vector<CEnemyBase*> CEnemyManager::GetNearbyEnemies(
    CEnemyBase* pObj,const VECTOR2& pos,const VECTOR2& size) const
{
    if (m_pQuadTreeIndex)
    {
        return m_pQuadTreeIndex->GetNearbyEnemies(pObj, pos, size);
    }
    return std::vector<CEnemyBase*>();
}

const CollisionStats& CEnemyManager::GetCollisionStats() const
{
    static CollisionStats emptyStats;

    if (m_pQuadTreeIndex)
    {
        return m_pQuadTreeIndex->GetCollisionStats();
    }
    return emptyStats;
}

void CEnemyManager::ResetCollisionStats() const
{
    if (m_pQuadTreeIndex)
    {
        m_pQuadTreeIndex->ResetCollisionStats();
    }
}
