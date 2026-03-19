#include "RandomPlacer.h"

RandomPlacer::RandomPlacer() = default;

Transform* RandomPlacer::GetTransform()
{
    return &transform;
}

void RandomPlacer::Draw()
{
    if (not is_draw_)return;
    DrawImGui();
    CSprite spr;
    DWORD color = 0xFF0000FF;
    spr.DrawLine3D(point_min_, VECTOR3(point_max_.x, point_min_.y, point_min_.z), color);
    spr.DrawLine3D(point_min_, VECTOR3(point_min_.x, point_max_.y, point_min_.z), color);
    spr.DrawLine3D(point_min_, VECTOR3(point_min_.x, point_min_.y, point_max_.z), color);

    spr.DrawLine3D(point_max_, VECTOR3(point_min_.x, point_max_.y, point_max_.z), color);
    spr.DrawLine3D(point_max_, VECTOR3(point_max_.x, point_min_.y, point_max_.z), color);
    spr.DrawLine3D(point_max_, VECTOR3(point_max_.x, point_max_.y, point_min_.z), color);

    VECTOR3 point_1 = VECTOR3(point_min_.x, point_max_.y, point_min_.z);
    spr.DrawLine3D(point_1, VECTOR3(point_max_.x, point_max_.y, point_min_.z), color);
    spr.DrawLine3D(point_1, VECTOR3(point_min_.x, point_max_.y, point_max_.z), color);

    VECTOR3 point_2 = VECTOR3(point_max_.x, point_min_.y, point_max_.z);
    spr.DrawLine3D(point_2, VECTOR3(point_max_.x, point_min_.y, point_min_.z), color);
    spr.DrawLine3D(point_2, VECTOR3(point_min_.x, point_min_.y, point_max_.z), color);

    VECTOR3 point_3 = VECTOR3(point_max_.x, point_max_.y, point_min_.z);
    spr.DrawLine3D(point_3, VECTOR3(point_max_.x, point_min_.y, point_min_.z), color);

    VECTOR3 point_4 = VECTOR3(point_min_.x, point_max_.y, point_max_.z);
    spr.DrawLine3D(point_4, VECTOR3(point_min_.x, point_min_.y, point_max_.z), color);
}

void RandomPlacer::Update()
{
    if (not is_draw_)return;
    point_max_ = transform.position + transform.scale + VECTOR3(0.5f, 0.5f, 0.5f);
    point_min_ = transform.position - transform.scale - VECTOR3(0.5f, 0.5f, 0.5f);
}

void RandomPlacer::SpawnObject(int model, bool flag)
{
    stage_data_ = ObjectManager::FindGameObject<StageData>();
    CFbxMesh* mesh = ResourceManager::GetModel(model_names_[model].c_str());
    if (mesh == nullptr) return;
    VECTOR3 mesh_min = mesh->m_vMin;
    VECTOR3 mesh_max = mesh->m_vMax;

    // 縺薙・Spawn繧ｻ繝・す繝ｧ繝ｳ縺ｧ驟咲ｽｮ貂医∩縺ｮAABB繝ｪ繧ｹ繝・
    
    std::vector<Aabb> placed;


    VECTOR3 center = transform.position;
    transform.scale = transform.scale.Abs();
    for (int i = 0; i < count_; i++)
    {
        constexpr int MAX_RETRY = 20;
        for (int retry = 0; retry < MAX_RETRY; retry++)
        {
            float x = Randomf(center.x - transform.scale.x, center.x + transform.scale.x);
            float z = Randomf(center.z - transform.scale.z, center.z + transform.scale.z);
            float y = flag ? height_ : Randomf(center.y - transform.scale.y, center.y + transform.scale.y);

            Aabb candidate =
            {
                .min = { x + mesh_max.x, y + mesh_max.y, z + mesh_max.z},
                .max = { x + mesh_min.x, y + mesh_min.y, z + mesh_min.z },
            };

            bool overlap = false;
            for (const auto& a : placed)
            {
                if (candidate.Intersects(a))
                {
                    overlap = true;
                    break;
                }
            }

            if (!overlap)
            {
                placed.push_back(candidate);
                stage_data_->AddModel(VECTOR3(x, y, z), model_names_[model]);
                break;
            }
        }
    }
}

void RandomPlacer::DrawImGui()
{
    static int selected_model = 0;
    static bool changed = false;
    ImGui::Begin("Setting");
    if (ImGui::Button("Spawn"))
    {
        SpawnObject(selected_model, changed);
    }
    ImGui::DragFloat3("Position", &transform.position.x, 0.1f);
    ImGui::DragFloat3("Scale", &transform.scale.x, 0.1f);
    ImGui::DragInt("Rotation", &count_, 1);

    ImGui::Checkbox("鬮倥＆繧貞崋螳壹↓縺吶ｋ", &changed);
    ImGui::SameLine();
    ImGui::InputFloat("", &height_);
    model_names_ = ResourceManager::GetModelNames();

    if (ImGui::BeginCombo("model", model_names_[selected_model].c_str()))
    {
        for (int i = 0; i < model_names_.size(); i++)
        {
            bool is_selected = (selected_model == i);
            if (ImGui::Selectable(model_names_[i].c_str(), i == selected_model))
            {
                selected_model = i;
            }
            if (is_selected)
            {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
    ImGui::End();
}

