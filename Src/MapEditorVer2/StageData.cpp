#include "StageData.h"

#include "ExportData.h"

namespace
{
    // レイ判定に使用するレイの長さ
    constexpr float kRayLength = 1000.0f;
}
StageData::StageData()
{
    model_storage_ = ObjectManager::FindGameObject<CModelStorage>();
}


// 指定座標とモデル名でオブジェクトをステージに追加する
void StageData::AddModel(const VECTOR3& pos, const std::string& modelName)
{
    StageDataInfo info(modelName, pos);
    m_stageData.push_back(info);
    m_selectedModel = m_stageData.size() - 1;
}

// 指定Transformとモデル名でオブジェクトをステージに追加する
void StageData::AddModel(const Transform& t, const std::string& modelName)
{
    StageDataInfo info(modelName, t);
    m_stageData.push_back(info);
    m_selectedModel = m_stageData.size() - 1;
}

// Transform全体を指定してオブジェクトをステージに追加する（インポート用）
void StageData::AddModelWithTransform(const std::string& modelName, const Transform& transform)
{
    StageDataInfo info;
    info.modelName = modelName;
    info.transform  = transform;
    m_stageData.push_back(info);
}

// ステージデータをJSONファイルにエクスポートする
void StageData::Export(const std::string& filename) const
{
    ObjectManager::FindGameObject<ExportData>()->ExportAllModels(filename, m_stageData);
}


// レイとステージ上の全オブジェクトのコライダーを判定し、最近接のインデックスを返す
int StageData::RayHitTest(const Ray& ray,MeshCollider::CollInfo* collOut)
{
    VECTOR3 to = ray.origin + ray.direction * kRayLength;
    int hit_index = -1;
    float nearest = FLT_MAX;

    // 全オブジェクトを走査して当たり判定を行い、最もレイ始点に近いものを選ぶ
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

// 現在選択中のオブジェクトをステージから削除する
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



// 選択中のオブジェクトインデックスを設定する
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

// 現在選択中のオブジェクトインデックスを返す
int StageData::GetSelectIndex() const
{
    return m_selectedModel;
}

const std::vector<StageDataInfo>& StageData::GetStageDataInfo() const
{
    return m_stageData;
}

// 現在選択中のオブジェクトのTransformポインタを返す。未選択時はnullptr
Transform* StageData::GetSelectedTransform()
{
    if (m_selectedModel < 0 || m_selectedModel >= m_stageData.size()) return nullptr;
    return &m_stageData[m_selectedModel].transform;
}

// 指定インデックスのオブジェクトのTransformを上書きする（Undo/Redo用）
void StageData::SetSelectedTransform(int index, const Transform& transform)
{
    m_stageData[index].transform = transform;
}
