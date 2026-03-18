#include "PCamera.h"
#include "Player.h"

namespace
{
    const VECTOR3 INIT_UP_DIR = VECTOR3(0, 1, 0);
    const VECTOR3 INIT_CAM_POS = VECTOR3(0, 8, -4);
    const VECTOR3 INIT_CAM_LOOK = VECTOR3(0, 1, -1.5);
    const VECTOR3 INIT_SUCTION_CAM_POS = VECTOR3(0, 4, -7);
    constexpr float REFERENCE_HEIGHT = 5.0f; // 蝓ｺ貅夜ｫ倥＆
    const VECTOR3 offsetPoint1 = VECTOR3(0, 17.1f, -10.3f);
    const VECTOR3 offsetPoint2 = VECTOR3(0, 2.3f, -10.3f);
}

CPlayerCamera::CPlayerCamera()
{
    m_camLook = INIT_CAM_LOOK;
    m_camPos = INIT_CAM_POS;
    state = 1;
    m_animStartPlayerPos = VECTOR3(0, 0, 0);
    m_playerOffset = VECTOR3(0, 0, 0);
}

CPlayerCamera::~CPlayerCamera()
{
}

void CPlayerCamera::Update()
{
   // DebugImGui();
    UpdateCameraBezier();
    GameDevice()->m_vEyePt = m_camPos;  // 繧ｫ繝｡繝ｩ菴咲ｽｮ繧呈峩譁ｰ
    GameDevice()->m_vLookatPt = m_camLook;
    GameDevice()->m_mView = XMMatrixLookAtLH(
        m_camPos, m_camLook, INIT_UP_DIR);
}

////////////////////
// 繝吶ず繧ｧ譖ｲ邱壹〒繧ｫ繝｡繝ｩ繧呈峩譁ｰ縺吶ｋ //
////////////////////
void CPlayerCamera::UpdateCameraBezier()
{
    if (m_camPosBezier.IsAnimating())
    {
        m_camPos = m_camPosBezier.Update(SceneManager::DeltaTime()) + m_playerOffset;
    }
    if (m_camLookBezier.IsAnimating())
    {
        m_camLook = m_camLookBezier.Update(SceneManager::DeltaTime()) + m_playerOffset;
    }
    else
    {
        // 繧｢繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ邨ゆｺ・凾縺ｫ繧ｪ繝輔そ繝・ヨ繧偵Μ繧ｻ繝・ヨ
        m_playerOffset = VECTOR3(0, 0, 0);
    }
}

////////////////////
// 繧ｫ繝｡繝ｩ菴咲ｽｮ繧定ｨｭ螳壹☆繧・
// @param pos 繝励Ξ繧､繝､繝ｼ縺ｮ菴咲ｽｮ
// @param coneHeight 繧ｳ繝ｼ繝ｳ縺ｮ鬮倥＆ //
////////////////////
void CPlayerCamera::PosSet(const VECTOR3& pos, const float& coneHeight)
{
    // 繝吶ず繧ｨ繧｢繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ荳ｭ縺ｯ蟾ｮ蛻・〒霑ｽ蠕・
    if (m_camPosBezier.IsAnimating() || m_camLookBezier.IsAnimating())
    {
        m_playerOffset = pos - m_animStartPlayerPos;
        return;
    }

    // 繧ｳ繝ｼ繝ｳ縺ｮ鬮倥＆縺ｫ蠢懊§縺ｦ繧ｫ繝｡繝ｩ霍晞屬繧偵せ繧ｱ繝ｼ繝ｪ繝ｳ繧ｰ//
    float scale = coneHeight / REFERENCE_HEIGHT;
    VECTOR3 scaledCamOffset = INIT_CAM_POS * scale;

    m_camPos = pos + scaledCamOffset;
    m_camLook = pos + INIT_CAM_LOOK;
}

////////////////////
// 繧ｫ繝｡繝ｩ繧偵ぜ繝ｼ繝繧､繝ｳ縺輔○繧・
// @param pos 繝励Ξ繧､繝､繝ｼ縺ｮ菴咲ｽｮ //
////////////////////
void CPlayerCamera::ZoomIn(const VECTOR3& pos)
{
    if (state == zoomIn) return;
    m_animStartPlayerPos = pos;  // 繧｢繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ髢句ｧ区凾縺ｮ繝励Ξ繧､繝､繝ｼ菴咲ｽｮ繧定ｨ倬鹸
    m_playerOffset = VECTOR3(0, 0, 0);

    VECTOR3 startPos = m_camPos;
    VECTOR3 targetPos = pos + INIT_SUCTION_CAM_POS;

    VECTOR3 startLook = m_camLook;
    VECTOR3 targetLook = pos + VECTOR3(0, -(pos.y / 2), 0);

    // // 繝・ヰ繝・げ逕ｨ縺ｫ菫晏ｭ・/
    // m_debugStartLook = startLook;
    // m_debugTargetLook = targetLook;

    // 蛻ｶ蠕｡轤ｹ縺ｯ繝励Ξ繧､繝､繝ｼ菴咲ｽｮ繧貞渕貅悶↓繧ｪ繝輔そ繝・ヨ//
    VECTOR3 controlPoint1 = startLook  + offsetPoint1;
    VECTOR3 controlPoint2 = targetLook + offsetPoint2;
    

    m_camPosBezier.Start(startPos, controlPoint1, controlPoint2, targetPos, 0.7f);
    m_camLookBezier.Start(startLook, targetLook, 0.7f);
    state = zoomIn;
}

