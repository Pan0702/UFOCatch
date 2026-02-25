//-----------------------------------------------------------------------------
//
//  Direct3Dを利用するためのライブラリ                ver 3.5         2024.11.2
// 
//  ① Direct3Dの初期化    
//  ② テクスチャサンプラーとブレンドステートの作成
//  ③ イメージファイルを読み込んでテクスチャを作成する関数
//  ④ コンパイル済みシェーダーの読み込みを行う関数
//  ⑤ レンダリングターゲットの設定を行う関数
//  ⑥ コンパイル済みシェーダーの読み込みを行う関数  x64対応
//
//                                               Direct3D.h
//
//-----------------------------------------------------------------------------

#pragma once

// ヘッダーファイルのインクルード
#include <stdio.h>
#include <windows.h>
#include <d3d11.h>
#include <d3dCompiler.h>
#include <d2d1.h>
#include <wincodec.h>

#include "../Game/Macro.h"
#include "../../Utils/MyMath.h"

// 必要なライブラリファイルのロード
#pragma comment(lib,"winmm.lib")
//#pragma comment(lib, "d2d1.lib")
//#pragma comment(lib,"windowscodecs.lib")
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dCompiler.lib")
#pragma comment(lib,"dxgi.lib")                    // -- 2024.11.2

// マクロ
#define ALIGN16 _declspec(align(16))


// アンチエイリアス設定 ------------ // -- 2018.8.3 ---------------------------
// アンチエイリアスを行わないとき
#define SAMPLE_COUNT    1
#define SAMPLE_QUALITY  0

// アンチエイリアスを行うとき
//#define SAMPLE_COUNT    4
//#define SAMPLE_QUALITY  0


// Direct3Dクラス
class CDirect3D
{
public:
    // メンバ変数
    HWND  m_hWnd;
    DWORD m_dwWindowWidth;
    DWORD m_dwWindowHeight;

    // Dx11コアオブジェクト
    ID3D11Device* m_pDevice;
    ID3D11DeviceContext* m_pDeviceContext;
    IDXGISwapChain* m_pSwapChain;
    ID3D11RenderTargetView* m_pBackBuffer_TexRTV;   // バックバッファのRTV
    ID3D11DepthStencilView* m_pBackBuffer_DSTexDSV; // デプスバッファのDSV
    ID3D11Texture2D* m_pBackBuffer_DSTex;    // デプス用テクスチャリソース
    IDXGIAdapter* m_pAdapter;             // -- 2024.11.2

    // ブレンドステート
    ID3D11BlendState* m_pBlendStateNormal;  // 通常（不透明）
    ID3D11BlendState* m_pBlendStateTrapen;  // 半透明ブレンド
    ID3D11BlendState* m_pBlendStateAdd;     // 加算合成

    // デプスステンシルステート
    ID3D11DepthStencilState* m_pDepthStencilStateDefault;  // デフォルト（ZWrite有効）
    ID3D11DepthStencilState* m_pDepthStencilStateNoWrite;  // ZWrite無効（透明描画用）
    ID3D11DepthStencilState* m_pDepthStencilStateNoZTest;  // ZTest/ZWrite無効（常に描画）

    // テクスチャのサンプラー
    ID3D11SamplerState* m_pSampleLinear; // 線形補間
    ID3D11SamplerState* m_pSampleBorder; // 境界色指定

    // WIC COM オブジェクト(CLSID_WICImagingFactory)
    // 画像読み込み用
    IWICImagingFactory* m_pFactory;    // -- 2018.12.26

    // ラスタライザステート
    ID3D11RasterizerState* m_pRStateR;   // 通常表示（裏面カリングあり）
    ID3D11RasterizerState* m_pRStateRW;  // ワイヤーフレーム表示用

    // レンダーターゲット管理
    ID3D11RenderTargetView* m_pTarget_TexRTV;   // 現在のレンダーターゲットRTV
    ID3D11DepthStencilView* m_pTarget_DSTexDSV; // 現在のデプスステンシルDSV

    // ---------------------------------------------------------------
    // メソッド
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

    // レンダーターゲット・深度バッファ制御
    void SetRenderTarget(ID3D11RenderTargetView* pTexRTV, ID3D11DepthStencilView* pDSTexDSV);
    void ClearRenderTarget(float ClearColor[]);
    void SetZBuffer(bool bZBuf); // Zバッファの有効/無効切り替え

    // --------------------------------------------------
    void DestroyD3D();
};