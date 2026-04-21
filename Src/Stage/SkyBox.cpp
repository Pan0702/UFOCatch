#include "SkyBox.h"

CSkyBox::CSkyBox(const char* meshPath)
{
    m_pMesh = new CFbxMesh();
    m_pMesh->Load(meshPath);
    SetDrawOrder(-1000);
    D3D11_RASTERIZER_DESC rdc = {};
    rdc.FillMode = D3D11_FILL_SOLID;
    rdc.CullMode = D3D11_CULL_NONE;
    rdc.DepthClipEnable = TRUE;
    GameDevice()->m_pD3D->m_pDevice->CreateRasterizerState(&rdc, &m_pRStateCullNone);
}

CSkyBox::~CSkyBox()
{
    SAFE_DELETE(m_pRStateCullNone);
}

void CSkyBox::Update()
{
    transform.position = GameDevice()->m_vEyePt;
    Object3D::Update();
}

void CSkyBox::Draw()
{
    GameDevice()->m_pD3D->m_pDeviceContext->RSSetState(m_pRStateCullNone);
    Object3D::Draw();
    GameDevice()->m_pD3D->m_pDeviceContext->RSSetState(GameDevice()->m_pD3D->m_pRStateR);
}
