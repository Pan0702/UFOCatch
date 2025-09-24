//-----------------------------------------------------------------------------
//
//  Direct3Dを利用するためのライブラリ                ver 3.5         2024.11.2
// 
//	①　Direct3Dの初期化	
//	②　テクスチャーサンプラーとブレンドステートの作成
//	③　イメージファイルを読み込んでテクスチャを作成する関数
//	④　コンパイル済みシェーダーの読み込みをする関数
//	⑤　レンダーターゲットの設定をする関数
//	⑥　コンパイル済みシェーダーの読み込みをする関数  x64対応
//
//																Direct3D.cpp
//
//-----------------------------------------------------------------------------

#include "Direct3D.h"
#include "Macro.h"

#pragma warning(disable : 6387)
//------------------------------------------------------------------------
//
//	Direct3Dのコンストラクタ	
//
//  引数　なし
//
//------------------------------------------------------------------------
CDirect3D::CDirect3D()
{
	ZeroMemory(this, sizeof(CDirect3D));
}
//------------------------------------------------------------------------
//
//	Direct3Dのデストラクタ	
//
//------------------------------------------------------------------------
CDirect3D::~CDirect3D()
{
	DestroyD3D();

}
//------------------------------------------------------------------------
//
//	Direct3Dの初期化	
//
//  HWND hWnd         ウィンドウハンドル
//  DWORD dwWidth     作成する画面の幅
//  DWORD dwHeight    作成する画面の高さ
//
//	戻り値 HRESULT
//		S_OK	= 正常
//		E_FAIL	= 異常
//
//------------------------------------------------------------------------
HRESULT CDirect3D::InitD3D(HWND hWnd, DWORD dwWidth, DWORD dwHeight)
{
	//
	m_hWnd = hWnd;
	m_dwWindowWidth = dwWidth;
	m_dwWindowHeight = dwHeight;
	
	// DXGIインターフェイス					// -- 2024.11.2
	IDXGIFactory1* pGFactory = nullptr;
	if (FAILED(CreateDXGIFactory1(__uuidof(IDXGIFactory1), (VOID**)(&pGFactory))))
	{
		return FALSE;
	}
	// グラフィックカードの情報	 					// -- 2024.11.2
	m_pAdapter = nullptr;
	UINT iAdapter = 0;
	IDXGIAdapter* pAdapterTemp = nullptr;
	SIZE_T  vramsize = 0;

	while (pGFactory->EnumAdapters(iAdapter, &pAdapterTemp) != DXGI_ERROR_NOT_FOUND)
	{
		// アダプター情報
		DXGI_ADAPTER_DESC descAdapter;
		pAdapterTemp->GetDesc(&descAdapter);
		if (m_pAdapter == nullptr)
		{
			m_pAdapter = pAdapterTemp;
			vramsize = descAdapter.DedicatedVideoMemory / 1024 / 1024;
		}
		else if (descAdapter.DedicatedVideoMemory / 1024 / 1024 > vramsize) {	   // VRAMの大きいものを選択
			SAFE_RELEASE(m_pAdapter);
			m_pAdapter = pAdapterTemp;
			vramsize = descAdapter.DedicatedVideoMemory / 1024 / 1024;
		}
		else {
			SAFE_RELEASE(pAdapterTemp);
		}
		++iAdapter;
	}
	SAFE_RELEASE(pGFactory);							// -- 2024.11.2

	// デバイスとスワップチェーンの作成
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
	//sd.Flags = DXGI_SWAP_CHAIN_FLAG_GDI_COMPATIBLE;  //  -- 2017.1.28  GetDC対策 エラーになる？

	D3D_FEATURE_LEVEL pFeatureLevels = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL* pFeatureLevel = nullptr;

	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		//nullptr,
		m_pAdapter,								// -- 2024.11.2
		//D3D_DRIVER_TYPE_HARDWARE, 
		D3D_DRIVER_TYPE_UNKNOWN,				// -- 2024.11.2
		nullptr,
		0, 
		&pFeatureLevels, 
		1, 
		D3D11_SDK_VERSION, 
		&sd, 
		&m_pSwapChain, 
		&m_pDevice,
		pFeatureLevel, 
		&m_pDeviceContext);

	if( hr == E_FAIL){
		MessageBox(0, _T("Direct3D.cpp D3Dデバイスとスワップチェーンの作成に失敗しました"), nullptr, MB_OK);
		return E_FAIL;
	}

	//各種テクスチャーと、それに付帯する各種ビューを作成

	//バックバッファーテクスチャーを取得（既にあるので作成ではない）
	ID3D11Texture2D *pBackBuffer_Tex;
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer_Tex);
	//そのテクスチャーに対しレンダーターゲットビュー(RTV)を作成
	m_pDevice->CreateRenderTargetView(pBackBuffer_Tex, nullptr, &m_pBackBuffer_TexRTV);
	SAFE_RELEASE(pBackBuffer_Tex);

	//デプスステンシルビュー用のテクスチャーを作成
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
	//そのテクスチャーに対しデプスステンシルビュー(DSV)を作成
	m_pDevice->CreateDepthStencilView(m_pBackBuffer_DSTex, nullptr, &m_pBackBuffer_DSTexDSV);

	//レンダーターゲットビューと深度ステンシルビューをパイプラインにバインド
	SetRenderTarget(nullptr, nullptr);    // レンダーターゲットの設定   // -- 2019.4.19

	//ビューポートの設定
	D3D11_VIEWPORT vp;
	vp.Width = (float)m_dwWindowWidth;
	vp.Height = (float)m_dwWindowHeight;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_pDeviceContext->RSSetViewports(1, &vp);

	//ラスタライズ設定  (ここの設定で表裏とワイヤーフレーム表示を決める)
	D3D11_RASTERIZER_DESC rdc;
	ZeroMemory(&rdc, sizeof(rdc));

	//rdc.CullMode = D3D11_CULL_NONE;	// 常にすべての三角形を描画
	rdc.CullMode = D3D11_CULL_BACK;		// 後ろ向きの三角形を描画しません

	rdc.FrontCounterClockwise = FALSE;		// 三角形の頂点がレンダーターゲット上で右回りならば三角形は前向きと見なされる  	 // -- 2024.3.23

	rdc.FillMode = D3D11_FILL_SOLID;
	m_pDevice->CreateRasterizerState(&rdc, &m_pRStateR);			 	 // -- 2024.3.23

	rdc.FillMode = D3D11_FILL_WIREFRAME;	// ワイヤーフレーム表示
	m_pDevice->CreateRasterizerState(&rdc, &m_pRStateRW);			 	 // -- 2024.3.23

	m_pDeviceContext->RSSetState(m_pRStateR);  // 右回りは前向きと設定する	 	 // -- 2024.3.23

	// テクスチャーサンプラーを作成する
	InitSampler();

	// ブレンドステートを作成する
	InitBlendState();

	// COM オブジェクト(CLSID_WICImagingFactory)の作成
	hr = CoCreateInstance(
			CLSID_WICImagingFactory,
			nullptr,
			CLSCTX_INPROC_SERVER,
			IID_PPV_ARGS(&m_pFactory)
	);
	if (hr == E_FAIL) {
		MessageBox(0, _T("Direct3D.cpp  COM オブジェクト(CLSID_WICImagingFactory)の作成に失敗しました"), nullptr, MB_OK);
		return E_FAIL;
	}

	// DirectXMathライブラリが使えるかどうかチェックする
	if (XMVerifyCPUSupport() != true)
	{
		MessageBox(0, _T("Direct3D.cpp  DirectXMathライブラリのＳＳＥが使えません。処理を終了いたします"), nullptr, MB_OK);
		return E_FAIL;
	}

	return S_OK;

}

