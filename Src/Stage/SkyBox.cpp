#include "SkyBox.h"

CSkyBox::CSkyBox(const char* meshPath)
{
    m_pMesh = new CFbxMesh();
    m_pMesh->Load(meshPath);
    if (m_pMesh == nullptr)
    {
        MessageBox(0, _T("NonSky"), nullptr, MB_OK);
    }
    transform.scale = VECTOR3(10, 10, 10);
    SetDrawOrder(-1000);
    D3D11_RASTERIZER_DESC rdc = {};
    rdc.FillMode = D3D11_FILL_SOLID;
    rdc.CullMode = D3D11_CULL_NONE;
    rdc.DepthClipEnable = TRUE;
    GameDevice()->m_pD3D->m_pDevice->CreateRasterizerState(&rdc, &m_pRStateCullNone);
}

CSkyBox::~CSkyBox()
{
}

void CSkyBox::Update()
{
    transform.position = GameDevice()->m_vEyePt;
    Object3D::Update();
}

void CSkyBox::Draw()
{
    GameDevice()->m_pD3D->m_pDeviceContext->RSSetState(m_pRStateCullNone);
    if (m_pMesh != nullptr)
    {
        const VECTOR3 flatLightDir = VECTOR3(0, 0, 0);

        m_pMesh->Render(
            transform.matrix(),
            GameDevice()->m_mView,
            GameDevice()->m_mProj,
            flatLightDir,
            GameDevice()->m_vEyePt
        );
    }
    GameDevice()->m_pD3D->m_pDeviceContext->RSSetState(GameDevice()->m_pD3D->m_pRStateR);
}
