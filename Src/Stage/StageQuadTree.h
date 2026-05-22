#pragma once
#include <vector>

#include "GroundHitResult.h"
#include "../Core/Spatial/Liner4Tree.h"
#include "../Framework/QuadtreeSystem.h"

/// <summary>ステージで使う Stage Object の情報と処理をまとめる型</summary>
class CStageObject;
using treePtr = std::unique_ptr<CLiner4Tree<CStageObject>>;

// 静的なステージオブジェクト用の空間インデックス
// シーン初期化時に1回だけBuild()を呼ぶ（毎フレーム更新不要）
class CStageQuadTree : public CQuadtreeSystem
{
public:
    /// CStageQuadTree を初期化する
    CStageQuadTree();
    /// CStageQuadTree の終了処理を行う
    ~CStageQuadTree();

    // シーンのステージオブジェクト生成後に1回呼ぶ
    /// Build の処理を行う
    void Build() const;

    // 近くの静的オブジェクトを取得
    /// Nearby Objects を取得する
    /// @param pos 座標
    /// @param size サイズ
    /// @return 取得した要素一覧
    std::vector<CStageObject*> GetNearbyObjects(
        const VECTOR2& pos, const VECTOR2& size) const;

    // 実際にAABBが重なっているオブジェクトのみ返す
    /// Overlapping Objects を取得する
    /// @param pos 座標
    /// @param size サイズ
    /// @return 取得した要素一覧
    std::vector<CStageObject*> GetOverlappingObjects(
        const VECTOR2& pos, const VECTOR2& size) const;

    /// Ground Below を検索する
    /// @param pos 座標
    /// @param size サイズ
    /// @param fromY fromY に渡す値
    /// @param toY toY に渡す値
    /// @param outHit outHit に渡す値
    /// @return 成功または条件を満たす場合 true
    bool FindGroundBelow(const VECTOR2& pos,
                         const VECTOR2& size,
                         float fromY, float toY, GroundHitResult* outHit);

private:
    /// Highest Ground Hit を検索する
    /// @param half half に渡す値
    /// @param pos 座標
    /// @param fromY fromY に渡す値
    /// @param toY toY に渡す値
    /// @param best best に渡す値
    /// @param obj 対象オブジェクト
    void FindHighestGroundHit(const VECTOR2& half,
                              const VECTOR2& pos, float fromY, float toY, GroundHitResult& best, CStageObject* obj);

private:
    treePtr m_pTree;
    static constexpr float HALF_SIZE = 0.5f;
};
