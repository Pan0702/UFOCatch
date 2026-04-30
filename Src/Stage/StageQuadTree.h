#pragma once
#include <vector>

#include "GroundHitResult.h"
#include "../Core/Spatial/Liner4Tree.h"
#include "../Framework/QuadtreeSystem.h"

class CStageObject;
using treePtr = std::unique_ptr<CLiner4Tree<CStageObject>>;

// 静的なステージオブジェクト用の空間インデックス
// シーン初期化時に1回だけBuild()を呼ぶ（毎フレーム更新不要）
class CStageQuadTree : public CQuadtreeSystem
{
public:
    CStageQuadTree();
    ~CStageQuadTree();

    // シーンのステージオブジェクト生成後に1回呼ぶ
    void Build() const;

    // 近くの静的オブジェクトを取得
    std::vector<CStageObject*> GetNearbyObjects(
        const VECTOR2& pos, const VECTOR2& size) const;

    // 実際にAABBが重なっているオブジェクトのみ返す
    std::vector<CStageObject*> GetOverlappingObjects(
        const VECTOR2& pos, const VECTOR2& size) const;

    /// 
    /// @param pos 敵のXZ中心
    /// @param size 敵のXZサイズ
    /// @param basePos 敵のY座標
    /// @param fromY 探索開始Y座標
    /// @param toY 探索終了Y座標
    /// @param outHit 探索結果
    /// @return 探索結果が見つかったかどうか
    bool FindGroundBelow(const VECTOR2& pos,
                         const VECTOR2& size, const VECTOR3& basePos,
                         float fromY, float toY, GroundHitResult* outHit);

private:
    void FindHighestGroundHit(const VECTOR2& half, const VECTOR3& basePos,
                              float fromY, float toY, GroundHitResult& best, CStageObject* obj);

private:
    treePtr m_pTree;
    static constexpr float HALF_SIZE = 0.5f;
};
