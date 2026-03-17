#include "MouseRay.h"

Ray MouseRay::Create()
{
    Ray ray{};

    auto device = GameDevice();
    if (!device) return ray;

    // --- スクリーン座標を取得 ---
    POINT mousePos = device->m_pDI->GetMousePos();
    float screenW  = static_cast<float>(device->m_pD3D->m_dwWindowWidth);
    float screenH  = static_cast<float>(device->m_pD3D->m_dwWindowHeight);

    // --- NDC座標 (-1〜+1) に変換 ---
    float ndcX =  (2.0f * mousePos.x / screenW) - 1.0f;
    float ndcY = -(2.0f * mousePos.y / screenH) + 1.0f; // Y は上が+

    // --- 射影行列からビュー空間のレイ方向を計算 ---
    // proj._11 = 1/(tan(fovY/2)*aspect), proj._22 = 1/tan(fovY/2)
    MATRIX4X4 proj = device->m_mProj;
    float viewX = ndcX / proj._11;
    float viewY = ndcY / proj._22;

    // ビュー空間レイ方向（左手座標系、Z正方向が奥）
    XMVECTOR rayDirView = XMVectorSet(viewX, viewY, 1.0f, 0.0f);

    // --- ビュー行列の逆行列でワールド空間に変換 ---
    XMMATRIX invView    = XMMatrixInverse(nullptr, device->m_mView);
    XMVECTOR rayDirWorld = XMVector3TransformNormal(rayDirView, invView);
    rayDirWorld          = XMVector3Normalize(rayDirWorld);

    ray.origin    = device->m_vEyePt;
    ray.direction = rayDirWorld;
    return ray;
}

bool MouseRay::HitTest(const Ray& ray, Object3D* obj,
                       MeshCollider::CollInfo* collOut,
                       float rayLength)
{
    if (!obj) return false;
    VECTOR3 to = ray.origin + ray.direction * rayLength;
    return obj->HitLineToMesh(ray.origin, to, collOut);
}

bool MouseRay::HitTestSphere(const Ray& ray, const SphereCollider& sphere)
{
    // レイと球の交差判定（解の公式）
    VECTOR3 oc = ray.origin - sphere.center;
    float a    = Dot(ray.direction, ray.direction);
    float b    = 2.0f * Dot(oc, ray.direction);
    float c    = Dot(oc, oc) - sphere.radius * sphere.radius;
    float disc = b * b - 4.0f * a * c;
    return disc >= 0.0f;
}