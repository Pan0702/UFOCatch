#pragma once
#include "../../Common/Object3D.h"
#include "ModelRegistry.h"
#include "EnemyQuadTree.h"
#include "../../Stage/StageQuadTree.h"
#include "../../Stage/GroundHitResult.h"
#include <vector>

/// <summary>プレイヤーUFOの移動、吸引、HP、レベル処理をまとめるクラス</summary>
class CPlayer;
/// <summary>敵キャラクター共通の移動、衝突、状態制御を持つ基底クラス</summary>
class CEnemyBase;
/// <summary>敵AIで使う Stage Object の情報と処理をまとめる型</summary>
class CStageObject;
/// <summary>敵AIで使う Fbx Mesh の情報と処理をまとめる型</summary>
class CFbxMesh;

// 敵管理クラス（ファサード）
// ModelRegistryとQuadTreeIndexを統合して使用
class CEnemyManager : public Object3D
{
public:
    /// CEnemyManager を初期化する
    CEnemyManager();
    /// CEnemyManager の終了処理を行う
    ~CEnemyManager();

    /// Rotation Y を設定する
    /// @param angle 角度
    void SetRotationY(const float& angle);

    // メッシュ取得：ModelRegistryに委譲
    /// Mesh List を返す
    /// @param str str に渡す値
    /// @return 対象のポインタ
    CFbxMesh* MeshList(const std::string& str) const;

    // 敵リスト管理：EnemyBaseの生成/破棄時に自動呼び出し
    /// Register Enemy の処理を行う
    /// @param enemy enemy に渡す値
    void RegisterEnemy(CEnemyBase* enemy);
    /// Unregister Enemy の処理を行う
    /// @param enemy enemy に渡す値
    void UnregisterEnemy(CEnemyBase* enemy);
    /// All Enemies を取得する
    /// @return 取得した要素一覧
    const std::vector<CEnemyBase*>& GetAllEnemies() const { return m_enemies; }

    // 周辺エネミー取得：QuadTreeIndexに委譲
    /// Nearby Enemies を取得する
    /// @param pObj 対象オブジェクト
    /// @param pos 座標
    /// @param size サイズ
    /// @return 取得した要素一覧
    std::vector<CEnemyBase*> GetNearbyEnemies(
        CEnemyBase* pObj, const VECTOR2& pos, const VECTOR2& size) const;

    // 統計情報：QuadTreeIndexに委譲
    /// Collision Stats を取得する
    /// @return 処理結果
    const CollisionStats& GetCollisionStats() const;
    /// Collision Stats を初期状態に戻す
    void ResetCollisionStats() const;

    // 静的ツリーをシーン初期化後に1回構築する
    /// Build Static Tree の処理を行う
    void BuildStaticTree() const;

    // 近くの静的なステージオブジェクトを取得：StaticQuadTreeIndexに委譲
    /// Nearby Stage Objects を取得する
    /// @param pos 座標
    /// @param size サイズ
    /// @return 取得した要素一覧
    std::vector<CStageObject*> GetNearbyStageObjects(
        const VECTOR2& pos, const VECTOR2& size) const;

    /// Ground Below を検索する
    /// @param pos 座標
    /// @param size サイズ
    /// @param basePos 座標
    /// @param from from に渡す値
    /// @param to to に渡す値
    /// @param groundType groundType に渡す値
    /// @return 成功または条件を満たす場合 true
    bool FindGroundBelow(const VECTOR2& pos, const VECTOR2& size
                         , const VECTOR3& basePos, float from, float to, GroundHitResult* groundType) const;
    // 各クラスへの直接アクセス（必要に応じて）
    /// Model Registry を取得する
    /// @return 対象のポインタ
    CModelRegistry* GetModelRegistry() const { return m_pModelRegistry; }
    /// Quad Tree Index を取得する
    /// @return 対象のポインタ
    CEnemyQuadTree* GetQuadTreeIndex() const { return m_pQuadTreeIndex; }
    /// Static Quad Tree Index を取得する
    /// @return 対象のポインタ
    CStageQuadTree* GetStaticQuadTreeIndex() const { return m_pStaticQuadTreeIndex; }

public:
    CPlayer* m_pPlayer;

private:
    /// 毎フレームの状態を更新する
    void Update() override;

    CModelRegistry* m_pModelRegistry;
    CEnemyQuadTree* m_pQuadTreeIndex;
    CStageQuadTree* m_pStaticQuadTreeIndex;
    std::vector<CEnemyBase*> m_enemies;

    // 互換性のために残す（未使用）
    CFbxMesh* m_pMesh;
    /// <summary>敵AIで使う Mesh Collider の情報と処理をまとめる型</summary>
    class CMeshCollider* m_pMeshCol;
};