//------------------------------------------------------------------------
//
//	テクスチャーサンプラーを作成する
//
//  引数　なし
//
//	戻り値 HRESULT
//		S_OK	= 正常
//		E_FAIL	= 異常
//
//------------------------------------------------------------------------
HRESULT CDirect3D::InitSampler()
{
	// ----------------------------------------------------------------------
	//テクスチャー用サンプラー作成
	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));

	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	//SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	//SamDesc.Filter = D3D11_FILTER_ANISOTROPIC;

	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	SamDesc.MipLODBias = 0;                            // 計算されたミップマップ レベルからのバイアス
	//SamDesc.MaxAnisotropy = 16;                        // サンプリングに異方性補間を使用している場合の限界値。有効な値は 1 ～ 16 。
	SamDesc.MaxAnisotropy = 1;                         // サンプリングに異方性補間を使用している場合の限界値。有効な値は 1 ～ 16 。
	SamDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;  // 比較オプション。
	SamDesc.MinLOD = 0;                                // アクセス可能なミップマップの下限値
	SamDesc.MaxLOD = D3D11_FLOAT32_MAX;                // アクセス可能なミップマップの上限値

	m_pDevice->CreateSamplerState(&SamDesc, &m_pSampleLinear);


	// ----------------------------------------------------------------------
	//テクスチャー用サンプラー作成 
	// (Border　ラップせず境界色で塗りつぶす)
	ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));

	//SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	//SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	SamDesc.Filter = D3D11_FILTER_ANISOTROPIC;

	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;

	static const VECTOR4 One = VECTOR4(1, 1, 1, 0);
	memcpy_s(SamDesc.BorderColor, sizeof(SamDesc.BorderColor), &One , sizeof(VECTOR4)); // Addressにて_BORDERが指定されている場合に使用する境界色 (範囲: 0.0~1.0)

	SamDesc.MipLODBias = 0;                            // 計算されたミップマップ レベルからのバイアス
	SamDesc.MaxAnisotropy = 16;                        // サンプリングに異方性補間を使用している場合の限界値。有効な値は 1 ～ 16 。
	//SamDesc.MaxAnisotropy = 1;                         // サンプリングに異方性補間を使用している場合の限界値。有効な値は 1 ～ 16 。
	SamDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;  // 比較オプション。
	SamDesc.MinLOD = 0;                                // アクセス可能なミップマップの下限値
	SamDesc.MaxLOD = D3D11_FLOAT32_MAX;                // アクセス可能なミップマップの上限値

	m_pDevice->CreateSamplerState(&SamDesc, &m_pSampleBorder);


	return S_OK;
}


