#include "EnemyQuadTree.h"
#include "../Base/EnemyBase.h"
#include "../../Common/Object3D.h"

CEnemyQuadTree::CEnemyQuadTree()
    : m_pTree(nullptr), m_lastEnemyCount(0), m_frameCount(0)
{
    // 4蛻・惠縺ｮ蛻晄悄蛹厄ｼ医Ξ繝吶Ν3縲∫ｯ・峇-20縲・0・・
    m_pTree = new CLiner4Tree<CEnemyBase>(3, VECTOR4(-40, -40, 40, 40));

    // 邨ｱ險域ュ蝣ｱ縺ｮ蛻晄悄蛹・
    m_processTimes.reserve(60);  // 60繝輔Ξ繝ｼ繝蛻・・螻･豁ｴ
}

CEnemyQuadTree::~CEnemyQuadTree()
{
    delete m_pTree;
    m_pTree = nullptr;
}

void CEnemyQuadTree::Update(const std::vector<CEnemyBase*>& enemies)
{
    if (m_pTree == nullptr) return;

    m_lastEnemyCount = static_cast<int>(enemies.size());
    m_pTree->AllClear();

    for (auto* enemy : enemies)
    {
        VECTOR2 pos, size;
        if (enemy->GetBounds2D(pos, size))
        {
            m_pTree->Register(enemy, pos, size);
        }
    }
}

std::vector<CEnemyBase*> CEnemyQuadTree::GetNearbyEnemies(
    CEnemyBase* pObj,const VECTOR2& pos,const VECTOR2& size) const
{
    std::vector<CEnemyBase*> result;

    if (m_pTree == nullptr)
    {
        return result;
    }

    // 蜃ｦ逅・凾髢薙・險域ｸｬ髢句ｧ・
    auto startTime = std::chrono::high_resolution_clock::now();

    // 蝗帛・譛ｨ縺九ｉ蜻ｨ霎ｺ繧ｪ繝悶ず繧ｧ繧ｯ繝医ｒ蜿門ｾ・
    result = m_pTree->GetObjects(pObj, pos, size);

    // 蜃ｦ逅・凾髢薙・險域ｸｬ邨ゆｺ・
    auto endTime = std::chrono::high_resolution_clock::now();
    float elapsedMs = std::chrono::duration<float, std::milli>(endTime - startTime).count();

    CalcCollisionStats(elapsedMs, result, m_lastEnemyCount);

    return result;
}

void CEnemyQuadTree::CalcCollisionStats(float elapsedMs, const std::vector<CEnemyBase*>& enemies, int totalEnemyCount) const
{
    // 邨ｱ險域ュ蝣ｱ縺ｮ譖ｴ譁ｰ
    m_processTimes.push_back(elapsedMs);
    if (m_processTimes.size() > 60)  // 60繝輔Ξ繝ｼ繝蛻・・縺ｿ菫晄戟
    {
        m_processTimes.erase(m_processTimes.begin());
    }

    // 蟷ｳ蝮・・譛螟ｧ蜃ｦ逅・凾髢薙・險育ｮ・
    float sum = 0.0f;
    m_stats.maxProcessTimeMs = 0.0f;
    for (float time : m_processTimes)
    {
        sum += time;
        if (time > m_stats.maxProcessTimeMs)
        {
            m_stats.maxProcessTimeMs = time;
        }
    }
    m_stats.avgProcessTimeMs = m_processTimes.empty() ? 0.0f : sum / m_processTimes.size();

    // 蛻､螳壼屓謨ｰ縺ｮ邨ｱ險・
    m_stats.totalChecks = static_cast<int>(enemies.size());

    // 謨ｵ縺ｮ邱乗焚・・nemyManager縺ｮ邂｡逅・Μ繧ｹ繝医°繧牙叙蠕玲ｸ医∩・・
    m_stats.enemyCount = totalEnemyCount;

    // 邱丞ｽ薙◆繧翫・蝣ｴ蜷医・蛻､螳壼屓謨ｰ・郁・蛻・ｻ･螟悶・蜈ｨ謨ｵ・・
    m_stats.potentialChecks = m_stats.enemyCount > 0 ? m_stats.enemyCount - 1 : 0;

    // 蜑頑ｸ帷紫縺ｮ險育ｮ・
    if (m_stats.potentialChecks > 0)
    {
        m_stats.reductionRate = 100.0f * (1.0f - static_cast<float>(m_stats.totalChecks) / static_cast<float>(m_stats.potentialChecks));
    }
    else
    {
        m_stats.reductionRate = 0.0f;
    }
}

void CEnemyQuadTree::ResetCollisionStats() const
{
    m_stats = CollisionStats();
    m_processTimes.clear();
    m_frameCount = 0;
}

