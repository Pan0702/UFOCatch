#include "EnemyManager.h"
#include "../../Utils/BBox.h"
#include "../AnimalDog/Dog.h"
#include "../HUman/Human.h"
#include "../../Core/Graphics/XAudio.h"
#include "../../Stage/StageObject.h"

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
    , m_pStaticQuadTreeIndex(nullptr)
    , m_pPlayer(nullptr)
{
    ObjectManager::DontDestroy(this);
    ObjectManager::SetVisible(this, false);

    // 各クラスを初期化
    m_pModelRegistry = new CModelRegistry();
    m_pQuadTreeIndex = new CEnemyQuadTree();
    m_pStaticQuadTreeIndex = new CStageQuadTree();
}

CEnemyManager::~CEnemyManager()
{
    SAFE_DELETE(m_pModelRegistry);
    m_pModelRegistry = nullptr;
    SAFE_DELETE(m_pQuadTreeIndex);
    m_pQuadTreeIndex = nullptr;
    SAFE_DELETE(m_pStaticQuadTreeIndex);
    m_pStaticQuadTreeIndex = nullptr;
}

void CEnemyManager::RegisterEnemy(CEnemyBase* enemy)
{
    m_enemies.push_back(enemy);
}

void CEnemyManager::UnregisterEnemy(CEnemyBase* enemy)
{
    // swap-and-pop で O(1) 削除
    for (size_t i = 0; i < m_enemies.size(); ++i)
    {
        if (m_enemies[i] == enemy)
        {
            m_enemies[i] = m_enemies.back();
            m_enemies.pop_back();
            return;
        }
    }
}

void CEnemyManager::Update()
{
    // QuadTreeIndexを更新（管理リストを渡す）
    if (m_pQuadTreeIndex)
    {
        m_pQuadTreeIndex->Update(m_enemies);
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

void CEnemyManager::BuildStaticTree()
{
    if (m_pStaticQuadTreeIndex)
    {
        m_pStaticQuadTreeIndex->Build();
    }
}

std::vector<CStageObject*> CEnemyManager::GetNearbyStageObjects(
    const VECTOR2& pos, const VECTOR2& size) const
{
    if (m_pStaticQuadTreeIndex)
    {
        return m_pStaticQuadTreeIndex->GetNearbyObjects(pos, size);
    }
    return {};
}
