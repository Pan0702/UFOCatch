#include "MouseRay.h"

Ray MouseRay::Create()
{
    Ray ray{};

    auto device = GameDevice();
    if (!device) return ray;

    //  マウス座標からスクリーン座標を取得 
    POINT mousePos = device->m_pDI->GetMousePos();
    const float screenW = static_cast<float>(device->m_pD3D->m_dwWindowWidth);
    const float screenH = static_cast<float>(device->m_pD3D->m_dwWindowHeight);

    // NDC座標を計算 
    const float ndcX = (2.0f * static_cast<float>(mousePos.x) / screenW) - 1.0f;
    const float ndcY = -(2.0f * static_cast<float>(mousePos.y) / screenH) + 1.0f;

    // 投影行列からビュー空間のレイ方向を計算
    // proj._11 = 1/(tan(fovY/2)*aspect), proj._22 = 1/tan(fovY/2)
    const MATRIX4X4 proj = device->m_mProj;
    const float viewX = ndcX / proj._11;
    const float viewY = ndcY / proj._22;
    // ビュー空間レイ方向（左手座標系。Z正方向が奥）
    const XMVECTOR rayDirView = XMVectorSet(viewX, viewY, 1.0f, 0.0f);

    // ビュー行列の逆行列でワールド空間に変換
    const XMMATRIX invView = XMMatrixInverse(nullptr, device->m_mView);
    const XMVECTOR rayDirWorld = XMVector3TransformNormal(rayDirView, invView);
    const XMVECTOR rayDirWorldNorm = XMVector3Normalize(rayDirWorld);

    ray.origin = device->m_vEyePt;
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
    //Rayと球と交差判定
    const VECTOR3 oc = ray.origin - sphere.center;
    const float a = Dot(ray.direction, ray.direction);
    const float b = 2.0f * Dot(oc, ray.direction);
    const float c = Dot(oc, oc) - sphere.radius * sphere.radius;
    const float disc = b * b - 4.0f * a * c;
    return disc >= 0.0f;
}
