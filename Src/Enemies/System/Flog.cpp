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

    VECTOR3 spawnCenter(0, 0, 0); // 生成位置の中心

    // 群れの中心点と半径を設定
    m_flockCenter = VECTOR3(0, 0, 0);
    m_flockRadius = 4.0f;

    // 羊を生成
    for (int i = 0; i < SHEEP_COUNT; ++i)
    {
        // ランダムな初期位置
        float angle = Randomf(0.0f, XM_2PI);
        const float spawnRadius = m_flockRadius; // 生成範囲の半径
        float radius = Randomf(0.0f, spawnRadius);
        VECTOR3 iniPos = spawnCenter + VECTOR3(
            cosf(angle) * radius,
            0,
            sinf(angle) * radius
        );

        // 羊を生成（ShepherdDog不要）
        CSheep* sheep = new CSheep(iniPos);
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
