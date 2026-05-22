//-----------------------------------------------------------------------------
//
//  Direct3Dを利用するためのライブラリ                ver 3.5         2024.11.2
// 
//  ④ コンパイル済みシェーダーの読み込みを行う関数
//
//                                               Direct3D.h
//
//-----------------------------------------------------------------------------

#pragma once

#include <stdio.h>
#include <windows.h>
#include <d3d11.h>
#include <d3dCompiler.h>
#include <d2d1.h>
#include <wincodec.h>

#include "../Game/Macro.h"
#include "../../Utils/MyMath.h"

#pragma comment(lib,"winmm.lib")
//#pragma comment(lib, "d2d1.lib")
//#pragma comment(lib,"windowscodecs.lib")
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dCompiler.lib")
#pragma comment(lib,"dxgi.lib")                    // -- 2024.11.2

// マクロ
#define ALIGN16 _declspec(align(16))


// アンチエイリアス設定------------ // -- 2018.8.3 ---------------------------
#define SAMPLE_COUNT    1
#define SAMPLE_QUALITY  0

// アンチエイリアスを行うと描画品質が上がるが負荷も増える
//#define SAMPLE_COUNT    4
//#define SAMPLE_QUALITY  0


// Direct3Dクラス
class CDirect3D
{
public:
    // メンバ変数
    HWND m_hWnd;
    DWORD m_dwWindowWidth;
    DWORD m_dwWindowHeight;

    // Dx11コアオブジェクト
    ID3D11Device* m_pDevice;
    ID3D11DeviceContext* m_pDeviceContext;
    IDXGISwapChain* m_pSwapChain;
    ID3D11RenderTargetView* m_pBackBuffer_TexRTV; // バックバッファのRTV
    ID3D11DepthStencilView* m_pBackBuffer_DSTexDSV;
    ID3D11Texture2D* m_pBackBuffer_DSTex;
    IDXGIAdapter* m_pAdapter; // -- 2024.11.2

    ID3D11BlendState* m_pBlendStateNormal;
    ID3D11BlendState* m_pBlendStateTrapen;
    ID3D11BlendState* m_pBlendStateAdd;

    ID3D11DepthStencilState* m_pDepthStencilStateDefault;
    ID3D11DepthStencilState* m_pDepthStencilStateNoWrite;
    ID3D11DepthStencilState* m_pDepthStencilStateNoZTest;

    ID3D11SamplerState* m_pSampleLinear; // 線形補間
    ID3D11SamplerState* m_pSampleBorder;

    // WIC COM オブジェクトCLSID_WICImagingFactory)
    // 画像読み込み用
    IWICImagingFactory* m_pFactory; // -- 2018.12.26

    ID3D11RasterizerState* m_pRStateR;
    ID3D11RasterizerState* m_pRStateRW; // ワイヤーフレーム表示用

    ID3D11RenderTargetView* m_pTarget_TexRTV;
    ID3D11DepthStencilView* m_pTarget_DSTexDSV;

    // ---------------------------------------------------------------
    /// CDirect3D を初期化する
    CDirect3D();
    /// CDirect3D の終了処理を行う
    ~CDirect3D();
    /// D3 D を初期化する
    /// @param hWnd hWnd に渡す値
    /// @param width 幅
    /// @param height 高さ
    /// @return 処理結果
    HRESULT InitD3D(HWND hWnd, DWORD width, DWORD height);
    /// Blend State を初期化する
    /// @return 処理結果
    HRESULT InitBlendState();
    /// Depth Stencil State を初期化する
    /// @return 処理結果
    HRESULT InitDepthStencilState();
    /// Sampler を初期化する
    /// @return 処理結果
    HRESULT InitSampler();
    /// Cso を読み込む
    /// @param csoName 名前
    /// @param ppByteArray 判定に使用するレイ
    /// @param pCsoSize サイズ
    /// @return 処理結果
    HRESULT ReadCso(const TCHAR* csoName, BYTE** ppByteArray, DWORD* pCsoSize);
    /// Texture From File を作成する
    /// @param TName 名前
    /// @param ppTexture2D 使用するテクスチャ
    /// @param dwImageWidth 幅
    /// @param dwImageHeight 高さ
    /// @param MipLevels MipLevels に渡す値
    /// @return 処理結果
    HRESULT CreateTextureFromFile(const TCHAR* TName, ID3D11Texture2D** ppTexture2D, DWORD& dwImageWidth,
                                  DWORD& dwImageHeight, UINT MipLevels = 1);
    /// No Mipmap Texture を作成する
    /// @param dwImageWidth 幅
    /// @param dwImageHeight 高さ
    /// @param initData initData に渡す値
    /// @param ppTexture2D 使用するテクスチャ
    /// @return 処理結果
    HRESULT MakeNoMipmapTexture(const DWORD& dwImageWidth, const DWORD& dwImageHeight,
                                const D3D11_SUBRESOURCE_DATA& initData, ID3D11Texture2D** ppTexture2D);
    /// Mipmap Texture を作成する
    /// @param dwImageWidth 幅
    /// @param dwImageHeight 高さ
    /// @param initData initData に渡す値
    /// @param MipLevels MipLevels に渡す値
    /// @param ppTexture2D 使用するテクスチャ
    /// @return 処理結果
    HRESULT MakeMipmapTexture(const DWORD& dwImageWidth, const DWORD& dwImageHeight,
                              const D3D11_SUBRESOURCE_DATA& initData, const UINT& MipLevels,
                              ID3D11Texture2D** ppTexture2D);
    /// Shader Resource View From File を作成する
    /// @param TName 名前
    /// @param ppTexture 使用するテクスチャ
    /// @param dwImageWidth 幅
    /// @param dwImageHeight 高さ
    /// @param MipLevels MipLevels に渡す値
    /// @return 処理結果
    HRESULT CreateShaderResourceViewFromFile(const TCHAR* TName, ID3D11ShaderResourceView** ppTexture,
                                             DWORD& dwImageWidth, DWORD& dwImageHeight, UINT MipLevels = 1);
    /// Shader Resource View From File を作成する
    /// @param TName 名前
    /// @param ppTexture 使用するテクスチャ
    /// @param MipLevels MipLevels に渡す値
    /// @return 処理結果
    HRESULT CreateShaderResourceViewFromFile(const TCHAR* TName, ID3D11ShaderResourceView** ppTexture,
                                             UINT MipLevels = 1);

    /// Render Target を設定する
    /// @param pTexRTV 使用するテクスチャ
    /// @param pDSTexDSV 使用するテクスチャ
    void SetRenderTarget(ID3D11RenderTargetView* pTexRTV, ID3D11DepthStencilView* pDSTexDSV);
    /// Render Target をクリアする
    void ClearRenderTarget(float ClearColor[]);
    /// ZBuffer を設定する
    /// @param bZBuf bZBuf に渡す値
    void SetZBuffer(bool bZBuf);

    // --------------------------------------------------
    /// D3 D を破棄する
    void DestroyD3D();
};
