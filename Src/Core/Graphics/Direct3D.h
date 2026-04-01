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
//                                               Direct3D.h
//
//-----------------------------------------------------------------------------

#pragma once

// 繝倥ャ繝繝ｼ繝輔ぃ繧､繝ｫ縺ｮ繧､繝ｳ繧ｯ繝ｫ繝ｼ繝・
#include <stdio.h>
#include <windows.h>
#include <d3d11.h>
#include <d3dCompiler.h>
#include <d2d1.h>
#include <wincodec.h>

#include "../Game/Macro.h"
#include "../../Utils/MyMath.h"

// 蠢・ｦ√↑繝ｩ繧､繝悶Λ繝ｪ繝輔ぃ繧､繝ｫ縺ｮ繝ｭ繝ｼ繝・
#pragma comment(lib,"winmm.lib")
//#pragma comment(lib, "d2d1.lib")
//#pragma comment(lib,"windowscodecs.lib")
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dCompiler.lib")
#pragma comment(lib,"dxgi.lib")                    // -- 2024.11.2

// 繝槭け繝ｭ
#define ALIGN16 _declspec(align(16))


// 繧｢繝ｳ繝√お繧､繝ｪ繧｢繧ｹ險ｭ螳・------------ // -- 2018.8.3 ---------------------------
// 繧｢繝ｳ繝√お繧､繝ｪ繧｢繧ｹ繧定｡後ｏ縺ｪ縺・→縺・
#define SAMPLE_COUNT    1
#define SAMPLE_QUALITY  0

// 繧｢繝ｳ繝√お繧､繝ｪ繧｢繧ｹ繧定｡後≧縺ｨ縺・
//#define SAMPLE_COUNT    4
//#define SAMPLE_QUALITY  0


// Direct3D繧ｯ繝ｩ繧ｹ
class CDirect3D
{
public:
    // 繝｡繝ｳ繝仙､画焚
    HWND  m_hWnd;
    DWORD m_dwWindowWidth;
    DWORD m_dwWindowHeight;

    // Dx11繧ｳ繧｢繧ｪ繝悶ず繧ｧ繧ｯ繝・
    ID3D11Device* m_pDevice;
    ID3D11DeviceContext* m_pDeviceContext;
    IDXGISwapChain* m_pSwapChain;
    ID3D11RenderTargetView* m_pBackBuffer_TexRTV;   // 繝舌ャ繧ｯ繝舌ャ繝輔ぃ縺ｮRTV
    ID3D11DepthStencilView* m_pBackBuffer_DSTexDSV; // 繝・・繧ｹ繝舌ャ繝輔ぃ縺ｮDSV
    ID3D11Texture2D* m_pBackBuffer_DSTex;    // 繝・・繧ｹ逕ｨ繝・け繧ｹ繝√Ε繝ｪ繧ｽ繝ｼ繧ｹ
    IDXGIAdapter* m_pAdapter;             // -- 2024.11.2

    // 繝悶Ξ繝ｳ繝峨せ繝・・繝・
    ID3D11BlendState* m_pBlendStateNormal;  // 騾壼ｸｸ・井ｸ埼乗・・・
    ID3D11BlendState* m_pBlendStateTrapen;  // 蜊企乗・繝悶Ξ繝ｳ繝・
    ID3D11BlendState* m_pBlendStateAdd;     // 蜉邂怜粋謌・

    // 繝・・繧ｹ繧ｹ繝・Φ繧ｷ繝ｫ繧ｹ繝・・繝・
    ID3D11DepthStencilState* m_pDepthStencilStateDefault;  // 繝・ヵ繧ｩ繝ｫ繝茨ｼ・Write譛牙柑・・
    ID3D11DepthStencilState* m_pDepthStencilStateNoWrite;  // ZWrite辟｡蜉ｹ・磯乗・謠冗判逕ｨ・・
    ID3D11DepthStencilState* m_pDepthStencilStateNoZTest;  // ZTest/ZWrite辟｡蜉ｹ・亥ｸｸ縺ｫ謠冗判・・

