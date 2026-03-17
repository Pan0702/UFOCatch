//-----------------------------------------------------------------------------
//
//  Direct3D繧貞茜逕ｨ縺吶ｋ縺溘ａ縺ｮ繝ｩ繧､繝悶Λ繝ｪ                ver 3.5         2024.11.2
// 
//  竭 Direct3D縺ｮ蛻晄悄蛹・   
//  竭｡ 繝・け繧ｹ繝√Ε繧ｵ繝ｳ繝励Λ繝ｼ縺ｨ繝悶Ξ繝ｳ繝峨せ繝・・繝医・菴懈・
//  竭｢ 繧､繝｡繝ｼ繧ｸ繝輔ぃ繧､繝ｫ繧定ｪｭ縺ｿ霎ｼ繧薙〒繝・け繧ｹ繝√Ε繧剃ｽ懈・縺吶ｋ髢｢謨ｰ
//  竭｣ 繧ｳ繝ｳ繝代う繝ｫ貂医∩繧ｷ繧ｧ繝ｼ繝繝ｼ縺ｮ隱ｭ縺ｿ霎ｼ縺ｿ繧定｡後≧髢｢謨ｰ
//  竭､ 繝ｬ繝ｳ繝繝ｪ繝ｳ繧ｰ繧ｿ繝ｼ繧ｲ繝・ヨ縺ｮ險ｭ螳壹ｒ陦後≧髢｢謨ｰ
//  竭･ 繧ｳ繝ｳ繝代う繝ｫ貂医∩繧ｷ繧ｧ繝ｼ繝繝ｼ縺ｮ隱ｭ縺ｿ霎ｼ縺ｿ繧定｡後≧髢｢謨ｰ  x64蟇ｾ蠢・
//
//                                               Direct3D.cpp
//
//-----------------------------------------------------------------------------

#include "Direct3D.h"
#include "../Game/Macro.h"

#pragma warning(disable : 6387)

//------------------------------------------------------------------------
//
//  Direct3D 繧ｳ繝ｳ繧ｹ繝医Λ繧ｯ繧ｿ    
//
//------------------------------------------------------------------------
CDirect3D::CDirect3D()
{
    ZeroMemory(this, sizeof(CDirect3D));
}

//------------------------------------------------------------------------
//
//  Direct3D 繝・せ繝医Λ繧ｯ繧ｿ  
//
//------------------------------------------------------------------------
CDirect3D::~CDirect3D()
{
    DestroyD3D();
}

