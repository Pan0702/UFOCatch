//=============================================================================
//
//  バウンディングボックス                             ver 3.3      2024.3.23
//
//   ポリゴンの表面判定を変更。右回り表とする
//									                                 BBox.cpp
//=============================================================================
#include "BBox.h"
#include "../Core/Game/GameMain.h"
#pragma warning(disable : 6387)


// -----------------------------------------------------------------------
//
// OBB (オリエント・ボックス:Oriented Bounding Box)
//
// -----------------------------------------------------------------------
//------------------------------------------------------------------------ // -- 2024.3.23
//
// コンストラクタ
//
//------------------------------------------------------------------------
CBBox::CBBox() : CBBox(GameDevice()->m_pShader)
{
}
CBBox::CBBox(const VECTOR3& vMin, const VECTOR3& vMax) : CBBox(GameDevice()->m_pShader, vMin,  vMax)
{
}
//------------------------------------------------------------------------
//
// コンストラクタ
//
//  CShader* pShader         シェーダー
//
//------------------------------------------------------------------------
CBBox::CBBox(CShader* pShader)
{
	ZeroMemory(this, sizeof(CBBox));
	m_pD3D = pShader->m_pD3D;
	m_pShader = pShader;

	m_vAxisX = VECTOR3(1, 0, 0);
	m_vAxisY = VECTOR3(0, 1, 0);
	m_vAxisZ = VECTOR3(0, 0, 1);

	m_mWorld = XMMatrixIdentity();		// ワールドマトリクスの初期化

	m_vDiffuse = VECTOR4(1.0f, 1.0f, 1.0f, 0.5f);    // ディフューズ色   // -- 2021.1.11

}
//------------------------------------------------------------------------
//
// コンストラクタ
//
//  CShader* pShader         シェーダー
//  const VECTOR3& vMin      バウンディングボックスの最小値
//  const VECTOR3& vMax      バウンディングボックスの最大値
//
//------------------------------------------------------------------------
CBBox::CBBox(CShader* pShader, const VECTOR3& vMin, const VECTOR3& vMax)
{
	ZeroMemory(this, sizeof(CBBox));
	m_pD3D = pShader->m_pD3D;
	m_pShader = pShader;

	m_vAxisX = VECTOR3(1, 0, 0);
	m_vAxisY = VECTOR3(0, 1, 0);
	m_vAxisZ = VECTOR3(0, 0, 1);

	m_mWorld = XMMatrixIdentity();		// ワールドマトリクスの初期化

	InitBBox(vMin, vMax);

	m_vDiffuse = VECTOR4(1.0f, 1.0f, 1.0f, 0.5f);    // ディフューズ色   // -- 2021.1.11

}
//------------------------------------------------------------------------
//
// デストラクタ
//
//------------------------------------------------------------------------
CBBox::~CBBox()
{
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pIndexBuffer);
	SAFE_DELETE_ARRAY(m_pIndexBuffer);
}

//------------------------------------------------------------------------
//	バウンディングボックスの初期化と生成
//
//  const VECTOR3& vMin      バウンディングボックスの最小値
//  const VECTOR3& vMax      バウンディングボックスの最大値
//
//  最小値〜最大値の大きさのバウンディングボックスを作成する
//
//  戻り値　なし
//------------------------------------------------------------------------
//BBoxの初期化
void CBBox::InitBBox(const VECTOR3& vMin, const VECTOR3& vMax)
{

	m_vMax = vMax;
	m_vMin = vMin;

	//軸ベクトル 軸の中心(軸の場合ボックスの各軸半径)を計算しておく
	m_fLengthX = (vMax.x - vMin.x) / 2;
	m_fLengthY = (vMax.y - vMin.y) / 2;
	m_fLengthZ = (vMax.z - vMin.z) / 2;

	// 表示用のボックスメッシュの生成
	InitMesh();

}

