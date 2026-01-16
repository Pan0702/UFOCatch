#include "PCamera.h"
#include "Player.h"

namespace
{
    const VECTOR3 INIT_UP_DIR = VECTOR3(0, 1, 0);
    const VECTOR3 INIT_CAM_POS = VECTOR3(0, 8, -4);
    const VECTOR3 INIT_CAM_LOOK = VECTOR3(0, 1, -1.5);
    const VECTOR3 INIT_SUCTION_CAM_POS = VECTOR3(0, 4, -7);
    constexpr float REFERENCE_HEIGHT = 5.0f; // 基準高さ
}

CPlayerCamera::CPlayerCamera()
{
    m_camLook = INIT_CAM_LOOK;
    m_camPos = INIT_CAM_POS;
}

CPlayerCamera::~CPlayerCamera()
{
}

void CPlayerCamera::Update()
{
    ImGui::Begin("Camera");
    ImGui::Text("Camera Pos: %f, %f, %f", t1.x, t1.y, t1.z);
    ImGui::Text("Camera Pos: %f, %f, %f", t2.x, t2.y, t2.z);
    ImGui::SliderFloat("point1.x",&t1.x, -10, 10);
    ImGui::SliderFloat("point1.y",&t1.y, -10, 10);
    ImGui::SliderFloat("point1.z",&t1.z, -10, 10);
    ImGui::SliderFloat("point2.x",&t2.x, -10, 10);
    ImGui::SliderFloat("point2.y",&t2.y, -10, 10);
    ImGui::SliderFloat("point2.z",&t2.z, -10, 10);
    if ( ImGui::Button("reset,t1"))
    {
        t1 = VECTOR3(0, 0, 0);
    }    if ( ImGui::Button("reset,t2"))
    {
        t2 = VECTOR3(0, 0, 0);
    }
        
    ImGui::End();
    UpdateCameraBezier();
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
            m_camPos = m_camPosBezier.Update(SceneManager::DeltaTime());
        }
        if (m_camLookBezier.IsAnimating())
        {
             m_camLook = m_camLookBezier.Update(SceneManager::DeltaTime());
        }
}

////////////////////
// カメラ位置を設定する
// @param pos プレイヤーの位置
// @param coneHeight コーンの高さ //
////////////////////
void CPlayerCamera::PosSet(const VECTOR3& pos, const float& coneHeight)
{
    // コーンの高さに応じてカメラ距離をスケーリング
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
    VECTOR3 startPos = m_camPos;
    VECTOR3 targetPos = pos + INIT_SUCTION_CAM_POS;

    VECTOR3 startLook = m_camLook;
    VECTOR3 targetLook = pos + VECTOR3(0, -pos.y / 2, 0);

    // ===== ここで制御点を決める（カメラ位置） =====
    VECTOR3 mid = (startPos + targetPos) * 0.5f;
    float distance = (targetPos - startPos).Length();
    float heightOffset = distance * 0.5f;  // 弧の高さ調整

    VECTOR3 controlPoint1 = startPos + (mid - startPos) * 0.5f;
    controlPoint1 += t1;  // 上に持ち上げる

    VECTOR3 controlPoint2 = targetPos + (mid - targetPos) * 0.5f;
    controlPoint2 += t2;  // 上に持ち上げる

    // ===== ここで制御点を決める（カメラ注視点） =====
    VECTOR3 midLook = (startLook + targetLook) * 0.5f;
    float heightOffsetLook = heightOffset * 0.5f;

    VECTOR3 controlPointLook1 = startLook + (midLook - startLook) * 0.5f;
    controlPointLook1 += t1;

    VECTOR3 controlPointLook2 = targetLook + (midLook - targetLook) * 0.5f;
    controlPointLook2 += t2;

    m_camPosBezier.StartWithControlPoints(startPos, controlPoint1, controlPoint2, targetPos, 0.5f);
    m_camLookBezier.StartWithControlPoints(startLook, controlPointLook1, controlPointLook2, targetLook, 0.5f);
}

////////////////////
// カメラをズームアウトさせる
// @param pos プレイヤーの位置 //
////////////////////
void CPlayerCamera::ZoomOut(const VECTOR3& pos)
{
    // カメラオフセットをスケーリング
    float scale = pos.y / REFERENCE_HEIGHT;
    VECTOR3 scaledCamOffset = INIT_CAM_POS * scale;

    VECTOR3 startPos = m_camPos;
    VECTOR3 targetPos = pos + scaledCamOffset;

    VECTOR3 startLook = m_camLook;
    VECTOR3 targetLook = pos + INIT_CAM_LOOK;

    // ===== ここで制御点を決める（カメラ位置） =====
    VECTOR3 mid = (startPos + targetPos) * 0.5f;
    float distance = (targetPos - startPos).LengthSquare();
    float heightOffset = distance * 0.5f;  // 弧の高さ調整

    VECTOR3 controlPoint1 = startPos + (mid - startPos) * 0.5f;
    controlPoint1.y += heightOffset;  // 上に持ち上げる

    VECTOR3 controlPoint2 = targetPos + (mid - targetPos) * 0.5f;
    controlPoint2.y += heightOffset;  // 上に持ち上げる

    // ===== ここで制御点を決める（カメラ注視点） =====
    VECTOR3 midLook = (startLook + targetLook) * 0.5f;
    float heightOffsetLook = heightOffset * 0.5f;

    VECTOR3 controlPointLook1 = startLook + (midLook - startLook) * 0.5f;
    controlPointLook1 += t1;

    VECTOR3 controlPointLook2 = targetLook + (midLook - targetLook) * 0.5f;
    controlPointLook2 += t2;

    m_camPosBezier.StartWithControlPoints(startPos, controlPoint1, controlPoint2, targetPos, 0.5f);
    m_camLookBezier.StartWithControlPoints(startLook, controlPointLook1, controlPointLook2, targetLook, 0.5f);
}