////////////////////
// 繧ｫ繝｡繝ｩ繧偵ぜ繝ｼ繝繧｢繧ｦ繝医＆縺帙ｋ
// @param pos 繝励Ξ繧､繝､繝ｼ縺ｮ菴咲ｽｮ //
////////////////////
void CPlayerCamera::ZoomOut(const VECTOR3& pos)
{
    if (state == zoomOut) return;
    m_animStartPlayerPos = pos;  // 繧｢繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ髢句ｧ区凾縺ｮ繝励Ξ繧､繝､繝ｼ菴咲ｽｮ繧定ｨ倬鹸
    m_playerOffset = VECTOR3(0, 0, 0);

    // 繧ｫ繝｡繝ｩ繧ｪ繝輔そ繝・ヨ繧偵せ繧ｱ繝ｼ繝ｪ繝ｳ繧ｰ
    float scale = pos.y / REFERENCE_HEIGHT;
    VECTOR3 scaledCamOffset = INIT_CAM_POS * scale;

    VECTOR3 startPos = m_camPos;
    VECTOR3 targetPos = pos + scaledCamOffset;

    VECTOR3 startLook = m_camLook;
    VECTOR3 targetLook = pos + INIT_CAM_LOOK;

    // 蛻ｶ蠕｡轤ｹ縺ｯ繝励Ξ繧､繝､繝ｼ菴咲ｽｮ繧貞渕貅悶↓繧ｪ繝輔そ繝・ヨ//
    VECTOR3 controlPoint1 = pos + offsetPoint2 ;
    VECTOR3 controlPoint2 = pos + offsetPoint1;

    m_camPosBezier.Start(startPos, controlPoint1, controlPoint2, targetPos, 0.5f);
    m_camLookBezier.Start(startLook, targetLook, 0.5f);
    state = zoomOut;
}

#if 0

// 蛻ｶ蠕｡轤ｹDebug逕ｨ //
void CPlayerCamera::DebugImGui()
{
        ImGui::Begin("Camera Bezier Control");

    // === ZoomIn 蛻ｶ蠕｡轤ｹ ===
    if (ImGui::CollapsingHeader("ZoomIn Control Points", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::Text("Camera Position Control Points:");
        ImGui::SliderFloat3("Ctrl1 (Pos)", &m_zoomInCtrl1.x, -20.0f, 20.0f);
        ImGui::SliderFloat3("Ctrl2 (Pos)", &m_zoomInCtrl2.x, -20.0f, 20.0f);

        ImGui::Separator();
        ImGui::Text("Look At Control Points:");
        ImGui::SliderFloat3("Ctrl1 (Look)", &m_zoomInLookCtrl1.x, -20.0f, 20.0f);
        ImGui::SliderFloat3("Ctrl2 (Look)", &m_zoomInLookCtrl2.x, -20.0f, 20.0f);
    }

    // === ZoomOut 蛻ｶ蠕｡轤ｹ ===
    if (ImGui::CollapsingHeader("ZoomOut Control Points"))
    {
        ImGui::Text("Camera Position Control Points:");
        ImGui::SliderFloat3("Ctrl1 (Pos)##Out", &m_zoomOutCtrl1.x, -20.0f, 20.0f);
        ImGui::SliderFloat3("Ctrl2 (Pos)##Out", &m_zoomOutCtrl2.x, -20.0f, 20.0f);

        ImGui::Separator();
        ImGui::Text("Look At Control Points:");
        ImGui::SliderFloat3("Ctrl1 (Look)##Out", &m_zoomOutLookCtrl1.x, -20.0f, 20.0f);
        ImGui::SliderFloat3("Ctrl2 (Look)##Out", &m_zoomOutLookCtrl2.x, -20.0f, 20.0f);
    }

    // === 迴ｾ蝨ｨ縺ｮ繧ｫ繝｡繝ｩ諠・ｱ ===
    ImGui::Separator();
    ImGui::Text("Current Camera Pos: %.2f, %.2f, %.2f", m_camPos.x, m_camPos.y, m_camPos.z);
    ImGui::Text("Current Camera Look: %.2f, %.2f, %.2f", m_camLook.x, m_camLook.y, m_camLook.z);

    // === 繝・ヰ繝・げ・壼ｧ狗せ繝ｻ邨らせ ===
    ImGui::Separator();
    ImGui::Text("DEBUG - Look Animation:");
    ImGui::Text("  Start:  %.2f, %.2f, %.2f", m_debugStartLook.x, m_debugStartLook.y, m_debugStartLook.z);
    ImGui::Text("  Target: %.2f, %.2f, %.2f", m_debugTargetLook.x, m_debugTargetLook.y, m_debugTargetLook.z);
    ImGui::Text("  Animating: %s", m_camLookBezier.IsAnimating() ? "Yes" : "No");

    if (ImGui::Button("Reset1"))
    {
        m_zoomInCtrl1 = VECTOR3(0, 0, 0);
        m_zoomInCtrl2 = VECTOR3(0, 0, 0);
        m_zoomOutCtrl2 = VECTOR3(0, 0, 0);
        m_zoomOutCtrl1 = VECTOR3(0, 0, 0);
    }
    ImGui::End();
}
#endif

