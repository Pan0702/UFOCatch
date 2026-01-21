#include "EnemyRegistr.h"

#include "../../Utils/BBox.h""
#include <thread>
#include "../AnimalDog/Dog.h"
#include "../HUman/Human.h"
#include "../../Core/Graphics/XAudio.h"
namespace
{
    constexpr float HALF_ROTATION_DEG = 180.0f;
    constexpr float FULL_ROTATION_DEG = 360.0f;
}

CAnimalManager::CAnimalManager(int time)
{
    ObjectManager::DontDestroy(this); // 削除しない
    ObjectManager::SetVisible(this, false); // 表示しない
    
    m_pMesh = nullptr;
    m_pMeshCol = nullptr;
    
    meshstruct ms = {};
    m_meshList.push_back(ms);
    m_meshList.back().name = "Dog";
    m_meshList.back().mesh = new CFbxMesh();
    m_meshList.back().mesh->Load("data/NewAnimal/Dog/Dog.mesh");
    m_meshList.back().mesh->LoadAnimation(A_IDEL, "data/NewAnimal/Dog/Dog_Idle.anmx", false);
    m_meshList.back().mesh->LoadAnimation(A_RUN, "data/NewAnimal/Dog/Dog_Walk.anmx", true);
    m_meshList.back().mesh->LoadAnimation(A_WALK, "data/NewAnimal/Dog/Dog_Walk.anmx", true);
    m_meshList.push_back(ms);
    m_meshList.back().name = "Human";
    m_meshList.back().mesh = new CFbxMesh();
    m_meshList.back().mesh->Load("data/NewAnimal/Human/Human.mesh");
    m_meshList.back().mesh->LoadAnimation(A_IDEL, "data/NewAnimal/Human/Human_Idle.anmx", false);
    m_meshList.back().mesh->LoadAnimation(A_WALK, "data/NewAnimal/Human/Human_Walk.anmx", true);
    m_meshList.back().mesh->LoadAnimation(A_SEACH, "data/NewAnimal/Human/Human_Find.anmx", false);
    m_meshList.push_back(ms);
    m_meshList.back().name = "Chicken";
    m_meshList.back().mesh = new CFbxMesh();
    m_meshList.back().mesh->Load("data/NewAnimal/Chicken/Chicken.mesh");
    m_meshList.back().mesh->LoadAnimation(A_IDEL, "data/NewAnimal/Chicken/Chicken_Idle.anmx", false);
    m_meshList.back().mesh->LoadAnimation(A_WALK, "data/NewAnimal/Chicken/Chicken_Walk.anmx", true);
    m_meshList.back().mesh->LoadAnimation(A_RUN, "data/NewAnimal/Chicken/Chicken_Run.anmx", false);
    // 4分木の初期化（レベル3、範囲-20〜20）
    m_pTree = new CLiner4Tree<CEnemyBase>(3, VECTOR4(-20, -20, 20, 20));

    // 統計情報の初期化
    m_processTimes.reserve(60);  // 60フレーム分の履歴
    m_frameCount = 0;
}


CAnimalManager::~CAnimalManager()
{
    for (meshstruct& ms : m_meshList)
    {
        SAFE_DELETE(ms.mesh);
    }
    SAFE_DELETE(m_pTree);
}

void CAnimalManager::Update()
{
    m_pTree->AllClear();
    std::list<CEnemyBase*> enemyes = ObjectManager::FindGameObjects<CEnemyBase>();
    for (auto enemy : enemyes)
    {
        VECTOR2 pos, size;
        if (enemy->GetBounds2D(pos, size))
        {
            m_pTree->Register(enemy, pos, size);
        }
    }
}

VECTOR4 CAnimalManager::TimeColor(int time)
{
    switch (time)
    {
    case 0: return VECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
    case 1: return VECTOR4(0.5f, 0.5f, 0.5f, 1.0f);
    case 2: return VECTOR4(1.0f, 0.7f, 0.5f, 1.0f);
    default: assert("想定外の入力：AnimalManager：75Lines");
        break;
    }
    return VECTOR4(1.0f, 1.0f, 1.0f, 1.0f);;
}


CFbxMesh* CAnimalManager::MeshList(const std::string& str)
{
    for (meshstruct& ms : m_meshList)
    {
        if (str == ms.name) return ms.mesh;
    }
    MessageBox(nullptr, "EnemyManager::MeshList()", _T("エラー 指定のメッシュ名のメッシュはメッシュリストにありません エラー"),
               MB_OK);
    return nullptr;
}

void CAnimalManager::SetRotationY(const float& angle)
{
    float degAngle = angle * RadToDeg;

    while (degAngle > HALF_ROTATION_DEG)
    {
        degAngle -= FULL_ROTATION_DEG;
    }
    while (degAngle < -HALF_ROTATION_DEG)
    {
        degAngle += FULL_ROTATION_DEG;
    }

    transform.rotation.y = degAngle * DegToRad;
}

std::vector<CEnemyBase*> CAnimalManager::GetNearbyEnemies(CEnemyBase* pObj, const VECTOR2& pos,
                                                          const VECTOR2& size) 
{
    if (m_pTree == nullptr)
    {
        return std::vector<CEnemyBase*>();
    }

    // 処理時間の計測開始
    auto startTime = std::chrono::high_resolution_clock::now();

    // 四分木から周辺オブジェクトを取得
    std::vector<CEnemyBase*> nearbyEnemies = m_pTree->GetObjects(pObj, pos, size);

    // 処理時間の計測終了
    auto endTime = std::chrono::high_resolution_clock::now();
    float elapsedMs = std::chrono::duration<float, std::milli>(endTime - startTime).count();
    
    CalcCollisionStats(elapsedMs,nearbyEnemies);
    
    return nearbyEnemies;
}

void CAnimalManager::CalcCollisionStats(float elapsedMs,const std::vector<CEnemyBase*>& enemies) 

{
    // 統計情報の更新//
    m_processTimes.push_back(elapsedMs);
    if (m_processTimes.size() > 60)  // 60フレーム分のみ保持//
    {
        m_processTimes.erase(m_processTimes.begin());
    }
    // 平均・最大処理時間の計算//
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


void CAnimalManager::ResetCollisionStats()
{
    m_stats = CollisionStats();
    m_processTimes.clear();
    m_frameCount = 0;
}
