#include "PCamera.h"

namespace
{
    const VECTOR3 INIT_UP_DIR = VECTOR3(0, 1, 0);
    const VECTOR3 BASE_CAM_OFFSET = VECTOR3(0, 10, -1);
    const VECTOR3 INIT_CAM_LOOK = VECTOR3(0, 1, 0); 
    const float REFERENCE_HEIGHT = 5.0f; // 基準高さ（半径3の時）
}

CPlayerCamera::CPlayerCamera()
{

}

CPlayerCamera::~CPlayerCamera()
{
}

void CPlayerCamera::Update()
{
    GameDevice()->m_mView = XMMatrixLookAtLH(
        m_camPos, m_camLook, INIT_UP_DIR);
}

void CPlayerCamera::PosSet(const VECTOR3& pos, const float& coneHeight)
{
    // コーンの高さに応じてカメラ距離をスケーリング
    // 高さ10(半径3) → 高さ20(半径6)で見かけのサイズを同じにする
    float scale = coneHeight / REFERENCE_HEIGHT;
    
    // カメラオフセットをスケーリング
    VECTOR3 scaledCamOffset;
    scaledCamOffset.x = BASE_CAM_OFFSET.x * scale;
    scaledCamOffset.y = BASE_CAM_OFFSET.y * scale;
    scaledCamOffset.z = BASE_CAM_OFFSET.z * scale;
    
    //m_camPos = pos + BASE_CAM_OFFSET;
    m_camPos = pos + scaledCamOffset;
    m_camLook = pos + INIT_CAM_LOOK;
}