//------------------------------------------------------------------------
//	バウンディングボックスの頂点とインデックスデータを作成する
//
//  VECTOR3* pVertex      頂点データ(OUT)　        VECTOR3×8
//  DWORD*   pIndex       インデックスデータ(OUT)　DWORD×36
//
//  戻り値　なし
//------------------------------------------------------------------------
void CBBox::MakeVertexIndex(VECTOR3* pVertex, DWORD* pIndex)
{
	//バーテックスデータ作成
	pVertex[0] = VECTOR3(m_vMin.x, m_vMin.y, m_vMin.z);    //頂点0  下面　左前
	pVertex[1] = VECTOR3(m_vMin.x, m_vMin.y, m_vMax.z);    //頂点1  下面　左奥
	pVertex[2] = VECTOR3(m_vMax.x, m_vMin.y, m_vMax.z);    //頂点2  下面　右奥
	pVertex[3] = VECTOR3(m_vMax.x, m_vMin.y, m_vMin.z);    //頂点3  下面　右前
	pVertex[4] = VECTOR3(m_vMin.x, m_vMax.y, m_vMin.z);    //頂点4  上面　左前
	pVertex[5] = VECTOR3(m_vMin.x, m_vMax.y, m_vMax.z);    //頂点5  上面　左奥
	pVertex[6] = VECTOR3(m_vMax.x, m_vMax.y, m_vMax.z);    //頂点6  上面　右奥
	pVertex[7] = VECTOR3(m_vMax.x, m_vMax.y, m_vMin.z);    //頂点7  上面　右前

	// インデックスデータ作成　3角形リスト×2×6
	DWORD pIndexConst[] = {


		//	頂点・右回り表面とする			      // -- 2024.3.23
		0, 2, 1,   0, 3, 2,       // 下面
		4, 5, 6,   4, 6, 7,       // 上面
		0, 4, 7,   0, 7, 3,       // 前面
		2, 6, 5,   2, 5, 1,       // 背面
		1, 5, 4,   1, 4, 0,       // 左面
		3, 7, 6,   3, 6, 2,       // 右面

	};
	memcpy_s(pIndex, sizeof(DWORD) * 36, pIndexConst, sizeof(DWORD) * 36);

}
//------------------------------------------------------------------------
//	表示用のボックスメッシュを作成する
//
//  戻り値　HRESULT	正常:S_OK　　異常:E_FAIL
//------------------------------------------------------------------------
// 表示用のボックスメッシュを作成する
HRESULT CBBox::InitMesh()
{

	// 頂点データ
	VECTOR3 pVertexConst[8];
	// 頂点法線データ
	VECTOR3 pNormalConst[] =
	{
		VECTOR3(0, -1, 0),    //頂点0  下面　左前
		VECTOR3(0, -1, 0),    //頂点1  下面　左奥
		VECTOR3(0, -1, 0),    //頂点2  下面　右奥
		VECTOR3(0, -1, 0),    //頂点3  下面　右前
		VECTOR3(0,  1, 0),    //頂点4  上面　左前
		VECTOR3(0,  1, 0),    //頂点5  上面　左奥
		VECTOR3(0,  1, 0),    //頂点6  上面　右奥
		VECTOR3(0,  1, 0),    //頂点7  上面　右前
	};

	// テクスチャ座標データ
	VECTOR2 pTexConst[] =
	{
		VECTOR2(0, 1),    //頂点0  下面　左前
		VECTOR2(1, 1),    //頂点1  下面　左奥
		VECTOR2(0, 1),    //頂点2  下面　右奥
		VECTOR2(1, 1),    //頂点3  下面　右前
		VECTOR2(0, 0),    //頂点4  上面　左前
		VECTOR2(1, 0),    //頂点5  上面　左奥
		VECTOR2(0, 0),    //頂点6  上面　右奥
		VECTOR2(1, 0),    //頂点7  上面　右前
	};

	// インデックスデータ　3角形リスト×2×6
	DWORD pIndex[36];

	// ボックスの頂点とインデックスデータを作成
	MakeVertexIndex(pVertexConst, pIndex);

	// 必要分の頂点データを格納する配列を確保する
	BBOX_VERTEX  pVertices[8];
	for (DWORD i = 0; i<8; i++)
	{
		ZeroMemory(&pVertices[i], sizeof(BBOX_VERTEX));
		pVertices[i].vPos = pVertexConst[i];
		pVertices[i].vNorm = pNormalConst[i];
		pVertices[i].vTex = pTexConst[i];
	}

	D3D11_BUFFER_DESC bd;
	D3D11_SUBRESOURCE_DATA InitData;
	D3D11_MAPPED_SUBRESOURCE msr;

	// バーテックスバッファが既に作成済みかどうかチェックする
	if (m_pVertexBuffer == nullptr)
	{
		//バーテックスバッファーを作成
		//bd.Usage = D3D11_USAGE_DEFAULT;
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(BBOX_VERTEX) * 8;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		//bd.CPUAccessFlags = 0;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd.MiscFlags = 0;
		InitData.pSysMem = pVertices;
		if (FAILED(m_pD3D->m_pDevice->CreateBuffer(&bd, &InitData, &m_pVertexBuffer)))
		{
			MessageBox(0, _T("BBerra:false"), nullptr, MB_OK);
			return E_FAIL;
		}
	}
	else {
		// 既に作成済みのため、バーテックスバッファの書き換えのみ行う
		if (SUCCEEDED(m_pD3D->m_pDeviceContext->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr)))
		{
			memcpy(msr.pData, pVertices, sizeof(BBOX_VERTEX) * 8); // 8頂点をコピー
			m_pD3D->m_pDeviceContext->Unmap(m_pVertexBuffer, 0);
		}
		else {
			MessageBox(0, _T("BBerrar:changeFalse"), nullptr, MB_OK);
			return E_FAIL;

		}
	}

	// インデックスバッファが既に作成済みかどうかチェックする
	// 既に作成済みの時は、値が変わらないのでインデックスバッファの書き換えは不要
	if (m_pIndexBuffer == nullptr)
	{
		//インデックスバッファーを作成
		bd.Usage = D3D11_USAGE_DEFAULT;
		//bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(DWORD) * 2 * 3 * 6;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		//bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd.MiscFlags = 0;
		InitData.pSysMem = pIndex;
		InitData.SysMemPitch = 0;
		InitData.SysMemSlicePitch = 0;
		if (FAILED(m_pD3D->m_pDevice->CreateBuffer(&bd, &InitData, &m_pIndexBuffer)))
		{
			MessageBox(0, _T("BBerrar:indexMakeFalse"), nullptr, MB_OK);
			return E_FAIL;
		}
	}

	return S_OK;

}

