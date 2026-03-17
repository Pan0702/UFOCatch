#pragma once
#include <vector>
#include <chrono>
#include "../Liner4Tree.h"

class CEnemyBase;

// 蠖薙◆繧雁愛螳壹・邨ｱ險域ュ蝣ｱ
struct CollisionStats
{
    float avgProcessTimeMs;      // 蟷ｳ蝮・・逅・凾髢難ｼ医Α繝ｪ遘抵ｼ・
    float maxProcessTimeMs;      // 譛螟ｧ蜃ｦ逅・凾髢難ｼ医Α繝ｪ遘抵ｼ・
    int totalChecks;             // 螳滄圀縺ｫ蛻､螳壹ｒ陦後▲縺溷屓謨ｰ
    int potentialChecks;         // 邱丞ｽ薙◆繧翫・蝣ｴ蜷医・蛻､螳壼屓謨ｰ
    int enemyCount;              // 謨ｵ縺ｮ邱乗焚
    float reductionRate;         // 蜑頑ｸ帷紫・・・・

    CollisionStats()
        : avgProcessTimeMs(0.0f), maxProcessTimeMs(0.0f),
          totalChecks(0), potentialChecks(0), enemyCount(0), reductionRate(0.0f) {}
};

// 蝗帛・譛ｨ縺ｫ繧医ｋ遨ｺ髢薙う繝ｳ繝・ャ繧ｯ繧ｹ
class CEnemyQuadTree
{
public:
    CEnemyQuadTree();
    ~CEnemyQuadTree();

    // 蝗帛・譛ｨ縺ｮ譖ｴ譁ｰ・域ｯ弱ヵ繝ｬ繝ｼ繝蜻ｼ縺ｶ縲∫ｮ｡逅・Μ繧ｹ繝医ｒ蜿励￠蜿悶ｋ・・
    void Update(const std::vector<CEnemyBase*>& enemies);

    // 蜻ｨ霎ｺ縺ｮ繧ｨ繝阪Α繝ｼ繧貞叙蠕・
    std::vector<CEnemyBase*> GetNearbyEnemies(
        CEnemyBase* pObj,
        const VECTOR2& pos,
        const VECTOR2& size) const;

    // 邨ｱ險域ュ蝣ｱ
    const CollisionStats& GetCollisionStats() const { return m_stats; }
    void ResetCollisionStats() const;

private:
    void CalcCollisionStats(float elapsedMs, const std::vector<CEnemyBase*>& enemies, int totalEnemyCount) const;

    CLiner4Tree<CEnemyBase>* m_pTree;
    int m_lastEnemyCount;

    // 邨ｱ險域ュ蝣ｱ
    mutable CollisionStats m_stats;
    mutable std::vector<float> m_processTimes;
    mutable int m_frameCount;
};
