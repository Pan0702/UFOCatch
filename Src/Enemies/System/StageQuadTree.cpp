#include "StageQuadTree.h"
#include "../../Stage/StageObject.h"
#include <list>

CStageQuadTree::CStageQuadTree()
    : m_pTree(nullptr)
{
    // NormalScene(±40)・PlayScene(±30)の両方をカバーする範囲
    m_pTree = new CLiner4Tree<CStageObject>(3, VECTOR4(-50, -50, 50, 50));
}

CStageQuadTree::~CStageQuadTree()
{
    delete m_pTree;
    m_pTree = nullptr;
}

void CStageQuadTree::Build()
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

    // nullptr渡しで自己除外なし（静的オブジェクトは自分自身を除く必要がない）
    return m_pTree->GetObjects(nullptr, pos, size);
}