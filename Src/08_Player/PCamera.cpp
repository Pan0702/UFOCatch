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
    VECTOR3 targetPos = pos + INIT_SUCTION_CAM_POS;
    VECTOR3 targetLook = pos + VECTOR3(0, -pos.y / 2, 0);

    // 弧の高さを移動距離に応じて設定（30%くらいの高さ）
    float distance = (targetPos - m_camPos).Length();
    float heightOffset = distance * 0.3f;

    m_camPosBezier.Start(m_camPos, targetPos, 0.05f, heightOffset);
    m_camLookBezier.Start(m_camLook, targetLook, 0.05f, heightOffset * 0.5f);
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

    VECTOR3 targetPos = pos + scaledCamOffset;
    VECTOR3 targetLook = pos + INIT_CAM_LOOK;

    // 弧の高さを移動距離に応じて設定（ZoomInと同じ弧を描く）
    float distance = (targetPos - m_camPos).Length();
    float heightOffset = distance * 0.3f;

    // 現在位置から通常位置へ戻る
    m_camPosBezier.Start(m_camPos, targetPos, 0.04f, heightOffset);
    m_camLookBezier.Start(m_camLook, targetLook, 0.04f, heightOffset * 0.5f);
}