    // 繝・け繧ｹ繝√Ε縺ｮ繧ｵ繝ｳ繝励Λ繝ｼ
    ID3D11SamplerState* m_pSampleLinear; // 邱壼ｽ｢陬憺俣
    ID3D11SamplerState* m_pSampleBorder; // 蠅・阜濶ｲ謖・ｮ・

    // WIC COM 繧ｪ繝悶ず繧ｧ繧ｯ繝・CLSID_WICImagingFactory)
    // 逕ｻ蜒剰ｪｭ縺ｿ霎ｼ縺ｿ逕ｨ
    IWICImagingFactory* m_pFactory;    // -- 2018.12.26

    // 繝ｩ繧ｹ繧ｿ繝ｩ繧､繧ｶ繧ｹ繝・・繝・
    ID3D11RasterizerState* m_pRStateR;   // 騾壼ｸｸ陦ｨ遉ｺ・郁｣城擇繧ｫ繝ｪ繝ｳ繧ｰ縺ゅｊ・・
    ID3D11RasterizerState* m_pRStateRW;  // 繝ｯ繧､繝､繝ｼ繝輔Ξ繝ｼ繝陦ｨ遉ｺ逕ｨ

    // 繝ｬ繝ｳ繝繝ｼ繧ｿ繝ｼ繧ｲ繝・ヨ邂｡逅・
    ID3D11RenderTargetView* m_pTarget_TexRTV;   // 迴ｾ蝨ｨ縺ｮ繝ｬ繝ｳ繝繝ｼ繧ｿ繝ｼ繧ｲ繝・ヨRTV
    ID3D11DepthStencilView* m_pTarget_DSTexDSV; // 迴ｾ蝨ｨ縺ｮ繝・・繧ｹ繧ｹ繝・Φ繧ｷ繝ｫDSV

    // ---------------------------------------------------------------
    // 繝｡繧ｽ繝・ラ
    CDirect3D();
    ~CDirect3D();
    HRESULT InitD3D(HWND hWnd, DWORD width, DWORD height);
    HRESULT InitBlendState();
    HRESULT InitDepthStencilState();
    HRESULT InitSampler();
    HRESULT ReadCso(const TCHAR* csoName, BYTE** ppByteArray, DWORD* pCsoSize);
    HRESULT CreateTextureFromFile(const TCHAR* TName, ID3D11Texture2D** ppTexture2D, DWORD& dwImageWidth, DWORD& dwImageHeight, UINT MipLevels = 1);
    HRESULT MakeNoMipmapTexture(const DWORD& dwImageWidth, const DWORD& dwImageHeight, const D3D11_SUBRESOURCE_DATA& initData, ID3D11Texture2D** ppTexture2D);
    HRESULT MakeMipmapTexture(const DWORD& dwImageWidth, const DWORD& dwImageHeight, const D3D11_SUBRESOURCE_DATA& initData, const UINT& MipLevels, ID3D11Texture2D** ppTexture2D);
    HRESULT CreateShaderResourceViewFromFile(const TCHAR* TName, ID3D11ShaderResourceView** ppTexture, DWORD& dwImageWidth, DWORD& dwImageHeight, UINT MipLevels = 1);
    HRESULT CreateShaderResourceViewFromFile(const TCHAR* TName, ID3D11ShaderResourceView** ppTexture, UINT MipLevels = 1);

    // 繝ｬ繝ｳ繝繝ｼ繧ｿ繝ｼ繧ｲ繝・ヨ繝ｻ豺ｱ蠎ｦ繝舌ャ繝輔ぃ蛻ｶ蠕｡
    void SetRenderTarget(ID3D11RenderTargetView* pTexRTV, ID3D11DepthStencilView* pDSTexDSV);
    void ClearRenderTarget(float ClearColor[]);
    void SetZBuffer(bool bZBuf); // Z繝舌ャ繝輔ぃ縺ｮ譛牙柑/辟｡蜉ｹ蛻・ｊ譖ｿ縺・

    // --------------------------------------------------
    void DestroyD3D();
};
