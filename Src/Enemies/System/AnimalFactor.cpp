#include "AnimalFactor.h"
#include "../../Stage/StageQuadTree.h"
#include "../AnimalChicken/Chicken.h"
#include "../AnimalDog/Dog.h"
#include "../Human/Human.h"

namespace
{
    constexpr int MAX_SPAWN_ATTEMPTS = 30;
    constexpr float SPAWN_CHECK_HALF = 0.5f; // モブのフットプリント半径（ワールド単位）

    // 障害物のないスポーン位置を探す。見つからなければ最後の候補をそのまま返す
    VECTOR2 FindFreeSpawn(float sizeX, float sizeZ, const CStageQuadTree* pTree)
    {
        VECTOR2 pos = {Randomf(-sizeX, sizeX), Randomf(-sizeZ, sizeZ)};
        if (pTree == nullptr) return pos;

        for (int attempt = 0; attempt < MAX_SPAWN_ATTEMPTS; ++attempt)
        {
            const VECTOR2 topLeft = {pos.x - SPAWN_CHECK_HALF, pos.y - SPAWN_CHECK_HALF};
            const VECTOR2 size = {SPAWN_CHECK_HALF * 2.0f, SPAWN_CHECK_HALF * 2.0f};
            if (pTree->GetOverlappingObjects(topLeft, size).empty())
                return pos;
            pos = VECTOR2(Randomf(-sizeX, sizeX), Randomf(-sizeZ, sizeZ));
        }
        return pos;
    }
}

///動物を生戁E
CAnimalFactor::CAnimalFactor(float sizeX, float sizeZ, int type)
{
    CStageQuadTree* pTree = ObjectManager::FindQuadTree<CStageQuadTree>();
    if (pTree == nullptr)
        assert(false);

    for (int i = 0; i < 60; i++)
    {
        VECTOR2 p = FindFreeSpawn(sizeX, sizeZ, pTree);
        Instantiate<CADog>(VECTOR3(p.x, 0, p.y), VECTOR2(sizeX, sizeZ));
    }
    {
        VECTOR2 p = FindFreeSpawn(sizeX, sizeZ, pTree);
        Instantiate<CHuman>(VECTOR3(p.x, 0, p.y), VECTOR2(sizeX, sizeZ));
    }
    if (type == 1)
    {
        for (int i = 0; i < 10; i++)
        {
            VECTOR2 p = FindFreeSpawn(sizeX, sizeZ, pTree);
            Instantiate<CAnimalChicken>(VECTOR3(p.x, 0, p.y), VECTOR2(sizeX, sizeZ));
        }
        for (int i = 0; i < 3; i++)
        {
            VECTOR2 p = FindFreeSpawn(sizeX, sizeZ, pTree);
            Instantiate<CHuman>(VECTOR3(p.x, 0, p.y), VECTOR2(sizeX, sizeZ));
        }
    }
}

CAnimalFactor::~CAnimalFactor() = default;

void CAnimalFactor::Normal(float sizeX, float sizeZ)
{
}
