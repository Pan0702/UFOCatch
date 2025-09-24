// ========================================================================================
//
// シェーダーの処理                                               ver 3.0        2021.1.11
//
//   シェーダーグラムの読み込みとシェーダーの作成
//   インプットレイアウトの作成
//   コンスタントバッファの作成
//
//
//    登録されているシェーダー
//
//		・シンプルなシェーダー                          InitShaderSimple();
//		・スプライト用のシェーダー                      InitShaderSprite();
//		・FBXStaticMesh/FBXSkinMesh用のシェーダー       InitShaderFbx();
//		・ディスプレイスメントマッピング用のシェーダー  InitShaderDisplace();
//		・エフェクト用のシェーダー                      InitShaderEffect();
//
//                                                                              Shader.cpp
// ========================================================================================

#include "Shader.h"
#include "FbxMesh.h"

//------------------------------------------------------------------------
//
//	シェーダーのコンストラクタ	
//
//  引数　CDirect3D* pD3D
//
//------------------------------------------------------------------------
CShader::CShader(CDirect3D* pD3D)
{
	ZeroMemory(this, sizeof(CShader));
	m_pD3D = pD3D;
}
//------------------------------------------------------------------------
//
//	シェーダーのデストラクタ	
//
//------------------------------------------------------------------------
CShader::~CShader()
{
	// シェーダーの解放  ------------------------------------------

	// 通常用のシンプルなシェーダー
	SAFE_RELEASE(m_pSimple_VertexLayout); 
	SAFE_RELEASE(m_pSimple_VS);
	SAFE_RELEASE(m_pSimple_PS);

	// 3Dスプライト用のシェーダー
	SAFE_RELEASE(m_pSprite3D_VertexLayout);
	SAFE_RELEASE(m_pSprite3D_VS);
	SAFE_RELEASE(m_pSprite3D_PS);
	SAFE_RELEASE(m_pSprite3D_VS_BILL);

	// ディスプレースメントマッピング(波)用のシェーダー
	SAFE_RELEASE(m_pDisplaceWave_VertexLayout);
	SAFE_RELEASE(m_pDisplaceWave_VS);
	SAFE_RELEASE(m_pDisplaceWave_HS);
	SAFE_RELEASE(m_pDisplaceWave_DS);
	SAFE_RELEASE(m_pDisplaceWave_PS);

	// ディスプレースメントマッピング(メッシュ)用のシェーダー
	SAFE_RELEASE(m_pDisplaceSkinMesh_VS);
	SAFE_RELEASE(m_pDisplaceSkinMesh_HS);
	SAFE_RELEASE(m_pDisplaceSkinMesh_DS);
	SAFE_RELEASE(m_pDisplaceSkinMesh_PS);

	SAFE_RELEASE(m_pDisplaceStaticMesh_VS);
	SAFE_RELEASE(m_pDisplaceStaticMesh_HS);
	SAFE_RELEASE(m_pDisplaceStaticMesh_DS);
	SAFE_RELEASE(m_pDisplaceStaticMesh_PS);

	// エフェクト用のシェーダー
	SAFE_RELEASE(m_pEffect3D_VertexLayout);
	SAFE_RELEASE(m_pEffect3D_VS_POINT);
	SAFE_RELEASE(m_pEffect3D_GS_POINT);
	SAFE_RELEASE(m_pEffect3D_PS);
	SAFE_RELEASE(m_pEffect3D_VertexLayout_BILL);
	SAFE_RELEASE(m_pEffect3D_VS_BILL);
	SAFE_RELEASE(m_pEffect3D_VS_BILLMESH);              // -- 2019.7.17

	// Fbxモデル　スタティツクメッシュ用のシェーダー
	// （Normalマッピング）
	SAFE_RELEASE(m_pFbxStaticMesh_VertexLayout);
	SAFE_RELEASE(m_pFbxStaticMesh_VS);
	SAFE_RELEASE(m_pFbxStaticMesh_PS);

	// Fbxモデル　スキンメッシュ用のシェーダー
	// （Normalマッピング）
	SAFE_RELEASE(m_pFbxSkinMesh_VertexLayout);
	SAFE_RELEASE(m_pFbxSkinMesh_VS);
	SAFE_RELEASE(m_pFbxSkinMesh_PS);


	// コンスタントバッファの解放 ---------------------------

	SAFE_RELEASE(m_pConstantBufferDisplace);
	SAFE_RELEASE(m_pConstantBufferEffect);
	SAFE_RELEASE(m_pConstantBufferSprite3D);

	SAFE_RELEASE(m_pConstantBufferWVLED);
	SAFE_RELEASE(m_pConstantBufferBone2);
	SAFE_RELEASE(m_pConstantBufferMaterial);      // -- 2020.12.15

}

