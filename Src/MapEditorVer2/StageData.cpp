#include "StageData.h"

#include "ExportData.h"

namespace
{
    // 繝ｬ繧､蛻､螳壹↓菴ｿ逕ｨ縺吶ｋ繝ｬ繧､縺ｮ髟ｷ縺・
    constexpr float kRayLength = 1000.0f;
}
StageData::StageData()
{
    model_storage_ = ObjectManager::FindGameObject<CModelStorage>();
}


// 謖・ｮ壼ｺｧ讓吶→繝｢繝・Ν蜷阪〒繧ｪ繝悶ず繧ｧ繧ｯ繝医ｒ繧ｹ繝・・繧ｸ縺ｫ霑ｽ蜉縺吶ｋ
void StageData::AddModel(const VECTOR3& pos, const std::string& modelName)
{
    StageDataInfo info(modelName, pos);
    m_stageData.push_back(info);
    m_selectedModel = m_stageData.size() - 1;
}

// 謖・ｮ啜ransform縺ｨ繝｢繝・Ν蜷阪〒繧ｪ繝悶ず繧ｧ繧ｯ繝医ｒ繧ｹ繝・・繧ｸ縺ｫ霑ｽ蜉縺吶ｋ
void StageData::AddModel(const Transform& t, const std::string& modelName)
{
    StageDataInfo info(modelName, t);
    m_stageData.push_back(info);
    m_selectedModel = m_stageData.size() - 1;
}

// Transform蜈ｨ菴薙ｒ謖・ｮ壹＠縺ｦ繧ｪ繝悶ず繧ｧ繧ｯ繝医ｒ繧ｹ繝・・繧ｸ縺ｫ霑ｽ蜉縺吶ｋ・医う繝ｳ繝昴・繝育畑・・
void StageData::AddModelWithTransform(const std::string& modelName, const Transform& transform)
{
    StageDataInfo info;
    info.modelName = modelName;
    info.transform  = transform;
    m_stageData.push_back(info);
}

// 繧ｹ繝・・繧ｸ繝・・繧ｿ繧谷SON繝輔ぃ繧､繝ｫ縺ｫ繧ｨ繧ｯ繧ｹ繝昴・繝医☆繧・
void StageData::Export(const std::string& filename) const
{
    ObjectManager::FindGameObject<ExportData>()->ExportAllModels(filename, m_stageData);
}


// 繝ｬ繧､縺ｨ繧ｹ繝・・繧ｸ荳翫・蜈ｨ繧ｪ繝悶ず繧ｧ繧ｯ繝医・繧ｳ繝ｩ繧､繝繝ｼ繧貞愛螳壹＠縲∵怙霑第磁縺ｮ繧､繝ｳ繝・ャ繧ｯ繧ｹ繧定ｿ斐☆
int StageData::RayHitTest(const Ray& ray,MeshCollider::CollInfo* collOut)
{
    VECTOR3 to = ray.origin + ray.direction * kRayLength;
    int hit_index = -1;
    float nearest = FLT_MAX;

    // 蜈ｨ繧ｪ繝悶ず繧ｧ繧ｯ繝医ｒ襍ｰ譟ｻ縺励※蠖薙◆繧雁愛螳壹ｒ陦後＞縲∵怙繧ゅΞ繧､蟋狗せ縺ｫ霑代＞繧ゅ・繧帝∈縺ｶ
    for (int i = 0; i < m_stageData.size(); i++)
    {
        MeshCollider* coll = model_storage_->GetCollider(m_stageData[i].modelName);
        if (coll == nullptr) continue;

        MeshCollider::CollInfo info;
        MATRIX4X4 mat = m_stageData[i].transform.matrix();
        if (coll->CheckCollisionLine(mat,ray.origin,to,&info))
        {
            float dist = (info.hitPosition - ray.origin).LengthSquare();
            if (dist < nearest)
            {
                nearest = dist;
                hit_index = i;
                if (collOut) *collOut = info;
            }
        }
    }
    return hit_index;
}

// 迴ｾ蝨ｨ驕ｸ謚樔ｸｭ縺ｮ繧ｪ繝悶ず繧ｧ繧ｯ繝医ｒ繧ｹ繝・・繧ｸ縺九ｉ蜑企勁縺吶ｋ
void StageData::DeleteModel()
{
    if (m_stageData.empty())return;
    m_stageData.erase(m_stageData.begin() + m_selectedModel);
    m_selectedModel = -1;
}


void StageData::DeleteModel(int index)
{
    if (m_stageData.empty())return;
    m_stageData.erase(m_stageData.begin() + index);
}

void StageData::Draw()
{
    for (auto& data : m_stageData) {
        CFbxMesh* mesh = model_storage_->GetModel(data.modelName);
        if (mesh == nullptr) continue;
        mesh->Render(data.transform.matrix());
    }
}



// 驕ｸ謚樔ｸｭ縺ｮ繧ｪ繝悶ず繧ｧ繧ｯ繝医う繝ｳ繝・ャ繧ｯ繧ｹ繧定ｨｭ螳壹☆繧・
void StageData::SetModel(int index)
{
    if (index < 0 || index >= m_stageData.size()) return;
    m_selectedModel = index;
}

void StageData::CopyModel(int index)
{
    if (index < 0 || index >= m_stageData.size()) return;
    StageDataInfo info;
    info.modelName = m_stageData[index].modelName;
    info.transform = m_stageData[index].transform;
    info.transform.position.z += 5;
    m_stageData.push_back(info);
    m_selectedModel = m_stageData.size() - 1;
}

// 迴ｾ蝨ｨ驕ｸ謚樔ｸｭ縺ｮ繧ｪ繝悶ず繧ｧ繧ｯ繝医う繝ｳ繝・ャ繧ｯ繧ｹ繧定ｿ斐☆
int StageData::GetSelectIndex() const
{
    return m_selectedModel;
}

const std::vector<StageDataInfo>& StageData::GetStageDataInfo() const
{
    return m_stageData;
}

// 迴ｾ蝨ｨ驕ｸ謚樔ｸｭ縺ｮ繧ｪ繝悶ず繧ｧ繧ｯ繝医・Transform繝昴う繝ｳ繧ｿ繧定ｿ斐☆縲よ悴驕ｸ謚樊凾縺ｯnullptr
Transform* StageData::GetSelectedTransform()
{
    if (m_selectedModel < 0 || m_selectedModel >= m_stageData.size()) return nullptr;
    return &m_stageData[m_selectedModel].transform;
}

// 謖・ｮ壹う繝ｳ繝・ャ繧ｯ繧ｹ縺ｮ繧ｪ繝悶ず繧ｧ繧ｯ繝医・Transform繧剃ｸ頑嶌縺阪☆繧具ｼ・ndo/Redo逕ｨ・・
void StageData::SetSelectedTransform(int index, const Transform& transform)
{
    m_stageData[index].transform = transform;
}

