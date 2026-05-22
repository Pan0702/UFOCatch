//-----------------------------------------------------------------------------
//
//  Direct3Dを利用するためのライブラリ                ver 3.5         2024.11.2
// 
//  ④ コンパイル済みシェーダーの読み込みを行う関数
//
//                                               Direct3D.cpp
//
//-----------------------------------------------------------------------------

#include "Direct3D.h"
#include "../Game/Macro.h"

#pragma warning(disable : 6387)

//------------------------------------------------------------------------
//
//  Direct3D コンストラクタ    
//
//------------------------------------------------------------------------
CDirect3D::CDirect3D()
{
    ZeroMemory(this, sizeof(CDirect3D));
}

//------------------------------------------------------------------------
//
//
//------------------------------------------------------------------------
CDirect3D::~CDirect3D()
{
    DestroyD3D();
}

//------------------------------------------------------------------------
//
//
//  HWND hWnd         ウィンドウハンドル
//
//  戻り値 HRESULT
//     E_FAIL = 逡ｰ蟶ｸ
//
//------------------------------------------------------------------------
HRESULT CDirect3D::InitD3D(HWND hWnd, DWORD dwWidth, DWORD dwHeight)
{
    m_hWnd = hWnd;
    m_dwWindowWidth = dwWidth;
    m_dwWindowHeight = dwHeight;

    IDXGIFactory1* pGFactory = nullptr;
    if (FAILED(CreateDXGIFactory1(__uuidof(IDXGIFactory1), (VOID**)(&pGFactory))))
    {
        return FALSE;
    }

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
        else if (descAdapter.DedicatedVideoMemory / 1024 / 1024 > vramsize)
        {
            SAFE_RELEASE(m_pAdapter);
            m_pAdapter = pAdapterTemp;
            vramsize = descAdapter.DedicatedVideoMemory / 1024 / 1024;
        }
        else
        {
            SAFE_RELEASE(pAdapterTemp);
        }
        ++iAdapter;
    }
    SAFE_RELEASE(pGFactory);

    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferDesc.Width = m_dwWindowWidth;
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

    if (FAILED(hr))
    {
        MessageBox(0, _T("Direct3D.cpp D3D初期化 : 処理に失敗しました。"), nullptr, MB_OK);
        return E_FAIL;
    }

    ID3D11Texture2D* pBackBuffer_Tex;
    m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer_Tex);
    m_pDevice->CreateRenderTargetView(pBackBuffer_Tex, nullptr, &m_pBackBuffer_TexRTV);
    SAFE_RELEASE(pBackBuffer_Tex);

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

    SetRenderTarget(nullptr, nullptr);

    D3D11_VIEWPORT vp;
    vp.Width = (float)m_dwWindowWidth;
    vp.Height = (float)m_dwWindowHeight;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    m_pDeviceContext->RSSetViewports(1, &vp);

    D3D11_RASTERIZER_DESC rdc;
    ZeroMemory(&rdc, sizeof(rdc));
    rdc.CullMode = D3D11_CULL_BACK; // 背面をカリングする
    rdc.FrontCounterClockwise = FALSE; // 時計回りが表面
    rdc.FillMode = D3D11_FILL_SOLID;
    m_pDevice->CreateRasterizerState(&rdc, &m_pRStateR);

    rdc.FillMode = D3D11_FILL_WIREFRAME; // ワイヤーフレーム表示用
    m_pDevice->CreateRasterizerState(&rdc, &m_pRStateRW);

    m_pDeviceContext->RSSetState(m_pRStateR);

    InitSampler();
    InitBlendState();
    InitDepthStencilState();

    hr = CoCreateInstance(
        CLSID_WICImagingFactory,
        nullptr,
        CLSCTX_INPROC_SERVER,
        IID_PPV_ARGS(&m_pFactory)
    );
    if (FAILED(hr))
    {
        MessageBox(0, _T("Direct3D.cpp WICImagingFactory : 処理に失敗しました。"), nullptr, MB_OK);
        return E_FAIL;
    }

    if (XMVerifyCPUSupport() != true)
    {
        MessageBox(0, _T("Direct3D.cpp DirectXMathNotSupportCPU"), nullptr, MB_OK);
        return E_FAIL;
    }

    return S_OK;
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
HRESULT CDirect3D::InitSampler()
{
    D3D11_SAMPLER_DESC SamDesc;
    ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));

    // 通常の線形補完サンプラー
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
//------------------------------------------------------------------------
HRESULT CDirect3D::InitBlendState()
{
    UINT mask = 0xffffffff;
    m_pDeviceContext->OMGetBlendState(&m_pBlendStateNormal, nullptr, &mask);

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

    // 加算合成設定
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

    m_pDeviceContext->OMSetBlendState(m_pBlendStateTrapen, nullptr, mask);

    return S_OK;
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
HRESULT CDirect3D::InitDepthStencilState()
{
    D3D11_DEPTH_STENCIL_DESC depthStencilDesc;

    ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
    depthStencilDesc.DepthEnable = TRUE;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
    m_pDevice->CreateDepthStencilState(&depthStencilDesc, &m_pDepthStencilStateDefault);

    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
    m_pDevice->CreateDepthStencilState(&depthStencilDesc, &m_pDepthStencilStateNoWrite);

    depthStencilDesc.DepthEnable = FALSE;
    m_pDevice->CreateDepthStencilState(&depthStencilDesc, &m_pDepthStencilStateNoZTest);

    return S_OK;
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void CDirect3D::SetRenderTarget(ID3D11RenderTargetView* pTexRTV, ID3D11DepthStencilView* pDSTexDSV)
{
    if (pTexRTV == nullptr)
    {
        m_pTarget_TexRTV = m_pBackBuffer_TexRTV;
        m_pTarget_DSTexDSV = m_pBackBuffer_DSTexDSV;
    }
    else
    {
        m_pTarget_TexRTV = pTexRTV;
        m_pTarget_DSTexDSV = pDSTexDSV;
    }
    m_pDeviceContext->OMSetRenderTargets(1, &m_pTarget_TexRTV, m_pTarget_DSTexDSV);
}

//------------------------------------------------------------------------
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
//------------------------------------------------------------------------
void CDirect3D::SetZBuffer(bool bZBuf)
{
    if (bZBuf)
    {
        m_pDeviceContext->OMSetRenderTargets(1, &m_pTarget_TexRTV, m_pTarget_DSTexDSV);
    }
    else
    {
        m_pDeviceContext->OMSetRenderTargets(1, &m_pTarget_TexRTV, nullptr);
    }
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
HRESULT CDirect3D::CreateTextureFromFile(const TCHAR* TName, ID3D11Texture2D** ppTexture2D, DWORD& dwImageWidth,
                                         DWORD& dwImageHeight, UINT MipLevels)
{
    IWICBitmapDecoder* decoder = nullptr;

#if _UNICODE
    // 複数の状態や境界条件をまとめて判定する。
    if (FAILED(m_pFactory->CreateDecoderFromFilename(TName, 0, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &decoder)))
        return E_FAIL;
#else
    WCHAR WName[512] = {0x00};
    MultiByteToWideChar(CP_OEMCP, MB_PRECOMPOSED, TName, (int)strlen(TName), WName, 512);
    // 複数の状態や境界条件をまとめて判定する。
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

    // 常に32bpp RGBA形式に変換
    if (pixelFormat != GUID_WICPixelFormat32bppRGBA)
    {
        IWICFormatConverter* FC = nullptr;
        m_pFactory->CreateFormatConverter(&FC);
        FC->Initialize(frame, GUID_WICPixelFormat32bppRGBA, WICBitmapDitherTypeErrorDiffusion, 0, 0,
                       WICBitmapPaletteTypeCustom);
        FC->CopyPixels(0, ImageStride, ImageSize, pImageData);
        SAFE_RELEASE(FC);
    }
    else
    {
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


//------------------------------------------------------------------------
//------------------------------------------------------------------------
HRESULT CDirect3D::MakeNoMipmapTexture(const DWORD& dwImageWidth, const DWORD& dwImageHeight,
                                       const D3D11_SUBRESOURCE_DATA& initData, ID3D11Texture2D** ppTexture2D)
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
//------------------------------------------------------------------------
HRESULT CDirect3D::MakeMipmapTexture(const DWORD& dwImageWidth, const DWORD& dwImageHeight,
                                     const D3D11_SUBRESOURCE_DATA& initData, const UINT& MipLevels,
                                     ID3D11Texture2D** ppTexture2D)
{
    ID3D11Texture2D* pWTexture2D;
    D3D11_TEXTURE2D_DESC desc;

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

    desc.MipLevels = MipLevels;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

    m_pDevice->CreateTexture2D(&desc, nullptr, ppTexture2D);

    D3D11_MAPPED_SUBRESOURCE mappedResource;
    m_pDeviceContext->Map(pWTexture2D, NULL, D3D11_MAP_READ, 0, &mappedResource);

    m_pDeviceContext->UpdateSubresource(*ppTexture2D, D3D11CalcSubresource(0, 0, desc.MipLevels), nullptr,
                                        mappedResource.pData, mappedResource.RowPitch, 0);

    m_pDeviceContext->Unmap(pWTexture2D, 0);
    SAFE_RELEASE(pWTexture2D);

    return S_OK;
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
HRESULT CDirect3D::CreateShaderResourceViewFromFile(const TCHAR* TName, ID3D11ShaderResourceView** ppTexture,
                                                    DWORD& dwImageWidth, DWORD& dwImageHeight, UINT MipLevels)
{
    ID3D11Texture2D* pTexture2D = nullptr;
    if (FAILED(CreateTextureFromFile(TName, &pTexture2D, dwImageWidth, dwImageHeight, MipLevels)))
        return E_FAIL;

    D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
    SRVDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    SRVDesc.Texture2D.MipLevels = MipLevels;

    m_pDevice->CreateShaderResourceView(pTexture2D, &SRVDesc, ppTexture);

    if (MipLevels > 1) m_pDeviceContext->GenerateMips(*ppTexture);

    SAFE_RELEASE(pTexture2D);
    return S_OK;
}

HRESULT CDirect3D::CreateShaderResourceViewFromFile(const TCHAR* TName, ID3D11ShaderResourceView** ppTexture,
                                                    UINT MipLevels)
{
    DWORD w, h;
    return CreateShaderResourceViewFromFile(TName, ppTexture, w, h, MipLevels);
}

//------------------------------------------------------------------------
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

    if (ret != 0)
    {
        MessageBox(0, FName, _T("CSOファイルの読み込みに失敗しました"), MB_OK);
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
//  全Direct3Dリソースの解放
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