//------------------------------------------------------------------------
//
//	各種シェーダーの作成	
//
//  ・シェーダーとコンスタントバッファを作成する
//  ・テクスチャーサンプラーとブレンドステートを作成する
//
//  引数　なし
//
//	戻り値 HRESULT
//		S_OK	= 正常
//		E_FAIL	= 異常
//
//------------------------------------------------------------------------
HRESULT CShader::InitShader()
{
	//  通常メッシュ用のシェーダー作成
	InitShaderSimple();

	//  Fbxスタティック・スキンメッシュ用のシェーダー作成
	InitShaderFbx();

	//  ディスプレースメントマッピング用のシェーダー作成
	InitShaderDisplace();

	//  エフェクト用のシェーダー作成
	InitShaderEffect();

	//  スプライト用のシェーダー作成
	InitShaderSprite();

	//  コンスタントバッファ作成
	InitShaderConstant();

	return S_OK;
}
//------------------------------------------------------------------------
//
//  通常用(Simple Shader)のシェーダー作成
//
//  引数　なし
//
//	戻り値 HRESULT
//		S_OK	= 正常
//		E_FAIL	= 異常
//
//------------------------------------------------------------------------
HRESULT CShader::InitShaderSimple()
{

	// 頂点インプットレイアウトを定義
	UINT numElements = 0;
	D3D11_INPUT_ELEMENT_DESC Simplelayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	numElements = sizeof(Simplelayout) / sizeof(Simplelayout[0]);

	// バーテックスシェーダ・ピクセルシェーダ・頂点インプットレイアウトの作成
	MakeShader(_T("VS"), _T("Simple_VS.cso"), (void**)&m_pSimple_VS, Simplelayout, numElements, &m_pSimple_VertexLayout);
	MakeShader(_T("PS"), _T("Simple_PS.cso"), (void**)&m_pSimple_PS);

	return S_OK;
}