//------------------------------------------------------------------------
//
//  Direct3D 縺ｮ蛻晄悄蛹・   
//
//  HWND hWnd         繧ｦ繧｣繝ｳ繝峨え繝上Φ繝峨Ν
//  DWORD dwWidth     菴懈・縺吶ｋ逕ｻ髱｢縺ｮ蟷・
//  DWORD dwHeight    菴懈・縺吶ｋ逕ｻ髱｢縺ｮ鬮倥＆
//
//  謌ｻ繧雁､ HRESULT
//     S_OK   = 謌仙粥
//     E_FAIL = 逡ｰ蟶ｸ
//
//------------------------------------------------------------------------
HRESULT CDirect3D::InitD3D(HWND hWnd, DWORD dwWidth, DWORD dwHeight)
{
    m_hWnd = hWnd;
    m_dwWindowWidth = dwWidth;
    m_dwWindowHeight = dwHeight;
    
    // DXGI繝輔ぃ繧ｯ繝医Μ縺ｮ菴懈・
    IDXGIFactory1* pGFactory = nullptr;
    if (FAILED(CreateDXGIFactory1(__uuidof(IDXGIFactory1), (VOID**)(&pGFactory))))
    {
       return FALSE;
    }

    // 繧ｰ繝ｩ繝輔ぅ繝・け繧ｹ繧ｫ繝ｼ繝会ｼ医い繝繝励ち・峨・諠・ｱ繧貞叙蠕励＠縲∵怙驕ｩ縺ｪ繧ゅ・繧帝∈謚・
    m_pAdapter = nullptr;
    UINT iAdapter = 0;
    IDXGIAdapter* pAdapterTemp = nullptr;
    SIZE_T vramsize = 0;

    while (pGFactory->EnumAdapters(iAdapter, &pAdapterTemp) != DXGI_ERROR_NOT_FOUND)
    {
       DXGI_ADAPTER_DESC descAdapter;
       pAdapterTemp->GetDesc(&descAdapter);
       if (m_pAdapter == nullptr)
       {
          m_pAdapter = pAdapterTemp;
          vramsize = descAdapter.DedicatedVideoMemory / 1024 / 1024;
       }
       else if (descAdapter.DedicatedVideoMemory / 1024 / 1024 > vramsize) {
          // VRAM・亥ｰら畑繝薙ョ繧ｪ繝｡繝｢繝ｪ・峨′繧医ｊ螟ｧ縺阪＞繧｢繝繝励ち繧貞━蜈医＠縺ｦ驕ｸ謚・
          SAFE_RELEASE(m_pAdapter);
          m_pAdapter = pAdapterTemp;
          vramsize = descAdapter.DedicatedVideoMemory / 1024 / 1024;
       }
       else {
          SAFE_RELEASE(pAdapterTemp);
       }
       ++iAdapter;
    }
    SAFE_RELEASE(pGFactory);

    // 繧ｹ繝ｯ繝・・繝√ぉ繧､繝ｳ縺ｮ險ｭ螳・
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferDesc.Width  = m_dwWindowWidth;
    sd.BufferDesc.Height = m_dwWindowHeight;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.SampleDesc.Count = SAMPLE_COUNT;
    sd.SampleDesc.Quality = SAMPLE_QUALITY;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.BufferCount = 1;
    sd.OutputWindow = m_hWnd;
    sd.Windowed = true;

    D3D_FEATURE_LEVEL pFeatureLevels = D3D_FEATURE_LEVEL_11_0;
    D3D_FEATURE_LEVEL pFeatureLevel;

    // 繝・ヰ繧､繧ｹ縺ｨ繧ｹ繝ｯ繝・・繝√ぉ繧､繝ｳ繧貞酔譎ゅ↓菴懈・
    HRESULT hr = D3D11CreateDeviceAndSwapChain(
       m_pAdapter,
       D3D_DRIVER_TYPE_UNKNOWN,
       nullptr,
       0, 
       &pFeatureLevels, 
       1, 
       D3D11_SDK_VERSION, 
       &sd, 
       &m_pSwapChain, 
       &m_pDevice,
       &pFeatureLevel, 
       &m_pDeviceContext);

    if(FAILED(hr)){
       MessageBox(0, _T("Direct3D.cpp D3D繝・ヰ繧､繧ｹ縺ｨ繧ｹ繝ｯ繝・・繝√ぉ繧､繝ｳ縺ｮ菴懈・縺ｫ螟ｱ謨励＠縺ｾ縺励◆"), nullptr, MB_OK);
       return E_FAIL;
    }

    // 繝ｬ繝ｳ繝繝ｼ繧ｿ繝ｼ繧ｲ繝・ヨ繝薙Η繝ｼ(RTV)縺ｮ菴懈・
    ID3D11Texture2D *pBackBuffer_Tex;
    m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer_Tex);
    m_pDevice->CreateRenderTargetView(pBackBuffer_Tex, nullptr, &m_pBackBuffer_TexRTV);
    SAFE_RELEASE(pBackBuffer_Tex);

    // 繝・・繧ｹ繧ｹ繝・Φ繧ｷ繝ｫ繝薙Η繝ｼ(DSV)逕ｨ縺ｮ繝・け繧ｹ繝√Ε菴懈・
    D3D11_TEXTURE2D_DESC descDepth;
    descDepth.Width = m_dwWindowWidth;
    descDepth.Height = m_dwWindowHeight;
    descDepth.MipLevels = 1;
    descDepth.ArraySize = 1;
    descDepth.Format = DXGI_FORMAT_D32_FLOAT;
    descDepth.SampleDesc.Count = SAMPLE_COUNT;
    descDepth.SampleDesc.Quality = SAMPLE_QUALITY;
    descDepth.Usage = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    descDepth.CPUAccessFlags = 0;
    descDepth.MiscFlags = 0;

    m_pDevice->CreateTexture2D(&descDepth, nullptr, &m_pBackBuffer_DSTex);
    m_pDevice->CreateDepthStencilView(m_pBackBuffer_DSTex, nullptr, &m_pBackBuffer_DSTexDSV);

    // 繝代う繝励Λ繧､繝ｳ縺ｫ繝ｬ繝ｳ繝繝ｼ繧ｿ繝ｼ繧ｲ繝・ヨ繧偵そ繝・ヨ
    SetRenderTarget(nullptr, nullptr);

    // 繝薙Η繝ｼ繝昴・繝医・險ｭ螳・
    D3D11_VIEWPORT vp;
    vp.Width = (float)m_dwWindowWidth;
    vp.Height = (float)m_dwWindowHeight;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    m_pDeviceContext->RSSetViewports(1, &vp);

    // 繝ｩ繧ｹ繧ｿ繝ｩ繧､繧ｶ繧ｹ繝・・繝医・險ｭ螳夲ｼ郁レ髱｢繧ｫ繝ｪ繝ｳ繧ｰ縲∝｡励ｊ縺､縺ｶ縺励Δ繝ｼ繝臥ｭ会ｼ・
    D3D11_RASTERIZER_DESC rdc;
    ZeroMemory(&rdc, sizeof(rdc));
    rdc.CullMode = D3D11_CULL_BACK;       // 閭碁擇繧偵き繝ｪ繝ｳ繧ｰ縺吶ｋ
    rdc.FrontCounterClockwise = FALSE;    // 譎りｨ亥屓繧翫′陦ｨ髱｢
    rdc.FillMode = D3D11_FILL_SOLID;
    m_pDevice->CreateRasterizerState(&rdc, &m_pRStateR);

    rdc.FillMode = D3D11_FILL_WIREFRAME;   // 繝ｯ繧､繝､繝ｼ繝輔Ξ繝ｼ繝陦ｨ遉ｺ逕ｨ
    m_pDevice->CreateRasterizerState(&rdc, &m_pRStateRW);

    m_pDeviceContext->RSSetState(m_pRStateR); // 騾壼ｸｸ繝｢繝ｼ繝峨ｒ繝・ヵ繧ｩ繝ｫ繝医↓

    // 蜷・ｨｮ繧ｹ繝・・繝茨ｼ医し繝ｳ繝励Λ繝ｼ縲√ヶ繝ｬ繝ｳ繝峨√ョ繝励せ・峨・蛻晄悄蛹・
    InitSampler();
    InitBlendState();
    InitDepthStencilState();

    // WIC Imaging Factory 縺ｮ菴懈・・育判蜒剰ｪｭ縺ｿ霎ｼ縺ｿ逕ｨ・・
    hr = CoCreateInstance(
          CLSID_WICImagingFactory,
          nullptr,
          CLSCTX_INPROC_SERVER,
          IID_PPV_ARGS(&m_pFactory)
    );
    if (FAILED(hr)) {
       MessageBox(0, _T("Direct3D.cpp WICImagingFactory縺ｮ菴懈・縺ｫ螟ｱ謨励＠縺ｾ縺励◆"), nullptr, MB_OK);
       return E_FAIL;
    }

    // CPU縺轡irectXMath繧偵し繝昴・繝医＠縺ｦ縺・ｋ縺狗｢ｺ隱・
    if (XMVerifyCPUSupport() != true)
    {
       MessageBox(0, _T("Direct3D.cpp DirectXMathNotSupportCPU"), nullptr, MB_OK);
       return E_FAIL;
    }

    return S_OK;
}

