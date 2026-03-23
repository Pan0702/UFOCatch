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
        VECTOR2 pos, size;
        if (obj->GetBounds2D(pos, size))
        {
            m_pTree->Register(obj, pos, size);
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
