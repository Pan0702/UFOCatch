#include "TRS.h"
#include "../stageData.h"

namespace
{
    // 繧ｹ繧ｯ繝ｪ繝ｼ繝ｳ荳翫・霆ｸ繝吶け繝医Ν縺後⊇縺ｼ繧ｼ繝ｭ縺ｮ縺ｨ縺埼勁邂励ｒ髦ｲ縺舌＠縺阪＞蛟､
    constexpr float kAxisScreenLenEpsilon = 0.000001f;
}

TRS::TRS()
{
    m_pTranslation_ = std::make_unique<CTranslation>();
    m_pRotation = std::make_unique<CRotation>();
    m_pScaling = std::make_unique<CScaling>();
    m_pStageData = ObjectManager::FindGameObject<StageData>();
}

// 迴ｾ蝨ｨ縺ｮ繝｢繝ｼ繝峨↓蠢懊§縺溘ぐ繧ｺ繝｢縺ｨ繝ｬ繧､縺ｮ蠖薙◆繧雁愛螳壹ｒ陦後＞縲∝ｽ薙◆縺｣縺溯ｻｸ繧定ｿ斐☆
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
    
    // 繧ｫ繝｡繝ｩ霍晞屬縺ｫ蠢懊§縺ｦ繧ｹ繧ｱ繝ｼ繝ｫ繧定ｪｿ謨ｴ縺励∝ｸｸ縺ｫ蜷後§隕九°縺代し繧､繧ｺ繧堤ｶｭ謖√☆繧・
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

// 繝峨Λ繝・げ荳ｭ縺ｮ霆ｸ縺ｫ豐ｿ縺｣縺ｦ驕ｸ謚槭が繝悶ず繧ｧ繧ｯ繝医・Transform繧呈峩譁ｰ縺吶ｋ
void TRS::SetTransform()
{
    if (m_draggingAxis == Axis::None) return;

    Transform* t = GetTarget();
    if (t == nullptr) return;

    float delta = AddTransform(m_draggingAxis, t->position);

    // 繝峨Λ繝・げ霆ｸ縺ｫ蟇ｾ蠢懊☆繧・VECTOR3 縺ｮ謌仙・・・/y/z・峨ｒ蜿ら・縺ｧ霑斐☆繝ｩ繝繝
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
        // 繧ｹ繝翫ャ繝怜腰菴阪↓荳ｸ繧√※遘ｻ蜍・
        SnapTranslation(getComponent(t->position), delta);
        break;
    case kRotation:
        // 繧ｹ繝翫ャ繝怜腰菴阪↓荳ｸ繧√※蝗櫁ｻ｢
        SnapRotation(getComponent(t->rotation) , delta );
        break;
    case kScaling:
        // 繧ｹ繧ｱ繝ｼ繝ｫ縺ｯ繧ｹ繝翫ャ繝励↑縺励〒逶ｴ謗･蜉邂・
        getComponent(t->scale) += delta * m_scaleSpeed;
        break;
    default: break;
    }
}