//------------------------------------------------------------------------
//	バウンディングボックスと他のバウンディングボックスとの接触判定
//
//	CBBox* pOtherBBox      相手のバウンディングボックス(IN/OUT)
//  VECTOR3* vHit          ヒットしたときのヒット位置(OUT)
//  VECTOR3* vNrm          ヒットしたときの法線座標(OUT)
//
//  戻り値　bool	ret　  false:ヒットしないとき　true:ヒットしたとき
//------------------------------------------------------------------------
bool CBBox::OBBCollisionDetection( CBBox* pOtherBBox, VECTOR3* vHit, VECTOR3* vNrm)
{
	MATRIX4X4 mOtherWorld = pOtherBBox->m_mWorld;

	//ボックスの距離ベクトル(中心と中心を結んだベクトル)を求める
	// 自BBOX(BBoxA)の中心点までの移動マトリックスを作成する
	MATRIX4X4 mWorldCenterA;
	mWorldCenterA = XMMatrixTranslation( m_fLengthX + m_vMin.x, m_fLengthY + m_vMin.y, m_fLengthZ + m_vMin.z);
	mWorldCenterA = mWorldCenterA * m_mWorld;		// 中心点のワールドマトリックスを求める
	// 他BBOX(BBoxB)の中心点までの移動マトリックスを作成する
	MATRIX4X4 mWorldCenterB;
	mWorldCenterB = XMMatrixTranslation( pOtherBBox->m_fLengthX + pOtherBBox->m_vMin.x,
							pOtherBBox->m_fLengthY + pOtherBBox->m_vMin.y, pOtherBBox->m_fLengthZ + pOtherBBox->m_vMin.z);
	mWorldCenterB = mWorldCenterB * mOtherWorld;	// 中心点のワールドマトリックスを求める
	// ボックスの中心点間の距離ベクトルを求める
	VECTOR3 vDistance = VECTOR3(mWorldCenterB._41, mWorldCenterB._42, mWorldCenterB._43)
	                      - VECTOR3(mWorldCenterA._41, mWorldCenterA._42, mWorldCenterA._43);

	// 回転行列(位置を含まず、回転のみの行列)を求める
	MATRIX4X4 mTrans;
	mTrans = XMMatrixTranslation(-m_mWorld._41, -m_mWorld._42, -m_mWorld._43);
	MATRIX4X4 mRot = m_mWorld * mTrans;

	mTrans = XMMatrixTranslation( -mOtherWorld._41, -mOtherWorld._42, -mOtherWorld._43);
	MATRIX4X4 mOtherRot = mOtherWorld * mTrans;

	//分離軸
	VECTOR3 vSeparate;
	//それぞれのローカル座標軸ベクトルに、それぞれの回転を反映させる
	m_vAxisX = VECTOR3(1, 0, 0);
	m_vAxisY = VECTOR3(0, 1, 0);
	m_vAxisZ = VECTOR3(0, 0, 1);

	pOtherBBox->m_vAxisX = VECTOR3(1, 0, 0);
	pOtherBBox->m_vAxisY = VECTOR3(0, 1, 0);
	pOtherBBox->m_vAxisZ = VECTOR3(0, 0, 1);

	m_vAxisX = XMVector3TransformCoord(m_vAxisX, mRot);
	m_vAxisY = XMVector3TransformCoord(m_vAxisY, mRot);
	m_vAxisZ = XMVector3TransformCoord(m_vAxisZ, mRot);

	pOtherBBox->m_vAxisX = XMVector3TransformCoord(pOtherBBox->m_vAxisX, mOtherRot);
	pOtherBBox->m_vAxisY = XMVector3TransformCoord(pOtherBBox->m_vAxisY, mOtherRot);
	pOtherBBox->m_vAxisZ = XMVector3TransformCoord(pOtherBBox->m_vAxisZ, mOtherRot);

	//ボックスA(自BBOX)のローカル座標軸による、影長さの算出(3パターン)
	{
		//X軸を分離軸とした場合
		if (!CompareLength(this, pOtherBBox, &m_vAxisX, &vDistance)) return false;
		//Y軸を分離軸とした場合
		if (!CompareLength(this, pOtherBBox, &m_vAxisY, &vDistance)) return false;
		//Z軸を分離軸とした場合
		if (!CompareLength(this, pOtherBBox, &m_vAxisZ, &vDistance)) return false;
	}
	//ボックスB(他BBOX)のローカル座標軸による、影長さの算出(3パターン)
	{
		//X軸を分離軸とした場合
		if (!CompareLength(this, pOtherBBox, &pOtherBBox->m_vAxisX, &vDistance)) return false;
		//Y軸を分離軸とした場合
		if (!CompareLength(this, pOtherBBox, &pOtherBBox->m_vAxisY, &vDistance)) return false;
		//Z軸を分離軸とした場合
		if (!CompareLength(this, pOtherBBox, &pOtherBBox->m_vAxisZ, &vDistance)) return false;
	}
	//互いの座標軸2本の外積ベクトル軸による、影長さの算出(9パターン)
	{
		//ボックスA(自BBOX)のX軸
		{
			//と　ボックスB(他BBOX)のX軸との外積ベクトルを分離軸とした場合
			vSeparate = cross(m_vAxisX, pOtherBBox->m_vAxisX);
			if (!CompareLength(this, pOtherBBox, &vSeparate, &vDistance)) return false;
			//と　ボックスB(他BBOX)のY軸との外積ベクトルを分離軸とした場合
			vSeparate = cross(m_vAxisX, pOtherBBox->m_vAxisY);
			if (!CompareLength(this, pOtherBBox, &vSeparate, &vDistance)) return false;
			//と　ボックスB(他BBOX)のZ軸との外積ベクトルを分離軸とした場合
			vSeparate = cross(m_vAxisX, pOtherBBox->m_vAxisZ);
			if (!CompareLength(this, pOtherBBox, &vSeparate, &vDistance)) return false;
		}
		//ボックスA(自BBOX)のY軸
		{
			//と　ボックスB(他BBOX)のX軸との外積ベクトルを分離軸とした場合
			vSeparate = cross( m_vAxisY, pOtherBBox->m_vAxisX);
			if (!CompareLength(this, pOtherBBox, &vSeparate, &vDistance)) return false;
			//と　ボックスB(他BBOX)のY軸との外積ベクトルを分離軸とした場合
			vSeparate = cross( m_vAxisY, pOtherBBox->m_vAxisY);
			if (!CompareLength(this, pOtherBBox, &vSeparate, &vDistance)) return false;
			//と　ボックスB(他BBOX)のZ軸との外積ベクトルを分離軸とした場合
			vSeparate = cross( m_vAxisY, pOtherBBox->m_vAxisZ);
			if (!CompareLength(this, pOtherBBox, &vSeparate, &vDistance)) return false;
		}
		//ボックスA(自BBOX)のZ軸
		{
			//と　ボックスB(他BBOX)のX軸との外積ベクトルを分離軸とした場合
			vSeparate = cross( m_vAxisZ, pOtherBBox->m_vAxisX);
			if (!CompareLength(this, pOtherBBox, &vSeparate, &vDistance)) return false;
			//と　ボックスB(他BBOX)のY軸との外積ベクトルを分離軸とした場合
			vSeparate = cross( m_vAxisZ, pOtherBBox->m_vAxisY);
			if (!CompareLength(this, pOtherBBox, &vSeparate, &vDistance)) return false;
			//と　ボックスB(他BBOX)のZ軸との外積ベクトルを分離軸とした場合
			vSeparate = cross( m_vAxisZ, pOtherBBox->m_vAxisZ);
			if (!CompareLength(this, pOtherBBox, &vSeparate, &vDistance)) return false;
		}
	}

	// ヒットしたとき
	MATRIX4X4 mHitWorld;
	// BBox中心点の座標をヒット位置とする
	mHitWorld = XMMatrixTranslation((pOtherBBox->m_vMax.x + pOtherBBox->m_vMin.x) / 2,
									(pOtherBBox->m_vMax.y + pOtherBBox->m_vMin.y) / 2,
									(pOtherBBox->m_vMax.z + pOtherBBox->m_vMin.z) / 2);
	mHitWorld = mHitWorld * pOtherBBox->m_mWorld;
	*vHit = GetPositionVector(mHitWorld);
	*vNrm = VECTOR3(0.0f, 1.0f, 0.0f);

	return true;
}
//------------------------------------------------------------------------
//	分離ベクトルを分離軸に投影し、2つの影が重なっているかどうかを調べる処理
//
//  const CBBox* pBBoxA            バウンディングボックスＡ
//  const CBBox* pBBoxB            バウンディングボックスＢ
//  const VECTOR3* pvSeparate      分離軸
//  const VECTOR3* pvDistance      ボックスの中心点間の距離
//
//  戻り値　bool	ret　  false:離れているとき　true:接触しているとき
//------------------------------------------------------------------------
bool CBBox::CompareLength(const CBBox* pBBoxA, const CBBox* pBBoxB, const VECTOR3* pvSeparate, const VECTOR3* pvDistance)
{
	//分離軸方向で、ボックスＡの中心からボックスＢの中心までの距離
	FLOAT fDistance = fabsf(dot(*pvDistance, *pvSeparate));
	//分離軸方向でボックスAの中心から最も遠いボックスAの頂点までの距離
	FLOAT fShadowA = 0;
	//分離軸方向でボックスBの中心から最も遠いボックスBの頂点までの距離
	FLOAT fShadowB = 0;
	//それぞれのボックスの、影長さを算出
	fShadowA = fabsf(dot(pBBoxA->m_vAxisX, *pvSeparate) * pBBoxA->m_fLengthX) +
				fabsf(dot(pBBoxA->m_vAxisY, *pvSeparate) * pBBoxA->m_fLengthY) +
				fabsf(dot(pBBoxA->m_vAxisZ, *pvSeparate) * pBBoxA->m_fLengthZ);

	fShadowB = fabsf(dot(pBBoxB->m_vAxisX, *pvSeparate) * pBBoxB->m_fLengthX) +
				fabsf(dot(pBBoxB->m_vAxisY, *pvSeparate) * pBBoxB->m_fLengthY) +
				fabsf(dot(pBBoxB->m_vAxisZ, *pvSeparate) * pBBoxB->m_fLengthZ);
	if (fDistance > fShadowA + fShadowB)
	{
		return false;
	}
	return true;
}