//------------------------------------------------------------------------
//  繝・け繧ｹ繝√Ε繧ｵ繝ｳ繝励Λ繝ｼ縺ｮ蛻晄悄蛹・
//------------------------------------------------------------------------
HRESULT CDirect3D::InitSampler()
{
    D3D11_SAMPLER_DESC SamDesc;
    ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));

    // 騾壼ｸｸ縺ｮ邱壼ｽ｢陬懷ｮ後し繝ｳ繝励Λ繝ｼ
    SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    SamDesc.MipLODBias = 0;
    SamDesc.MaxAnisotropy = 1;
    SamDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    SamDesc.MinLOD = 0;
    SamDesc.MaxLOD = D3D11_FLOAT32_MAX;
    m_pDevice->CreateSamplerState(&SamDesc, &m_pSampleLinear);

    // 蠅・阜濶ｲ謖・ｮ壹し繝ｳ繝励Λ繝ｼ
    ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));
    SamDesc.Filter = D3D11_FILTER_ANISOTROPIC;
    SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
    SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
    SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
    static const VECTOR4 One = VECTOR4(1, 1, 1, 0);
    memcpy_s(SamDesc.BorderColor, sizeof(SamDesc.BorderColor), &One, sizeof(VECTOR4));
    SamDesc.MipLODBias = 0;
    SamDesc.MaxAnisotropy = 16;
    SamDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    SamDesc.MinLOD = 0;
    SamDesc.MaxLOD = D3D11_FLOAT32_MAX;
    m_pDevice->CreateSamplerState(&SamDesc, &m_pSampleBorder);

    return S_OK;
}