//------------------------------------------------------------------------
//
//	ブレンドステートを作成する
//
//  引数　なし
//
//	戻り値 HRESULT
//		S_OK	= 正常
//		E_FAIL	= 異常
//
//------------------------------------------------------------------------
HRESULT CDirect3D::InitBlendState()
{

	// -------------------------------------------------------------------------------
	// 現在のブレンドステートを取得する
	UINT mask = 0xffffffff;
	m_pDeviceContext->OMGetBlendState(&m_pBlendStateNormal, nullptr, &mask);


	//アルファブレンド用ブレンドステート作成
	//pngファイル内にアルファ情報がある。アルファにより透過するよう指定している
	D3D11_BLEND_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BLEND_DESC));
	bd.IndependentBlendEnable = false;
	bd.AlphaToCoverageEnable = false;

	bd.RenderTarget[0].BlendEnable = true;
	bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	//bd.RenderTarget[0].DestBlend = D3D11_BLEND_DEST_COLOR;  // 半透明?
	bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	if (FAILED(m_pDevice->CreateBlendState(&bd, &m_pBlendStateTrapen)))
	{
		MessageBox(0, _T("Direct3D.cpp アルファブレンド用ブレンドステート作成できません"), _T(""), MB_OK);
		return E_FAIL;
	}

	// 加算合成用ブレンドステート作成
	ZeroMemory(&bd, sizeof(D3D11_BLEND_DESC));
	bd.IndependentBlendEnable = false;
	bd.AlphaToCoverageEnable = false;

	bd.RenderTarget[0].BlendEnable = true;
	bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	bd.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	//bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	if (FAILED(m_pDevice->CreateBlendState(&bd, &m_pBlendStateAdd)))
	{
		MessageBox(0, _T("Direct3D.cpp 加算合成用ブレンドステート作成できません"), _T(""), MB_OK);
		return E_FAIL;
	}

	// デフォルトのブレンドステートをアルファブレンド用ブレンドステートにする            // -- 2020.1.15
	// (透明色のブレンディングを設定)
	m_pDeviceContext->OMSetBlendState(m_pBlendStateTrapen, nullptr, mask);

	return S_OK;
}