// 遘ｻ蜍暮㍼繧偵せ繝翫ャ繝怜腰菴阪↓荳ｸ繧√※compo縺ｫ邏ｯ遨榊刈邂励☆繧・
void TRS::SnapTranslation(float& compo, float delta)
{
    // 繝輔Ξ繝ｼ繝縺斐→縺ｮ delta 繧堤ｴｯ遨阪＠縲√せ繝翫ャ繝怜ｹ・step 繧定ｶ・∴繧九◆縺ｳ縺ｫ step 蜊倅ｽ阪〒遒ｺ螳・
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

// 蝗櫁ｻ｢驥上ｒ繧ｹ繝翫ャ繝怜腰菴阪↓荳ｸ繧√※compo縺ｫ邏ｯ遨榊刈邂励☆繧・
void TRS::SnapRotation(float& compo, float delta)
{
    // 繝輔Ξ繝ｼ繝縺斐→縺ｮ delta 繧堤ｴｯ遨阪＠縲√せ繝翫ャ繝怜ｹ・step 繧定ｶ・∴繧九◆縺ｳ縺ｫ step 蜊倅ｽ阪〒遒ｺ螳・
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

// 繝槭え繧ｹ遘ｻ蜍暮㍼繧呈欠螳夊ｻｸ縺ｮ繧ｹ繧ｯ繝ｪ繝ｼ繝ｳ謚募ｽｱ繝吶け繝医Ν縺ｫ蟆・ｽｱ縺励√Ρ繝ｼ繝ｫ繝臥ｩｺ髢薙・螟牙喧驥上ｒ霑斐☆
float TRS::AddTransform(Axis axis, const VECTOR3& objPos)
{
    auto device = GameDevice();
    DIMOUSESTATE mouse = device->m_pDI->GetMouseState();
    if (mouse.lX == 0 && mouse.lY == 0) return 0.0f;

    // 霆ｸ縺ｮ譁ｹ蜷代・繧ｯ繝医Ν
    VECTOR3 axisDir = {};
    if (axis == Axis::X) axisDir = VECTOR3(1, 0, 0);
    if (axis == Axis::Y) axisDir = VECTOR3(0, 1, 0);
    if (axis == Axis::Z) axisDir = VECTOR3(0, 0, 1);

    float screenW = static_cast<float>(device->m_pD3D->m_dwWindowWidth);
    float screenH = static_cast<float>(device->m_pD3D->m_dwWindowHeight);
    MATRIX4X4 viewProj = device->m_mView * device->m_mProj;

    // 繝ｯ繝ｼ繝ｫ繝牙ｺｧ讓・竊・繧ｹ繧ｯ繝ｪ繝ｼ繝ｳ蠎ｧ讓吶↓謚募ｽｱ
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

    // 繧ｪ繝悶ず繧ｧ繧ｯ繝井ｽ咲ｽｮ縺ｨ霆ｸ譁ｹ蜷・蜊倅ｽ榊・繧偵せ繧ｯ繝ｪ繝ｼ繝ｳ縺ｫ謚募ｽｱ
    VECTOR2 p1 = projectToScreen(objPos);
    VECTOR2 p2 = projectToScreen(objPos + axisDir);

    // 繧ｹ繧ｯ繝ｪ繝ｼ繝ｳ遨ｺ髢薙〒縺ｮ霆ｸ繝吶け繝医Ν
    float sx = p2.x - p1.x;
    float sy = p2.y - p1.y;
    float lenSq = sx * sx + sy * sy;
    if (lenSq < kAxisScreenLenEpsilon) return 0.0f;

    // 繝槭え繧ｹ遘ｻ蜍輔ｒ繧ｹ繧ｯ繝ｪ繝ｼ繝ｳ霆ｸ譁ｹ蜷代↓蟆・ｽｱ (projected/len) / len = dot / lenSq
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
    DrawImGui();
}


// ImGui縺ｧTRS繝｢繝ｼ繝牙・譖ｿ縺ｨ蜷・ｨｭ螳啅I繧偵∪縺ｨ繧√※謠冗判縺吶ｋ
void TRS::DrawImGui()
{
    ImGui::SetNextWindowPos(ImVec2(350, 10), ImGuiCond_Once);
    ImGui::Begin("MoveAmount", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

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

    ImGui::End();
}


// ImGui縺ｧ遘ｻ蜍輔せ繝翫ャ繝鈴㍼繧帝∈謚槭☆繧九Λ繧ｸ繧ｪ繝懊ち繝ｳ繧呈緒逕ｻ縺吶ｋ
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

// ImGui縺ｧ蝗櫁ｻ｢繧ｹ繝翫ャ繝鈴㍼繧帝∈謚槭☆繧九Λ繧ｸ繧ｪ繝懊ち繝ｳ繧呈緒逕ｻ縺吶ｋ
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

// ImGui縺ｧ繧ｹ繧ｱ繝ｼ繝ｫ繧ｹ繝斐・繝峨ｒ驕ｸ謚槭☆繧九Λ繧ｸ繧ｪ繝懊ち繝ｳ繧呈緒逕ｻ縺吶ｋ
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

