#include "Flog.h"

void CFlog::RemoveFromArray(const CSheep* sheep)
{
    // Swap and Pop方式で高速削除
    for (size_t i = 0; i < m_allSheep.size(); ++i)
    {
        if (m_allSheep[i] == sheep)
        {
            // 最後の要素と入れ替え
            m_allSheep[i] = m_allSheep.back();
            // 最後を削除
            m_allSheep.pop_back();

            // メモリ解放
            SAFE_DELETE(sheep);
            break;
        }
    }
}


CFlog::CFlog()
{
    Initialize();
}

CFlog::~CFlog() = default;

void CFlog::Initialize()
{
    // パラメータ
    constexpr int SHEEP_COUNT = 10; // 羊の総数
    constexpr int DOG_COUNT = 1; // 犬の数
    constexpr int SHEEP_PER_DOG = SHEEP_COUNT / DOG_COUNT; // 1匹の犬あたりの羊数

    VECTOR3 spawnCenter(0, 0, 0); // 生成位置の中心

    VECTOR2 areaSize(25.0f, 25.0f); // 移動可能エリア

    // 犬を生成
    for (int i = 0; i < DOG_COUNT; ++i)
    {
        CAShepherdDog* dog = new CAShepherdDog();
        // 犬の初期位置設定など
        m_shepherdDogs.push_back(dog);
    }

    // 羊を生成して犬に割り振る
    for (int i = 0; i < SHEEP_COUNT; ++i)
    {
        // ランダムな初期位置
        float angle = Randomf(0.0f, XM_2PI);
        constexpr float spawnRadius = 10.0f; // 生成範囲の半径
        float radius = Randomf(0.0f, spawnRadius);
        VECTOR3 iniPos = spawnCenter + VECTOR3(
            cosf(angle) * radius,
            0,
            sinf(angle) * radius
        );

        // どの犬の担当か決定
        int dogIndex = i / SHEEP_PER_DOG;
        if (dogIndex >= DOG_COUNT) dogIndex = DOG_COUNT - 1; // 余りは最後の犬に

        CAShepherdDog* assignedDog = m_shepherdDogs[dogIndex];

        // 羊を生成
        CSheep* sheep = new CSheep(assignedDog, iniPos, areaSize);

        // 相互参照を設定
        assignedDog->AddSheep(sheep); // 犬に羊を追加
        m_allSheep.push_back(sheep);
    }
}

FlogInfo CFlog::CalcFlogInfo(const std::vector<CSheep*>& manySheep) const
{
    FlogInfo info;
    info.centroid = VECTOR3(0, 0, 0);
    info.maxDistance = 0.0f;
    info.furthestSheep = nullptr;

    if (manySheep.empty())return info;
    for (auto sheep : manySheep)
    {
        info.centroid += sheep->GetTransform().position;
    }
    info.centroid /= manySheep.size();

    for (auto sheep : manySheep)
    {
        float distanceSq = (info.centroid - sheep->GetTransform().position).LengthSquare();
        if (distanceSq > info.maxDistance)
        {
            info.maxDistance = distanceSq;
            info.furthestSheep = sheep;
        }
    }
    // maxDistanceは二乗値のまま返す（比較側でPow2()を使う）
    return info;
}