//------------------------------------------------------------------------
//
//  Fbxモデル　スタティツク＆スキンメッシュ用のシェーダー作成
//
//  引数　なし
//
//	戻り値 HRESULT
//		S_OK	= 正常
//		E_FAIL	= 異常
//
//------------------------------------------------------------------------
HRESULT CShader::InitShaderFbx()
{

	// -------------------------------------------------------------------
	// 
	// スタティックメッシュ  FbxStaticMesh
	// 
	// -------------------------------------------------------------------
	// 頂点インプットレイアウトを定義
	UINT numElements = 0;
	D3D11_INPUT_ELEMENT_DESC FbxStaticNM_layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },		// 計32byte
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT,0,32, D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT,0,44, D3D11_INPUT_PER_VERTEX_DATA,0 }, // 計56byte
	};
	numElements = sizeof(FbxStaticNM_layout) / sizeof(FbxStaticNM_layout[0]);

	// バーテックスシェーダ・ピクセルシェーダ・頂点インプットレイアウトの作成
	MakeShader(_T("VS"), _T("FbxStaticMesh_VS.cso"), (void**)&m_pFbxStaticMesh_VS, FbxStaticNM_layout, numElements, &m_pFbxStaticMesh_VertexLayout);
	MakeShader(_T("PS"), _T("FbxStaticMesh_PS.cso"), (void**)&m_pFbxStaticMesh_PS);


	// -------------------------------------------------------------------
	// 
	// スキンメッシュ  FbxSkinMesh
	// 
	// -------------------------------------------------------------------
	// 頂点インプットレイアウトを定義
	numElements = 0;
	D3D11_INPUT_ELEMENT_DESC FbxSkinNM_layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BONE_INDEX", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BONE_WEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 64, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 76, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // 計76Byte
	};
	numElements = sizeof(FbxSkinNM_layout) / sizeof(FbxSkinNM_layout[0]);

	// バーテックスシェーダ・ピクセルシェーダ・頂点インプットレイアウトの作成
	MakeShader(_T("VS"), _T("FbxSkinMesh_VS.cso"), (void**)&m_pFbxSkinMesh_VS, FbxSkinNM_layout, numElements, &m_pFbxSkinMesh_VertexLayout);
	MakeShader(_T("PS"), _T("FbxSkinMesh_PS.cso"), (void**)&m_pFbxSkinMesh_PS);

	return S_OK;
}
//------------------------------------------------------------------------
//
//  ディスプレースメントマッピング用のシェーダー作成
//
//  引数　なし
//
//	戻り値 HRESULT
//		S_OK	= 正常
//		E_FAIL	= 異常
//
//------------------------------------------------------------------------
HRESULT CShader::InitShaderDisplace()
{

	// -----------------------------------------------------------------------------------------------
	// 波のディスプレイスマッピング DisplaceWave
	// -----------------------------------------------------------------------------------------------
	// 頂点インプットレイアウトを定義
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);

	// バーテックスシェーダ・ピクセルシェーダ・頂点インプットレイアウトの作成
	MakeShader(_T("VS"), _T("DisplaceWave_VS.cso"), (void**)&m_pDisplaceWave_VS, layout, numElements, &m_pDisplaceWave_VertexLayout);
	MakeShader(_T("HS"), _T("DisplaceWave_HS.cso"), (void**)&m_pDisplaceWave_HS);
	MakeShader(_T("DS"), _T("DisplaceWave_DS.cso"), (void**)&m_pDisplaceWave_DS);
	MakeShader(_T("PS"), _T("DisplaceWave_PS.cso"), (void**)&m_pDisplaceWave_PS);


	// -----------------------------------------------------------------------------------------------
	// スタティックメッシュのディスプレイスメントマッピング DisplaceStaticMesh
	// -----------------------------------------------------------------------------------------------
	// バーテックスシェーダ・ピクセルシェーダ・ハルシェーダ・ドメインシェーダの作成
	//  !!!!! 頂点インプットレイアウトは、スタティックメッシュのレイアウトを使用	
	MakeShader(_T("VS"), _T("DisplaceStaticMesh_VS.cso"), (void**)&m_pDisplaceStaticMesh_VS);
	MakeShader(_T("HS"), _T("DisplaceStaticMesh_HS.cso"), (void**)&m_pDisplaceStaticMesh_HS);
	MakeShader(_T("DS"), _T("DisplaceStaticMesh_DS.cso"), (void**)&m_pDisplaceStaticMesh_DS);
	MakeShader(_T("PS"), _T("DisplaceStaticMesh_PS.cso"), (void**)&m_pDisplaceStaticMesh_PS);

	// -----------------------------------------------------------------------------------------------
	// スキンメッシュのディスプレイスメントマッピング DisplaceSkinMesh
	// -----------------------------------------------------------------------------------------------
	// バーテックスシェーダ・ピクセルシェーダ・ハルシェーダ・ドメインシェーダの作成
	//  !!!!! 頂点インプットレイアウトは、スキンメッシュのレイアウトを使用	
	MakeShader(_T("VS"), _T("DisplaceSkinMesh_VS.cso"), (void**)&m_pDisplaceSkinMesh_VS);
	MakeShader(_T("HS"), _T("DisplaceSkinMesh_HS.cso"), (void**)&m_pDisplaceSkinMesh_HS);
	MakeShader(_T("DS"), _T("DisplaceSkinMesh_DS.cso"), (void**)&m_pDisplaceSkinMesh_DS);
	MakeShader(_T("PS"), _T("DisplaceSkinMesh_PS.cso"), (void**)&m_pDisplaceSkinMesh_PS);


	return S_OK;
}

