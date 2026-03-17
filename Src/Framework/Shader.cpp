// ========================================================================================
//
// 繧ｷ繧ｧ繝ｼ繝繝ｼ縺ｮ蜃ｦ逅・                                              ver 3.0        2021.1.11
//
//   繧ｷ繧ｧ繝ｼ繝繝ｼ繧ｪ繝悶ず繧ｧ繧ｯ繝医・隱ｭ縺ｿ霎ｼ縺ｿ縺ｨ繧ｷ繧ｧ繝ｼ繝繝ｼ縺ｮ菴懈・
//   繧､繝ｳ繝励ャ繝医Ξ繧､繧｢繧ｦ繝医・菴懈・
//   螳壽焚繝舌ャ繝輔ぃ縺ｮ菴懈・
//
//
//    逋ｻ骭ｲ縺輔ｌ縺ｦ縺・ｋ繧ｷ繧ｧ繝ｼ繝繝ｼ
//
//     繝ｻ繧ｷ繝ｳ繝励Ν縺ｪ繧ｷ繧ｧ繝ｼ繝繝ｼ                          InitShaderSimple();
//     繝ｻ繧ｹ繝励Λ繧､繝育畑縺ｮ繧ｷ繧ｧ繝ｼ繝繝ｼ                      InitShaderSprite();
//     繝ｻFBXStaticMesh/FBXSkinMesh逕ｨ縺ｮ繧ｷ繧ｧ繝ｼ繝繝ｼ       InitShaderFbx();
//     繝ｻ繝・ぅ繧ｹ繝励Ξ繧､繧ｹ繝｡繝ｳ繝医・繝・ヴ繝ｳ繧ｰ逕ｨ縺ｮ繧ｷ繧ｧ繝ｼ繝繝ｼ  InitShaderDisplace();
//     繝ｻ繧ｨ繝輔ぉ繧ｯ繝育畑縺ｮ繧ｷ繧ｧ繝ｼ繝繝ｼ                      InitShaderEffect();
//
//                                                                              Shader.cpp
// ========================================================================================

#include "Shader.h"
#include "../Utils/FbxMesh.h"

