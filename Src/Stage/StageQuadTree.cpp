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
        if (obj->GetOBB() == nullptr) continue;  // OBBなし（地面など）は登録しない
        VECTOR2 pos, size;
        if (obj->GetBounds2D(pos, size))
        {
            // posは中心座標なので左上に変換
            VECTOR2 topLeft = { pos.x - size.x * 0.5f, pos.y - size.y * 0.5f };
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

    const VECTOR2 botRight = { pos.x + size.x, pos.y + size.y };
    std::vector<CStageObject*> result;
    for (auto* obj : candidates)
    {
        VECTOR2 objPos, objSize;
        if (!obj->GetBounds2D(objPos, objSize)) continue;
        const bool overlapX = objPos.x + objSize.x * 0.5f > pos.x &&
                              objPos.x - objSize.x * 0.5f < botRight.x;
        const bool overlapY = objPos.y + objSize.y * 0.5f > pos.y &&
                              objPos.y - objSize.y * 0.5f < botRight.y;
        if (overlapX && overlapY) result.push_back(obj);
    }
    return result;
}
