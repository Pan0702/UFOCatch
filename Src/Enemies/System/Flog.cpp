#include "Flog.h"

// void CFlog::RemoveFromArray(const CSheep* sheep)
// {
//     // Swap and Pop譁ｹ蠑上〒鬮倬溷炎髯､
//     for (size_t i = 0; i < m_allSheep.size(); ++i)
//     {
//         if (m_allSheep[i] == sheep)
//         {
//             // 譛蠕後・隕∫ｴ縺ｨ蜈･繧梧崛縺・
//             m_allSheep[i] = m_allSheep.back();
//             // 譛蠕後ｒ蜑企勁
//             m_allSheep.pop_back();
//
//             // 繝｡繝｢繝ｪ隗｣謾ｾ
//             SAFE_DELETE(sheep);
//             break;
//         }
//     }
// }


CFlog::CFlog()
{
    Initialize();
}

CFlog::~CFlog() = default;

void CFlog::Initialize()
{
    // 繝代Λ繝｡繝ｼ繧ｿ
    constexpr int SHEEP_COUNT = 10; // 鄒翫・邱乗焚

    VECTOR3 spawnCenter(0, 0, 0); // 逕滓・菴咲ｽｮ縺ｮ荳ｭ蠢・

    // 鄒､繧後・荳ｭ蠢・せ縺ｨ蜊雁ｾ・ｒ險ｭ螳・
    m_flockCenter = VECTOR3(0, 0, 0);
    m_flockRadius = 4.0f;

    // 鄒翫ｒ逕滓・
    for (int i = 0; i < SHEEP_COUNT; ++i)
    {
        // 繝ｩ繝ｳ繝繝縺ｪ蛻晄悄菴咲ｽｮ
        float angle = Randomf(0.0f, XM_2PI);
        const float spawnRadius = m_flockRadius; // 逕滓・遽・峇縺ｮ蜊雁ｾ・
        float radius = Randomf(0.0f, spawnRadius);
        VECTOR3 iniPos = spawnCenter + VECTOR3(
            cosf(angle) * radius,
            0,
            sinf(angle) * radius
        );

        // 鄒翫ｒ逕滓・・・hepherdDog荳崎ｦ・ｼ・
        CSheep* sheep = Instantiate<CSheep>(iniPos);
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
    // maxDistance縺ｯ莠御ｹ怜､縺ｮ縺ｾ縺ｾ霑斐☆・域ｯ碑ｼ・・縺ｧPow2()繧剃ｽｿ縺・ｼ・
    return info;
}

