#pragma once
#include "../../Common/Object3D.h"
#include "ModelRegistry.h"
#include "QuadTreeIndex.h"

class CPlayer;
class CEnemyBase;
class CFbxMesh;

// 動物管理クラス（ファサード）
// ModelRegistryとQuadTreeIndexを統合して使用
class CAnimalManager : public Object3D
{
public:
    CAnimalManager(int time);
    ~CAnimalManager();

    void SetRotationY(const float& angle);

    // メッシュ取得（ModelRegistryに委譲）
    CFbxMesh* MeshList(const std::string& str) const;

    // 周辺エネミー取得（QuadTreeIndexに委譲）
    const std::vector<CEnemyBase*>& GetNearbyEnemies(
        CEnemyBase* pObj,
        const VECTOR2& pos,
        const VECTOR2& size) const;

    // 統計情報（QuadTreeIndexに委譲）
    const CollisionStats& GetCollisionStats() const;
    void ResetCollisionStats() const;

    // 各クラスへの直接アクセス（必要に応じて）
    CModelRegistry* GetModelRegistry() const { return m_pModelRegistry; }
    CQuadTreeIndex* GetQuadTreeIndex() const { return m_pQuadTreeIndex; }

public:
    CPlayer* m_pPlayer;

private:
    void Update() override;

    CModelRegistry* m_pModelRegistry;
    CQuadTreeIndex* m_pQuadTreeIndex;

    // 互換性のため残す（未使用）
    CFbxMesh* m_pMesh;
    class CMeshCollider* m_pMeshCol;
};
