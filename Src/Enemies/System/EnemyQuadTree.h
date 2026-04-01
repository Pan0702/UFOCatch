#pragma once
#include <vector>
#include <chrono>
#include "../../Core/Spatial/Liner4Tree.h"
#include "../../Framework/QuadtreeSystem.h"
class CEnemyBase;


// 当たり判定の統計情報
struct CollisionStats
{
    float avgProcessTimeMs;      // 平均処理時間（ミリ秒）
    float maxProcessTimeMs;      // 最大処理時間（ミリ秒）
    int totalChecks;             // 実際に判定を行った回数
    int potentialChecks;         // 総当たりの場合の判定回数
    int enemyCount;              // 敵の総数
    float reductionRate;         // 削減率（％）

    CollisionStats()
        : avgProcessTimeMs(0.0f), maxProcessTimeMs(0.0f),
          totalChecks(0), potentialChecks(0), enemyCount(0), reductionRate(0.0f) {}
};

// 四分木による空間インデックス
class CEnemyQuadTree : public CQuadtreeSystem
{
public:
    CEnemyQuadTree();
    ~CEnemyQuadTree();

    // 四分木を再構築する。毎フレーム呼び出し、敵リストを空間インデックスに登録する
    // @param enemies 登録対象の敵リスト（GetBounds2D が失敗したオブジェクトは登録をスキップ）
    void Update(const std::vector<CEnemyBase*>& enemies);

    // 指定した AABB と重なる空間ノードに登録されている敵を返す
    // pObj 自身は結果から除外される
    // @param pObj   検索の起点となる敵（自身を除外するために使用）
    // @param pos    検索範囲の中心座標（2D）
    // @param size   検索範囲のサイズ（2D）
    // @return 範囲内に存在する可能性がある敵のリスト
    std::vector<CEnemyBase*> GetNearbyEnemies(
        CEnemyBase* pObj,
        const VECTOR2& pos,
        const VECTOR2& size) const;

    // 直近 60 フレームの当たり判定統計情報を返す
    const CollisionStats& GetCollisionStats() const { return m_stats; }

    // 統計情報をリセットし、処理時間の履歴とフレームカウントを初期化する
    void ResetCollisionStats() const;

private:
    // GetNearbyEnemies の呼び出しごとに統計情報を更新する
    // @param elapsedMs      今回の処理時間（ミリ秒）
    // @param enemies        今回のクエリで返った敵リスト（totalChecks に使用）
    // @param totalEnemyCount 全敵数（総当たり比較数の算出に使用）
    void CalcCollisionStats(float elapsedMs, const std::vector<CEnemyBase*>& enemies, int totalEnemyCount) const;

    std::unique_ptr<CLiner4Tree<CEnemyBase>> m_pTree;
    int m_lastEnemyCount;

    // 統計情報
    mutable CollisionStats m_stats;
    mutable std::vector<float> m_processTimes;
    mutable int m_frameCount;
};