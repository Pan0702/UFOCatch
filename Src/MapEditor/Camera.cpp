#include "Camera.h"
#include "StageData.h"

namespace
{
    VECTOR3 up = VECTOR3(0.0f, 1.0f, 0.0f);

    // マウスホイールの生値をズーム移動量に変換するスケール
    constexpr float WHEEL_SCALE = 100.0f;
    constexpr float SPEED_SCALE_BY_DISTANCE = 0.001f;
    constexpr float MIN_MOVE_SPEED = 0.01f;

    void UpdateView()
    {
        auto device = GameDevice();
        if (device == nullptr) return;
        device->m_mView = XMMatrixLookAtLH(device->m_vEyePt, device->m_vLookatPt, up);
    }
}


// 選択オブジェクトを仰角45度で見下ろす位置にカメラを配置する
void Camera::Focus()
{
    auto device = GameDevice();
    if (device == nullptr) return;

    StageData* stage_data = ObjectManager::FindGameObject<StageData>();
    if (stage_data == nullptr) return;

    Transform* t = stage_data->GetSelectedTransform();
    if (t == nullptr) return;

    // 現在の Eye-LookAt 距離を維持する
    VECTOR3 toEye = device->m_vEyePt - device->m_vLookatPt;
    const float dist = sqrtf(toEye.x * toEye.x + toEye.y * toEye.y + toEye.z * toEye.z);

    // 現在の水平方向（XZ平面）を取り出して正規化
    VECTOR3 horiz = VECTOR3(toEye.x, 0.0f, toEye.z);
    const float hLen = sqrtf(horiz.x * horiz.x + horiz.z * horiz.z);
    if (hLen > NEAR_ZERO_LENSQ)
        horiz = VECTOR3(horiz.x / hLen, 0.0f, horiz.z / hLen);
    else
        horiz = VECTOR3(1.0f, 0.0f, 0.0f); // 真上にいた場合のフォールバック

    // 仰角45度: cos45° = sin45° = 1/√2 なので水平・垂直成分が等しい
    const float COS_45 = cosf(XM_PI / 4.0f);
    device->m_vLookatPt = t->position;
    device->m_vEyePt = t->position
        + horiz * (dist * COS_45) // 水平成分
        + VECTOR3(0.0f, dist * COS_45, 0.0f); // 垂直成分

    device->m_mView = XMMatrixLookAtLH(device->m_vEyePt, device->m_vLookatPt, up);
}

// WASDキーで前後左右にカメラを平行移動する
void Camera::Move()
{
    auto device = GameDevice();
    if (device == nullptr) return;

    // カメラの向きからワールド空間の前方・右方向を計算
    const VECTOR3 forward = normalize((device->m_vLookatPt - device->m_vEyePt));
    const VECTOR3 right = XMVector3Normalize(XMVector3Cross(up, forward));

    // 押されたキーに対応する方向を加算
    VECTOR3 move = VECTOR3(0.0f, 0.0f, 0.0f);
    auto input = device->m_pDI;
    if (input->CheckKey(KD_DAT, DIK_W)) move = move + forward;
    if (input->CheckKey(KD_DAT, DIK_S)) move = move - forward;
    if (input->CheckKey(KD_DAT, DIK_A)) move = move - right;
    if (input->CheckKey(KD_DAT, DIK_D)) move = move + right;

    // Eye と LookAt を同量移動して視点方向を維持
    constexpr float speed = 0.1f;
    device->m_vEyePt = device->m_vEyePt + move * speed;
    device->m_vLookatPt = device->m_vLookatPt + move * speed;

    device->m_mView = XMMatrixLookAtLH(device->m_vEyePt, device->m_vLookatPt, up);
}

