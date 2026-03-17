#include "StageQuadTree.h"
#include "../../Stage/StageObject.h"
#include <list>

CStageQuadTree::CStageQuadTree()
    : m_pTree(nullptr)
{
    // NormalScene(ﾂｱ40)繝ｻPlayScene(ﾂｱ30)縺ｮ荳｡譁ｹ繧偵き繝舌・縺吶ｋ遽・峇
    m_pTree = new CLiner4Tree<CStageObject>(3, VECTOR4(-50, -50, 50, 50));
}

CStageQuadTree::~CStageQuadTree()
{
    delete m_pTree;
    m_pTree = nullptr;
}

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

    // nullptr貂｡縺励〒閾ｪ蟾ｱ髯､螟悶↑縺暦ｼ磯撕逧・が繝悶ず繧ｧ繧ｯ繝医・閾ｪ蛻・・霄ｫ繧帝勁縺丞ｿ・ｦ√′縺ｪ縺・ｼ・
    return m_pTree->GetObjects(nullptr, pos, size);
}