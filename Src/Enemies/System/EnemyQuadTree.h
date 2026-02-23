#pragma once
#include <vector>
#include <chrono>
#include "../Liner4Tree.h"

class CEnemyBase;

// 当たり判定の統計情報
struct CollisionStats
{
    float avgProcessTimeMs;      // 平均処理時間（ミリ秒）
    float maxProcessTimeMs;      // 最大処理時間（ミリ秒）
    int totalChecks;             // 実際に判定を行った回数
    int potentialChecks;         // 総当たりの場合の判定回数
    int enemyCount;              // 敵の総数
    float reductionRate;         // 削減率（%）

    CollisionStats()
        : avgProcessTimeMs(0.0f), maxProcessTimeMs(0.0f),
          totalChecks(0), potentialChecks(0), enemyCount(0), reductionRate(0.0f) {}
};

// 四分木による空間インデックス
class CEnemyQuadTree
{
public:
    CEnemyQuadTree();
    ~CEnemyQuadTree();

    // 四分木の更新（毎フレーム呼ぶ）
    void Update();

    // 周辺のエネミーを取得
    std::vector<CEnemyBase*> GetNearbyEnemies(
        CEnemyBase* pObj,
        const VECTOR2& pos,
        const VECTOR2& size) const;

    // 統計情報
    const CollisionStats& GetCollisionStats() const { return m_stats; }
    void ResetCollisionStats() const;

private:
    void CalcCollisionStats(float elapsedMs, const std::vector<CEnemyBase*>& enemies) const;

    CLiner4Tree<CEnemyBase>* m_pTree;

    // 統計情報
    mutable CollisionStats m_stats;
    mutable std::vector<float> m_processTimes;
    mutable int m_frameCount;
};