//------------------------------------------------------------------------
//  繝悶Ξ繝ｳ繝峨せ繝・・繝医・蛻晄悄蛹・
//------------------------------------------------------------------------
HRESULT CDirect3D::InitBlendState()
{
    UINT mask = 0xffffffff;
    m_pDeviceContext->OMGetBlendState(&m_pBlendStateNormal, nullptr, &mask);

    // 蜊企乗・繝悶Ξ繝ｳ繝芽ｨｭ螳・
    D3D11_BLEND_DESC bd;
    ZeroMemory(&bd, sizeof(D3D11_BLEND_DESC));
    bd.IndependentBlendEnable = false;
    bd.AlphaToCoverageEnable = false;
    bd.RenderTarget[0].BlendEnable = true;
    bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    m_pDevice->CreateBlendState(&bd, &m_pBlendStateTrapen);

    // 蜉邂怜粋謌占ｨｭ螳・
    ZeroMemory(&bd, sizeof(D3D11_BLEND_DESC));
    bd.RenderTarget[0].BlendEnable = true;
    bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    bd.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
    bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
    bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    m_pDevice->CreateBlendState(&bd, &m_pBlendStateAdd);

    // 蛻晄悄迥ｶ諷九ｒ蜊企乗・繝悶Ξ繝ｳ繝峨↓繧ｻ繝・ヨ
    m_pDeviceContext->OMSetBlendState(m_pBlendStateTrapen, nullptr, mask);

    return S_OK;
}

//------------------------------------------------------------------------
//  繝・・繧ｹ繧ｹ繝・Φ繧ｷ繝ｫ繧ｹ繝・・繝医・蛻晄悄蛹・
//------------------------------------------------------------------------
HRESULT CDirect3D::InitDepthStencilState()
{
    D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
    
    // 騾壼ｸｸ・・繝・せ繝医〇譖ｸ縺崎ｾｼ縺ｿ荳｡譁ｹ譛牙柑・・
    ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
    depthStencilDesc.DepthEnable = TRUE;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
    m_pDevice->CreateDepthStencilState(&depthStencilDesc, &m_pDepthStencilStateDefault);

    // Z譖ｸ縺崎ｾｼ縺ｿ辟｡蜉ｹ・亥濠騾乗・繧ｪ繝悶ず繧ｧ繧ｯ繝医・謠冗判逕ｨ・・
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
    m_pDevice->CreateDepthStencilState(&depthStencilDesc, &m_pDepthStencilStateNoWrite);

    // Z繝・せ繝育┌蜉ｹ・亥ｸｸ縺ｫ謇句燕縺ｫ謠冗判・・
    depthStencilDesc.DepthEnable = FALSE;
    m_pDevice->CreateDepthStencilState(&depthStencilDesc, &m_pDepthStencilStateNoZTest);

    return S_OK;
}

