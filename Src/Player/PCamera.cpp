#include "PCamera.h"
#include "Player.h"

namespace
{
    const VECTOR3 INIT_UP_DIR = VECTOR3(0, 1, 0);
    const VECTOR3 INIT_CAM_POS = VECTOR3(0, 8, -4);
    const VECTOR3 INIT_CAM_LOOK = VECTOR3(0, 1, -1.5);
    const VECTOR3 INIT_SUCTION_CAM_POS = VECTOR3(0, 4, -7);
    constexpr float REFERENCE_HEIGHT = 5.0f; // 基準高さ
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
    GameDevice()->m_vEyePt = m_camPos; // カメラ位置を更新
    GameDevice()->m_vLookatPt = m_camLook;
    GameDevice()->m_mView = XMMatrixLookAtLH(
        m_camPos, m_camLook, INIT_UP_DIR);
}

////////////////////
// ベジェ曲線でカメラを更新する //
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
        // アニメーション終了時にオフセットをリセット
        m_playerOffset = VECTOR3(0, 0, 0);
    }
}

////////////////////
// カメラ位置を設定する
// @param pos プレイヤーの位置
// @param coneHeight コーンの高さ //
////////////////////
void CPlayerCamera::PosSet(const VECTOR3& pos, const float& coneHeight)
{
    // ベジェアニメーション中は差分で追従
    if (m_camPosBezier.IsAnimating() || m_camLookBezier.IsAnimating())
    {
        m_playerOffset = pos - m_animStartPlayerPos;
        return;
    }

    // コーンの高さに応じてカメラ距離をスケーリング //
    float scale = coneHeight / REFERENCE_HEIGHT;
    VECTOR3 scaledCamOffset = INIT_CAM_POS * scale;

    m_camPos = pos + scaledCamOffset;
    m_camLook = pos + INIT_CAM_LOOK;
}

////////////////////
// カメラをズームインさせる
// @param pos プレイヤーの位置 //
////////////////////
void CPlayerCamera::ZoomIn(const VECTOR3& pos)
{
    if (state == zoomIn) return;
    m_animStartPlayerPos = pos; // アニメーション開始時のプレイヤー位置を記録
    m_playerOffset = VECTOR3(0, 0, 0);

    VECTOR3 startPos = m_camPos;
    VECTOR3 targetPos = pos + INIT_SUCTION_CAM_POS;

    VECTOR3 startLook = m_camLook;
    VECTOR3 targetLook = pos + VECTOR3(0, -(pos.y / 2), 0);

    // // デバッグ用に保存 //
    // m_debugStartLook = startLook;
    // m_debugTargetLook = targetLook;

    // 制御点はプレイヤー位置を基準にオフセット //
    VECTOR3 controlPoint1 = startLook + offsetPoint1;
    VECTOR3 controlPoint2 = targetLook + offsetPoint2;


    m_camPosBezier.Start(startPos, controlPoint1, controlPoint2, targetPos, 0.7f);
    m_camLookBezier.Start(startLook, targetLook, 0.7f);
    state = zoomIn;
}

////////////////////
// カメラをズームアウトさせる
// @param pos プレイヤーの位置 //
////////////////////
void CPlayerCamera::ZoomOut(const VECTOR3& pos, const float& coneHeight)
{
    if (state == zoomOut) return;
    m_animStartPlayerPos = pos; // アニメーション開始時のプレイヤー位置を記録
    m_playerOffset = VECTOR3(0, 0, 0);

    // カメラオフセットをスケーリング (PosSetと同じ計算式)
    float scale = coneHeight / REFERENCE_HEIGHT;
    VECTOR3 scaledCamOffset = INIT_CAM_POS * scale;

    VECTOR3 startPos = m_camPos;
    VECTOR3 targetPos = pos + scaledCamOffset;

    VECTOR3 startLook = m_camLook;
    VECTOR3 targetLook = pos + INIT_CAM_LOOK;

    // 制御点はプレイヤー位置を基準にオフセット //
    VECTOR3 controlPoint1 = pos + offsetPoint2;
    VECTOR3 controlPoint2 = pos + offsetPoint1;

    m_camPosBezier.Start(startPos, controlPoint1, controlPoint2, targetPos, 0.5f);
    m_camLookBezier.Start(startLook, targetLook, 0.5f);
    state = zoomOut;
}

#if 0

// 制御点Debug用 //
void CPlayerCamera::DebugImGui()
{
    ImGui::Begin("Camera Bezier Control");

    // === ZoomIn 制御点 ===
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

    // === ZoomOut 制御点 ===
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

    // === 現在のカメラ情報 ===
    ImGui::Separator();
    ImGui::Text("Current Camera Pos: %.2f, %.2f, %.2f", m_camPos.x, m_camPos.y, m_camPos.z);
    ImGui::Text("Current Camera Look: %.2f, %.2f, %.2f", m_camLook.x, m_camLook.y, m_camLook.z);

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