//------------------------------------------------------------------------
//
//  繧ｷ繧ｧ繝ｼ繝繝ｼ縺ｮ繧ｳ繝ｳ繧ｹ繝医Λ繧ｯ繧ｿ  
//
//  蠑墓焚・咾Direct3D* pD3D
//
//------------------------------------------------------------------------
CShader::CShader(CDirect3D* pD3D)
{
    ZeroMemory(this, sizeof(CShader));
    m_pD3D = pD3D;
}
//------------------------------------------------------------------------
//
//  繧ｷ繧ｧ繝ｼ繝繝ｼ縺ｮ繝・せ繝医Λ繧ｯ繧ｿ    
//
//------------------------------------------------------------------------
CShader::~CShader()
{
    // 繧ｷ繧ｧ繝ｼ繝繝ｼ縺ｮ隗｣謾ｾ  ------------------------------------------

    // 騾壼ｸｸ逕ｨ縺ｮ繧ｷ繝ｳ繝励Ν縺ｪ繧ｷ繧ｧ繝ｼ繝繝ｼ
    SAFE_RELEASE(m_pSimple_VertexLayout); 
    SAFE_RELEASE(m_pSimple_VS);
    SAFE_RELEASE(m_pSimple_PS);

    // 3D繧ｹ繝励Λ繧､繝育畑縺ｮ繧ｷ繧ｧ繝ｼ繝繝ｼ
    SAFE_RELEASE(m_pSprite3D_VertexLayout);
    SAFE_RELEASE(m_pSprite3D_VS);
    SAFE_RELEASE(m_pSprite3D_PS);
    SAFE_RELEASE(m_pSprite3D_VS_BILL);

    // 繝・ぅ繧ｹ繝励Ξ繧､繧ｹ繝｡繝ｳ繝医・繝・ヴ繝ｳ繧ｰ(豕｢)逕ｨ縺ｮ繧ｷ繧ｧ繝ｼ繝繝ｼ
    SAFE_RELEASE(m_pDisplaceWave_VertexLayout);
    SAFE_RELEASE(m_pDisplaceWave_VS);
    SAFE_RELEASE(m_pDisplaceWave_HS);
    SAFE_RELEASE(m_pDisplaceWave_DS);
    SAFE_RELEASE(m_pDisplaceWave_PS);

    // 繝・ぅ繧ｹ繝励Ξ繧､繧ｹ繝｡繝ｳ繝医・繝・ヴ繝ｳ繧ｰ(繝｡繝・す繝･)逕ｨ縺ｮ繧ｷ繧ｧ繝ｼ繝繝ｼ
    SAFE_RELEASE(m_pDisplaceSkinMesh_VS);
    SAFE_RELEASE(m_pDisplaceSkinMesh_HS);
    SAFE_RELEASE(m_pDisplaceSkinMesh_DS);
    SAFE_RELEASE(m_pDisplaceSkinMesh_PS);

    SAFE_RELEASE(m_pDisplaceStaticMesh_VS);
    SAFE_RELEASE(m_pDisplaceStaticMesh_HS);
    SAFE_RELEASE(m_pDisplaceStaticMesh_DS);
    SAFE_RELEASE(m_pDisplaceStaticMesh_PS);

    // 繧ｨ繝輔ぉ繧ｯ繝育畑縺ｮ繧ｷ繧ｧ繝ｼ繝繝ｼ
    SAFE_RELEASE(m_pEffect3D_VertexLayout);
    SAFE_RELEASE(m_pEffect3D_VS_POINT);
    SAFE_RELEASE(m_pEffect3D_GS_POINT);
    SAFE_RELEASE(m_pEffect3D_PS);
    SAFE_RELEASE(m_pEffect3D_VertexLayout_BILL);
    SAFE_RELEASE(m_pEffect3D_VS_BILL);
    SAFE_RELEASE(m_pEffect3D_VS_BILLMESH);              // -- 2019.7.17

    // Fbx繝｢繝・Ν・壹せ繧ｿ繝・ぅ繝・け繝｡繝・す繝･逕ｨ縺ｮ繧ｷ繧ｧ繝ｼ繝繝ｼ
    // (Normal繝槭ャ繝斐Φ繧ｰ蟇ｾ蠢・
    SAFE_RELEASE(m_pFbxStaticMesh_VertexLayout);
    SAFE_RELEASE(m_pFbxStaticMesh_VS);
    SAFE_RELEASE(m_pFbxStaticMesh_PS);

    // Fbx繝｢繝・Ν・壹せ繧ｭ繝ｳ繝｡繝・す繝･逕ｨ縺ｮ繧ｷ繧ｧ繝ｼ繝繝ｼ
    // (Normal繝槭ャ繝斐Φ繧ｰ蟇ｾ蠢・
    SAFE_RELEASE(m_pFbxSkinMesh_VertexLayout);
    SAFE_RELEASE(m_pFbxSkinMesh_VS);
    SAFE_RELEASE(m_pFbxSkinMesh_PS);


    // 螳壽焚繝舌ャ繝輔ぃ縺ｮ隗｣謾ｾ ---------------------------

    SAFE_RELEASE(m_pConstantBufferDisplace);
    SAFE_RELEASE(m_pConstantBufferEffect);
    SAFE_RELEASE(m_pConstantBufferSprite3D);

    SAFE_RELEASE(m_pConstantBufferWVLED);
    SAFE_RELEASE(m_pConstantBufferBone2);
    SAFE_RELEASE(m_pConstantBufferMaterial);      // -- 2020.12.15

}