//------------------------------------------------------------------------
//  繝ｬ繝ｳ繝繝ｼ繧ｿ繝ｼ繧ｲ繝・ヨ繧貞・繧頑崛縺医ｋ
//------------------------------------------------------------------------
void CDirect3D::SetRenderTarget(ID3D11RenderTargetView* pTexRTV, ID3D11DepthStencilView* pDSTexDSV)
{
    if (pTexRTV == nullptr)
    {
       m_pTarget_TexRTV = m_pBackBuffer_TexRTV;
       m_pTarget_DSTexDSV = m_pBackBuffer_DSTexDSV;
    }
    else {
       m_pTarget_TexRTV = pTexRTV;
       m_pTarget_DSTexDSV = pDSTexDSV;
    }
    m_pDeviceContext->OMSetRenderTargets(1, &m_pTarget_TexRTV, m_pTarget_DSTexDSV);
}

//------------------------------------------------------------------------
//  迴ｾ蝨ｨ縺ｮ繝ｬ繝ｳ繝繝ｼ繧ｿ繝ｼ繧ｲ繝・ヨ繧偵け繝ｪ繧｢縺吶ｋ
//------------------------------------------------------------------------
void CDirect3D::ClearRenderTarget(float ClearColor[])
{
    m_pDeviceContext->ClearRenderTargetView(m_pTarget_TexRTV, ClearColor);
    if (m_pTarget_DSTexDSV)
    {
       m_pDeviceContext->ClearDepthStencilView(m_pTarget_DSTexDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);
    }
}

//------------------------------------------------------------------------
//  Z繝舌ャ繝輔ぃ・域ｷｱ蠎ｦ繝舌ャ繝輔ぃ・峨・菴ｿ逕ｨ譛臥┌繧貞・繧頑崛縺医ｋ
//------------------------------------------------------------------------
void CDirect3D::SetZBuffer(bool bZBuf)
{
    if (bZBuf)
    {
       m_pDeviceContext->OMSetRenderTargets(1, &m_pTarget_TexRTV, m_pTarget_DSTexDSV);
    }
    else {
       m_pDeviceContext->OMSetRenderTargets(1, &m_pTarget_TexRTV, nullptr);
    }
}

//------------------------------------------------------------------------
//  繝輔ぃ繧､繝ｫ縺九ｉ繝・け繧ｹ繝√Ε繧定ｪｭ縺ｿ霎ｼ繧薙〒逕滓・縺吶ｋ・・IC蛻ｩ逕ｨ・・
//------------------------------------------------------------------------
HRESULT CDirect3D::CreateTextureFromFile(const TCHAR* TName, ID3D11Texture2D** ppTexture2D, DWORD& dwImageWidth, DWORD& dwImageHeight, UINT MipLevels)
{
    IWICBitmapDecoder* decoder = nullptr;

#if _UNICODE
    if (FAILED(m_pFactory->CreateDecoderFromFilename(TName, 0, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &decoder)))
       return E_FAIL;
#else
    WCHAR WName[512] = { 0x00 };
    MultiByteToWideChar(CP_OEMCP, MB_PRECOMPOSED, TName, (int)strlen(TName), WName, 512);
    if (FAILED(m_pFactory->CreateDecoderFromFilename(WName, 0, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &decoder)))
       return E_FAIL;
#endif

    IWICBitmapFrameDecode* frame = nullptr;
    decoder->GetFrame(0, &frame);

    UINT width, height;
    frame->GetSize(&width, &height);
    dwImageWidth = width;
    dwImageHeight = height;

    WICPixelFormatGUID pixelFormat;
    frame->GetPixelFormat(&pixelFormat);

    UINT ImageStride = dwImageWidth * 4;
    UINT ImageSize = dwImageWidth * dwImageHeight * 4;
    BYTE* pImageData = new BYTE[ImageSize];

    // 蟶ｸ縺ｫ32bpp RGBA蠖｢蠑上↓螟画鋤
    if (pixelFormat != GUID_WICPixelFormat32bppRGBA)
    {
       IWICFormatConverter* FC = nullptr;
       m_pFactory->CreateFormatConverter(&FC);
       FC->Initialize(frame, GUID_WICPixelFormat32bppRGBA, WICBitmapDitherTypeErrorDiffusion, 0, 0, WICBitmapPaletteTypeCustom);
       FC->CopyPixels(0, ImageStride, ImageSize, pImageData);
       SAFE_RELEASE(FC);
    }
    else {
       frame->CopyPixels(0, ImageStride, ImageSize, pImageData);
    }

    D3D11_SUBRESOURCE_DATA initData;
    initData.pSysMem = pImageData;
    initData.SysMemPitch = ImageStride;
    initData.SysMemSlicePitch = ImageSize;

    if (MipLevels == 1)
       MakeNoMipmapTexture(dwImageWidth, dwImageHeight, initData, ppTexture2D);
    else
       MakeMipmapTexture(dwImageWidth, dwImageHeight, initData, MipLevels, ppTexture2D);

    SAFE_RELEASE(frame);
    SAFE_RELEASE(decoder);
    SAFE_DELETE_ARRAY(pImageData);

    return S_OK;
}

