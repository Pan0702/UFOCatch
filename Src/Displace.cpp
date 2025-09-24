//------------------------------------------------------------------------
//
//   ディスプレイスメントマッピングを利用するライブラリ
//                                                ver 3.3
//
//      (波に加え、溶岩等の処理に対応する)
//
//																 2024.3.23
//------------------------------------------------------------------------

#include "Displace.h"
#include "GameMain.h"

//------------------------------------------------------------------------
//
//	波のコンストラクタ	
//
//  引数　
//
//------------------------------------------------------------------------
CWave::CWave() : CWave(GameDevice()->m_pShader)				 // -- 2024.3.23
{
}
CWave::CWave(const int& iWidth, const int& iHeight, const TCHAR* FName, const TCHAR* TexName, const TCHAR* SpecName)
					: CWave(GameDevice()->m_pShader, iWidth, iHeight, FName, TexName, SpecName)
{
}
// -----------------------------------------------------------------------
CWave::CWave(CShader* pShader)
{
	ZeroMemory(this, sizeof(CWave));
	m_pD3D = pShader->m_pD3D;
	m_pShader = pShader;

	Init();  // 初期化関数
}
//------------------------------------------------------------------------
//
//	波のコンストラクタ	
//
//  CShader* pShader
//  const int iWidth      波の大きさ　幅
//  const int iHeight     波の大きさ　奥行き
//  const TCHAR* FName     ディスプレイスメントマッピング用のノーマルテクスチャ
//
//------------------------------------------------------------------------
CWave::CWave(CShader* pShader, const int& iWidth, const int& iHeight, const TCHAR* FName, const TCHAR* TexName, const TCHAR* SpecName)
{
	ZeroMemory(this, sizeof(CWave));
	m_pD3D = pShader->m_pD3D;
	m_pShader = pShader;

	Init();  // 初期化関数
	SetSrc(iWidth, iHeight, FName, TexName, SpecName);  //	波の描画用に平面ポリゴンを生成する
}
//------------------------------------------------------------------------
//
//	波のデストラクタ	
//
//------------------------------------------------------------------------
CWave::~CWave()
{
	DeleteAll();
}
//------------------------------------------------------------------------
//	初期化関数
//																		2017.2.18
//
//  各種設定値の初期化
//
//  引数　　なし
//
//  戻り値　　　なし
//------------------------------------------------------------------------
void CWave::Init()
{
	m_vWaveMove    = VECTOR4(0.0f, 0.0f, 0.0f, 0.0f);
	m_vWaveSpeed   = VECTOR2(0.0004f, 0.0002f);		      // 波の移動速度
	m_fWaveHeight  = 0.35f;							      // 波の高さ
	m_vDiffuse     = VECTOR4(0.0f, 0.35f, 0.65f, 0.8f);   // ディフューズカラー
	m_vSpecular    = VECTOR4(0.8f, 0.8f, 0.8f, 1.0f);     // スペキュラー
	m_fMinDistance = 1.0;    // 最小距離(最大分割数に達する距離)
	m_fMaxDistance = 30.0;   // 最大距離(この距離から分割が始まる)
	m_iMaxDevide   = 128;    // １つのポリゴンの最大分割数

}

//------------------------------------------------------------------------
//	解放・削除関数
//																		2017.2.18
//
//  各種リソースの解放と削除
//
//  引数　　なし
//
//  戻り値　　　なし
//------------------------------------------------------------------------
void CWave::DeleteAll()
{
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pIndexBuffer);
	SAFE_DELETE_ARRAY(m_pIndexBuffer);
	SAFE_RELEASE(m_pTexture);
	SAFE_RELEASE(m_pNormalTexture);
	SAFE_RELEASE(m_pSpecularTexture);         // -- 2021.2.4
}

