#include "Camera.h"
#include "StageData.h"

namespace
{
    VECTOR3 up = VECTOR3(0.0f, 1.0f, 0.0f);

    // 繝槭え繧ｹ繝帙う繝ｼ繝ｫ縺ｮ逕溷､繧偵ぜ繝ｼ繝遘ｻ蜍暮㍼縺ｫ螟画鋤縺吶ｋ繧ｹ繧ｱ繝ｼ繝ｫ
    constexpr float WHEEL_SCALE = 100.0f;
}

Camera::Camera()
{
}

Camera::~Camera() = default;

void Camera::Update()
{
}

// 驕ｸ謚槭が繝悶ず繧ｧ繧ｯ繝医ｒ莉ｰ隗・5蠎ｦ縺ｧ隕倶ｸ九ｍ縺吩ｽ咲ｽｮ縺ｫ繧ｫ繝｡繝ｩ繧帝・鄂ｮ縺吶ｋ
void Camera::Focus()
{
    auto device = GameDevice();
    if (device == nullptr) return;

    StageData* stage_data = ObjectManager::FindGameObject<StageData>();
    if (stage_data == nullptr) return;

    Transform* t = stage_data->GetSelectedTransform();
    if (t == nullptr) return;

    // 迴ｾ蝨ｨ縺ｮ Eye-LookAt 霍晞屬繧堤ｶｭ謖√☆繧・
    VECTOR3 toEye = device->m_vEyePt - device->m_vLookatPt;
    const float dist = sqrtf(toEye.x * toEye.x + toEye.y * toEye.y + toEye.z * toEye.z);

    // 迴ｾ蝨ｨ縺ｮ豌ｴ蟷ｳ譁ｹ蜷托ｼ・Z蟷ｳ髱｢・峨ｒ蜿悶ｊ蜃ｺ縺励※豁｣隕丞喧
    VECTOR3 horiz = VECTOR3(toEye.x, 0.0f, toEye.z);
    const float hLen = sqrtf(horiz.x * horiz.x + horiz.z * horiz.z);
    if (hLen > 0.0001f)
        horiz = VECTOR3(horiz.x / hLen, 0.0f, horiz.z / hLen);
    else
        horiz = VECTOR3(1.0f, 0.0f, 0.0f); // 逵滉ｸ翫↓縺・◆蝣ｴ蜷医・繝輔か繝ｼ繝ｫ繝舌ャ繧ｯ

    // 莉ｰ隗・5蠎ｦ: cos45ﾂｰ = sin45ﾂｰ = 1/竏・ 縺ｪ縺ｮ縺ｧ豌ｴ蟷ｳ繝ｻ蝙ら峩謌仙・縺檎ｭ峨＠縺・
    const float COS_45 = cosf(XM_PI / 4.0f);
    device->m_vLookatPt = t->position;
    device->m_vEyePt    = t->position
                        + horiz                        * (dist * COS_45)   // 豌ｴ蟷ｳ謌仙・
                        + VECTOR3(0.0f, dist * COS_45, 0.0f);              // 蝙ら峩謌仙・

    device->m_mView = XMMatrixLookAtLH(device->m_vEyePt, device->m_vLookatPt, up);
}

// WASD繧ｭ繝ｼ縺ｧ蜑榊ｾ悟ｷｦ蜿ｳ縺ｫ繧ｫ繝｡繝ｩ繧貞ｹｳ陦檎ｧｻ蜍輔☆繧・
void Camera::Move()
{
    auto device = GameDevice();
    if (device == nullptr) return;

    // 繧ｫ繝｡繝ｩ縺ｮ蜷代″縺九ｉ繝ｯ繝ｼ繝ｫ繝臥ｩｺ髢薙・蜑肴婿繝ｻ蜿ｳ譁ｹ蜷代ｒ險育ｮ・
    const VECTOR3 forward = normalize((device->m_vLookatPt - device->m_vEyePt));
    const VECTOR3 right   = XMVector3Normalize(XMVector3Cross(up, forward));

    // 謚ｼ縺輔ｌ縺溘く繝ｼ縺ｫ蟇ｾ蠢懊☆繧区婿蜷代ｒ蜉邂・
    VECTOR3 move = VECTOR3(0.0f, 0.0f, 0.0f);
    auto input = device->m_pDI;
    if (input->CheckKey(KD_DAT, DIK_W)) move = move + forward;
    if (input->CheckKey(KD_DAT, DIK_S)) move = move - forward;
    if (input->CheckKey(KD_DAT, DIK_A)) move = move - right;
    if (input->CheckKey(KD_DAT, DIK_D)) move = move + right;

    // Eye 縺ｨ LookAt 繧貞酔驥冗ｧｻ蜍輔＠縺ｦ隕也せ譁ｹ蜷代ｒ邯ｭ謖・
    constexpr float speed = 0.1f;
    device->m_vEyePt    = device->m_vEyePt    + move * speed;
    device->m_vLookatPt = device->m_vLookatPt + move * speed;

    device->m_mView = XMMatrixLookAtLH(device->m_vEyePt, device->m_vLookatPt, up);
}

// 繝槭え繧ｹ遘ｻ蜍暮㍼縺ｫ蠢懊§縺ｦ繧ｫ繝｡繝ｩ縺ｮ豕ｨ隕也せ繧貞屓霆｢縺吶ｋ
void Camera::Rotate()
{
    auto device = GameDevice();
    if (device == nullptr) return;

    DIMOUSESTATE mouse = device->m_pDI->GetMouseState();

    const VECTOR3 forward = normalize(device->m_vLookatPt - device->m_vEyePt);
    const VECTOR3 right = XMVector3Normalize(XMVector3Cross(up, forward));

    // lX・域ｰｴ蟷ｳ遘ｻ蜍包ｼ峨・ right 霆ｸ譁ｹ蜷代〕Y・亥桙逶ｴ遘ｻ蜍包ｼ峨・ -up 譁ｹ蜷代↓豕ｨ隕也せ繧偵★繧峨☆
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

// 繝槭え繧ｹ繝帙う繝ｼ繝ｫ縺ｧ繧ｫ繝｡繝ｩ繧貞燕蠕梧婿蜷代↓繧ｺ繝ｼ繝縺吶ｋ
void Camera::Zoom()
{
    auto device = GameDevice();
    if (device == nullptr) return;

    const VECTOR3 forward = normalize(device->m_vLookatPt - device->m_vEyePt);
    const float mouse_wheel = device->m_pDI->GetMouseWheel() / WHEEL_SCALE;

    const VECTOR3 move = forward * mouse_wheel;

    device->m_vEyePt = device->m_vEyePt + move;
    // Eye 繧貞虚縺九＠縺溷ｾ後´ookAt 繧・Eye + forward 縺ｫ蜀崎ｨ育ｮ励＠縺ｦ隕也せ霍晞屬繧堤ｶｭ謖・
    device->m_vLookatPt = device->m_vEyePt + forward;

    device->m_mView = XMMatrixLookAtLH(device->m_vEyePt, device->m_vLookatPt,
                                       up);
}