// (荳ｭ逡･: MakeNoMipmapTexture, MakeMipmapTexture縺ｮ螳溯｣・

//------------------------------------------------------------------------
//  繝溘ャ繝励・繝・・縺ｪ縺励・繝・け繧ｹ繝√Ε繧剃ｽ懈・
//------------------------------------------------------------------------
HRESULT CDirect3D::MakeNoMipmapTexture(const DWORD& dwImageWidth, const DWORD& dwImageHeight, const D3D11_SUBRESOURCE_DATA& initData, ID3D11Texture2D** ppTexture2D)
{
    D3D11_TEXTURE2D_DESC desc;
    desc.Width = dwImageWidth;
    desc.Height = dwImageHeight;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;

    return m_pDevice->CreateTexture2D(&desc, &initData, ppTexture2D);
}

//------------------------------------------------------------------------
//  繝溘ャ繝励・繝・・縺ゅｊ縺ｮ繝・け繧ｹ繝√Ε繧剃ｽ懈・
//------------------------------------------------------------------------
HRESULT CDirect3D::MakeMipmapTexture(const DWORD& dwImageWidth, const DWORD& dwImageHeight, const D3D11_SUBRESOURCE_DATA& initData, const UINT& MipLevels, ID3D11Texture2D** ppTexture2D)
{
    ID3D11Texture2D* pWTexture2D;
    D3D11_TEXTURE2D_DESC desc;
    
    // 繧ｹ繝・・繧ｸ繝ｳ繧ｰ繝・け繧ｹ繝√Ε縺ｮ菴懈・・・PU縺九ｉ隱ｭ縺ｿ霎ｼ繧縺溘ａ・・
    desc.Width = dwImageWidth;
    desc.Height = dwImageHeight;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Usage = D3D11_USAGE_STAGING;
    desc.BindFlags = 0;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
    desc.MiscFlags = 0;

    m_pDevice->CreateTexture2D(&desc, &initData, &pWTexture2D);

    // 螳滄圀縺ｫ菴ｿ逕ｨ縺吶ｋ繝・け繧ｹ繝√Ε縺ｮ菴懈・
    desc.MipLevels = MipLevels;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

    m_pDevice->CreateTexture2D(&desc, nullptr, ppTexture2D);

    D3D11_MAPPED_SUBRESOURCE mappedResource;
    m_pDeviceContext->Map(pWTexture2D, NULL, D3D11_MAP_READ, 0, &mappedResource);
    
    // 譛蛻昴・繝ｬ繝吶Ν縺ｮ縺ｿ繝・・繧ｿ繧定ｻ｢騾√＠縲√≠縺ｨ縺ｯGPU縺ｫ逕滓・縺輔○繧・
    m_pDeviceContext->UpdateSubresource(*ppTexture2D, D3D11CalcSubresource(0, 0, desc.MipLevels), nullptr, mappedResource.pData, mappedResource.RowPitch, 0);

    m_pDeviceContext->Unmap(pWTexture2D, 0);
    SAFE_RELEASE(pWTexture2D);

    return S_OK;
}

