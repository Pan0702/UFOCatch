#include "MouseRay.h"

Ray MouseRay::Create()
{
    Ray ray{};

    auto device = GameDevice();
    if (!device) return ray;

    // --- 繧ｹ繧ｯ繝ｪ繝ｼ繝ｳ蠎ｧ讓吶ｒ蜿門ｾ・---
    POINT mousePos = device->m_pDI->GetMousePos();
    const float screenW  = static_cast<float>(device->m_pD3D->m_dwWindowWidth);
    const float screenH  = static_cast<float>(device->m_pD3D->m_dwWindowHeight);

    // --- NDC蠎ｧ讓・(-1縲・1) 縺ｫ螟画鋤 ---
    const float ndcX =  (2.0f * static_cast<float>(mousePos.x) / screenW) - 1.0f;
    const float ndcY = -(2.0f * static_cast<float>(mousePos.y) / screenH) + 1.0f; // Y 縺ｯ荳翫′+

    // --- 蟆・ｽｱ陦悟・縺九ｉ繝薙Η繝ｼ遨ｺ髢薙・繝ｬ繧､譁ｹ蜷代ｒ險育ｮ・---
    // proj._11 = 1/(tan(fovY/2)*aspect), proj._22 = 1/tan(fovY/2)
    const MATRIX4X4 proj = device->m_mProj;
    const float viewX = ndcX / proj._11;
    const float viewY = ndcY / proj._22;

    // 繝薙Η繝ｼ遨ｺ髢薙Ξ繧､譁ｹ蜷托ｼ亥ｷｦ謇句ｺｧ讓咏ｳｻ縲〇豁｣譁ｹ蜷代′螂･・・
    const XMVECTOR rayDirView = XMVectorSet(viewX, viewY, 1.0f, 0.0f);

    // --- 繝薙Η繝ｼ陦悟・縺ｮ騾・｡悟・縺ｧ繝ｯ繝ｼ繝ｫ繝臥ｩｺ髢薙↓螟画鋤 ---
    const XMMATRIX invView    = XMMatrixInverse(nullptr, device->m_mView);
    const XMVECTOR rayDirWorld = XMVector3TransformNormal(rayDirView, invView);
    const XMVECTOR rayDirWorldNorm = XMVector3Normalize(rayDirWorld);

    ray.origin    = device->m_vEyePt;
    ray.direction = rayDirWorldNorm;
    return ray;
}

bool MouseRay::HitTest(const Ray& ray, Object3D* obj,
                       MeshCollider::CollInfo* collOut,
                       float rayLength)
{
    if (!obj) return false;
    const VECTOR3 to = ray.origin + ray.direction * rayLength;
    return obj->HitLineToMesh(ray.origin, to, collOut);
}

bool MouseRay::HitTestSphere(const Ray& ray, const SphereCollider& sphere)
{
    // 繝ｬ繧､縺ｨ逅・・莠､蟾ｮ蛻､螳夲ｼ郁ｧ｣縺ｮ蜈ｬ蠑擾ｼ・
    const VECTOR3 oc = ray.origin - sphere.center;
    const float a    = Dot(ray.direction, ray.direction);
    const float b    = 2.0f * Dot(oc, ray.direction);
    const float c    = Dot(oc, oc) - sphere.radius * sphere.radius;
    const float disc = b * b - 4.0f * a * c;
    return disc >= 0.0f;
}
