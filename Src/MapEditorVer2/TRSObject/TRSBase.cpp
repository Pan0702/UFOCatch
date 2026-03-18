#include "TRSBase.h"

namespace
{
    // 繧ｮ繧ｺ繝｢縺ｮ繧ｹ繧ｯ繝ｪ繝ｼ繝ｳ荳翫〒縺ｮ隕九°縺代し繧､繧ｺ繧呈ｱｺ繧√ｋ菫よ焚・郁ｷ晞屬1蜊倅ｽ阪≠縺溘ｊ縺ｮ繝ｯ繝ｼ繝ｫ繝峨せ繧ｱ繝ｼ繝ｫ・・

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

// X/Y/Z霆ｸ縺ｮ繧ｮ繧ｺ繝｢繝｡繝・す繝･繧偵☆縺ｹ縺ｦ謠冗判縺吶ｋ・域ｷｱ蠎ｦ繝・せ繝医ｒ辟｡蜉ｹ蛹悶＠縺ｦ蟶ｸ縺ｫ謇句燕縺ｫ陦ｨ遉ｺ・・
void TRSBase::Render()
{
    auto* ctx = GameDevice()->m_pD3D->m_pDeviceContext;

    // 迴ｾ蝨ｨ縺ｮ豺ｱ蠎ｦ繧ｹ繝・・繝医ｒ菫晏ｭ・
    ID3D11DepthStencilState* prev_state = nullptr;
    UINT prev_stencil_ref = 0;
    ctx->OMGetDepthStencilState(&prev_state, &prev_stencil_ref);

    // 豺ｱ蠎ｦ繝・せ繝医ｒ辟｡蜉ｹ蛹悶＠縺ｦ繧ｮ繧ｺ繝｢繧貞ｿ・★謇句燕縺ｫ謠冗判
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

    // 豺ｱ蠎ｦ繧ｹ繝・・繝医ｒ蜈・↓謌ｻ縺・
    ctx->OMSetDepthStencilState(prev_state, prev_stencil_ref);
    if (prev_state) prev_state->Release();
}

// 繧ｮ繧ｺ繝｢縺ｮ陦ｨ遉ｺ菴咲ｽｮ繧呈欠螳壼ｺｧ讓吶↓險ｭ螳壹☆繧・
void TRSBase::SetPosition(const VECTOR3& pos)
{
    transform.position = pos;
}

// 繧ｫ繝｡繝ｩ霍晞屬縺ｫ豈比ｾ九＠縺ｦ繧ｮ繧ｺ繝｢縺ｮ繧ｹ繧ｱ繝ｼ繝ｫ繧呈峩譁ｰ縺励∝ｸｸ縺ｫ蜷後§隕九°縺代し繧､繧ｺ縺ｫ縺吶ｋ
void TRSBase::UpdateScaleByCamera(const VECTOR3& camPos)
{
    const VECTOR3 diff = camPos - transform.position;
    const float dist = diff.Length();
    constexpr float kGizmoScaleFactor = 0.4f;
    const float s = dist * kGizmoScaleFactor;
    transform.scale = VECTOR3(s, s, s);
}

// 繝ｬ繧､縺ｨX/Y/Z繧ｮ繧ｺ繝｢縺ｮ繧ｳ繝ｩ繧､繝繝ｼ繧貞愛螳壹＠縲∝ｽ薙◆縺｣縺溯ｻｸ繧定ｿ斐☆
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