//------------------------------------------------------------------------
//  ShaderResourceView(SRV)繧偵ヵ繧｡繧､繝ｫ縺九ｉ逶ｴ謗･菴懈・
//------------------------------------------------------------------------
HRESULT CDirect3D::CreateShaderResourceViewFromFile(const TCHAR* TName, ID3D11ShaderResourceView** ppTexture, DWORD& dwImageWidth, DWORD& dwImageHeight, UINT MipLevels)
{
    ID3D11Texture2D* pTexture2D = nullptr;
    if (FAILED(CreateTextureFromFile(TName, &pTexture2D, dwImageWidth, dwImageHeight, MipLevels)))
       return E_FAIL;

    D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
    SRVDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    SRVDesc.Texture2D.MipLevels = MipLevels;

    m_pDevice->CreateShaderResourceView(pTexture2D, &SRVDesc, ppTexture);
    
    // 繝溘ャ繝励・繝・・縺ｮ閾ｪ蜍慕函謌舌ｒ螳溯｡・
    if (MipLevels > 1) m_pDeviceContext->GenerateMips(*ppTexture);

    SAFE_RELEASE(pTexture2D);
    return S_OK;
}

HRESULT CDirect3D::CreateShaderResourceViewFromFile(const TCHAR* TName, ID3D11ShaderResourceView** ppTexture, UINT MipLevels)
{
    DWORD w, h;
    return CreateShaderResourceViewFromFile(TName, ppTexture, w, h, MipLevels);
}

//------------------------------------------------------------------------
//  繧ｳ繝ｳ繝代う繝ｫ貂医∩繧ｷ繧ｧ繝ｼ繝繝ｼ(CSO)縺ｮ隱ｭ縺ｿ霎ｼ縺ｿ・・64蟇ｾ蠢懶ｼ・
//------------------------------------------------------------------------
HRESULT CDirect3D::ReadCso(const TCHAR* csoName, BYTE** ppByteArray, DWORD* pCsoSize)
{
    FILE* fp;
    TCHAR FName[256];

#ifdef _M_AMD64
#ifdef _DEBUG
    _tcscpy_s(FName, _T("x64/Debug/"));
#else
    _tcscpy_s(FName, _T("x64/Release/"));
#endif
#else
#ifdef _DEBUG
    _tcscpy_s(FName, _T("Debug/"));
#else
    _tcscpy_s(FName, _T("Release/"));
#endif
#endif

    _tcscat_s(FName, csoName);

#if _UNICODE
    int ret = _wfopen_s(&fp, FName, L"rb");
#else
    int ret = fopen_s(&fp, FName, "rb");
#endif

    if (ret != 0) {
       MessageBox(0, FName, _T("CSO繝輔ぃ繧､繝ｫ縺ｮ隱ｭ縺ｿ霎ｼ縺ｿ縺ｫ螟ｱ謨励＠縺ｾ縺励◆"), MB_OK);
       return E_FAIL;
    }
    
    fseek(fp, 0, SEEK_END);
    *pCsoSize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    *ppByteArray = new BYTE[*pCsoSize];
    fread(*ppByteArray, *pCsoSize, 1, fp);
    fclose(fp);

    return S_OK;
}

//------------------------------------------------------------------------
//  蜈ｨDirect3D繝ｪ繧ｽ繝ｼ繧ｹ縺ｮ隗｣謾ｾ
//------------------------------------------------------------------------
void CDirect3D::DestroyD3D()
{
    SAFE_RELEASE(m_pSwapChain);
    SAFE_RELEASE(m_pBackBuffer_TexRTV);
    SAFE_RELEASE(m_pBackBuffer_DSTexDSV);
    SAFE_RELEASE(m_pBackBuffer_DSTex);
    SAFE_RELEASE(m_pSampleLinear);
    SAFE_RELEASE(m_pSampleBorder);
    SAFE_RELEASE(m_pBlendStateNormal);
    SAFE_RELEASE(m_pBlendStateTrapen);
    SAFE_RELEASE(m_pBlendStateAdd);
    SAFE_RELEASE(m_pDepthStencilStateDefault);
    SAFE_RELEASE(m_pDepthStencilStateNoWrite);
    SAFE_RELEASE(m_pDepthStencilStateNoZTest);
    SAFE_RELEASE(m_pRStateR);
    SAFE_RELEASE(m_pRStateRW);
    SAFE_RELEASE(m_pFactory);
    SAFE_RELEASE(m_pDevice);
    SAFE_RELEASE(m_pAdapter);
}