//------------------------------------------------------------------------  // -- 2021.2.4
//	波の描画用に平面ポリゴンを生成する
//
//
//	幅と奥行きを指定して、複数の四角ポリゴンを生成する
//	ノーマルテクスチャを読み込んで設定する
//
//	const int&   iWidth  :全体の幅
//  const int&   iHeight :全体の奥行き
//  const TCHAR* FName    ディスプレイスメントマッピング用のノーマルテクスチャ(ハイトマップとして使用)
//  const TCHAR* TexName  ディフューズテクスチャ名(省略可)
//  const TCHAR* SpecName スペキュラテクスチャ名(省略可)
//
//  戻り値　　　正常:S_OK   異常:E_FAILE
//------------------------------------------------------------------------
HRESULT CWave::SetSrc(const int& iWidth, const int& iHeight, const TCHAR* FName, const TCHAR* TexName, const TCHAR* SpecName)
{
	// 平面の四角形モデル

	DeleteAll();  // 各種リソースの削除。もし、２度目以降に呼び出されたときの対策。

/*
	// １枚ポリゴンのバーテックスデータを一つ（三角ポリゴン2つ）のみ作成するとき

	float RSize = 2.0f;    // テクスチャのサイズ

	//バーテックスデータ作成
	DISPLACE_VERTEX pVertices[] =
	{
		{ VECTOR3(-iWidth/2, 0, -iHeight/2), VECTOR2(           0, iHeight/RSize), VECTOR3(0, 1, 0) },    //頂点0  左下
		{ VECTOR3(-iWidth/2, 0,  iHeight/2), VECTOR2(           0,             0), VECTOR3(0, 1, 0) },    //頂点1  左上
		{ VECTOR3( iWidth/2, 0,  iHeight/2), VECTOR2(iWidth/RSize,             0), VECTOR3(0, 1, 0) },    //頂点2  右上
		{ VECTOR3( iWidth/2, 0, -iHeight/2), VECTOR2(iWidth/RSize, iHeight/RSize), VECTOR3(0, 1, 0) },    //頂点3  右下
	};
	// インデックスデータ作成　3角形リスト×２
	DWORD pIndex[] = { 
		0, 1, 2, 0, 2, 3         // -- 2024.3.23
		//0, 2, 1, 0, 3, 2
	};

	m_dwNumIndex = 2 * 3;
*/

	// １辺RSizeの四角ポリゴン（三角ポリゴン2つ）のバーテックスデータを複数作成するとき
	// 指定サイズがRSizeの整数倍でなかったとき、端数は切り捨てされる

	float RSize = 4.0f;    // ポリゴン４角形　１辺の長さ。テクスチャのサイズでもある。

	// 一つの四角ポリゴンの基本データ
	DISPLACE_VERTEX VertexConst[] =
	{
		{ VECTOR3(    0, 0,     0), VECTOR2(0, 1), VECTOR3(0, 1, 0) },    //頂点0  左下
		{ VECTOR3(    0, 0, RSize), VECTOR2(0, 0), VECTOR3(0, 1, 0) },    //頂点1  左上
		{ VECTOR3(RSize, 0, RSize), VECTOR2(1, 0), VECTOR3(0, 1, 0) },    //頂点2  右上
		{ VECTOR3(RSize, 0,     0), VECTOR2(1, 1), VECTOR3(0, 1, 0) },    //頂点3  右下
	};
	// インデックスデータ作成　3角形リスト×２の基本データ
	DWORD IndexConst[] = {
		0, 1, 2, 0, 2, 3      // -- 2024.3.23
		//0, 2, 1, 0, 3, 2
	};

	// 横方向と縦方向に何個のポリゴンが必要か
	DWORD numX = (DWORD)(iWidth / RSize);
	DWORD numZ = (DWORD)(iHeight / RSize);

	// 必要数の頂点データとインデックスデータを格納する配列を確保する
	DISPLACE_VERTEX* pVertices = new DISPLACE_VERTEX[4 * numX * numZ];
	DWORD*  pIndex = new DWORD[2 * 3 * numX * numZ];
	// インデックス数を保存する
	m_dwNumIndex = 2 * 3 * numX * numZ;

	// 頂点データを作成する
	for (DWORD z=0; z < numZ; z++)
	{
		for (DWORD x = 0; x < numX; x++)
		{
			pVertices[(z*numX + x) * 4 + 0] = VertexConst[0];
			pVertices[(z*numX + x) * 4 + 1] = VertexConst[1];
			pVertices[(z*numX + x) * 4 + 2] = VertexConst[2];
			pVertices[(z*numX + x) * 4 + 3] = VertexConst[3];

			pVertices[(z*numX + x) * 4 + 0].vPos.x = VertexConst[0].vPos.x + x*RSize;
			pVertices[(z*numX + x) * 4 + 0].vPos.z = VertexConst[0].vPos.z + z*RSize;
			pVertices[(z*numX + x) * 4 + 1].vPos.x = VertexConst[1].vPos.x + x*RSize;
			pVertices[(z*numX + x) * 4 + 1].vPos.z = VertexConst[1].vPos.z + z*RSize;
			pVertices[(z*numX + x) * 4 + 2].vPos.x = VertexConst[2].vPos.x + x*RSize;
			pVertices[(z*numX + x) * 4 + 2].vPos.z = VertexConst[2].vPos.z + z*RSize;
			pVertices[(z*numX + x) * 4 + 3].vPos.x = VertexConst[3].vPos.x + x*RSize;
			pVertices[(z*numX + x) * 4 + 3].vPos.z = VertexConst[3].vPos.z + z*RSize;
		}
	}

	// インデックスデータを作成する
	for (DWORD z = 0; z < numZ; z++)
	{
		for (DWORD x = 0; x < numX; x++)
		{
			pIndex[(z*numX + x) * 6 + 0] = IndexConst[0] + (z*numX + x) * 4;
			pIndex[(z*numX + x) * 6 + 1] = IndexConst[1] + (z*numX + x) * 4;
			pIndex[(z*numX + x) * 6 + 2] = IndexConst[2] + (z*numX + x) * 4;
			pIndex[(z*numX + x) * 6 + 3] = IndexConst[3] + (z*numX + x) * 4;
			pIndex[(z*numX + x) * 6 + 4] = IndexConst[4] + (z*numX + x) * 4;
			pIndex[(z*numX + x) * 6 + 5] = IndexConst[5] + (z*numX + x) * 4;
		}
	}

	//バーテックスバッファーを作成
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(DISPLACE_VERTEX) * 4 * numX * numZ;
	//bd.ByteWidth = sizeof(DISPLACE_VERTEX) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = pVertices;
	if (FAILED(m_pD3D->m_pDevice->CreateBuffer(&bd, &InitData, &m_pVertexBuffer)))
	{
		MessageBox(0, _T("ディスプレイスメントマッピング　バーテックスバッファ 作成失敗"), nullptr, MB_OK);
		return false;
	}

	//インデックスバッファーを作成
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(DWORD) * 2 * 3 * numX * numZ;
	//bd.ByteWidth = sizeof(DWORD) * 2 * 3;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	InitData.pSysMem = pIndex;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;
	if (FAILED(m_pD3D->m_pDevice->CreateBuffer(&bd, &InitData, &m_pIndexBuffer))) 
	{
		MessageBox(0,_T("ディスプレイスメントマッピング　インデックスバッファ 作成失敗"), nullptr, MB_OK);
		return false;
	}

	SAFE_DELETE_ARRAY(pVertices); 
	SAFE_DELETE_ARRAY(pIndex);

	// ディフューズテクスチャーを作成                                                // -- 2021.2.4
	if (TexName)
	{
		if (FAILED(m_pD3D->CreateShaderResourceViewFromFile(TexName, &m_pTexture)))
		{
			MessageBox(0, _T("ディスプレイスメントマッピング　テクスチャ 読み込み失敗"), nullptr, MB_OK);
			return E_FAIL;
		}
	}

	//ノーマルテクスチャーを作成(ハイトマップとして使用)
	if (FName)
	{
		if (FAILED(m_pD3D->CreateShaderResourceViewFromFile(FName, &m_pNormalTexture)))
		{
			MessageBox(0, _T("ディスプレイスメントマッピング　ノーマルテクスチャ 読み込み失敗"), nullptr, MB_OK);
			return E_FAIL;
		}
	}

	//スペキュラテクスチャーを作成                                                // -- 2021.2.4
	if (SpecName)
	{
		if (FAILED(m_pD3D->CreateShaderResourceViewFromFile(SpecName, &m_pSpecularTexture)))
		{
			MessageBox(0, _T("ディスプレイスメントマッピング　スペキュラテクスチャ 読み込み失敗"), nullptr, MB_OK);
			return E_FAIL;
		}
	}

	return S_OK;
}
//------------------------------------------------------------------------
//  シーンを画面にレンダリング
//																		2024.3.23
//
//	位置を指定して、波をレンダリングする
//
//	引数　なし
//
//  戻り値　　　なし
//------------------------------------------------------------------------
void CWave::Render(const MATRIX4X4& mWorld)
{
	Render(mWorld, GameDevice()->m_mView, GameDevice()->m_mProj, GameDevice()->m_vLightDir, GameDevice()->m_vEyePt);
}
//------------------------------------------------------------------------
//  シーンを画面にレンダリング
//																		2017.2.18
//
//	位置と環境情報を指定して、波をレンダリングする
//
//	const MATRIX4X4& mWorld	:ワールドマトリックス 
//　const MATRIX4X4& mView	:ビューマトリックス 
//　const MATRIX4X4& mProj		:プロジェクションマトリックス 
//　const VECTOR3& vLight	:ライトの座標 
//　const VECTOR3& vEye	:視点座標
//
//  戻り値　　　なし
//------------------------------------------------------------------------
void CWave::Render(const MATRIX4X4& mWorld, const MATRIX4X4& mView, const MATRIX4X4& mProj, const VECTOR3& vLight, const VECTOR3& vEye)
{

	//m_pD3D->m_pDeviceContext->RSSetState(m_pD3D->m_pRStateLW);  // 左回り、ワイヤーフレーム表示    // -- 2021.1.11

	// 使用するシェーダーの登録
	m_pD3D->m_pDeviceContext->VSSetShader(m_pShader->m_pDisplaceWave_VS, nullptr, 0);
	m_pD3D->m_pDeviceContext->HSSetShader(m_pShader->m_pDisplaceWave_HS, nullptr, 0);
	m_pD3D->m_pDeviceContext->DSSetShader(m_pShader->m_pDisplaceWave_DS, nullptr, 0);
	m_pD3D->m_pDeviceContext->PSSetShader(m_pShader->m_pDisplaceWave_PS, nullptr, 0);


	// シェーダーのコンスタントバッファーに各種データを渡す	
	D3D11_MAPPED_SUBRESOURCE pData;
	CONSTANT_BUFFER_WVLED cb;
	if (SUCCEEDED(m_pD3D->m_pDeviceContext->Map(m_pShader->m_pConstantBufferWVLED, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		// ワールド行列を渡す
		cb.mW = XMMatrixTranspose(mWorld);
		 
		// ワールド、カメラ、射影行列を渡す
		cb.mWVP = XMMatrixTranspose(mWorld * mView * mProj);

		// 視点位置を渡す
		cb.vEyePos = VECTOR4(vEye.x, vEye.y, vEye.z, 1);

		// ライトの方向を渡す
		cb.vLightDir.x = vLight.x;
		cb.vLightDir.y = vLight.y;
		cb.vLightDir.z = vLight.z;
		cb.vLightDir.w = 0;

		// カラーをシェーダーに渡す
		cb.vDiffuse  = m_vDiffuse;

		// 各種情報を渡す。(使っていない)
		cb.vDrawInfo = VECTOR4(0, 0, 0, 0);

		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		m_pD3D->m_pDeviceContext->Unmap(m_pShader->m_pConstantBufferWVLED, 0);
	}
	// このコンスタントバッファーを、どのシェーダーで使うかを指定
	m_pD3D->m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pShader->m_pConstantBufferWVLED); // バーテックスシェーダーで使う
	m_pD3D->m_pDeviceContext->DSSetConstantBuffers(0, 1, &m_pShader->m_pConstantBufferWVLED); // ドメインシェーダーで使う
	m_pD3D->m_pDeviceContext->HSSetConstantBuffers(0, 1, &m_pShader->m_pConstantBufferWVLED); // ハルシェーダーで使う
	m_pD3D->m_pDeviceContext->PSSetConstantBuffers(0, 1, &m_pShader->m_pConstantBufferWVLED); // ピクセルシェーダーで使う

	// ディスプレースメントマッピングの各要素をシェーダーに渡す
	CONSTANT_BUFFER_DISPLACE sg;
	if (SUCCEEDED(m_pD3D->m_pDeviceContext->Map(m_pShader->m_pConstantBufferDisplace, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{

		// モデルから見た視点位置（つまり、モデルの逆ワールドをかけた視点）を渡す
		sg.vEyePosInv = XMVector3TransformCoord(vEye, XMMatrixInverse(nullptr, mWorld));

		// 最小距離、最大距離
		sg.fMinDistance = m_fMinDistance;
		sg.fMaxDistance = m_fMaxDistance;
		// 最大分割数
		sg.iMaxDevide = m_iMaxDevide;

		// 波　関連
		m_vWaveMove.x += m_vWaveSpeed.x;
		m_vWaveMove.y += m_vWaveSpeed.y;

		// 波の位置変化量を渡す
		sg.vWaveMove  = m_vWaveMove;

		// 波の高さを渡す
		sg.vHeight  = VECTOR2(0.0f, m_fWaveHeight);

		// スペキュラを渡す
		sg.vSpecular = m_vSpecular;

		memcpy_s(pData.pData, pData.RowPitch, (void*)&sg, sizeof(CONSTANT_BUFFER_DISPLACE));
		m_pD3D->m_pDeviceContext->Unmap(m_pShader->m_pConstantBufferDisplace, 0);
	}
	// このコンスタントバッファーを使うシェーダーの登録
	m_pD3D->m_pDeviceContext->VSSetConstantBuffers(1, 1, &m_pShader->m_pConstantBufferDisplace); // バーテックスシェーダーで使う
	m_pD3D->m_pDeviceContext->DSSetConstantBuffers(1, 1, &m_pShader->m_pConstantBufferDisplace); // ドメインシェーダーで使う
	m_pD3D->m_pDeviceContext->HSSetConstantBuffers(1, 1, &m_pShader->m_pConstantBufferDisplace); // ハルシェーダーで使う
	m_pD3D->m_pDeviceContext->PSSetConstantBuffers(1, 1, &m_pShader->m_pConstantBufferDisplace); // ピクセルシェーダーで使う


	// テクスチャーサンプラーをドメインシェーダーとピクセルシェーダーに渡す
	m_pD3D->m_pDeviceContext->DSSetSamplers(0, 1, &m_pD3D->m_pSampleLinear);
	m_pD3D->m_pDeviceContext->PSSetSamplers(0, 1, &m_pD3D->m_pSampleLinear);

	// ディフューズテクスチャーをドメインシェーダーとピクセルシェーダーに渡す    // -- 2021.2.4
	if (m_pTexture)
	{
		m_pD3D->m_pDeviceContext->DSSetShaderResources(0, 1, &m_pTexture);
		m_pD3D->m_pDeviceContext->PSSetShaderResources(0, 1, &m_pTexture);
	}
	else {
		ID3D11ShaderResourceView* Nothing[1] = { 0 };
		m_pD3D->m_pDeviceContext->DSSetShaderResources(0, 1, Nothing);
		m_pD3D->m_pDeviceContext->PSSetShaderResources(0, 1, Nothing);
	}
	// ノーマルテクスチャーをドメインシェーダーとピクセルシェーダーに渡す(ハイトマップとして使用)
	if (m_pNormalTexture)
	{
		m_pD3D->m_pDeviceContext->DSSetShaderResources(1, 1, &m_pNormalTexture);
		m_pD3D->m_pDeviceContext->PSSetShaderResources(1, 1, &m_pNormalTexture);
	}
	else {
		ID3D11ShaderResourceView* Nothing[1] = { 0 };
		m_pD3D->m_pDeviceContext->DSSetShaderResources(1, 1, Nothing);
		m_pD3D->m_pDeviceContext->PSSetShaderResources(1, 1, Nothing);
	}
	// スペキュラテクスチャーをドメインシェーダーとピクセルシェーダーに渡す    // -- 2021.2.4
	if (m_pSpecularTexture)
	{
		m_pD3D->m_pDeviceContext->DSSetShaderResources(3, 1, &m_pSpecularTexture);
		m_pD3D->m_pDeviceContext->PSSetShaderResources(3, 1, &m_pSpecularTexture);
	}
	else {
		ID3D11ShaderResourceView* Nothing[1] = { 0 };
		m_pD3D->m_pDeviceContext->DSSetShaderResources(3, 1, Nothing);
		m_pD3D->m_pDeviceContext->PSSetShaderResources(3, 1, Nothing);
	}


	// 頂点インプットレイアウトをセット
	m_pD3D->m_pDeviceContext->IASetInputLayout(m_pShader->m_pDisplaceWave_VertexLayout);
	// プリミティブ・トポロジーをセット
	m_pD3D->m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);

	// バーテックスバッファーをセット
	UINT stride = sizeof(DISPLACE_VERTEX);
	UINT offset = 0;
	m_pD3D->m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	// インデックスバッファーをセット
	stride = sizeof(DWORD);
	offset = 0;
	m_pD3D->m_pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// プリミティブをレンダリング
	m_pD3D->m_pDeviceContext->DrawIndexed(m_dwNumIndex, 0, 0);

	// ハルシェーダーとドメインシェーダーをリセットする
	m_pD3D->m_pDeviceContext->HSSetShader(nullptr, nullptr, 0);
	m_pD3D->m_pDeviceContext->DSSetShader(nullptr, nullptr, 0);


}
