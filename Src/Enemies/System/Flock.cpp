#include "Flock.h"

CFlock::CFlock(const VECTOR3& center, float radius, int sheepCount)
{
    m_flockCenter = center;
    m_flockRadius = radius;
    for (int i = 0; i < sheepCount; ++i)
    {
        //向きを生成//
        float angle = Randomf(0.0f, XM_2PI);
        //スポーン場所を計算
        //中心点から半径以内で生成//
        const float spawnDistance = Randomf(0.0f, m_flockRadius);
        VECTOR3 iniPos = m_flockCenter + VECTOR3(
            cosf(angle) * spawnDistance,
            0,
            sinf(angle) * spawnDistance
        );

        //集団に追加
        CSheep* sheep = Instantiate<CSheep>(iniPos);
        sheep->SetFlock(this);
    }
    // 犬スポーン（境界の外側、東に3m）
    const VECTOR3 dogPos = center + VECTOR3(radius + 3.0f, 0, 0);
    CAShepherdDog* dog = Instantiate<CAShepherdDog>(dogPos);
    dog->SetFlock(this);
    SetShepherdDog(dog); // 既存の API

    // 犬の m_sheeps を埋める
    for (CSheep* s : m_allSheep)
    {
        dog->AddSheep(s);
    }
}

void CFlock::AddSheep(CSheep* sheep)
{
    m_allSheep.push_back(sheep);
}

void CFlock::RemoveSheep(const CSheep* sheep)
{
    auto it = std::ranges::find(m_allSheep, sheep);
    if (it != m_allSheep.end())
    {
        m_allSheep.erase(it);
    }
}

bool CFlock::ContainPos(const VECTOR3& pos) const
{
    VECTOR3 diff = m_flockCenter - pos;
    diff.y = 0;
    return diff.LengthSquare() <= m_flockRadius * m_flockRadius;
}


FlockInfo CFlock::CalCFlockInfoStatic(const std::vector<CSheep*>& manySheep)
{
    FlockInfo info;
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
    // maxDistanceは二乗値のまま返す//
    return info;
}

float CFlock::GetMoveRadius() const
{
    constexpr float FLOCK_MOVE_RADIUS_SCALE = 3.0f;
    return m_flockRadius + FLOCK_MOVE_RADIUS_SCALE;
}

float CFlock::GetCollectRadius() const
{
    constexpr float COLLECT_RADIUS_RATE = 1.3f;
    return m_flockRadius * COLLECT_RADIUS_RATE;
}

bool CFlock::ContainMoveArea(const VECTOR3& pos) const
{
    VECTOR3 diff = m_flockCenter - pos;
    diff.y = 0;
    return diff.LengthSquare() <= GetMoveRadius() * GetMoveRadius();
}

bool CFlock::ContainCollectArea(const VECTOR3& pos) const
{
    VECTOR3 diff = m_flockCenter - pos;
    diff.y = 0;
    return diff.LengthSquare() <= GetCollectRadius() * GetCollectRadius();
}
