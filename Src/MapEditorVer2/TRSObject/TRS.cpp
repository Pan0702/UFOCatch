#include "TRS.h"
#include "../stageData.h"

namespace
{
    // スクリーン上の軸ベクトルがほぼゼロのとき除算を防ぐしきい値
    constexpr float kAxisScreenLenEpsilon = 0.000001f;
}

TRS::TRS()
{
    translation_ = std::make_unique<CTranslation>();
    rotation_ = std::make_unique<CRotation>();
    scaling_ = std::make_unique<CScaling>();
    stage_data_ = ObjectManager::FindGameObject<StageData>();
}

// 現在のモードに応じたギズモとレイの当たり判定を行い、当たった軸を返す
Axis TRS::RayHitTest(const Ray& ray) const
{
    switch (state_)
    {
    case kTranslation: return translation_->RayHitTest(ray);
    case kRotation: return rotation_->RayHitTest(ray);
    case kScaling: return scaling_->RayHitTest(ray);
    default: return Axis::None;
    }
}

void TRS::Update()
{
    SetTransform();

    Transform* t = GetTarget();
    if (t == nullptr) return;
    
    // カメラ距離に応じてスケールを調整し、常に同じ見かけサイズを維持する
    const VECTOR3 cam_pos = GameDevice()->m_vEyePt;
    
    if (translation_)
    {
        VECTOR3& p = t->position;
        translation_->SetPosition(p);
        translation_->UpdateScaleByCamera(cam_pos);
    }
    if (rotation_)
    {
        rotation_->SetPosition(t->position);
        rotation_->UpdateScaleByCamera(cam_pos);
    }
    if (scaling_)
    {
        scaling_->SetPosition(t->position);
        scaling_->UpdateScaleByCamera(cam_pos);
    }
}

// ドラッグ中の軸に沿って選択オブジェクトのTransformを更新する
void TRS::SetTransform()
{
    if (dragging_axis_ == Axis::None) return;

    Transform* t = GetTarget();
    if (t == nullptr) return;

    float delta = AddTransform(dragging_axis_, t->position);

    // ドラッグ軸に対応する VECTOR3 の成分（x/y/z）を参照で返すラムダ
    auto getComponent = [&](VECTOR3& vec) -> float&
    {
        if (dragging_axis_ == Axis::X) return vec.x;
        if (dragging_axis_ == Axis::Y) return vec.y;
        if (dragging_axis_ == Axis::Z) return vec.z;
        return vec.z;
    };

    switch (state_)
    {
    case kTranslation:
        // スナップ単位に丸めて移動
        SnapTranslation(getComponent(t->position), delta);
        break;
    case kRotation:
        // スナップ単位に丸めて回転
        SnapRotation(getComponent(t->rotation) , delta );
        break;
    case kScaling:
        // スケールはスナップなしで直接加算
        getComponent(t->scale) += delta * scale_speed_;
        break;
    default: break;
    }
}

// 移動量をスナップ単位に丸めてcompoに累積加算する
void TRS::SnapTranslation(float& compo, float delta)
{
    // フレームごとの delta を累積し、スナップ幅 step を超えるたびに step 単位で確定
    translate_accum_ += delta;
    float step = translate_speed_;
    if (step > 0.0f)
    {
        while (translate_accum_ >= step)
        {
            compo += step;
            translate_accum_ -= step;
        }
        while (translate_accum_ <= -step)
        {
            compo -= step;
            translate_accum_ += step;
        }
    }
}

// 回転量をスナップ単位に丸めてcompoに累積加算する
void TRS::SnapRotation(float& compo, float delta)
{
    // フレームごとの delta を累積し、スナップ幅 step を超えるたびに step 単位で確定
    rotate_accum_ += delta;
    float step = rotate_speed_;
    if (step > 0.0f)
    {
        while (rotate_accum_ >= step)
        {
            compo += step;
            rotate_accum_ -= step;
        }
        while (rotate_accum_ <= -step)
        {
            compo -= step;
            rotate_accum_ += step;
        }
    }
}

Transform* TRS::GetTarget()
{
    if (override_target_ != nullptr)
        return override_target_;
    return stage_data_->GetSelectedTransform();
}

// マウス移動量を指定軸のスクリーン投影ベクトルに射影し、ワールド空間の変化量を返す
float TRS::AddTransform(Axis axis, const VECTOR3& objPos)
{
    auto device = GameDevice();
    DIMOUSESTATE mouse = device->m_pDI->GetMouseState();
    if (mouse.lX == 0 && mouse.lY == 0) return 0.0f;

    // 軸の方向ベクトル
    VECTOR3 axisDir = {};
    if (axis == Axis::X) axisDir = VECTOR3(1, 0, 0);
    if (axis == Axis::Y) axisDir = VECTOR3(0, 1, 0);
    if (axis == Axis::Z) axisDir = VECTOR3(0, 0, 1);

    float screenW = static_cast<float>(device->m_pD3D->m_dwWindowWidth);
    float screenH = static_cast<float>(device->m_pD3D->m_dwWindowHeight);
    MATRIX4X4 viewProj = device->m_mView * device->m_mProj;

    // ワールド座標 → スクリーン座標に投影
    auto projectToScreen = [&](const VECTOR3& pos) -> VECTOR2
    {
        VECTOR4 p = VECTOR4(pos.x, pos.y, pos.z, 1.0f);
        VECTOR4 clip = XMVector4Transform(p, viewProj);
        VECTOR4 c = clip;
        if (c.w == 0.0f) return {0, 0};
        float ndcX = c.x / c.w;
        float ndcY = c.y / c.w;
        return {
            (ndcX + 1.0f) * 0.5f * screenW,
            (1.0f - ndcY) * 0.5f * screenH
        };
    };

    // オブジェクト位置と軸方向1単位先をスクリーンに投影
    VECTOR2 p1 = projectToScreen(objPos);
    VECTOR2 p2 = projectToScreen(objPos + axisDir);

    // スクリーン空間での軸ベクトル
    float sx = p2.x - p1.x;
    float sy = p2.y - p1.y;
    float lenSq = sx * sx + sy * sy;
    if (lenSq < kAxisScreenLenEpsilon) return 0.0f;

    // マウス移動をスクリーン軸方向に射影 (projected/len) / len = dot / lenSq
    float dot = static_cast<float>(mouse.lX) * sx + static_cast<float>(mouse.lY) * sy;
    return dot / lenSq;
}

