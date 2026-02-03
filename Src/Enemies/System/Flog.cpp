#include "Flog.h"

void CFlog::RemoveFromArray(const CSheep* sheep)
{
    // Swap and Pop方式で高速削除
    for (size_t i = 0; i < m_allSheeps.size(); ++i)
    {
        if (m_allSheeps[i] == sheep)
        {
            // 最後の要素と入れ替え
            m_allSheeps[i] = m_allSheeps.back();
            // 最後を削除
            m_allSheeps.pop_back();
            
            // メモリ解放
            SAFE_DELETE(sheep);
            break;
        }
    }
}


void CFlog::Initialize()
{
    // パラメータ
    const int SHEEP_COUNT = 30;      // 羊の総数
    const int DOG_COUNT = 3;         // 犬の数
    const int SHEEP_PER_DOG = SHEEP_COUNT / DOG_COUNT;  // 1匹の犬あたりの羊数
    
    VECTOR3 spawnCenter(0, 0, 0);    // 生成位置の中心
    float spawnRadius = 10.0f;       // 生成範囲の半径
    VECTOR2 areaSize(50.0f, 50.0f);  // 移動可能エリア
    
    // 犬を生成
    for (int i = 0; i < DOG_COUNT; ++i)
    {
        CAShepherdDog* dog = new CAShepherdDog();
        // 犬の初期位置設定など
        m_allDogs.push_back(dog);
    }
    
    // 羊を生成して犬に割り振る
    for (int i = 0; i < SHEEP_COUNT; ++i)
    {
        // ランダムな初期位置
        float angle = Randomf(0.0f, XM_2PI);
        float radius = Randomf(0.0f, spawnRadius);
        VECTOR3 iniPos = spawnCenter + VECTOR3(
            cosf(angle) * radius,
            0,
            sinf(angle) * radius
        );
        
        // どの犬の担当か決定
        int dogIndex = i / SHEEP_PER_DOG;
        if (dogIndex >= DOG_COUNT) dogIndex = DOG_COUNT - 1;  // 余りは最後の犬に
        
        CAShepherdDog* assignedDog = m_shepherdDogs[dogIndex];
        
        // 羊を生成
        CSheep* sheep = new CSheep(assignedDog, iniPos, areaSize);
        
        // 相互参照を設定
        assignedDog->AddSheep(sheep);  // 犬に羊を追加
        m_allSheeps.push_back(sheep);
    }
}

FlogInfo CFlog::CalcFlogInfo(const std::vector<CSheep*>& sheeps) const
{
    FlogInfo info;
    info.centroid = VECTOR3(0,0,0);
    info.maxDistance = 0.0f;
    info.furthestSheep = nullptr;
    
    if (sheeps.empty())return info;
    for (auto sheep : sheeps)   
    {  
        info.centroid += sheep->GetTransform().position;
    }
    info.centroid /= sheeps.size();
    
    for (auto sheep : sheeps)
    {
        float distance = (info.centroid - sheep->GetTransform().position).LengthSquare();
        if (distance > Pow2(info.maxDistance))
        {
            info.maxDistance = distance;
            info.furthestSheep = sheep;
        }
    }
    return info;
}