//------------------------------------------------------------------------ // -- 2019.4.19
//
//	レンダーターゲットを設定する	
//	(設定したレンダーターゲットテクスチャ情報は、次に設定するまで記録されている)
//
//	引数
//		ID3D11RenderTargetView* m_pTarget_TexRTV;
//		ID3D11DepthStencilView* m_pTarget_DSTexDSV;
//
//	戻り値
//		なし
//------------------------------------------------------------------------
void   CDirect3D::SetRenderTarget(ID3D11RenderTargetView* pTexRTV, ID3D11DepthStencilView* pDSTexDSV)
{
	if (pTexRTV == nullptr)
	{
		// nullptrの時は、バックバッファ
		m_pTarget_TexRTV = m_pBackBuffer_TexRTV;
		m_pTarget_DSTexDSV = m_pBackBuffer_DSTexDSV;
	}
	else {
		m_pTarget_TexRTV = pTexRTV;
		m_pTarget_DSTexDSV = pDSTexDSV;
	}
	m_pDeviceContext->OMSetRenderTargets(1, &m_pTarget_TexRTV, m_pTarget_DSTexDSV);
}

//------------------------------------------------------------------------ // -- 2019.4.19
//
//	レンダーターゲットをクリヤー色でクリヤーする	
//	(設定したレンダーターゲットテクスチャ情報でクリヤー)
//	(ＺバッファがあるときはＺバッファもクリヤー)
//
//	引数
//		float ClearColor[4]:クリヤー色
//
//	戻り値
//		なし
//------------------------------------------------------------------------
void   CDirect3D::ClearRenderTarget(float ClearColor[])
{
	m_pDeviceContext->ClearRenderTargetView(m_pTarget_TexRTV, ClearColor); // 画面クリア
	if (m_pTarget_DSTexDSV)
	{
		m_pDeviceContext->ClearDepthStencilView(m_pTarget_DSTexDSV, D3D11_CLEAR_DEPTH, 1.0f, 0); // 深度バッファクリア
	}
}
//------------------------------------------------------------------------ // -- 2019.4.19
//
//	レンダーターゲットのＺバッファを設定する	
//	(設定したレンダーターゲットテクスチャ情報でＺバッファをＯＮ／ＯＦＦする)
//
//	引数
//		bool bZbuf;    true:ＺバッファＯＮ  false:ＺバッファＯＦＦ
//
//	戻り値
//		なし
//------------------------------------------------------------------------
void   CDirect3D::SetZBuffer(bool bZBuf)
{
	// レンダーターゲットのＺバッファ　ＯＮ／ＯＦＦ
	if (bZBuf)
	{
		m_pDeviceContext->OMSetRenderTargets(1, &m_pTarget_TexRTV, m_pTarget_DSTexDSV);
	}
	else {
		m_pDeviceContext->OMSetRenderTargets(1, &m_pTarget_TexRTV, nullptr);
	}
}