void TRS::SetOverrideTarget(Transform* t)
{
    override_target_ = t;
}

void TRS::Draw()
{
    switch (state_)
    {
    case kTranslation:
        translation_->Render();
        break;
    case kRotation:
        rotation_->Render();
        break;
    case kScaling:
        scaling_->Render();
        break;
    default: break;
    }
    DrawImGui();
}


// ImGuiでTRSモード切替と各設定UIをまとめて描画する
void TRS::DrawImGui()
{
    ImGui::SetNextWindowPos(ImVec2(350, 10), ImGuiCond_Once);
    ImGui::Begin("MoveAmount", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    ImGui::RadioButton("Translation", &selected_, kTranslation);
    ImGui::SameLine();
    ImGui::RadioButton("Rotation", &selected_, kRotation);
    ImGui::SameLine();
    ImGui::RadioButton("Scaling", &selected_, kScaling);
    ImGui::SameLine();
    ImGui::RadioButton("None", &selected_, kNone);
    ImGui::Text("state : %d", selected_);
    ImGui::Separator();

    if (selected_ == kTranslation)
    {
        RadioTranslate();
    }
    else if (selected_ == kRotation)
    {
        RadioRotate();
    }
    else if (selected_ == kScaling)
    {
        RadioScale();
    }
    else if (selected_ == kNone)
    {
        ImGui::Text("None");
    }

    ImGui::End();
}


// ImGuiで移動スナップ量を選択するラジオボタンを描画する
void TRS::RadioTranslate()
{
    ImGui::Text("Translation");
    if (ImGui::RadioButton("0.1", translate_speed_ == 0.1f))
    {
        translate_speed_ = 0.1f;
    }
    if (ImGui::RadioButton("0.5", translate_speed_ == 0.5f))
    {
        translate_speed_ = 0.5f;
    }
    if (ImGui::RadioButton("1", translate_speed_ == 1))
    {
        translate_speed_ = 1;
    }
    if (ImGui::RadioButton("5", translate_speed_ == 5))
    {
        translate_speed_ = 5;
    }
    if (ImGui::RadioButton("10", translate_speed_ == 10))
    {
        translate_speed_ = 10;
    }
    if (ImGui::RadioButton("15", translate_speed_ == 50))
    {
        translate_speed_ = 50;
    }
}

// ImGuiで回転スナップ量を選択するラジオボタンを描画する
void TRS::RadioRotate()
{
    ImGui::Text("Rotation");
    if (ImGui::RadioButton("1", rotate_speed_ == 1 * DegToRad))
    {
        rotate_speed_ = 1 * DegToRad;
    }
    if (ImGui::RadioButton("5", rotate_speed_ == 5 * DegToRad))
    {
        rotate_speed_ = 5 * DegToRad;
    }
    if (ImGui::RadioButton("10", rotate_speed_ == 10 * DegToRad))
    {
        rotate_speed_ = 10 * DegToRad;
    }
    if (ImGui::RadioButton("15", rotate_speed_ == 15 * DegToRad))
    {
        rotate_speed_ = 15 * DegToRad;
    }
    if (ImGui::RadioButton("30", rotate_speed_ == 30 * DegToRad))
    {
        rotate_speed_ = 30 * DegToRad;
    }
    if (ImGui::RadioButton("60", rotate_speed_ == 60 * DegToRad))
    {
        rotate_speed_ = 60 * DegToRad;
    }
    if (ImGui::RadioButton("90", rotate_speed_ == 90 * DegToRad ))
    {
        rotate_speed_ = 90 * DegToRad;
    }
}

// ImGuiでスケールスピードを選択するラジオボタンを描画する
void TRS::RadioScale()
{
    ImGui::Text("Scaling");
    if (ImGui::RadioButton("10", scale_speed_ == 10))
    {
        scale_speed_ = 10;
    }
    if (ImGui::RadioButton("1", scale_speed_ == 1))
    {
        scale_speed_ = 1;
    }
    if (ImGui::RadioButton("0.5f", scale_speed_ == 0.5f))
    {
        scale_speed_ = 0.5f;
    }
    if (ImGui::RadioButton("0.25f", scale_speed_ == 0.25f))
    {
        scale_speed_ = 0.25f;
    }
    if (ImGui::RadioButton("0.1f", scale_speed_ == 0.1f))
    {
        scale_speed_ = 0.1f;
    }
}
