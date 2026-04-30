#pragma once
#include "../../Common/Object3D.h"
#include "ModelRegistry.h"
#include "EnemyQuadTree.h"
#include "../../Stage/StageQuadTree.h"
#include "../../Stage/GroundHitResult.h"
#include <vector>

class CPlayer;
class CEnemyBase;
class CStageObject;
class CFbxMesh;

// 敵管理クラス（ファサード）
// ModelRegistryとQuadTreeIndexを統合して使用
class CEnemyManager : public Object3D
{
public:
    CEnemyManager();
    ~CEnemyManager();

    void SetRotationY(const float& angle);

    // メッシュ取得：ModelRegistryに委譲
    CFbxMesh* MeshList(const std::string& str) const;

    // 敵リスト管理：EnemyBaseの生成/破棄時に自動呼び出し
    void RegisterEnemy(CEnemyBase* enemy);
    void UnregisterEnemy(CEnemyBase* enemy);
    const std::vector<CEnemyBase*>& GetAllEnemies() const { return m_enemies; }

    // 周辺エネミー取得：QuadTreeIndexに委譲
    std::vector<CEnemyBase*> GetNearbyEnemies(
        CEnemyBase* pObj, const VECTOR2& pos, const VECTOR2& size) const;

    // 統計情報：QuadTreeIndexに委譲
    const CollisionStats& GetCollisionStats() const;
    void ResetCollisionStats() const;

    // 静的ツリーをシーン初期化後に1回構築する
    void BuildStaticTree() const;

    // 近くの静的なステージオブジェクトを取得：StaticQuadTreeIndexに委譲
    std::vector<CStageObject*> GetNearbyStageObjects(
        const VECTOR2& pos, const VECTOR2& size) const;

    bool FindGroundBelow(const VECTOR2& pos, const VECTOR2& size
                         , const VECTOR3& basePos, float from, float to, GroundHitResult* groundType) const;
    // 各クラスへの直接アクセス（必要に応じて）
    CModelRegistry* GetModelRegistry() const { return m_pModelRegistry; }
    CEnemyQuadTree* GetQuadTreeIndex() const { return m_pQuadTreeIndex; }
    CStageQuadTree* GetStaticQuadTreeIndex() const { return m_pStaticQuadTreeIndex; }

public:
    CPlayer* m_pPlayer;

private:
    void Update() override;

    CModelRegistry* m_pModelRegistry;
    CEnemyQuadTree* m_pQuadTreeIndex;
    CStageQuadTree* m_pStaticQuadTreeIndex;
    std::vector<CEnemyBase*> m_enemies;

    // 互換性のために残す（未使用）
    CFbxMesh* m_pMesh;
    class CMeshCollider* m_pMeshCol;
};