// マウス移動量に応じてカメラの注視点を回転する
void Camera::Rotate()
{
    auto device = GameDevice();
    if (device == nullptr) return;

    DIMOUSESTATE mouse = device->m_pDI->GetMouseState();

    const VECTOR3 forward = normalize(device->m_vLookatPt - device->m_vEyePt);
    const VECTOR3 right = XMVector3Normalize(XMVector3Cross(up, forward));

    // lX（水平移動）は right 軸方向、lY（垂直移動）は -up 方向に注視点をずらす
    constexpr float speed = 0.005f;
    const VECTOR3 move = right * static_cast<float>(mouse.lX) * speed
        - up * static_cast<float>(mouse.lY) * speed;

    device->m_vLookatPt = device->m_vLookatPt + move;

    device->m_mView = XMMatrixLookAtLH(
        device->m_vEyePt,
        device->m_vLookatPt,
        up
    );
}

// マウスホイールでカメラを前後方向にズームする
void Camera::Zoom()
{
    auto device = GameDevice();
    if (device == nullptr) return;

    const VECTOR3 forward = normalize(device->m_vLookatPt - device->m_vEyePt);
    const float mouse_wheel = device->m_pDI->GetMouseWheel() / WHEEL_SCALE;

    const VECTOR3 move = forward * mouse_wheel;

    device->m_vEyePt = device->m_vEyePt + move;
    // Eye を動かした後、LookAt を Eye + forward に再計算して視点距離を維持
    device->m_vLookatPt = device->m_vEyePt + forward;

    device->m_mView = XMMatrixLookAtLH(device->m_vEyePt, device->m_vLookatPt,
                                       up);
}

// Alt+左ドラッグで注視点を中心にカメラを回転する
void Camera::Orbit()
{
    auto device = GameDevice();
    if (device == nullptr) return;

    const DIMOUSESTATE mouse = device->m_pDI->GetMouseState();
    VECTOR3 offset = device->m_vEyePt - device->m_vLookatPt;
    if (offset.LengthSquare() <= NEAR_ZERO_LENSQ) return;

    const VECTOR3 forward = normalize(device->m_vLookatPt - device->m_vEyePt);
    const VECTOR3 right = XMVector3Normalize(XMVector3Cross(up, forward));

    constexpr float SPEED = 0.005f;
    const XMMATRIX yaw = XMMatrixRotationAxis(up, static_cast<float>(mouse.lX) * SPEED);
    const XMMATRIX pitch = XMMatrixRotationAxis(right, static_cast<float>(mouse.lY) * SPEED);

    offset = XMVector3TransformNormal(offset, pitch * yaw);
    device->m_vEyePt = device->m_vLookatPt + offset;
    UpdateView();
}

// Alt+中ドラッグでカメラを平行移動する
void Camera::Pan()
{
    auto device = GameDevice();
    if (device == nullptr) return;

    const DIMOUSESTATE mouse = device->m_pDI->GetMouseState();
    const VECTOR3 forward = normalize(device->m_vLookatPt - device->m_vEyePt);
    const VECTOR3 right = XMVector3Normalize(XMVector3Cross(up, forward));

    const float distance = (device->m_vLookatPt - device->m_vEyePt).Length();
    const float scaledSpeed = distance * SPEED_SCALE_BY_DISTANCE;
    const float speed = scaledSpeed > MIN_MOVE_SPEED ? scaledSpeed : MIN_MOVE_SPEED;
    const VECTOR3 move = right * -static_cast<float>(mouse.lX) * speed
        + up * static_cast<float>(mouse.lY) * speed;

    device->m_vEyePt = device->m_vEyePt + move;
    device->m_vLookatPt = device->m_vLookatPt + move;
    UpdateView();
}

// Alt+右ドラッグでカメラを前後移動する
void Camera::Dolly()
{
    auto device = GameDevice();
    if (device == nullptr) return;

    const DIMOUSESTATE mouse = device->m_pDI->GetMouseState();
    const VECTOR3 forward = normalize(device->m_vLookatPt - device->m_vEyePt);

    const float distance = (device->m_vLookatPt - device->m_vEyePt).Length();
    const float scaledSpeed = distance * 0.01f;
    const float speed = scaledSpeed > 0.05f ? scaledSpeed : 0.05f;
    const VECTOR3 move = forward * static_cast<float>(mouse.lX + mouse.lY) * speed;

    device->m_vEyePt = device->m_vEyePt + move;
    UpdateView();
}