//------------------------------------------------------------------------
//
//  エフェクト用のシェーダー作成
//
//  引数　なし
//
//	戻り値 HRESULT
//		S_OK	= 正常
//		E_FAIL	= 異常
//
//------------------------------------------------------------------------
HRESULT CShader::InitShaderEffect()
{

	// -------------------------------------------------------------------
	// 
	// パーティクルのシェーダー
	// 
	// -------------------------------------------------------------------
	// 頂点インプットレイアウトを定義
	UINT numElements = 0;
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	numElements = sizeof(layout) / sizeof(layout[0]);

	// バーテックスシェーダ・ジオメトリシェーダ・頂点インプットレイアウトの作成
	MakeShader(_T("VS"), _T("Effect3D_VS_POINT.cso"), (void**)&m_pEffect3D_VS_POINT, layout, numElements, &m_pEffect3D_VertexLayout);
	MakeShader(_T("GS"), _T("Effect3D_GS_POINT.cso"), (void**)&m_pEffect3D_GS_POINT);


	// -----------------------------------------------------------------------------------------------------
	// 
	// ビルボードのシェーダー
	// 
	// -----------------------------------------------------------------------------------------------------
	//頂点インプットレイアウトを定義
	D3D11_INPUT_ELEMENT_DESC layoutbill[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	numElements = sizeof(layoutbill) / sizeof(layoutbill[0]);

	// バーテックスシェーダ・頂点インプットレイアウトの作成
	MakeShader(_T("VS"), _T("Effect3D_VS_BILL.cso"), (void**)&m_pEffect3D_VS_BILL, layoutbill, numElements, &m_pEffect3D_VertexLayout_BILL);

	// -----------------------------------------------------------------------------------------------------
	// 
	// ビルボードメッシュのシェーダー
	// 
	// -----------------------------------------------------------------------------------------------------
	// バーテックスシェーダの作成
	MakeShader(_T("VS"), _T("Effect3D_VS_BILLMESH.cso"), (void**)&m_pEffect3D_VS_BILLMESH);

	// -----------------------------------------------------------------------------------------------------
	// 
	// 共通のシェーダー
	// 
	// -----------------------------------------------------------------------------------------------------
	// ピクセルシェーダの作成
	MakeShader(_T("PS"), _T("Effect3D_PS.cso"), (void**)&m_pEffect3D_PS);

	return S_OK;
}

//------------------------------------------------------------------------
//
//  スプライト用のシェーダー作成
//
//  引数　なし
//
//	戻り値 HRESULT
//		S_OK	= 正常
//		E_FAIL	= 異常
//
//------------------------------------------------------------------------
HRESULT CShader::InitShaderSprite()
{
	// 頂点インプットレイアウトを定義	
	D3D11_INPUT_ELEMENT_DESC layout_sprite[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof(layout_sprite) / sizeof(layout_sprite[0]);

	// バーテックスシェーダ・ピクセルシェーダ・頂点インプットレイアウトの作成
	MakeShader(_T("VS"), _T("Sprite3D_VS.cso"), (void**)&m_pSprite3D_VS, layout_sprite, numElements, &m_pSprite3D_VertexLayout);
	MakeShader(_T("VS"), _T("Sprite3D_VS_BILL.cso"), (void**)&m_pSprite3D_VS_BILL);
	MakeShader(_T("PS"), _T("Sprite3D_PS.cso"), (void**)&m_pSprite3D_PS);

	return S_OK;
}

//------------------------------------------------------------------------
//
//  シェーダーの作成関数
//
//  引数	TCHAR ProfileName[]	作成するシェーダー種類
//								(VS,PS,GS,HS,DS,CS)
//			TCHAR FileName[]	ＨＬＳＬファイル名
//			void** ppShader		作成するシェーダー(OUT)
//			D3D11_INPUT_ELEMENT_DESC Fluid_layout[]	頂点レイアウト定義(省略可)
//			UINT numElements						頂点レイアウトエレメント数(省略可)
//			ID3D11InputLayout** ppInputLayout		作成する頂点レイアウト(OUT)(省略可)
//
//	戻り値 HRESULT
//		S_OK	= 正常
//		E_FAIL	= 異常
//
//------------------------------------------------------------------------
HRESULT CShader::MakeShader(const TCHAR ProfileName[], const TCHAR FileName[], void** ppShader, D3D11_INPUT_ELEMENT_DESC Fluid_layout[], UINT numElements, ID3D11InputLayout** ppInputLayout)
{
	// コンパイル済みシェーダの読み込み配列
	BYTE* pCso = nullptr;
	DWORD dwCsoSize = 0;

	// コンパイル済みシェーダの読み込みをする
	m_pD3D->ReadCso(FileName, &pCso, &dwCsoSize);

	// シェーダー種類ごとの作成処理
	if (_tcscmp(ProfileName, _T("VS")) == 0)	// バーテックスシェーダー
	{
		if (FAILED(m_pD3D->m_pDevice->CreateVertexShader(pCso, dwCsoSize, nullptr, (ID3D11VertexShader**)ppShader)))
		{
			SAFE_DELETE_ARRAY(pCso);
			MessageBox(0, _T("バーテックスシェーダー作成失敗"), FileName, MB_OK);
			return E_FAIL;
		}
		if (ppInputLayout)	// 頂点インプットレイアウトを作成するとき
		{
			// 頂点インプットレイアウトを作成
			if (FAILED(m_pD3D->m_pDevice->CreateInputLayout(Fluid_layout, numElements, pCso, dwCsoSize, ppInputLayout)))
			{
				MessageBox(0, _T("インプット レイアウト作成失敗"), FileName, MB_OK);
				return E_FAIL;
			}
		}
	}
	else if (_tcscmp(ProfileName, _T("PS")) == 0)	// ピクセルシェーダー
	{
		if (FAILED(m_pD3D->m_pDevice->CreatePixelShader(pCso, dwCsoSize, nullptr, (ID3D11PixelShader**)ppShader)))
		{
			SAFE_DELETE_ARRAY(pCso);
			MessageBox(0, _T("ピクセルシェーダー作成失敗"), FileName, MB_OK);
			return E_FAIL;
		}
	}
	else if (_tcscmp(ProfileName, _T("GS")) == 0)	// ジオメトリシェーダー
	{
		if (FAILED(m_pD3D->m_pDevice->CreateGeometryShader(pCso, dwCsoSize, nullptr, (ID3D11GeometryShader**)ppShader)))
		{
			SAFE_DELETE_ARRAY(pCso);
			MessageBox(0, _T("ジオメトリシェーダー作成失敗"), FileName, MB_OK);
			return E_FAIL;
		}
	}
	else if (_tcscmp(ProfileName, _T("HS")) == 0)	// ハルシェーダー
	{
		if (FAILED(m_pD3D->m_pDevice->CreateHullShader(pCso, dwCsoSize, nullptr, (ID3D11HullShader**)ppShader)))
		{
			SAFE_DELETE_ARRAY(pCso);
			MessageBox(0, _T("ハルシェーダー作成失敗"), FileName, MB_OK);
			return E_FAIL;
		}
	}
	else if (_tcscmp(ProfileName, _T("DS")) == 0)	// ドメインシェーダー
	{
		if (FAILED(m_pD3D->m_pDevice->CreateDomainShader(pCso, dwCsoSize, nullptr, (ID3D11DomainShader**)ppShader)))
		{
			SAFE_DELETE_ARRAY(pCso);
			MessageBox(0, _T("ドメインシェーダー作成失敗"), FileName, MB_OK);
			return E_FAIL;
		}
	}
	else if (_tcscmp(ProfileName, _T("CS")) == 0)	// コンピュートシェーダ
	{
		if (FAILED(m_pD3D->m_pDevice->CreateComputeShader(pCso, dwCsoSize, nullptr, (ID3D11ComputeShader**)ppShader)))
		{
			SAFE_DELETE_ARRAY(pCso);
			MessageBox(0, _T("コンピュートシェーダ作成失敗"), FileName, MB_OK);
			return E_FAIL;
		}
	}
	else {
		SAFE_DELETE_ARRAY(pCso);
		MessageBox(0, _T("シェーダ種類指定エラー"), ProfileName, MB_OK);
		return E_FAIL;
	}

	SAFE_DELETE_ARRAY(pCso);
	return S_OK;

}

//------------------------------------------------------------------------
//
//  各種コンスタントバッファー作成
//
//  引数　なし
//
//	戻り値 HRESULT
//		S_OK	= 正常
//		E_FAIL	= 異常
//
//------------------------------------------------------------------------
HRESULT CShader::InitShaderConstant()
{

	// ディスプレイスメントマッピング用 コンスタントバッファー作成
	MakeConstantBuffer(sizeof(CONSTANT_BUFFER_DISPLACE), &m_pConstantBufferDisplace);

	// エフェクト用 コンスタントバッファー作成
	MakeConstantBuffer(sizeof(CONSTANT_BUFFER_EFFECT), &m_pConstantBufferEffect);

	// スプライト用 コンスタントバッファー作成　ここでは変換行列渡し用
	MakeConstantBuffer(sizeof(CONSTANT_BUFFER_SPRITE), &m_pConstantBufferSprite3D);

	// Fbxコンスタントバッファー作成　ここでは変換行列渡し用
	MakeConstantBuffer(sizeof(CONSTANT_BUFFER_WVLED), &m_pConstantBufferWVLED);

	// Fbxコンスタントバッファー作成　ここではボーン行列渡し用
	MakeConstantBuffer(sizeof(MATRIX4X4) * MAX_BONES, &m_pConstantBufferBone2);

	// コンスタントバッファー作成　マテリアル渡し用                                    // -- 2020.12.15
	MakeConstantBuffer(sizeof(CONSTANT_BUFFER_MATERIAL), &m_pConstantBufferMaterial);

	return S_OK;
}

//------------------------------------------------------------------------
//
//  コンスタントバッファーの作成関数
//
//  引数	UINT	size						作成するコンスタントバッファーのサイズ
//			ID3D11Buffer**  pppConstantBuffer	作成するコンスタントバッファー(OUT)
//
//	戻り値 HRESULT
//		S_OK	= 正常
//		E_FAIL	= 異常
//
//------------------------------------------------------------------------
HRESULT CShader::MakeConstantBuffer(UINT size, ID3D11Buffer**  ppConstantBuffer)
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
		MessageBox(0, _T("コンスタントバッファー 作成失敗"), nullptr, MB_OK);
		return E_FAIL;
	}
	return S_OK;
}

