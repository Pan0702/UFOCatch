#include "TRS.h"
#include "../stageData.h"

namespace
{
    // スクリーン上の軸ベクトルがほぼゼロのとき除算をふせぐしきい値
    constexpr float kAxisScreenLenEpsilon = 0.000001f;
}

TRS::TRS()
{
    m_pTranslation_ = std::make_unique<CTranslation>();
    m_pRotation = std::make_unique<CRotation>();
    m_pScaling = std::make_unique<CScaling>();
    m_pStageData = ObjectManager::FindGameObject<StageData>();
}

// 現在のモードに応じたギズモとレイの当たり判定を行い、当たった軸を返す
Axis TRS::RayHitTest(const Ray& ray) const
{
    switch (m_state)
    {
    case kTranslation: return m_pTranslation_->RayHitTest(ray);
    case kRotation: return m_pRotation->RayHitTest(ray);
    case kScaling: return m_pScaling->RayHitTest(ray);
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
    
    if (m_pTranslation_)
    {
        VECTOR3& p = t->position;
        m_pTranslation_->SetPosition(p);
        m_pTranslation_->UpdateScaleByCamera(cam_pos);
    }
    if (m_pRotation)
    {
        m_pRotation->SetPosition(t->position);
        m_pRotation->UpdateScaleByCamera(cam_pos);
    }
    if (m_pScaling)
    {
        m_pScaling->SetPosition(t->position);
        m_pScaling->UpdateScaleByCamera(cam_pos);
    }
}

// ドラッグ中の軸に沿って選択オブジェクトのTransformを更新する
void TRS::SetTransform()
{
    if (m_draggingAxis == Axis::None) return;

    Transform* t = GetTarget();
    if (t == nullptr) return;

    float delta = AddTransform(m_draggingAxis, t->position);

    // ドラッグ軸に対応する VECTOR3 の成分（x/y/z）を参照で返すラムダ
    auto getComponent = [&](VECTOR3& vec) -> float&
    {
        if (m_draggingAxis == Axis::X) return vec.x;
        if (m_draggingAxis == Axis::Y) return vec.y;
        if (m_draggingAxis == Axis::Z) return vec.z;
        return vec.z;
    };

    switch (m_state)
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
        getComponent(t->scale) += delta * m_scaleSpeed;
        break;
    default: break;
    }
}

// 移動量をスナップ単位に丸めてcompoに累積加算する
void TRS::SnapTranslation(float& compo, float delta)
{
    // フレームごとの delta を累積し、スナップ幅 step を超えるたびに step 単位で確定
    m_translateAccum += delta;
    float step = m_translateSpeed;
    if (step > 0.0f)
    {
        while (m_translateAccum >= step)
        {
            compo += step;
            m_translateAccum -= step;
        }
        while (m_translateAccum <= -step)
        {
            compo -= step;
            m_translateAccum += step;
        }
    }
}

// 回転量をスナップ単位に丸めてcompoに累積加算する
void TRS::SnapRotation(float& compo, float delta)
{
    // フレームごとの delta を累積し、スナップ幅 step を超えるたびに step 単位で確定
    m_rotateAccum += delta;
    float step = m_rotateSpeed;
    if (step > 0.0f)
    {
        while (m_rotateAccum >= step)
        {
            compo += step;
            m_rotateAccum -= step;
        }
        while (m_rotateAccum <= -step)
        {
            compo -= step;
            m_rotateAccum += step;
        }
    }
}

Transform* TRS::GetTarget() const
{
    if (m_pOverrideTarget != nullptr)
        return m_pOverrideTarget;
    return m_pStageData->GetSelectedTransform();
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

    // オブジェクト位置と軸方向 単位分をスクリーンに投影
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
    m_pOverrideTarget = t;
}

void TRS::Draw()
{
    switch (m_state)
    {
    case kTranslation:
        m_pTranslation_->Render();
        break;
    case kRotation:
        m_pRotation->Render();
        break;
    case kScaling:
        m_pScaling->Render();
        break;
    default: break;
    }
}