//------------------------------------------------------------------------  // -- 2019.10.8
//	バウンディングボックスと移動前後点(レイ)との接触判定
//
//	const MATRIX4X4&  mLay       点の移動後のワールドマトリックス
//  const MATRIX4X4&  mLayOld    点の移動前のワールドマトリックス
//  VECTOR3* vHit                ヒットしたときのヒット位置(OUT)
//  VECTOR3* vNrm                ヒットしたときの法線座標(OUT)
//
//  戻り値　bool	ret　  false:ヒットしないとき　true:ヒットしたとき
//------------------------------------------------------------------------
bool CBBox::OBBCollisionLay( const MATRIX4X4& mLay, const MATRIX4X4& mLayOld, VECTOR3* vHit, VECTOR3* vNrm)
{
	return OBBCollisionLay( GetPositionVector(mLay), GetPositionVector(mLayOld), vHit, vNrm );
}

//------------------------------------------------------------------------  // -- 2019.10.8
//	バウンディングボックスと移動前後点(レイ)との接触判定
//
//	const VECTOR3&  vNow      点の移動後の座標
//  const VECTOR3&  vOld      点の移動前の座標
//  VECTOR3* vHit             ヒットしたときのヒット位置(OUT)
//  VECTOR3* vNrm             ヒットしたときの法線座標(OUT)
//
//  戻り値　bool	ret　  false:ヒットしないとき　true:ヒットしたとき
//------------------------------------------------------------------------
bool CBBox::OBBCollisionLay( const VECTOR3&  vNow, const VECTOR3& vOld, VECTOR3* vHit, VECTOR3* vNrm)
{

	bool	ret = false;
	int		i;

	VECTOR3 vFaceNorm;
	float    fNowDist, fOldDist, fLayDist;
	float    fLenMin = 9999999.0f;				// 交点と移動前点との距離の最小値

	// 戻り値のクリアー
	vHit->x = 0.0f;
	vHit->y = 0.0f;
	vHit->z = 0.0f;

	// 頂点データ
	VECTOR3 pVertex[8];
	// インデックスデータ　3角形リスト×2×6
	DWORD pIndex[36];
	// バウンディングボックスの頂点とインデックスデータを作成
	MakeVertexIndex(pVertex, pIndex);

	// 頂点データをワールド座標に変換する
	for (i = 0; i < 8; i++)
	{
		MATRIX4X4 mTemp;
		mTemp = XMMatrixTranslation(pVertex[i].x, pVertex[i].y, pVertex[i].z);
		mTemp = mTemp * m_mWorld;
		pVertex[i].x = mTemp._41;
		pVertex[i].y = mTemp._42;
		pVertex[i].z = mTemp._43;
	}

	// バウンディングボックスの６面１２ポリゴンと点線との交差判定を行う
	for (i = 0; i<12; i++)
	{
		VECTOR3 vVert[3] = { VECTOR3(0,0,0) };
		// 三角形ポリゴンの頂点の値を得る
		vVert[0] = pVertex[pIndex[i * 3 + 0]];
		vVert[1] = pVertex[pIndex[i * 3 + 1]];
		vVert[2] = pVertex[pIndex[i * 3 + 2]];
		// 面法線を作成する
		vFaceNorm = normalize(cross(vVert[1] - vVert[0], vVert[2] - vVert[0]));   // 右回り表とする	    // -- 2024.3.23
		//vFaceNorm = normalize(cross(vVert[2] - vVert[0], vVert[1] - vVert[0]));     // 左回り表とする

		// 現在の三角形ポリゴン　原点からの距離を求める
		float   fFaceDist = dot(vFaceNorm, vVert[0]);		// 原点から三角形面までの距離
		fNowDist = dot(vFaceNorm, vNow) - fFaceDist;		// 点の移動後点と三角形面との距離。正の時は表側、負の時は裏側
		fOldDist = dot(vFaceNorm, vOld) - fFaceDist;		// 点の移動前点と三角形面との距離。正の時は表側、負の時は裏側
		fLayDist = fOldDist - fNowDist;						// 点の移動ベクトルLayの法線方向の距離

		// 三角形ポリゴンと点線(レイ)との接触判定を行う
		if (fNowDist <= 0.0f && fOldDist >= 0.0f)	// 点の移動前点が三角形面の表で、移動後点が面の裏側の時のみ判定
		{
			if (fLayDist != 0.0f)		// 点の移動ベクトルが法線方向の距離が０の時は、点線と三角形面が平行なので対象外
			{
				// 面との交点vInsPtを求める
				//   １　移動ベクトルの交点から移動後点までの距離の比率を求める　　　　　fNowDist / fLayDist
				//   ２　交点から移動後点までの移動ベクトルを求める　　　　　　　　　　　(vOld - vNow) * fNowDist / fLayDist
				//   ３　上記２を移動後点から引くことによって、交点のベクトルを求める　　vNow - ( (vOld - vNow) * fNowDist / fLayDist)
				VECTOR3 vInsPt = vNow - ((vOld - vNow) * fNowDist / fLayDist);

				// 三角形ポリゴンと接触しているかどうかの判定
				//   交点を原点とした時に、三角形ポリゴンの各頂点と交点との角度を合計する関数AddAngleで行う
				//   この関数の戻り値が３６０度(２π)の時は、接触点が３つの三つ辺ベクトルの中にある
				//   しかし、誤差が有るので２πではなく1.99f * πで判定している。
				if (AddAngle(vVert[0] - vInsPt, vVert[1] - vInsPt, vVert[2] - vInsPt) >= 1.99f * XM_PI)
				{
					ret = true;
					if (fLenMin > magnitude(vOld - vInsPt))   // 一番近い点を探す
					{
						fLenMin = magnitude(vOld - vInsPt);
						*vHit = vInsPt;
						*vNrm = vFaceNorm;
					}
				}
			}
		}
	}
	return ret;
}
//------------------------------------------------------------------------  // -- 2019.10.7
//	バウンディングボックスと三角形との接触判定
//
//	const VECTOR3* pTri      相手の三角形
//  const MATRIX4X4& mWorld  相手の三角形のワールドマトリックス
//  VECTOR3* vHit            ヒットしたときのヒット位置(OUT)
//
//  戻り値　bool	ret　  false:ヒットしないとき　true:ヒットしたとき
//------------------------------------------------------------------------
bool CBBox::OBBCollisionTri(const VECTOR3* pTri, const MATRIX4X4& mWorld, VECTOR3* vhit)
{
	VECTOR3 vTri[3] = { VECTOR3(0,0,0) };

	// 三角形の頂点をワールド座標変換する
	vTri[0] = XMVector3TransformCoord(*(pTri + 0), mWorld);
	vTri[1] = XMVector3TransformCoord(*(pTri + 1), mWorld);
	vTri[2] = XMVector3TransformCoord(*(pTri + 2), mWorld);

	//	バウンディングボックスと三角形との接触判定
	return OBBCollisionTri(vTri, vhit);
}
//------------------------------------------------------------------------  // -- 2019.10.7
//	バウンディングボックスと三角形との接触判定
//
//	const VECTOR3* pTri      相手の三角形
//  VECTOR3* vHit            ヒットしたときのヒット位置(OUT)
//
//  戻り値　bool	ret　  false:ヒットしないとき　true:ヒットしたとき
//------------------------------------------------------------------------
bool CBBox::OBBCollisionTri(const VECTOR3* pTri, VECTOR3* vhit)
{
	float  p0, p1, p2, r;

	// 自ＢＢＸ各軸の中心(半径)ベクトルを求める
	VECTOR3 AeX = VECTOR3(m_fLengthX, 0, 0);
	VECTOR3 AeY = VECTOR3(0, m_fLengthY, 0);
	VECTOR3 AeZ = VECTOR3(0, 0, m_fLengthZ);

	// 自ＢＢＸ各軸の中心(半径)を求める
	float eX = m_fLengthX;
	float eY = m_fLengthY;
	float eZ = m_fLengthZ;

	// OBBのワールドマトリックスの逆行列を求める
	// (次の段階で、自ＢＢＸはそのローカル座標がワールド座標系と一致する＝＝ＢＢＸと同値になる)
	MATRIX4X4 mWorldInv = XMMatrixInverse(nullptr, m_mWorld);

	// 三角形にOBBのワールドマトリックスの逆行列を掛け合わせ、OBBのローカル座標系に変換する
	VECTOR3 vT[3] = { VECTOR3(0,0,0) };
	vT[0] = XMVector3TransformCoord(*(pTri + 0), mWorldInv);
	vT[1] = XMVector3TransformCoord(*(pTri + 1), mWorldInv);
	vT[2] = XMVector3TransformCoord(*(pTri + 2), mWorldInv);

	// 三角形を自ＢＢＸの中心位置を原点とした座標系に移動する
	VECTOR3 vOffset = VECTOR3(m_fLengthX + m_vMin.x, m_fLengthY + m_vMin.y, m_fLengthZ + m_vMin.z);
	vT[0] -= vOffset;
	vT[1] -= vOffset;
	vT[2] -= vOffset;

	// 三角形に対して辺のベクトルを計算
	VECTOR3  f0 = vT[1] - vT[0];
	VECTOR3  f1 = vT[2] - vT[1];
	VECTOR3  f2 = vT[0] - vT[2];

	// 自ＢＢＸの各辺と三角形の各辺の組み合わせに対して分離平面の外積を分離軸として判定する
	// (ところが、自ＢＢＸは原点において、ワールド座標系に一致する＝＝ＢＢＸとなっているため、計算が簡略化できる)

	// 自ＢＢＸのｚ軸(ワールド座標のｚ軸)と三角形の頂点０〜頂点２辺で分離平面の外積を分離軸とする判定(a00)
	p0 = vT[0].z * vT[1].y - vT[0].y * vT[1].z;
	p2 = vT[2].z * (vT[1].y - vT[0].y) - vT[2].y * (vT[1].z - vT[0].z);
	r = eY * fabsf(f0.z) + eZ * fabsf(f0.y);
	if (max(-max(p0, p2), min(p0, p2)) > r)
		return  false;

	// 自ＢＢＸのｚ軸(ワールド座標のｚ軸)と三角形の頂点０〜頂点１辺で分離平面の外積を分離軸とする判定(a01)
	p0 = vT[0].z * (vT[2].y - vT[1].y) - vT[0].y * (vT[2].z - vT[1].z);
	p1 = vT[1].z * vT[2].y - vT[1].y * vT[2].z;
	r = eY * fabsf(f1.z) + eZ * fabsf(f1.y);
	if (max(-max(p0, p1), min(p0, p1)) > r)
		return  false;

	// 自ＢＢＸのｚ軸(ワールド座標のｚ軸)と三角形の頂点１〜頂点２辺で分離平面の外積を分離軸とする判定(a02)
	p1 = vT[1].z * (vT[0].y - vT[2].y) - vT[1].y * (vT[0].z - vT[2].z);
	p2 = vT[2].z * vT[0].y - vT[2].y * vT[0].z;
	r = eY * fabsf(f2.z) + eZ * fabsf(f2.y);
	if (max(-max(p1, p2), min(p1, p2)) > r)
		return  false;

	// 自ＢＢＸのｘ軸(ワールド座標のｘ軸)と三角形の頂点０〜頂点２辺で分離平面の外積を分離軸とする判定(a10)
	p0 = vT[0].x * vT[1].z - vT[0].z * vT[1].x;
	p2 = vT[2].x * (vT[1].z - vT[0].z) - vT[2].z * (vT[1].x - vT[0].x);
	r = eZ * fabsf(f0.x) + eX * fabsf(f0.z);
	if (max(-max(p0, p2), min(p0, p2)) > r)
		return  false;

	// 自ＢＢＸのｘ軸(ワールド座標のｘ軸)と三角形の頂点０〜頂点１辺で分離平面の外積を分離軸とする判定(a11)
	p0 = vT[0].x * (vT[2].z - vT[1].z) - vT[0].z * (vT[2].x - vT[1].x);
	p1 = vT[1].x * vT[2].z - vT[1].z * vT[2].x;
	r = eZ * fabsf(f1.x) + eX * fabsf(f1.z);
	if (max(-max(p0, p1), min(p0, p1)) > r)
		return  false;

	// 自ＢＢＸのｘ軸(ワールド座標のｘ軸)と三角形の頂点１〜頂点２辺で分離平面の外積を分離軸とする判定(a12)
	p1 = vT[1].x * (vT[0].z - vT[2].z) - vT[1].z * (vT[0].x - vT[2].x);
	p2 = vT[2].x * vT[0].z - vT[2].z * vT[0].x;
	r = eZ * fabsf(f2.x) + eX * fabsf(f2.z);
	if (max(-max(p1, p2), min(p1, p2)) > r)
		return  false;

	// 自ＢＢＸのｙ軸(ワールド座標のｙ軸)と三角形の頂点０〜頂点２辺で分離平面の外積を分離軸とする判定(a20)
	p0 = vT[0].y * vT[1].x - vT[0].x * vT[1].y;
	p2 = vT[2].y * (vT[1].x - vT[0].x) - vT[2].x * (vT[1].y - vT[0].y);
	r = eX * fabsf(f0.y) + eY * fabsf(f0.x);
	if (max(-max(p0, p2), min(p0, p2)) > r)
		return  false;

	// 自ＢＢＸのｙ軸(ワールド座標のｙ軸)と三角形の頂点０〜頂点１辺で分離平面の外積を分離軸とする判定(a21)
	p0 = vT[0].y * (vT[2].x - vT[1].x) - vT[0].x * (vT[2].y - vT[1].y);
	p1 = vT[1].y * vT[2].x - vT[1].x * vT[2].y;
	r = eX * fabsf(f1.y) + eY * fabsf(f1.x);
	if (max(-max(p0, p1), min(p0, p1)) > r)
		return  false;

	// 自ＢＢＸのｙ軸(ワールド座標のｙ軸)と三角形の頂点１〜頂点２辺で分離平面の外積を分離軸とする判定(a22)
	p1 = vT[1].y * (vT[0].x - vT[2].x) - vT[1].x * (vT[0].y - vT[2].y);
	p2 = vT[2].y * vT[0].x - vT[2].x * vT[0].y;
	r = eX * fabsf(f2.y) + eY * fabsf(f2.x);
	if (max(-max(p1, p2), min(p1, p2)) > r)
		return  false;

	// 自ＢＢＸの面法線に一致する分離軸の判定
	// ｚ軸
	if (max(vT[0].x, max(vT[1].x, vT[2].x)) < -eX || min(vT[0].x, min(vT[1].x, vT[2].x)) > eX)
		return  false;
	// ｘ軸
	if (max(vT[0].y, max(vT[1].y, vT[2].y)) < -eY || min(vT[0].y, min(vT[1].y, vT[2].y)) > eY)
		return  false;
	// ｙ軸
	if (max(vT[0].z, max(vT[1].z, vT[2].z)) < -eZ || min(vT[0].z, min(vT[1].z, vT[2].z)) > eZ)
		return  false;

	// 三角形の面の法線に一致する分離軸の判定
	VECTOR3 vNormal;
	vNormal = cross(f0, f1);

	p0 = LenSegOnSeparateAxis(&vNormal, &AeX, &AeY, &AeZ);
	r = fabsf(dot(vNormal, vT[0]));

	if (r > p0)
		return  false;

	// 分離平面が見つからないので「衝突している」

	// 衝突座標の設定　三角形の中心をヒット位置とする
	*vhit = (*(pTri + 0) + *(pTri + 1) + *(pTri + 2)) / 3;

	// 衝突座標の設定　三角形の中心と自ＢＢＸ中心の中点をヒット位置とする
	//*vhit = ( GetPositionVector(m_mWorld) + ( *(pTri+0) + *(pTri+1) + *(pTri+2) ) / 3 ) / 2;

	return true;
}

