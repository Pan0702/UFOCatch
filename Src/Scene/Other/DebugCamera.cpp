#include "DebugCamera.h"

namespace
{
    const VECTOR3 up = VECTOR3(0.0f, 1.0f, 0.0f);
    constexpr float WHEEL_SCALE = 100.0f;
    constexpr float MOVE_SPEED = 0.1f;
    constexpr float ROTATE_SPEED = 0.005f;
}


CDebugCamera::CDebugCamera()
{
    auto device = GameDevice();
    device->m_vEyePt = VECTOR3(0, 15, -15);
    device->m_vLookatPt = VECTOR3(0, 0, 0);
}

CDebugCamera::~CDebugCamera()
{
}

void CDebugCamera::Update()
{
    Move();

    // 右クリック中のみ回転（任意）
    if (GameDevice()->m_pDI->CheckMouse(KD_DAT,
                                        DIM_RBUTTON))
    {
        Rotate();
    }
    Zoom();
}

void CDebugCamera::Move()
{
    auto device = GameDevice();
    if (device == nullptr) return;

    const VECTOR3 forward =
        normalize(device->m_vLookatPt - device->m_vEyePt);
    const VECTOR3 right =
        XMVector3Normalize(XMVector3Cross(up, forward));

    VECTOR3 move = VECTOR3(0.0f, 0.0f, 0.0f);
    auto input = device->m_pDI;
    if (input->CheckKey(KD_DAT, DIK_W))
        move = move +
            forward;
    if (input->CheckKey(KD_DAT, DIK_S))
        move = move -
            forward;
    if (input->CheckKey(KD_DAT, DIK_A))
        move = move -
            right;
    if (input->CheckKey(KD_DAT, DIK_D))
        move = move +
            right;
    // 上下移動も追加（任意）
    if (input->CheckKey(KD_DAT, DIK_E))
        move = move +
            up;
    if (input->CheckKey(KD_DAT, DIK_Q))
        move = move -
            up;

    device->m_vEyePt = device->m_vEyePt + move *
        MOVE_SPEED;
    device->m_vLookatPt = device->m_vLookatPt + move *
        MOVE_SPEED;

    device->m_mView =
        XMMatrixLookAtLH(device->m_vEyePt,
                         device->m_vLookatPt, up);
}

void CDebugCamera::Rotate()
{
    auto device = GameDevice();
    if (device == nullptr) return;

    DIMOUSESTATE mouse =
        device->m_pDI->GetMouseState();

    const VECTOR3 forward =
        normalize(device->m_vLookatPt - device->m_vEyePt);
    const VECTOR3 right =
        XMVector3Normalize(XMVector3Cross(up, forward));

    const VECTOR3 move = right *
        static_cast<float>(mouse.lX) * ROTATE_SPEED
        - up *
        static_cast<float>(mouse.lY) * ROTATE_SPEED;

    device->m_vLookatPt = device->m_vLookatPt + move;
    device->m_mView =
        XMMatrixLookAtLH(device->m_vEyePt,
                         device->m_vLookatPt, up);
}

void CDebugCamera::Zoom()
{
    auto device = GameDevice();
    if (device == nullptr) return;

    const VECTOR3 forward =
        normalize(device->m_vLookatPt - device->m_vEyePt);
    const float wheel = device->m_pDI->GetMouseWheel()
        / WHEEL_SCALE;
    const VECTOR3 move = forward * wheel;

    device->m_vEyePt = device->m_vEyePt + move;
    device->m_vLookatPt = device->m_vEyePt + forward;
    device->m_mView =
        XMMatrixLookAtLH(device->m_vEyePt,
                         device->m_vLookatPt, up);
}
