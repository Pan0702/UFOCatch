#include "PCamera.h"

#include "Player.h"

namespace
{
    const VECTOR3 INIT_UP_DIR = VECTOR3(0, 1, 0);
    const VECTOR3 INIT_CAM_POS = VECTOR3(0, 10, -5);
    const VECTOR3 INIT_CAM_LOOK = VECTOR3(0, 1, 0);
    const VECTOR3 INIT_SUCTION_CAM_POS = VECTOR3(0, 2, -7);
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
    UpdateCameraLerp();
    GameDevice()->m_mView = XMMatrixLookAtLH(
        m_camPos, m_camLook, INIT_UP_DIR);
}

void CPlayerCamera::UpdateCameraLerp()
{
        if (m_camPosLerp.IsLerping())
        {
            m_camPos = m_camPosLerp.Update(SceneManager::DeltaTime());
        }    
        if (m_camPosLerp.IsLerping())
        {
             m_camLook = m_camLookLerp.Update(SceneManager::DeltaTime());
        }
}

void CPlayerCamera::PosSet(const VECTOR3& pos, const float& coneHeight)
{
    // コーンの高さに応じてカメラ距離をスケーリング
    // 高さ10(半径3) → 高さ20(半径6)で見かけのサイズを同じにする
    float scale = coneHeight / REFERENCE_HEIGHT;

    // カメラオフセットをスケーリング
    VECTOR3 scaledCamOffset;
    scaledCamOffset.x = INIT_CAM_POS.x * scale;
    scaledCamOffset.y = INIT_CAM_POS.y * scale;
    scaledCamOffset.z = INIT_CAM_POS.z * scale;

    //m_camPos = pos + BASE_CAM_OFFSET;
    m_camPos = pos + scaledCamOffset;
    m_camLook = pos + INIT_CAM_LOOK;
}

void CPlayerCamera::ZoomIn(const VECTOR3& pos)
{
    m_camPosLerp.Start(m_camPos,pos + INIT_SUCTION_CAM_POS,0.05f);
    float len = pos.y / 2;
    m_camLookLerp.Start(m_camLook,pos + VECTOR3(0,-len,0),0.05f);
}

void CPlayerCamera::ZoomOut(const VECTOR3& pos)
{
    // PosSetと同じ計算で通常カメラ位置を求める
    float scale = pos.y / REFERENCE_HEIGHT;
    VECTOR3 scaledCamOffset;
    scaledCamOffset.x = INIT_CAM_POS.x * scale;
    scaledCamOffset.y = INIT_CAM_POS.y * scale;
    scaledCamOffset.z = INIT_CAM_POS.z * scale;

    // 現在位置から通常位置へ戻る
    m_camPosLerp.Start(m_camPos, pos + scaledCamOffset, 0.04f);
    m_camLookLerp.Start(m_camLook, pos + INIT_CAM_LOOK, 0.04f);
}
