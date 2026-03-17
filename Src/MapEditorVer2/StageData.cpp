#include "StageData.h"

#include "ExportData.h"

namespace
{
    // レイ判定に使用するレイの長さ
    constexpr float kRayLength = 1000.0f;
}

// 指定座標とモデル名でオブジェクトをステージに追加する
void StageData::AddModel(const VECTOR3& pos, const std::string& model_name)
{
    StageDataInfo info(model_name, pos);
    stage_data_.push_back(info);
    selected_model_ = stage_data_.size() - 1;
}

// 指定Transformとモデル名でオブジェクトをステージに追加する
void StageData::AddModel(const Transform& t, const std::string& model_name)
{
    StageDataInfo info(model_name, t);
    stage_data_.push_back(info);
    selected_model_ = stage_data_.size() - 1;
}

// Transform全体を指定してオブジェクトをステージに追加する（インポート用）
void StageData::AddModelWithTransform(const std::string& model_name, const Transform& transform)
{
    StageDataInfo info;
    info.model_name_ = model_name;
    info.transform_  = transform;
    stage_data_.push_back(info);
}

// ステージデータをJSONファイルにエクスポートする
void StageData::Export(const std::string& filename) const
{
    ObjectManager::FindGameObject<ExportData>()->ExportAllModels(filename, stage_data_);
}


// レイとステージ上の全オブジェクトのコライダーを判定し、最近接のインデックスを返す
int StageData::RayHitTest(const Ray& ray,MeshCollider::CollInfo* collOut)
{
    VECTOR3 to = ray.origin + ray.direction * kRayLength;
    int hit_index = -1;
    float nearest = FLT_MAX;

    // 全オブジェクトを走査して当たり判定を行い、最もレイ始点に近いものを選ぶ
    for (int i = 0; i < stage_data_.size(); i++)
    {
        MeshCollider* coll = model_storage_->GetCollider(stage_data_[i].model_name_);
        if (coll == nullptr) continue;

        MeshCollider::CollInfo info;
        MATRIX4X4 mat = stage_data_[i].transform_.matrix();
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
    if (stage_data_.empty())return;
    stage_data_.erase(stage_data_.begin() + selected_model_);
    selected_model_ = -1;
}


void StageData::DeleteModel(int index)
{
    if (stage_data_.empty())return;
    stage_data_.erase(stage_data_.begin() + index);
}

void StageData::Draw()
{
    for (auto& data : stage_data_) {
        CFbxMesh* mesh = model_storage_->GetModel(data.model_name_);
        if (mesh == nullptr) continue;
        mesh->Render(data.transform_.matrix());
    }
}

StageData::StageData()
{
    model_storage_ = ObjectManager::FindGameObject<CModelStorage>();
}


// 選択中のオブジェクトインデックスを設定する
void StageData::SetModel(int index)
{
    if (index < 0 || index >= stage_data_.size()) return;
    selected_model_ = index;
}

void StageData::CopyModel(int index)
{
    if (index < 0 || index >= stage_data_.size()) return;
    StageDataInfo info;
    info.model_name_ = stage_data_[index].model_name_;
    info.transform_ = stage_data_[index].transform_;
    info.transform_.position.z += 5;
    stage_data_.push_back(info);
    selected_model_ = stage_data_.size() - 1;
}

// 現在選択中のオブジェクトインデックスを返す
int StageData::GetSelectIndex() const
{
    return selected_model_;
}

const std::vector<StageDataInfo>& StageData::GetStageDataInfo() const
{
    return stage_data_;
}

// 現在選択中のオブジェクトのTransformポインタを返す。未選択時はnullptr
Transform* StageData::GetSelectedTransform()
{
    if (selected_model_ < 0 || selected_model_ >= stage_data_.size()) return nullptr;
    return &stage_data_[selected_model_].transform_;
}

// 指定インデックスのオブジェクトのTransformを上書きする（Undo/Redo用）
void StageData::SetSelectedTransform(int index, const Transform& transform)
{
    stage_data_[index].transform_ = transform;
}
