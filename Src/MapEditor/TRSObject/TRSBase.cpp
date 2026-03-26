#include "TRSBase.h"

namespace
{
    // ギズモのスクリーン上での見かけサイズを決める係数（距離1単位あたりのワールドスケール）

}



TRSBase::TRSBase()
{
    D3D11_DEPTH_STENCIL_DESC desc = {};
    desc.DepthEnable    = true;
    desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    desc.DepthFunc      = D3D11_COMPARISON_ALWAYS;
    GameDevice()->m_pD3D->m_pDevice->CreateDepthStencilState(&desc, &m_pDepthOffState);
}

TRSBase::~TRSBase()
{
    if (m_pDepthOffState)
    {
        m_pDepthOffState->Release();
        m_pDepthOffState = nullptr;
    }
}

// X/Y/Z軸のギズモメッシュをすべて描画する（深度テストを無効化して常に手前に表示）
void TRSBase::Render()
{
    auto* ctx = GameDevice()->m_pD3D->m_pDeviceContext;

    // 現在の深度ステートを保存
    ID3D11DepthStencilState* prev_state = nullptr;
    UINT prev_stencil_ref = 0;
    ctx->OMGetDepthStencilState(&prev_state, &prev_stencil_ref);

    // 深度テストを無効化してギズモを必ず手前に描画
    ctx->OMSetDepthStencilState(m_pDepthOffState, 0);

    if (xInfo.mesh != nullptr)
    {
        xInfo.mesh->Render(transform.matrix());
    }
    if (zInfo.mesh != nullptr)
    {
        zInfo.mesh->Render(transform.matrix());
    }
    if (yInfo.mesh != nullptr)
    {
        yInfo.mesh->Render(transform.matrix());
    }

    // 深度ステートを元に戻す
    ctx->OMSetDepthStencilState(prev_state, prev_stencil_ref);
    if (prev_state) prev_state->Release();
}

// ギズモの表示位置を指定座標に設定する
void TRSBase::SetPosition(const VECTOR3& pos)
{
    transform.position = pos;
}

// カメラ距離に比例してギズモのスケールを更新し、常に同じ見かけサイズにする
void TRSBase::UpdateScaleByCamera(const VECTOR3& camPos)
{
    const VECTOR3 diff = camPos - transform.position;
    const float dist = diff.Length();
    constexpr float kGizmoScaleFactor = 0.4f;
    const float s = dist * kGizmoScaleFactor;
    transform.scale = VECTOR3(s, s, s);
}

// レイとX/Y/Zギズモのコライダーを判定し、当たった軸を返す
Axis TRSBase::RayHitTest(const Ray& ray, float length)
{
    const VECTOR3 to  = ray.origin + ray.direction * length;
    const MATRIX4X4 mat = transform.matrix();

    if (xInfo.coll && xInfo.coll->CheckCollisionLine(mat, ray.origin, to))
        return Axis::X;
    if (yInfo.coll && yInfo.coll->CheckCollisionLine(mat, ray.origin, to))
        return Axis::Y;
    if (zInfo.coll && zInfo.coll->CheckCollisionLine(mat, ray.origin, to))
        return Axis::Z;

    return Axis::None;
}

