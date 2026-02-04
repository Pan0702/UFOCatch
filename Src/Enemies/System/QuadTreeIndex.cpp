#include "QuadTreeIndex.h"
#include "../Base/EnemyBase.h"
#include "../../Common/Object3D.h"
#include <list>

CQuadTreeIndex::CQuadTreeIndex()
    : m_pTree(nullptr), m_frameCount(0)
{
    // 4分木の初期化（レベル3、範囲-20〜20）
    m_pTree = new CLiner4Tree<CEnemyBase>(3, VECTOR4(-20, -20, 20, 20));

    // 統計情報の初期化
    m_processTimes.reserve(60);  // 60フレーム分の履歴
}

CQuadTreeIndex::~CQuadTreeIndex()
{
    delete m_pTree;
    m_pTree = nullptr;
}

void CQuadTreeIndex::Update()
{
    if (m_pTree == nullptr) return;

    m_pTree->AllClear();

    std::list<CEnemyBase*> enemies = ObjectManager::FindGameObjects<CEnemyBase>();
    for (auto enemy : enemies)
    {
        VECTOR2 pos, size;
        if (enemy->GetBounds2D(pos, size))
        {
            m_pTree->Register(enemy, pos, size);
        }
    }
}

const std::vector<CEnemyBase*>& CQuadTreeIndex::GetNearbyEnemies(
    CEnemyBase* pObj,const VECTOR2& pos,const VECTOR2& size) const
{
    static std::vector<CEnemyBase*> result;
    result.clear();

    if (m_pTree == nullptr)
    {
        return result;
    }

    // 処理時間の計測開始
    auto startTime = std::chrono::high_resolution_clock::now();

    // 四分木から周辺オブジェクトを取得
    result = m_pTree->GetObjects(pObj, pos, size);

    // 処理時間の計測終了
    auto endTime = std::chrono::high_resolution_clock::now();
    float elapsedMs = std::chrono::duration<float, std::milli>(endTime - startTime).count();

    CalcCollisionStats(elapsedMs, result);

    return result;
}

void CQuadTreeIndex::CalcCollisionStats(float elapsedMs, const std::vector<CEnemyBase*>& enemies) const
{
    // 統計情報の更新
    m_processTimes.push_back(elapsedMs);
    if (m_processTimes.size() > 60)  // 60フレーム分のみ保持
    {
        m_processTimes.erase(m_processTimes.begin());
    }

    // 平均・最大処理時間の計算
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

    // 判定回数の統計
    m_stats.totalChecks = static_cast<int>(enemies.size());

    // 敵の総数を取得
    std::list<CEnemyBase*> allEnemies = ObjectManager::FindGameObjects<CEnemyBase>();
    m_stats.enemyCount = static_cast<int>(allEnemies.size());

    // 総当たりの場合の判定回数（自分以外の全敵）
    m_stats.potentialChecks = m_stats.enemyCount > 0 ? m_stats.enemyCount - 1 : 0;

    // 削減率の計算
    if (m_stats.potentialChecks > 0)
    {
        m_stats.reductionRate = 100.0f * (1.0f - static_cast<float>(m_stats.totalChecks) / static_cast<float>(m_stats.potentialChecks));
    }
    else
    {
        m_stats.reductionRate = 0.0f;
    }
}

void CQuadTreeIndex::ResetCollisionStats()
{
    m_stats = CollisionStats();
    m_processTimes.clear();
    m_frameCount = 0;
}