//------------------------------------------------------------------------
//
//	イメージファイルを読み込んでテクスチャを作成する関数
//
//	指定したファイル名のイメージを読み込む
//	テクスチャを作成する
//
//  const TCHAR*		TName				イメージファイル名
//  ID3D11Texture2D**	ppTexture2D(OUT)	作成されたテクスチャ
//  DWORD&				dwImageWidth(OUT)	イメージの幅(OUT)
//  DWORD&				dwImageHeight(OUT)	イメージの高さ(OUT)
//  UINT				MipLevels			ミップマップレベル(省略値は１)
//
//	戻り値 HRESULT
//		S_OK	= 正常
//		E_FAIL	= ファイル名が見つからない
//------------------------------------------------------------------------
HRESULT CDirect3D::CreateTextureFromFile(const TCHAR* TName, ID3D11Texture2D** ppTexture2D, DWORD& dwImageWidth, DWORD& dwImageHeight, UINT MipLevels)
{
	// ---------------------------------------------------------------------------------------
	//
	// Windows Imaging Component を使った画像ファイルの読み書き
	// 
	//     Windows Imaging Component は Windows SDK に含まれており、
	//     静止画や動画を読み書きするための API が提供されています。
	//     作業手順は下記の通りです。
	//       1.画像ファイルから画素イメージを取得する。
	//       2.画像フォーマットのコンバート
	//       3.画像データからテクスチャを生成する
	//
	// ---------------------------------------------------------------------------------------

	// ---------------------------------------------------------------------------
	//
	//     1.画像ファイルから画素イメージを取得する。
	//     		①　IWICImaginFactory オブジェクトからのデコーダの生成（画像ファイルの読み込み）
	//     		②　デコーダからのフレームの生成
	//     		③　フレームからの画像サイズの取得
	//     		④　フレームから画像フォーマットの取得
	//
	// ---------------------------------------------------------------------------

	IWICBitmapDecoder* decoder = nullptr;

	// 画像ファイルを読み込んでデコーダを作成する

#if _UNICODE
	// unicodeの場合
	if (FAILED(m_pFactory->CreateDecoderFromFilename(TName, 0,
				GENERIC_READ, WICDecodeMetadataCacheOnDemand, &decoder)))
	{
		//MessageBox(0, _T("Direct3D.cpp スプライト　テクスチャーを読み込めません"), TName, MB_OK);
		return E_FAIL;
	}
#else
	// マルチバイト文字の場合、
	// 文字列のCHAR型をWCHAR型に変換
	WCHAR WName[512] = { 0x00 };   // 変換後に文字列最後の\0が作成されないのでまず受取側をゼロクリヤーしておく
	MultiByteToWideChar(CP_OEMCP, MB_PRECOMPOSED, TName, (int)strlen(TName), WName, (sizeof(WName)) / 2);
	if (FAILED(m_pFactory->CreateDecoderFromFilename(WName, 0,
				GENERIC_READ, WICDecodeMetadataCacheOnDemand, &decoder)))
	{
		//MessageBox(0, _T("Direct3D.cpp スプライト　テクスチャーを読み込めません"), TName, MB_OK);
		return E_FAIL;
	}
#endif

	// デコーダからフレームを作成する
	IWICBitmapFrameDecode* frame = nullptr;
	if (FAILED(decoder->GetFrame(0, &frame)))
	{
		MessageBox(0, _T("Direct3D.cpp decoderからframeを取得できません"), TName, MB_OK);
		return E_FAIL;
	}

	// フレームから画像サイズの取得
	UINT width, height;
	if (FAILED(frame->GetSize(&width, &height)))
	{
		MessageBox(0, _T("Direct3D.cpp フレームからサイズを取得できません"), TName, MB_OK);
		return E_FAIL;

	}

	dwImageWidth = width;
	dwImageHeight = height;

	// フレームから画像フォーマットの取得
	WICPixelFormatGUID pixelFormat;
	if (FAILED(frame->GetPixelFormat(&pixelFormat)))
	{
		MessageBox(0, _T("Direct3D.cpp フレームからピクセルフォーマットを取得できません"), TName, MB_OK);
		return E_FAIL;

	}

	// ---------------------------------------------------------------------------
	//
	//     2.画像フォーマットのコンバート
	//     		①　画像フォーマットがRGBA（32ビット）の形式であるかどうか調べる
	//     		②　RGBA（32ビット）形式でない場合はフォーマットをRGBA形式にコンバートする
	//     		③　デコーダ（コンバート後）から画像データをコピーする
	//
	// ---------------------------------------------------------------------------

	// 作成するイメージデータ（RGBA（32ビット）形式）
	UINT  ImageStride = dwImageWidth * 4;					// イメージの幅(バイト数)　(注)1ピクセルが32ビット(4バイト)
	UINT  ImageSize = dwImageWidth * dwImageHeight * 4;		// イメージの大きさ(バイト数)
	BYTE* pImageData = new BYTE[ImageSize];					// イメージを格納するバッファ

	// RGBA（32ビット）の形式でなかったら、RGBA形式にフォーマットコンバートする
	if (pixelFormat != GUID_WICPixelFormat32bppRGBA)  // 画像フォーマットがRGBA（32ビット）か
	{
		// RGBA（32ビット）の形式でないので、RGBA形式にフォーマットコンバートする
		IWICFormatConverter* FC = nullptr;

		if (FAILED(m_pFactory->CreateFormatConverter(&FC)))   // フォーマットコンバータの生成
		{
			MessageBox(0, _T("Direct3D.cpp フォーマットコンバータ作成できません"), TName, MB_OK);
			return E_FAIL;
		}

		if (FAILED(FC->Initialize(frame, GUID_WICPixelFormat32bppRGBA      // フォーマットコンバートする
			, WICBitmapDitherTypeErrorDiffusion
			, 0, 0, WICBitmapPaletteTypeCustom)))
		{
			MessageBox(0, _T("Direct3D.cpp フォーマットコンバートInitializeできません"), TName, MB_OK);
			return E_FAIL;
		}
		FC->CopyPixels(0, ImageStride, ImageSize, pImageData);   // コンバート後の画像データをpImageDataにコピーする

		SAFE_RELEASE(FC);

	}
	else {
		frame->CopyPixels(0, ImageStride, ImageSize, pImageData); // frameの画像データをpImageDataにコピーする
	}

	// ---------------------------------------------------------------------------------------
	//
	//     3.画像データからテクスチャを生成する
	//
	// ---------------------------------------------------------------------------------------

	// サブリソースデーターを作成する
	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = pImageData;
	initData.SysMemPitch = ImageStride;
	initData.SysMemSlicePitch = ImageSize;

	// ミップマップテクスチャを作成するか
	if (MipLevels == 1)
	{
		// ミップマップテクスチャを作成しない
		if (FAILED(MakeNoMipmapTexture(dwImageWidth, dwImageHeight, initData, ppTexture2D)))
		{
			MessageBox(0, _T("Direct3D.cpp ミップマップ無し　テクスチャを生成できません"), TName, MB_OK);
			return E_FAIL;
		}
	}
	else {
		// ミップマップテクスチャを作成する
		if (FAILED(MakeMipmapTexture(dwImageWidth, dwImageHeight, initData, MipLevels, ppTexture2D)))
		{
			MessageBox(0, _T("Direct3D.cpp ミップマップ　テクスチャを生成できません"), TName, MB_OK);
			return E_FAIL;
		}
	}


	//
	// 一時ファイルの解放
	//
	SAFE_RELEASE(frame);
	SAFE_RELEASE(decoder);
	SAFE_DELETE_ARRAY(pImageData);

	return S_OK;

}