//------------------------------------------------------------------------
//
//  蜷・ｨｮ繧ｷ繧ｧ繝ｼ繝繝ｼ縺ｮ菴懈・   
//
//  繝ｻ繧ｷ繧ｧ繝ｼ繝繝ｼ縺ｨ螳壽焚繝舌ャ繝輔ぃ繧剃ｽ懈・縺吶ｋ
//  繝ｻ繝・け繧ｹ繝√Ε繧ｵ繝ｳ繝励Λ繝ｼ縺ｨ繝悶Ξ繝ｳ繝峨せ繝・・繝医ｒ菴懈・縺吶ｋ
//
//  蠑墓焚・壹↑縺・
//
//  謌ｻ繧雁､ HRESULT
//     S_OK   = 謌仙粥
//     E_FAIL = 逡ｰ蟶ｸ
//
//------------------------------------------------------------------------
HRESULT CShader::InitShader()
{
    // 騾壼ｸｸ繝｡繝・す繝･逕ｨ縺ｮ繧ｷ繧ｧ繝ｼ繝繝ｼ菴懈・
    InitShaderSimple();

    // Fbx繧ｹ繧ｿ繝・ぅ繝・け繝ｻ繧ｹ繧ｭ繝ｳ繝｡繝・す繝･逕ｨ縺ｮ繧ｷ繧ｧ繝ｼ繝繝ｼ菴懈・
    InitShaderFbx();

    // 繝・ぅ繧ｹ繝励Ξ繧､繧ｹ繝｡繝ｳ繝医・繝・ヴ繝ｳ繧ｰ逕ｨ縺ｮ繧ｷ繧ｧ繝ｼ繝繝ｼ菴懈・
    InitShaderDisplace();

    // 繧ｨ繝輔ぉ繧ｯ繝育畑縺ｮ繧ｷ繧ｧ繝ｼ繝繝ｼ菴懈・
    InitShaderEffect();

    // 繧ｹ繝励Λ繧､繝育畑縺ｮ繧ｷ繧ｧ繝ｼ繝繝ｼ菴懈・
    InitShaderSprite();

    // 螳壽焚繝舌ャ繝輔ぃ菴懈・
    InitShaderConstant();

    return S_OK;
}
//------------------------------------------------------------------------
//
//  騾壼ｸｸ逕ｨ(Simple Shader)縺ｮ繧ｷ繧ｧ繝ｼ繝繝ｼ菴懈・
//
//  蠑墓焚・壹↑縺・
//
//  謌ｻ繧雁､ HRESULT
//     S_OK   = 謌仙粥
//     E_FAIL = 逡ｰ蟶ｸ
//
//------------------------------------------------------------------------
HRESULT CShader::InitShaderSimple()
{

    // 鬆らせ繧､繝ｳ繝励ャ繝医Ξ繧､繧｢繧ｦ繝亥ｮ夂ｾｩ
    UINT numElements = 0;
    D3D11_INPUT_ELEMENT_DESC Simplelayout[] =
    {
       { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
       { "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
       { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    numElements = sizeof(Simplelayout) / sizeof(Simplelayout[0]);

    // 鬆らせ繧ｷ繧ｧ繝ｼ繝繝ｼ繝ｻ繝斐け繧ｻ繝ｫ繧ｷ繧ｧ繝ｼ繝繝ｼ繝ｻ鬆らせ繧､繝ｳ繝励ャ繝医Ξ繧､繧｢繧ｦ繝医・菴懈・
    MakeShader(_T("VS"), _T("Simple_VS.cso"), (void**)&m_pSimple_VS, Simplelayout, numElements, &m_pSimple_VertexLayout);
    MakeShader(_T("PS"), _T("Simple_PS.cso"), (void**)&m_pSimple_PS);

    return S_OK;
}

//------------------------------------------------------------------------
//
//  Fbx繝｢繝・Ν・医せ繧ｿ繝・ぅ繝・け繝ｻ繧ｹ繧ｭ繝ｳ繝｡繝・す繝･・臥畑縺ｮ繧ｷ繧ｧ繝ｼ繝繝ｼ菴懈・
//
//  蠑墓焚・壹↑縺・
//
//  謌ｻ繧雁､ HRESULT
//     S_OK   = 謌仙粥
//     E_FAIL = 逡ｰ蟶ｸ
//
//------------------------------------------------------------------------
HRESULT CShader::InitShaderFbx()
{

    // -------------------------------------------------------------------
    // 
    // 繧ｹ繧ｿ繝・ぅ繝・け繝｡繝・す繝･  FbxStaticMesh
    // 
    // -------------------------------------------------------------------
    // 鬆らせ繧､繝ｳ繝励ャ繝医Ξ繧､繧｢繧ｦ繝亥ｮ夂ｾｩ
    UINT numElements = 0;
    D3D11_INPUT_ELEMENT_DESC FbxStaticNM_layout[] = {
       { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
       { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
       { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },       // 險・2byte
       { "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT,0,32, D3D11_INPUT_PER_VERTEX_DATA,0 },
       { "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT,0,44, D3D11_INPUT_PER_VERTEX_DATA,0 }, // 險・6byte
    };
    numElements = sizeof(FbxStaticNM_layout) / sizeof(FbxStaticNM_layout[0]);

    // 鬆らせ繧ｷ繧ｧ繝ｼ繝繝ｼ繝ｻ繝斐け繧ｻ繝ｫ繧ｷ繧ｧ繝ｼ繝繝ｼ繝ｻ鬆らせ繧､繝ｳ繝励ャ繝医Ξ繧､繧｢繧ｦ繝医・菴懈・
    MakeShader(_T("VS"), _T("FbxStaticMesh_VS.cso"), (void**)&m_pFbxStaticMesh_VS, FbxStaticNM_layout, numElements, &m_pFbxStaticMesh_VertexLayout);
    MakeShader(_T("PS"), _T("FbxStaticMesh_PS.cso"), (void**)&m_pFbxStaticMesh_PS);


    // -------------------------------------------------------------------
    // 
    // 繧ｹ繧ｭ繝ｳ繝｡繝・す繝･  FbxSkinMesh
    // 
    // -------------------------------------------------------------------
    // 鬆らせ繧､繝ｳ繝励ャ繝医Ξ繧､繧｢繧ｦ繝亥ｮ夂ｾｩ
    numElements = 0;
    D3D11_INPUT_ELEMENT_DESC FbxSkinNM_layout[] = {
       { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
       { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
       { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
       { "BONE_INDEX", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
       { "BONE_WEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0 },
       { "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 64, D3D11_INPUT_PER_VERTEX_DATA, 0 },
       { "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 76, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // 險・6Byte
    };
    numElements = sizeof(FbxSkinNM_layout) / sizeof(FbxSkinNM_layout[0]);

    // 鬆らせ繧ｷ繧ｧ繝ｼ繝繝ｼ繝ｻ繝斐け繧ｻ繝ｫ繧ｷ繧ｧ繝ｼ繝繝ｼ繝ｻ鬆らせ繧､繝ｳ繝励ャ繝医Ξ繧､繧｢繧ｦ繝医・菴懈・
    MakeShader(_T("VS"), _T("FbxSkinMesh_VS.cso"), (void**)&m_pFbxSkinMesh_VS, FbxSkinNM_layout, numElements, &m_pFbxSkinMesh_VertexLayout);
    MakeShader(_T("PS"), _T("FbxSkinMesh_PS.cso"), (void**)&m_pFbxSkinMesh_PS);

    return S_OK;
}
//------------------------------------------------------------------------
//
//  繝・ぅ繧ｹ繝励Ξ繧､繧ｹ繝｡繝ｳ繝医・繝・ヴ繝ｳ繧ｰ逕ｨ縺ｮ繧ｷ繧ｧ繝ｼ繝繝ｼ菴懈・
//
//  蠑墓焚・壹↑縺・
//
//  謌ｻ繧雁､ HRESULT
//     S_OK   = 謌仙粥
//     E_FAIL = 逡ｰ蟶ｸ
//
//------------------------------------------------------------------------
HRESULT CShader::InitShaderDisplace()
{

    // -----------------------------------------------------------------------------------------------
    // 豕｢縺ｮ繝・ぅ繧ｹ繝励Ξ繧､繧ｹ繝槭ャ繝斐Φ繧ｰ DisplaceWave
    // -----------------------------------------------------------------------------------------------
    // 鬆らせ繧､繝ｳ繝励ャ繝医Ξ繧､繧｢繧ｦ繝亥ｮ夂ｾｩ
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
       { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
       { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
       { "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    UINT numElements = sizeof(layout) / sizeof(layout[0]);

    // 鬆らせ繝ｻ繝上Ν繝ｻ繝峨Γ繧､繝ｳ繝ｻ繝斐け繧ｻ繝ｫ繧ｷ繧ｧ繝ｼ繝繝ｼ縺翫ｈ縺ｳ鬆らせ繝ｬ繧､繧｢繧ｦ繝医・菴懈・
    MakeShader(_T("VS"), _T("DisplaceWave_VS.cso"), (void**)&m_pDisplaceWave_VS, layout, numElements, &m_pDisplaceWave_VertexLayout);
    MakeShader(_T("HS"), _T("DisplaceWave_HS.cso"), (void**)&m_pDisplaceWave_HS);
    MakeShader(_T("DS"), _T("DisplaceWave_DS.cso"), (void**)&m_pDisplaceWave_DS);
    MakeShader(_T("PS"), _T("DisplaceWave_PS.cso"), (void**)&m_pDisplaceWave_PS);


    // -----------------------------------------------------------------------------------------------
    // 繧ｹ繧ｿ繝・ぅ繝・け繝｡繝・す繝･縺ｮ繝・ぅ繧ｹ繝励Ξ繧､繧ｹ繝｡繝ｳ繝医・繝・ヴ繝ｳ繧ｰ DisplaceStaticMesh
    // -----------------------------------------------------------------------------------------------
    // 鬆らせ繝ｻ繝斐け繧ｻ繝ｫ繝ｻ繝上Ν繝ｻ繝峨Γ繧､繝ｳ繧ｷ繧ｧ繝ｼ繝繝ｼ縺ｮ菴懈・
    //  !!!!! 鬆らせ繧､繝ｳ繝励ャ繝医Ξ繧､繧｢繧ｦ繝医・縲√せ繧ｿ繝・ぅ繝・け繝｡繝・す繝･縺ｮ繝ｬ繧､繧｢繧ｦ繝医ｒ菴ｿ逕ｨ 
    MakeShader(_T("VS"), _T("DisplaceStaticMesh_VS.cso"), (void**)&m_pDisplaceStaticMesh_VS);
    MakeShader(_T("HS"), _T("DisplaceStaticMesh_HS.cso"), (void**)&m_pDisplaceStaticMesh_HS);
    MakeShader(_T("DS"), _T("DisplaceStaticMesh_DS.cso"), (void**)&m_pDisplaceStaticMesh_DS);
    MakeShader(_T("PS"), _T("DisplaceStaticMesh_PS.cso"), (void**)&m_pDisplaceStaticMesh_PS);

    // -----------------------------------------------------------------------------------------------
    // 繧ｹ繧ｭ繝ｳ繝｡繝・す繝･縺ｮ繝・ぅ繧ｹ繝励Ξ繧､繧ｹ繝｡繝ｳ繝医・繝・ヴ繝ｳ繧ｰ DisplaceSkinMesh
    // -----------------------------------------------------------------------------------------------
    // 鬆らせ繝ｻ繝斐け繧ｻ繝ｫ繝ｻ繝上Ν繝ｻ繝峨Γ繧､繝ｳ繧ｷ繧ｧ繝ｼ繝繝ｼ縺ｮ菴懈・
    //  !!!!! 鬆らせ繧､繝ｳ繝励ャ繝医Ξ繧､繧｢繧ｦ繝医・縲√せ繧ｭ繝ｳ繝｡繝・す繝･縺ｮ繝ｬ繧､繧｢繧ｦ繝医ｒ菴ｿ逕ｨ   
    MakeShader(_T("VS"), _T("DisplaceSkinMesh_VS.cso"), (void**)&m_pDisplaceSkinMesh_VS);
    MakeShader(_T("HS"), _T("DisplaceSkinMesh_HS.cso"), (void**)&m_pDisplaceSkinMesh_HS);
    MakeShader(_T("DS"), _T("DisplaceSkinMesh_DS.cso"), (void**)&m_pDisplaceSkinMesh_DS);
    MakeShader(_T("PS"), _T("DisplaceSkinMesh_PS.cso"), (void**)&m_pDisplaceSkinMesh_PS);


    return S_OK;
}

//------------------------------------------------------------------------
//
//  繧ｨ繝輔ぉ繧ｯ繝育畑縺ｮ繧ｷ繧ｧ繝ｼ繝繝ｼ菴懈・
//
//  蠑墓焚・壹↑縺・
//
//  謌ｻ繧雁､ HRESULT
//     S_OK   = 謌仙粥
//     E_FAIL = 逡ｰ蟶ｸ
//
//------------------------------------------------------------------------
HRESULT CShader::InitShaderEffect()
{

    // -------------------------------------------------------------------
    // 
    // 繝代・繝・ぅ繧ｯ繝ｫ縺ｮ繧ｷ繧ｧ繝ｼ繝繝ｼ
    // 
    // -------------------------------------------------------------------
    // 鬆らせ繧､繝ｳ繝励ャ繝医Ξ繧､繧｢繧ｦ繝亥ｮ夂ｾｩ
    UINT numElements = 0;
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
       { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    numElements = sizeof(layout) / sizeof(layout[0]);

    // 鬆らせ繝ｻ繧ｸ繧ｪ繝｡繝医Μ繧ｷ繧ｧ繝ｼ繝繝ｼ縺翫ｈ縺ｳ鬆らせ繝ｬ繧､繧｢繧ｦ繝医・菴懈・
    MakeShader(_T("VS"), _T("Effect3D_VS_POINT.cso"), (void**)&m_pEffect3D_VS_POINT, layout, numElements, &m_pEffect3D_VertexLayout);
    MakeShader(_T("GS"), _T("Effect3D_GS_POINT.cso"), (void**)&m_pEffect3D_GS_POINT);


    // -----------------------------------------------------------------------------------------------------
    // 
    // 繝薙Ν繝懊・繝峨・繧ｷ繧ｧ繝ｼ繝繝ｼ
    // 
    // -----------------------------------------------------------------------------------------------------
    // 鬆らせ繧､繝ｳ繝励ャ繝医Ξ繧､繧｢繧ｦ繝亥ｮ夂ｾｩ
    D3D11_INPUT_ELEMENT_DESC layoutbill[] =
    {
       { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
       { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    numElements = sizeof(layoutbill) / sizeof(layoutbill[0]);

    // 鬆らせ繧ｷ繧ｧ繝ｼ繝繝ｼ縺翫ｈ縺ｳ鬆らせ繝ｬ繧､繧｢繧ｦ繝医・菴懈・
    MakeShader(_T("VS"), _T("Effect3D_VS_BILL.cso"), (void**)&m_pEffect3D_VS_BILL, layoutbill, numElements, &m_pEffect3D_VertexLayout_BILL);

    // -----------------------------------------------------------------------------------------------------
    // 
    // 繝薙Ν繝懊・繝峨Γ繝・す繝･縺ｮ繧ｷ繧ｧ繝ｼ繝繝ｼ
    // 
    // -----------------------------------------------------------------------------------------------------
    // 鬆らせ繧ｷ繧ｧ繝ｼ繝繝ｼ縺ｮ菴懈・
    MakeShader(_T("VS"), _T("Effect3D_VS_BILLMESH.cso"), (void**)&m_pEffect3D_VS_BILLMESH);

    // -----------------------------------------------------------------------------------------------------
    // 
    // 蜈ｱ騾壹・繧ｷ繧ｧ繝ｼ繝繝ｼ
    // 
    // -----------------------------------------------------------------------------------------------------
    // 繝斐け繧ｻ繝ｫ繧ｷ繧ｧ繝ｼ繝繝ｼ縺ｮ菴懈・
    MakeShader(_T("PS"), _T("Effect3D_PS.cso"), (void**)&m_pEffect3D_PS);

    return S_OK;
}

//------------------------------------------------------------------------
//
//  繧ｹ繝励Λ繧､繝育畑縺ｮ繧ｷ繧ｧ繝ｼ繝繝ｼ菴懈・
//
//  蠑墓焚・壹↑縺・
//
//  謌ｻ繧雁､ HRESULT
//     S_OK   = 謌仙粥
//     E_FAIL = 逡ｰ蟶ｸ
//
//------------------------------------------------------------------------
HRESULT CShader::InitShaderSprite()
{
    // 鬆らせ繧､繝ｳ繝励ャ繝医Ξ繧､繧｢繧ｦ繝亥ｮ夂ｾｩ    
    D3D11_INPUT_ELEMENT_DESC layout_sprite[] =
    {
       { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
       { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    UINT numElements = sizeof(layout_sprite) / sizeof(layout_sprite[0]);

    // 鬆らせ繝ｻ繝斐け繧ｻ繝ｫ繧ｷ繧ｧ繝ｼ繝繝ｼ縺翫ｈ縺ｳ鬆らせ繝ｬ繧､繧｢繧ｦ繝医・菴懈・
    MakeShader(_T("VS"), _T("Sprite3D_VS.cso"), (void**)&m_pSprite3D_VS, layout_sprite, numElements, &m_pSprite3D_VertexLayout);
    MakeShader(_T("VS"), _T("Sprite3D_VS_BILL.cso"), (void**)&m_pSprite3D_VS_BILL);
    MakeShader(_T("PS"), _T("Sprite3D_PS.cso"), (void**)&m_pSprite3D_PS);

    return S_OK;
}

//------------------------------------------------------------------------
//
//  繧ｷ繧ｧ繝ｼ繝繝ｼ縺ｮ菴懈・髢｢謨ｰ
//
//  蠑墓焚    TCHAR ProfileName[]    菴懈・縺吶ｋ繧ｷ繧ｧ繝ｼ繝繝ｼ遞ｮ蛻･
//                       (VS,PS,GS,HS,DS,CS)
//        TCHAR FileName[]   繧ｪ繝悶ず繧ｧ繧ｯ繝医ヵ繧｡繧､繝ｫ蜷・
//        void** ppShader       菴懈・縺輔ｌ縺溘す繧ｧ繝ｼ繝繝ｼ(OUT)
//        D3D11_INPUT_ELEMENT_DESC Fluid_layout[]    鬆らせ繝ｬ繧､繧｢繧ｦ繝亥ｮ夂ｾｩ(逵∫払蜿ｯ)
//        UINT numElements                  鬆らせ繝ｬ繧､繧｢繧ｦ繝郁ｦ∫ｴ謨ｰ(逵∫払蜿ｯ)
//        ID3D11InputLayout** ppInputLayout     菴懈・縺輔ｌ繧矩らせ繝ｬ繧､繧｢繧ｦ繝・OUT)(逵∫払蜿ｯ)
//
//  謌ｻ繧雁､ HRESULT
//     S_OK   = 謌仙粥
//     E_FAIL = 逡ｰ蟶ｸ
//
//------------------------------------------------------------------------
HRESULT CShader::MakeShader(const TCHAR ProfileName[], const TCHAR FileName[], void** ppShader, D3D11_INPUT_ELEMENT_DESC Fluid_layout[], UINT numElements, ID3D11InputLayout** ppInputLayout)
{
    // 繧ｳ繝ｳ繝代う繝ｫ貂医∩繧ｷ繧ｧ繝ｼ繝繝ｼ縺ｮ隱ｭ縺ｿ霎ｼ縺ｿ驟榊・
    BYTE* pCso = nullptr;
    DWORD dwCsoSize = 0;

    // 繧ｳ繝ｳ繝代う繝ｫ貂医∩繧ｷ繧ｧ繝ｼ繝繝ｼ縺ｮ隱ｭ縺ｿ霎ｼ縺ｿ螳溯｡・
    m_pD3D->ReadCso(FileName, &pCso, &dwCsoSize);

    // 繧ｷ繧ｧ繝ｼ繝繝ｼ遞ｮ蛻･縺斐→縺ｮ菴懈・蜃ｦ逅・
    if (_tcscmp(ProfileName, _T("VS")) == 0)   // 鬆らせ繧ｷ繧ｧ繝ｼ繝繝ｼ
    {
       if (FAILED(m_pD3D->m_pDevice->CreateVertexShader(pCso, dwCsoSize, nullptr, (ID3D11VertexShader**)ppShader)))
       {
          SAFE_DELETE_ARRAY(pCso);
          MessageBox(0, _T("VertexShaderMakeFalse"), FileName, MB_OK);
          return E_FAIL;
       }
       if (ppInputLayout) // 鬆らせ繧､繝ｳ繝励ャ繝医Ξ繧､繧｢繧ｦ繝医ｒ菴懈・縺吶ｋ縺ｨ縺・
       {
          // 鬆らせ繧､繝ｳ繝励ャ繝医Ξ繧､繧｢繧ｦ繝医ｒ菴懈・
          if (FAILED(m_pD3D->m_pDevice->CreateInputLayout(Fluid_layout, numElements, pCso, dwCsoSize, ppInputLayout)))
          {
             MessageBox(0, _T("InputLayoutMakeFalse"), FileName, MB_OK);
             return E_FAIL;
          }
       }
    }
    else if (_tcscmp(ProfileName, _T("PS")) == 0)  // 繝斐け繧ｻ繝ｫ繧ｷ繧ｧ繝ｼ繝繝ｼ
    {
       if (FAILED(m_pD3D->m_pDevice->CreatePixelShader(pCso, dwCsoSize, nullptr, (ID3D11PixelShader**)ppShader)))
       {
          SAFE_DELETE_ARRAY(pCso);
          MessageBox(0, _T("PixelShaderMakeFalse"), FileName, MB_OK);
          return E_FAIL;
       }
    }
    else if (_tcscmp(ProfileName, _T("GS")) == 0)  // 繧ｸ繧ｪ繝｡繝医Μ繧ｷ繧ｧ繝ｼ繝繝ｼ
    {
       if (FAILED(m_pD3D->m_pDevice->CreateGeometryShader(pCso, dwCsoSize, nullptr, (ID3D11GeometryShader**)ppShader)))
       {
          SAFE_DELETE_ARRAY(pCso);
          MessageBox(0, _T("GeometrieShaderMakeFalse"), FileName, MB_OK);
          return E_FAIL;
       }
    }
    else if (_tcscmp(ProfileName, _T("HS")) == 0)  // 繝上Ν繧ｷ繧ｧ繝ｼ繝繝ｼ
    {
       if (FAILED(m_pD3D->m_pDevice->CreateHullShader(pCso, dwCsoSize, nullptr, (ID3D11HullShader**)ppShader)))
       {
          SAFE_DELETE_ARRAY(pCso);
          MessageBox(0, _T("HullShaderMakeFalse"), FileName, MB_OK);
          return E_FAIL;
       }
    }
    else if (_tcscmp(ProfileName, _T("DS")) == 0)  // 繝峨Γ繧､繝ｳ繧ｷ繧ｧ繝ｼ繝繝ｼ
    {
       if (FAILED(m_pD3D->m_pDevice->CreateDomainShader(pCso, dwCsoSize, nullptr, (ID3D11DomainShader**)ppShader)))
       {
          SAFE_DELETE_ARRAY(pCso);
          MessageBox(0, _T("DomainShaderMakeFalse"), FileName, MB_OK);
          return E_FAIL;
       }
    }
    else if (_tcscmp(ProfileName, _T("CS")) == 0)  // 繧ｳ繝ｳ繝斐Η繝ｼ繝医す繧ｧ繝ｼ繝繝ｼ
    {
       if (FAILED(m_pD3D->m_pDevice->CreateComputeShader(pCso, dwCsoSize, nullptr, (ID3D11ComputeShader**)ppShader)))
       {
          SAFE_DELETE_ARRAY(pCso);
          MessageBox(0, _T("ComputeShaderMakeFalse"), FileName, MB_OK);
          return E_FAIL;
       }
    }
    else {
       SAFE_DELETE_ARRAY(pCso);
       MessageBox(0, _T("ShaderProfileError"), ProfileName, MB_OK);
       return E_FAIL;
    }

    SAFE_DELETE_ARRAY(pCso);
    return S_OK;

}

//------------------------------------------------------------------------
//
//  蜷・ｨｮ螳壽焚繝舌ャ繝輔ぃ菴懈・
//
//  蠑墓焚・壹↑縺・
//
//  謌ｻ繧雁､ HRESULT
//     S_OK   = 謌仙粥
//     E_FAIL = 逡ｰ蟶ｸ
//
//------------------------------------------------------------------------
HRESULT CShader::InitShaderConstant()
{

    // 繝・ぅ繧ｹ繝励Ξ繧､繧ｹ繝槭ャ繝斐Φ繧ｰ逕ｨ 螳壽焚繝舌ャ繝輔ぃ菴懈・
    MakeConstantBuffer(sizeof(CONSTANT_BUFFER_DISPLACE), &m_pConstantBufferDisplace);

    // 繧ｨ繝輔ぉ繧ｯ繝育畑 螳壽焚繝舌ャ繝輔ぃ菴懈・
    MakeConstantBuffer(sizeof(CONSTANT_BUFFER_EFFECT), &m_pConstantBufferEffect);

    // 繧ｹ繝励Λ繧､繝育畑 螳壽焚繝舌ャ繝輔ぃ菴懈・・井ｸｻ縺ｫ螟画鋤陦悟・貂｡縺礼畑・・
    MakeConstantBuffer(sizeof(CONSTANT_BUFFER_SPRITE), &m_pConstantBufferSprite3D);

    // Fbx逕ｨ 螳壽焚繝舌ャ繝輔ぃ菴懈・・井ｸｻ縺ｫ螟画鋤陦悟・貂｡縺礼畑・・
    MakeConstantBuffer(sizeof(CONSTANT_BUFFER_WVLED), &m_pConstantBufferWVLED);

    // Fbx逕ｨ 螳壽焚繝舌ャ繝輔ぃ菴懈・・医・繝ｼ繝ｳ陦悟・貂｡縺礼畑・・
    MakeConstantBuffer(sizeof(MATRIX4X4) * MAX_BONES, &m_pConstantBufferBone2);

    // 螳壽焚繝舌ャ繝輔ぃ菴懈・・壹・繝・Μ繧｢繝ｫ諠・ｱ逕ｨ                                    // -- 2020.12.15
    MakeConstantBuffer(sizeof(CONSTANT_BUFFER_MATERIAL), &m_pConstantBufferMaterial);

    return S_OK;
}

//------------------------------------------------------------------------
//
//  螳壽焚繝舌ャ繝輔ぃ縺ｮ菴懈・髢｢謨ｰ
//
//  蠑墓焚    UINT   size                  菴懈・縺吶ｋ螳壽焚繝舌ャ繝輔ぃ縺ｮ繧ｵ繧､繧ｺ
//        ID3D11Buffer** ppConstantBuffer  菴懈・縺輔ｌ縺溷ｮ壽焚繝舌ャ繝輔ぃ(OUT)
//
//  謌ｻ繧雁､ HRESULT
//     S_OK   = 謌仙粥
//     E_FAIL = 逡ｰ蟶ｸ
//
//------------------------------------------------------------------------
HRESULT CShader::MakeConstantBuffer(UINT size, ID3D11Buffer** ppConstantBuffer)
{
    D3D11_BUFFER_DESC cb = { 0 };

    cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cb.ByteWidth = size;
    cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    cb.MiscFlags = 0;
    cb.StructureByteStride = 0;
    cb.Usage = D3D11_USAGE_DYNAMIC;

    if (FAILED(m_pD3D->m_pDevice->CreateBuffer(&cb, nullptr, ppConstantBuffer)))
    {
       MessageBox(0, _T("ConstantBufferMakeFalse"), nullptr, MB_OK);
       return E_FAIL;
    }
    return S_OK;
}