//------------------------------------------------------------------------
// 分離軸に投影された軸成分から投影線分長を算出する
// (3つの軸成分の絶対値の和で投影線分長を計算)
// (分離軸Sepは標準化されていること)
//
// VECTOR3 *Sep : 分離軸
// VECTOR3 *e1  : 成分１
// VECTOR3 *e2  : 成分２
// VECTOR3 *e3  : 成分３
//
// 戻り値　float 投影線分長
//------------------------------------------------------------------------
float CBBox::LenSegOnSeparateAxis(const VECTOR3 *Sep, const VECTOR3 *e1, const VECTOR3 *e2, const VECTOR3 *e3)
{
	float r1 = fabsf(dot(*Sep, *e1));
	float r2 = fabsf(dot(*Sep, *e2));
	float r3 = e3 ? (fabsf(dot(*Sep, *e3))) : 0;
	return r1 + r2 + r3;
}

//------------------------------------------------------------------------  // -- 2021.2.4
// 表示用のバウンディングボックスをレンダリングする
//
// 戻り値　なし
//------------------------------------------------------------------------
void CBBox::Render()
{
	Render(m_mWorld, GameDevice()->m_mView, GameDevice()->m_mProj, GameDevice()->m_vLightDir, GameDevice()->m_vEyePt);
}
//------------------------------------------------------------------------  // -- 2021.2.4
// 表示用のバウンディングボックスをレンダリングする
//
// (Simple.hlsliのシェーダーを使用する)
//
// const MATRIX4X4& mView    ビューマトリックス
// const MATRIX4X4& mProj    プロジェクションマトリックス
// const VECTOR3& vLight     ライトの方向
// const VECTOR3& vEye       視点位置
//
// 戻り値　なし
//------------------------------------------------------------------------
void CBBox::Render(const MATRIX4X4& mView, const MATRIX4X4& mProj, const VECTOR3& vLight, const VECTOR3& vEye)
{
	Render(m_mWorld, mView, mProj, vLight, vEye);
}
//------------------------------------------------------------------------  // -- 2021.2.4
// 表示用のバウンディングボックスをレンダリングする
//
// (Simple.hlsliのシェーダーを使用する)
//
// const MATRIX4X4& mWorld   ワールドマトリックス
// const MATRIX4X4& mView    ビューマトリックス
// const MATRIX4X4& mProj    プロジェクションマトリックス
// const VECTOR3& vLight     ライトの方向
// const VECTOR3& vEye       視点位置
//
// 戻り値　なし
//------------------------------------------------------------------------
void CBBox::Render(const MATRIX4X4& mWorld, const MATRIX4X4& mView, const MATRIX4X4& mProj, const VECTOR3& vLight, const VECTOR3& vEye)
{

	// 使用するシェーダーの登録
	m_pD3D->m_pDeviceContext->VSSetShader(m_pShader->m_pSimple_VS, nullptr, 0);
	m_pD3D->m_pDeviceContext->PSSetShader(m_pShader->m_pSimple_PS, nullptr, 0);

	// シェーダーのコンスタントバッファーに各種データを渡す
	D3D11_MAPPED_SUBRESOURCE pData;
	CONSTANT_BUFFER_WVLED cb;
	if (SUCCEEDED(m_pD3D->m_pDeviceContext->Map(m_pShader->m_pConstantBufferWVLED, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		// ワールド行列を渡す
		cb.mW = XMMatrixTranspose(mWorld);                        // -- 2021.2.4

		// ワールド、カメラ、射影行列を渡す
		cb.mWVP = XMMatrixTranspose(mWorld * mView * mProj);      // -- 2021.2.4

		//ライト方向を渡す
		cb.vLightDir = VECTOR4(vLight.x, vLight.y, vLight.z, 0);

		// 視点を渡す
		cb.vEyePos = VECTOR4(vEye.x, vEye.y, vEye.z, 1);

		//カラーを渡す
		cb.vDiffuse = m_vDiffuse;

		// 各種情報を渡す。(使っていない)    // -- 2020.12.15
		cb.vDrawInfo = VECTOR4(0, 0, 0, 0);

		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		m_pD3D->m_pDeviceContext->Unmap(m_pShader->m_pConstantBufferWVLED, 0);
	}

	// このコンスタントバッファーを使うシェーダーの登録
	m_pD3D->m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pShader->m_pConstantBufferWVLED);
	m_pD3D->m_pDeviceContext->PSSetConstantBuffers(0, 1, &m_pShader->m_pConstantBufferWVLED);


	//プリミティブ・トポロジーをセット
	m_pD3D->m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	// バーテックスバッファーをセット
	UINT stride = sizeof(BBOX_VERTEX);
	UINT offset = 0;
	m_pD3D->m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	// インデックスバッファーをセット
	m_pD3D->m_pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);


	// 頂点インプットレイアウトをセット
	m_pD3D->m_pDeviceContext->IASetInputLayout(m_pShader->m_pSimple_VertexLayout);


	// テクスチャーサンプラーをシェーダーに渡す
	m_pD3D->m_pDeviceContext->PSSetSamplers(0, 1, &m_pD3D->m_pSampleLinear);

	// テクスチャーなし
	ID3D11ShaderResourceView* Nothing[1] = { 0 };
	m_pD3D->m_pDeviceContext->PSSetShaderResources(0, 1, Nothing);

	// プリミティブをレンダリング
	m_pD3D->m_pDeviceContext->DrawIndexed(2 * 3 * 6, 0, 0);
}