//------------------------------------------------------------------------
//
//	サブリソースデーターからテクスチャを作成する関数
//	（ミップマップ無し）
//
//  const DWORD&				dwImageWidth	イメージの幅
//  const DWORD&				dwImageHeight	イメージの高さ
//  D3D11_SUBRESOURCE_DATA		initData		サブリソースデーター
//  ID3D11Texture2D**			ppTexture2D(OUT)	作成されたテクスチャ
//
//	戻り値 HRESULT
//		S_OK	= 正常
//		E_FAIL	= ファイル名が見つからない
//------------------------------------------------------------------------
HRESULT CDirect3D::MakeNoMipmapTexture( const DWORD& dwImageWidth, const DWORD& dwImageHeight, const D3D11_SUBRESOURCE_DATA& initData, ID3D11Texture2D**	ppTexture2D)
{

	// テクスチャの作成
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

	if (FAILED(m_pDevice->CreateTexture2D(&desc, &initData, ppTexture2D)))
	{
		//MessageBox(0, _T("Direct3D.cpp ミップマップ無し　テクスチャを生成できません"), nullptr, MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

//------------------------------------------------------------------------
//
//	サブリソースデーターからテクスチャを作成する関数
//	（ミップマップ有り）
//
//  const DWORD&				dwImageWidth	イメージの幅
//  const DWORD&				dwImageHeight	イメージの高さ
//  const D3D11_SUBRESOURCE_DATA& initData		サブリソースデーター
//  const UINT&					MipLevels		ミップマップレベル
//  ID3D11Texture2D**			ppTexture2D(OUT)	作成されたテクスチャ
//
//	戻り値 HRESULT
//		S_OK	= 正常
//		E_FAIL	= ファイル名が見つからない
//------------------------------------------------------------------------
HRESULT CDirect3D::MakeMipmapTexture(const DWORD& dwImageWidth, const DWORD& dwImageHeight, const D3D11_SUBRESOURCE_DATA& initData, const UINT& MipLevels, ID3D11Texture2D**	ppTexture2D)
{

	// 画像データから一時テクスチャの作成
	ID3D11Texture2D* pWTexture2D;

	D3D11_TEXTURE2D_DESC desc;
	desc.Width = dwImageWidth;
	desc.Height = dwImageHeight;
	desc.MipLevels = 1;           // ここではミップマップは１にする
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_STAGING;             // GPU から CPU へのデータ転送 (コピー) をサポート
	desc.BindFlags = 0;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;  // リソースをマップ可能にし、CPU がそのリソースの内容を読み取れるようにする
	desc.MiscFlags = 0;

	if (FAILED(m_pDevice->CreateTexture2D(&desc, &initData, &pWTexture2D)))
	{
		//MessageBox(0, _T("Direct3D.cpp ミップマップ作成用一時テクスチャを生成できません"), nullptr, MB_OK);
		return E_FAIL;
	}

	// 空テクスチャー作成
	desc.Width = dwImageWidth;
	desc.Height = dwImageHeight;
	desc.MipLevels = MipLevels;            // ミップマップレベルの指定
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_RENDER_TARGET |           // 描画ターゲット（ミップマップの生成をするときは必須）
						D3D11_BIND_SHADER_RESOURCE;       // シェーダで使う
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;   // 後でミップマップの生成ができるようにする

	if (FAILED(m_pDevice->CreateTexture2D(&desc, nullptr, ppTexture2D)))
	{
		//MessageBox(0, _T("Direct3D.cpp 空テクスチャを生成できません"), nullptr, MB_OK);
		return E_FAIL;
	}

	D3D11_MAPPED_SUBRESOURCE mappedResource;

	// 画像データから作成したテクスチャーを、読み込みモードでマップする
	if (FAILED(m_pDeviceContext->Map(pWTexture2D, NULL, D3D11_MAP_READ, 0, &mappedResource)))
	{
		//MessageBox(0, _T("Direct3D.cpp Map処理できません"), nullptr, MB_OK);
		return E_FAIL;
	}

	// 空テクスチャーのサブリソースを画像データから作成したテクスチャーの内容で更新する
	// 本来は　for (UINT iMip = 0; iMip < desc.MipLevels; iMip++)　まで更新すべきだが
	// 元データにミップマップがないため、１レベルのみ更新する
	for (UINT iMip = 0; iMip < 1; iMip++)
	{
		m_pDeviceContext->UpdateSubresource(*ppTexture2D,
										D3D11CalcSubresource(iMip, 0, desc.MipLevels),
										nullptr,
										mappedResource.pData,
										mappedResource.RowPitch,
										0
		);
	}

	m_pDeviceContext->Unmap(pWTexture2D, 0);

	SAFE_RELEASE(pWTexture2D);

	return S_OK;

}
//------------------------------------------------------------------------
//
//	イメージファイルを読み込んでシェーダーリソースビューを作成する関数
//
//	指定したファイル名のイメージを読み込みテクスチャを作成する
//	(CreateTextureFromFileメソッドを使用する)
//	テクスチャからシェーダーリソースビューを作成する
//
//  const TCHAR*		TName					イメージファイル名
//  ID3D11ShaderResourceView** ppTexture(OUT)	作成されたシェーダーリソースビュー
//  DWORD&				dwImageWidth(OUT)		イメージの幅(OUT)
//  DWORD&				dwImageHeight(OUT)		イメージの高さ(OUT)
//  UINT				MipLevels				ミップマップレベル(省略値は１)
//
//	戻り値 HRESULT
//		S_OK	= 正常
//		E_FAIL	= ファイル名が見つからない
//------------------------------------------------------------------------
HRESULT CDirect3D::CreateShaderResourceViewFromFile(const TCHAR* TName, ID3D11ShaderResourceView** ppTexture, DWORD& dwImageWidth, DWORD& dwImageHeight, UINT MipLevels)
{
	// ---------------------------------------------------------------------------------------
	//
	//     画像データからテクスチャを生成する
	//     		①　画像データからテクスチャを生成
	//     		②　テクスチャからシェーダリソースビューを生成
	//
	// ---------------------------------------------------------------------------------------

	ID3D11Texture2D*	pTexture2D = nullptr;

	// ミップマップレベルを調整する
	UINT  MaxMipLevels = 8;
	if (MipLevels < 1 || MipLevels > MaxMipLevels)
	{
		MipLevels = MaxMipLevels;
	}

	//  画像データからテクスチャを生成する
	if (FAILED(CreateTextureFromFile(TName, &pTexture2D, dwImageWidth, dwImageHeight, MipLevels)))
	{
		return E_FAIL;
	}

	// テクスチャからシェーダーリソースビューm_pTextureの作成
	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
	SRVDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D.MipLevels = MipLevels;

	if (FAILED(m_pDevice->CreateShaderResourceView(pTexture2D, &SRVDesc, ppTexture)))
	{
		MessageBox(0, _T("Direct3D.cpp テクスチャShaderResourceViewを作成できません"), TName, MB_OK);
		return E_FAIL;
	}

	// ミップマップを生成(ミップマップレベルが２以上の場合のみ必要)
	// ミップマップが２以上の場合に、テクスチャ上に画像データが１レベル分
	// しかないため、ここで全てのミップマップを生成する
	if (MipLevels > 1) m_pDeviceContext->GenerateMips(*ppTexture);

	//
	// 一時ファイルの解放
	//
	SAFE_RELEASE(pTexture2D);

	return S_OK;
}

//------------------------------------------------------------------------
//
//	イメージファイルを読み込んでシェーダーリソースビューを作成する関数
//
//	指定したファイル名のイメージを読み込みテクスチャを作成する
//	(CreateTextureFromFileメソッドを使用する)
//	テクスチャからシェーダーリソースビューを作成する
//
//  const TCHAR*		TName					イメージファイル名
//  ID3D11ShaderResourceView** ppTexture(OUT)	作成されたシェーダーリソースビュー
//  UINT				MipLevels				ミップマップレベル(省略値は１)
//
//	戻り値 HRESULT
//		S_OK	= 正常
//		E_FAIL	= ファイル名が見つからない
//------------------------------------------------------------------------
HRESULT CDirect3D::CreateShaderResourceViewFromFile(const TCHAR* TName, ID3D11ShaderResourceView** ppTexture, UINT MipLevels)
{
	DWORD dwImageWidth;
	DWORD dwImageHeight;

	return CreateShaderResourceViewFromFile(TName, ppTexture, dwImageWidth, dwImageHeight, MipLevels);
}

//------------------------------------------------------------------------ // -- 2022.9.14
//
//	コンパイル済みシェーダーの読み込みをする関数
//
//  x64に対応している
//
// 引数
//      const TCHAR* csoName       ＣＳＯファイル名
//      BYTE** ppByteArray         ＣＳＯを受け取るBYTE配列の先頭アドレス(OUT)
//      DWORD* pCsoSize            ＣＳＯのサイズアドレス(OUT)
//
//	戻り値 HRESULT
//		S_OK	= 正常
//		E_FAIL	= 異常
//
//------------------------------------------------------------------------
HRESULT CDirect3D::ReadCso(const TCHAR* csoName, BYTE** ppByteArray, DWORD* pCsoSize)
{
	FILE* fp;

#ifdef _M_AMD64
	// x64の場合
#ifdef _DEBUG
	TCHAR FName[256] = _T("x64/Debug/");
#else
	TCHAR FName[256] = _T("x64/Release/");
#endif
#else
	// x86の場合
#ifdef _DEBUG
	TCHAR FName[256] = _T("Debug/");
#else
	TCHAR FName[256] = _T("Release/");
#endif
#endif

#if _UNICODE
	// unicodeの場合
	lstrcat(FName, csoName);
	int ret = _wfopen_s(&fp, FName, L"rb");
#else
	// マルチバイト文字の場合、
	strcat_s(FName, csoName);
	int ret = fopen_s(&fp, FName, "rb");
#endif

	if (ret != 0) {
		MessageBox(0, FName, _T("Direct3D.cpp CSOファイル 読み込み失敗"), MB_OK);
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
//
//	各種オブジェクトをリリース	
//
//  引数　なし
//
//	戻り値 なし
//
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

	SAFE_RELEASE(m_pRStateR);	   	 // -- 2024.3.23
	SAFE_RELEASE(m_pRStateRW);	   	 // -- 2024.3.23

	SAFE_RELEASE(m_pFactory);      // -- 2018.12.26

	SAFE_RELEASE(m_pDevice);

	SAFE_RELEASE(m_pAdapter);		// -- 2024.11.2
}