// "MoveAmount"ウィンドウの中身を描画する（Begin/Endなし）
void TRS::DrawPanel()
{
    ImGui::RadioButton("Translation", &m_selected, kTranslation);
    ImGui::SameLine();
    ImGui::RadioButton("Rotation", &m_selected, kRotation);
    ImGui::SameLine();
    ImGui::RadioButton("Scaling", &m_selected, kScaling);
    ImGui::SameLine();
    ImGui::RadioButton("None", &m_selected, kNone);
    ImGui::Text("state : %d", m_selected);
    ImGui::Separator();

    if (m_selected == kTranslation)
    {
        RadioTranslate();
    }
    else if (m_selected == kRotation)
    {
        RadioRotate();
    }
    else if (m_selected == kScaling)
    {
        RadioScale();
    }
    else if (m_selected == kNone)
    {
        ImGui::Text("None");
    }
}


// ImGuiで移動スナップ量を選択するラジオボタンを描画する
void TRS::RadioTranslate()
{
    ImGui::Text("Translation");
    if (ImGui::RadioButton("0.1", m_translateSpeed == 0.1f))
    {
        m_translateSpeed = 0.1f;
    }
    if (ImGui::RadioButton("0.5", m_translateSpeed == 0.5f))
    {
        m_translateSpeed = 0.5f;
    }
    if (ImGui::RadioButton("1", m_translateSpeed == 1))
    {
        m_translateSpeed = 1;
    }
    if (ImGui::RadioButton("5", m_translateSpeed == 5))
    {
        m_translateSpeed = 5;
    }
    if (ImGui::RadioButton("10", m_translateSpeed == 10))
    {
        m_translateSpeed = 10;
    }
    if (ImGui::RadioButton("15", m_translateSpeed == 50))
    {
        m_translateSpeed = 50;
    }
}

// ImGuiで回転スナップ量を選択するラジオボタンを描画する
void TRS::RadioRotate()
{
    ImGui::Text("Rotation");
    if (ImGui::RadioButton("1", m_rotateSpeed == 1 * DegToRad))
    {
        m_rotateSpeed = 1 * DegToRad;
    }
    if (ImGui::RadioButton("5", m_rotateSpeed == 5 * DegToRad))
    {
        m_rotateSpeed = 5 * DegToRad;
    }
    if (ImGui::RadioButton("10", m_rotateSpeed == 10 * DegToRad))
    {
        m_rotateSpeed = 10 * DegToRad;
    }
    if (ImGui::RadioButton("15", m_rotateSpeed == 15 * DegToRad))
    {
        m_rotateSpeed = 15 * DegToRad;
    }
    if (ImGui::RadioButton("30", m_rotateSpeed == 30 * DegToRad))
    {
        m_rotateSpeed = 30 * DegToRad;
    }
    if (ImGui::RadioButton("60", m_rotateSpeed == 60 * DegToRad))
    {
        m_rotateSpeed = 60 * DegToRad;
    }
    if (ImGui::RadioButton("90", m_rotateSpeed == 90 * DegToRad ))
    {
        m_rotateSpeed = 90 * DegToRad;
    }
}

// ImGuiでスケールスピードを選択するラジオボタンを描画する
void TRS::RadioScale()
{
    ImGui::Text("Scaling");
    if (ImGui::RadioButton("10", m_scaleSpeed == 10))
    {
        m_scaleSpeed = 10;
    }
    if (ImGui::RadioButton("1", m_scaleSpeed == 1))
    {
        m_scaleSpeed = 1;
    }
    if (ImGui::RadioButton("0.5f", m_scaleSpeed == 0.5f))
    {
        m_scaleSpeed = 0.5f;
    }
    if (ImGui::RadioButton("0.25f", m_scaleSpeed == 0.25f))
    {
        m_scaleSpeed = 0.25f;
    }
    if (ImGui::RadioButton("0.1f", m_scaleSpeed == 0.1f))
    {
        m_scaleSpeed = 0.1f;
    }
}
