#pragma once
#include <chrono>
#include "../Base/StateBase.h"
#include "../../Common/Object3D.h"
#include "../../Player/Player.h"
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

class CAnimalManager : public Object3D
{
public:
    CAnimalManager(int time);
    ~CAnimalManager();


    void SetRotationY(const float& angle);

    CFbxMesh* MeshList(const std::string& str);

    // 四分木から周辺のエネミーを取得
    std::vector<CEnemyBase*> GetNearbyEnemies(CEnemyBase* pObj, const VECTOR2& pos, const VECTOR2& size) ;

    // 統計情報の取得
    const CollisionStats& GetCollisionStats() const { return m_stats; }
    void ResetCollisionStats();

public:
    CPlayer* m_pPlayer;

private:
    void Update() override;
    VECTOR4 TimeColor(int time);
    void CalcCollisionStats(float elapsedMs, const std::vector<CEnemyBase*>& enemies);

    struct meshstruct
    {
        std::string name;
        CFbxMesh* mesh;
    };

    std::list<meshstruct> m_meshList;
    CLiner4Tree<CEnemyBase>* m_pTree;

    // 統計情報
    mutable CollisionStats m_stats;
    mutable std::vector<float> m_processTimes;  // 処理時間の履歴
    mutable int m_frameCount;
    
};

enum AnimationType
{
    A_IDEL = 0,
    A_WALK,
    A_RUN,
    A_SEACH
};
