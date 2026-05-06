#include "StageQuadTree.h"
#include "../Stage/StageObject.h"
#include <list>

#include "../System/GameInstance.h"

CStageQuadTree::CStageQuadTree()
    : m_pTree(nullptr)
{
    VECTOR4 size = CGameInstance::Get()->GetMapSize();

    m_pTree = std::make_unique<CLiner4Tree<CStageObject>>(7, size);
}

CStageQuadTree::~CStageQuadTree() = default;

void CStageQuadTree::Build() const
{
    if (m_pTree == nullptr) return;

    m_pTree->AllClear();

    std::list<CStageObject*> objects = ObjectManager::FindGameObjects<CStageObject>();
    for (auto* obj : objects)
    {
        if (obj->GetOBB() == nullptr) continue; // OBBなし（地面など）は登録しない
        VECTOR2 pos, size;
        if (obj->GetBounds2D(pos, size))
        {
            // posは中心座標なので左上に変換
            VECTOR2 topLeft = {pos.x - size.x * 0.5f, pos.y - size.y * 0.5f};
            m_pTree->Register(obj, topLeft, size);
        }
    }
}

std::vector<CStageObject*> CStageQuadTree::GetNearbyObjects(
    const VECTOR2& pos, const VECTOR2& size) const
{
    if (m_pTree == nullptr) return {};

    // nullptr渡しで自己除外なし（動的オブジェクトは自身の身を除く必要がない）
    return m_pTree->GetObjects(nullptr, pos, size);
}

std::vector<CStageObject*> CStageQuadTree::GetOverlappingObjects(
    const VECTOR2& pos, const VECTOR2& size) const
{
    if (m_pTree == nullptr) return {};

    auto candidates = m_pTree->GetObjects(nullptr, pos, size);

    const VECTOR2 botRight = {pos.x + size.x, pos.y + size.y};
    std::vector<CStageObject*> result;
    for (auto* obj : candidates)
    {
        VECTOR2 objPos, objSize;
        if (!obj->GetBounds2D(objPos, objSize)) continue;
        const bool overlapX = objPos.x + objSize.x * HALF_SIZE > pos.x &&
            objPos.x - objSize.x * HALF_SIZE < botRight.x;
        const bool overlapY = objPos.y + objSize.y * HALF_SIZE > pos.y &&
            objPos.y - objSize.y * HALF_SIZE < botRight.y;
        if (overlapX && overlapY) result.push_back(obj);
    }
    return result;
}

bool CStageQuadTree::FindGroundBelow(const VECTOR2& pos, const VECTOR2& size, float fromY,
                                     float toY, GroundHitResult* outHit)
{
    if (outHit == nullptr)
        return false;
    if (toY >= fromY)
        return false;

    // 足元rayを当たり判定の端から少し内側に寄せる割合
    static constexpr float FOOT_RAY_INSET_RATE = 0.8f;
    const VECTOR2 half = VECTOR2(size.x * HALF_SIZE * FOOT_RAY_INSET_RATE,
                                 size.y * HALF_SIZE * FOOT_RAY_INSET_RATE);


    GroundHitResult best;

    for (CStageObject* obj : GetNearbyObjects(pos, size))
    {
        if (obj == nullptr)
            continue;
        if (!obj->GetIsHitFlag())
            continue;
        if (!obj->MayHitGround(fromY, toY))
            continue;
        FindHighestGroundHit(half, pos, fromY, toY, best, obj);
    }
    if (!best.hit) return false;

    *outHit = best;
    return true;
}

void CStageQuadTree::FindHighestGroundHit(const VECTOR2& half, const VECTOR2& pos,
                                          float fromY, float toY, GroundHitResult& best,
                                          CStageObject* obj)
{
    const VECTOR3 offsets[] =
    {
        VECTOR3(0, 0, 0),
        VECTOR3(-half.x, 0, -half.y),
        VECTOR3(half.x, 0, -half.y),
        VECTOR3(-half.x, 0, half.y),
        VECTOR3(half.x, 0, half.y)
    };
    for (const VECTOR3& offset : offsets)
    {
        const VECTOR3 rayStart(pos.x + offset.x, fromY, pos.y + offset.z);
        const VECTOR3 rayEnd(pos.x + offset.x, toY, pos.y + offset.z);

        MeshCollider::CollInfo info;
        if (!obj->HitGround(rayStart, rayEnd, &info))
            continue;

        if (!best.hit || info.hitPosition.y > best.y)
        {
            best.hit = true;
            best.y = info.hitPosition.y;
            best.collInfo = info;
            best.object = obj;
        }
    }
}
