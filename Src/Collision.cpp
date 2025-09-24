//------------------------------------------------------------------------
//    メッシュ接触判定用のライブラリ
//	  								             ver 4.0        2025.1.3
//	                                                      Collision.cpp
//------------------------------------------------------------------------
#include "Collision.h"
#include "GameMain.h"

//------------------------------------------------------------------------
//	コンストラクタ
//
//	CDirect3D*	  pD3D        Direct3D
//
//------------------------------------------------------------------------
CCollision::CCollision() : CCollision(GameDevice()->m_pFbxMeshCtrl)	   		   // -- 2024.3.13
{
}
//------------------------------------------------------------------------
//	コンストラクタ
//
//	CDirect3D*	  pD3D        Direct3D							2019.8.6
//
//------------------------------------------------------------------------
CCollision::CCollision(CFbxMeshCtrl* pFbxMeshCtrl)
{
	clearAll();
	m_pD3D = pFbxMeshCtrl->m_pD3D;
	m_pShader = pFbxMeshCtrl->m_pShader;
	m_pFbxMeshCtrl = pFbxMeshCtrl; 
};
//------------------------------------------------------------------------
//	デストラクタ
//
//------------------------------------------------------------------------
CCollision::~CCollision()
{
	deleteAll();
};

//------------------------------------------------------------------------
//	全てのメンバ変数の初期化
//															2019.8.6
//
//------------------------------------------------------------------------
void CCollision::clearAll(void)
{

	// 高さ判定用の変数
	m_pIndex = nullptr;
	m_vNormalH = VECTOR3(0,0,0);
	m_fHeight = 0;
	m_vVertexH[0] = VECTOR3(0,0,0);
	m_vVertexH[1] = VECTOR3(0,0,0);
	m_vVertexH[2] = VECTOR3(0,0,0);

	// メッシュ接触判定配列添字
	m_nNum = 0;

	// コリジョン移動用
	m_bMoveFlag = false;				// 移動するかどうか 移動の時 真
	m_mWorldOld  = XMMatrixIdentity();	// 移動マトリックス一つ前
	m_mWorld     = XMMatrixIdentity();	// 移動マトリックス
	m_mWorldInv  = XMMatrixIdentity();	// 移動マトリックスの逆マトリックス

}
//------------------------------------------------------------------------
//	全ての配列の削除と初期化
//															2019.8.6
//
//------------------------------------------------------------------------
void CCollision::deleteAll(void)
{
	// メモリの解放
	for (int n = 0; n<m_nNum; n++)
	{
		// メッシュ配列の解放
		SAFE_DELETE_ARRAY(m_ColArray[n].pFace);
		SAFE_DELETE_ARRAY(m_ColArray[n].pVert);

		// 複数分割度マップの解放
		for (int i = 0; i < MESHCKTBL_FACE_MAX; i++)
		{
			if (m_ChkColMesh[n].ChkBlkArray[i].ppChkFace == nullptr) continue;
			int limit = m_ChkColMesh[n].ChkBlkArray[i].dwNumX
				* m_ChkColMesh[n].ChkBlkArray[i].dwNumY * m_ChkColMesh[n].ChkBlkArray[i].dwNumZ;
			for (int j = 0; j < limit; j++)
			{
				ChkFace* p = m_ChkColMesh[n].ChkBlkArray[i].ppChkFace[j];
				ChkFace* q;
				while (p)
				{
					q = p;
					p = p->pNext;
					SAFE_DELETE(q);
				}
			}
			SAFE_DELETE_ARRAY(m_ChkColMesh[n].ChkBlkArray[i].ppChkFace);
		}
	}

	// ゼロクリヤー（再使用時の対策）
	clearAll();
}

//------------------------------------------------------------------------
//
//	Fbxメッシュファイルからメッシュを読み込んで判定用配列に追加する
//																	2022.11.14
//
//	Fbxファイルを基にしてメッシュ接触判定用配列m_ColArrayを作成する
//	複数のFbxファイルを読み込むと、メッシュ接触判定用配列に追加していく
//	ことができる
//
//	const TCHAR* pFileName			メッシュファイルのファイル名　
//  
//  戻り値　　　bool  true　正常  false  メッシュが見つからない
//  
//------------------------------------------------------------------------
bool CCollision::AddFbxLoad(const TCHAR* pFileName)
{
	bool bRet;

	CFbxMesh* pFbxMesh;
	pFbxMesh = new CFbxMesh(m_pFbxMeshCtrl);					// -- 2021.2.4
	if (pFbxMesh->Load(pFileName) == false)  // Fbxファイルを読み込む
	{
		SAFE_DELETE(pFbxMesh);
		return false;
	}

	// メッシュオブジェクトから接触判定配列の作成
	bRet = AddFbxLoad(pFbxMesh, XMMatrixIdentity());  // メッシュオブジェクトをロードする  // -- 2022.11.14

	SAFE_DELETE(pFbxMesh);  // 読み込んだ一時メッシュを削除する

	return bRet;

}
//------------------------------------------------------------------------
//
//	Fbxメッシュファイルからメッシュを読み込んで判定用配列に追加する
//																	2019.8.6
//
//	Fbxファイルを基にしてメッシュ接触判定用配列m_ColArrayを作成する
//	複数のFbxファイルを読み込むと、メッシュ接触判定用配列に追加していく
//	ことができる
//
//	const TCHAR* pFileName			メッシュファイルのファイル名　
//  const VECTOR3& vOffset			配置位置の座標
//  
//  戻り値　　　bool  true　正常  false  メッシュが見つからない
//  
//------------------------------------------------------------------------
bool CCollision::AddFbxLoad(const TCHAR* pFileName, const VECTOR3& vOffset)
{
	bool bRet;

	CFbxMesh* pFbxMesh;
	pFbxMesh = new CFbxMesh(m_pFbxMeshCtrl);					// -- 2021.2.4
	if (pFbxMesh->Load(pFileName) == false)  // Fbxファイルを読み込む
	{
		SAFE_DELETE(pFbxMesh);
		return false;
	}

	// メッシュオブジェクトから接触判定配列の作成
	bRet = AddFbxLoad(pFbxMesh, vOffset);  // メッシュオブジェクトをロードする

	SAFE_DELETE(pFbxMesh);  // 読み込んだ一時メッシュを削除する

	return bRet;

}

//------------------------------------------------------------------------
//
//	Fbxメッシュファイルからメッシュを読み込んで判定用配列に追加する
//																	2022.11.14
//
//	Fbxファイルを基にしてメッシュ接触判定用配列m_ColArrayを作成する
//	複数のFbxファイルを読み込むと、メッシュ接触判定用配列に追加していく
//	ことができる
//
//	const TCHAR* pFileName			メッシュファイルのファイル名　
//  const MATRIX4X4& mOffset		配置位置のワールドマトリックス
//  
//  戻り値　　　bool  true　正常  false  メッシュが見つからない
//  
//------------------------------------------------------------------------
bool CCollision::AddFbxLoad(const TCHAR* pFileName, const MATRIX4X4& mOffset)
{
	bool bRet;

	CFbxMesh* pFbxMesh;
	pFbxMesh = new CFbxMesh(m_pFbxMeshCtrl);					// -- 2021.2.4
	if (pFbxMesh->Load(pFileName) == false)  // Fbxファイルを読み込む
	{
		SAFE_DELETE(pFbxMesh);
		return false;
	}

	// メッシュオブジェクトから接触判定配列の作成
	bRet = AddFbxLoad(pFbxMesh, mOffset);  // メッシュオブジェクトをロードする   // -- 2022.11.14

	SAFE_DELETE(pFbxMesh);  // 読み込んだ一時メッシュを削除する

	return bRet;

}

//  -------------------------------------------------------------------
//
//	Fbxメッシュオブジェクトからメッシュをロードして判定用配列に追加する
//																		2020.12.15
//
//	メッシュオブジェクトを基にしてメッシュ接触判定用配列m_ColArrayを
//	作成する
//	複数のスタティックメッシュオブジェクトを読み込むと、メッシュ接触判定用配列に
//	追加していくことができる
//
//  const CFbxMesh* pFbxMesh		メッシュオブジェクト
//
//  戻り値　bool  true　正常   false  エラー
//  -------------------------------------------------------------------
bool CCollision::AddFbxLoad(const CFbxMesh* pFbxMesh)
{
	return AddFbxLoad(pFbxMesh, XMMatrixIdentity());    // -- 2022.11.14
}
//  -------------------------------------------------------------------
//
//	Fbxメッシュオブジェクトからメッシュをロードして判定用配列に追加する
//																		2022.11.14
//
//	メッシュオブジェクトを基にしてメッシュ接触判定用配列m_ColArrayを
//	作成する
//	複数のスタティックメッシュオブジェクトを読み込むと、メッシュ接触判定用配列に
//	追加していくことができる
//
//  const CFbxMesh* pFbxMesh		メッシュオブジェクト
//  const VECTOR3& vOffset			配置位置の座標
//
//  戻り値　bool  true　正常   false  エラー
//  -------------------------------------------------------------------
bool CCollision::AddFbxLoad(const CFbxMesh* pFbxMesh, const VECTOR3& vOffset)
{
	return AddFbxLoad(pFbxMesh, XMMatrixTranslationFromVector(vOffset));    // -- 2022.11.14
}
//  -------------------------------------------------------------------
//
//	Fbxメッシュオブジェクトからメッシュをロードして判定用配列に追加する
//																		2022.11.14
//
//	メッシュオブジェクトを基にしてメッシュ接触判定用配列m_ColArrayを
//	作成する
//	複数のスタティックメッシュオブジェクトを読み込むと、メッシュ接触判定用配列に
//	追加していくことができる
//
//  const CFbxMesh* pFbxMesh		メッシュオブジェクト
//  const MATRIX4X4& mOffset		配置位置のワールドマトリックス
//
//  戻り値　bool  true　正常   false  エラー
//  -------------------------------------------------------------------
bool CCollision::AddFbxLoad(const CFbxMesh* pFbxMesh, const MATRIX4X4& mOffset)
{
	if (m_nNum >= MCKTBL_MAX)
	{
		MessageBox(0, _T("Collision.cpp : AddFbxLoad() : メッシュ接触判定用配列 MCKTBL_MAXをオーバーしました"), nullptr, MB_OK);
		return  false;
	}

	DWORD  i, j, nNumvert = 0, nNumidx = 0, nVertoffset = 0, f0, f1, f2;
	int n = m_nNum;
	VECTOR3 vMin = VECTOR3(9999999, 9999999, 9999999);
	VECTOR3 vMax = VECTOR3(-9999999, -9999999, -9999999);

	// 全ての子データの合計としての頂点データ数とインデックスデータ数の取得
	for (i = 0; i < pFbxMesh->m_dwMeshNum; i++)
	{
		// 頂点データ数の合計
		nNumvert += pFbxMesh->m_pMeshArray[i].m_dwVerticesNum;
		// インデックスデータ数の合計
		nNumidx += pFbxMesh->m_pMeshArray[i].m_dwIndicesNum;
	}

	// 全ての子データの合計としての頂点配列とインデックス配列を生成
	m_ColArray[n].pFace = new ColFace[nNumidx / 3];
	m_ColArray[n].pVert = new VECTOR3[nNumvert];
	m_ColArray[n].nNumFace = nNumidx / 3;
	m_ColArray[n].nNumVert = nNumvert;

	nNumvert = 0;
	nNumidx = 0;

	// 子データ毎の頂点データとインデックスデータをすべて一つの頂点・インデックスとして設定する
	for (i = 0; i < pFbxMesh->m_dwMeshNum; i++)
	{
		// 頂点データの設定
		// Offsetを加えて、実位置にする
		for (j = 0; j < pFbxMesh->m_pMeshArray[i].m_dwVerticesNum; j++, nNumvert++)
		{
			// 頂点データの設定
			// mOffsetのマトリックスによって頂点をワールド変換する   // -- 2022.11.14
			if (pFbxMesh->m_nMeshType == 1)   // メッシュタイプで頂点フォーマットが変わる    // -- 2020.12.15
			{
				m_ColArray[n].pVert[nNumvert] = XMVector3TransformCoord( pFbxMesh->m_pMeshArray[i].m_vStaticVerticesNormal[j].Pos, mOffset);    // -- 2022.11.14
			}
			else {
				m_ColArray[n].pVert[nNumvert] = XMVector3TransformCoord( pFbxMesh->m_pMeshArray[i].m_vSkinVerticesNormal[j].Pos, mOffset);      // -- 2022.11.14
			}

			// 全頂点の最大値と最小値を求める
			if (vMin.x > m_ColArray[n].pVert[nNumvert].x) vMin.x = m_ColArray[n].pVert[nNumvert].x;
			if (vMin.y > m_ColArray[n].pVert[nNumvert].y) vMin.y = m_ColArray[n].pVert[nNumvert].y;
			if (vMin.z > m_ColArray[n].pVert[nNumvert].z) vMin.z = m_ColArray[n].pVert[nNumvert].z;
			if (vMax.x < m_ColArray[n].pVert[nNumvert].x) vMax.x = m_ColArray[n].pVert[nNumvert].x;
			if (vMax.y < m_ColArray[n].pVert[nNumvert].y) vMax.y = m_ColArray[n].pVert[nNumvert].y;
			if (vMax.z < m_ColArray[n].pVert[nNumvert].z) vMax.z = m_ColArray[n].pVert[nNumvert].z;
		}

		// インデックスデータと面法線とＡＡＢＢの設定
		for (j = 0; j < pFbxMesh->m_pMeshArray[i].m_dwIndicesNum; j+=3, nNumidx+=3)
		{
			f0 = m_ColArray[n].pFace[nNumidx/3].dwIdx[0] = pFbxMesh->m_pMeshArray[i].m_nIndices[j+0] + nVertoffset;  // 全体の中でのインデックス値に変換するためにオフセットを加える
			f1 = m_ColArray[n].pFace[nNumidx/3].dwIdx[1] = pFbxMesh->m_pMeshArray[i].m_nIndices[j+1] + nVertoffset;  // 全体の中でのインデックス値に変換するためにオフセットを加える
			f2 = m_ColArray[n].pFace[nNumidx/3].dwIdx[2] = pFbxMesh->m_pMeshArray[i].m_nIndices[j+2] + nVertoffset;  // 全体の中でのインデックス値に変換するためにオフセットを加える
			//m_ColArray[n].pFace[nNumidx / 3].vNormal = normalize(cross(m_ColArray[n].pVert[f2] - m_ColArray[n].pVert[f0],		// 面法線を作成する		  // -- 2024.3.23
			//																m_ColArray[n].pVert[f1] - m_ColArray[n].pVert[f0]));
			m_ColArray[n].pFace[nNumidx / 3].vNormal = normalize(cross(m_ColArray[n].pVert[f1] - m_ColArray[n].pVert[f0],		// 面法線を作成する		  // -- 2024.3.23
																			m_ColArray[n].pVert[f2] - m_ColArray[n].pVert[f0]));
			m_ColArray[n].pFace[nNumidx / 3].AABB.MakeAABB(m_ColArray[n].pVert[f0], m_ColArray[n].pVert[f1], m_ColArray[n].pVert[f2]);  // ポリゴンのAABBを作成する
		}

		nVertoffset = nNumvert;  // 子毎のインデックスデータを全体の中でのインデックス値に変換するためのオフセット
	}

	// 複数分割度マップを作成する
	makeChkColMesh(m_nNum, vMin, vMax);

	m_nNum++;	// メッシュ接触判定配列添字を１増やす

	return true;
}

//  -------------------------------------------------------------------
//
//	複数分割度マップを作成する
//
//	複数分割度マップm_ChkColMeshを作成する目的は、マップを構成する
//	三角ポリゴンをその大きさに応じた、細分化された格納場所に置くことで、
//	接触判定の際に、判定に必要な位置の三角ポリゴンのみを対象とした処理を
//	行ない、処理時間の大幅な短縮を行うことができる点にある。
//	なお、複数分割度マップには、あくまでも面（三角ポリゴン）データの
//	アドレスのみを格納し、データそのものは、メッシュ接触判定配列
//	m_ColArrayに置いている。
//
//	引数
//		const int&		nNum   メッシュ配列添字
//		const VECTOR3&	vMin   メッシュの全頂点の中の最小値
//		const VECTOR3&	vMax   メッシュの全頂点の中の最大値
//
//  戻り値　なし
//  -------------------------------------------------------------------
void CCollision::makeChkColMesh( const int& nNum, const VECTOR3& vMin, const VECTOR3& vMax)
{
	const float fSpare = 1.05f;  // 正立方体の大きさ５％の余裕

	int     n = nNum;
	int     i, j;
	float   fx, fy, fz;

	int ChkIdx[8], ChkIMax;

	// 全メッシュの頂点の最小値の取得
	m_ChkColMesh[n].vMin = vMin;
	m_ChkColMesh[n].vMax = vMax;					// -- 2020.12.3

	// 複数分割度マップm_ChkColMesh配列の新規作成
	// 全体を表す配列から、１／２づつの大きさの配列を順次作成していく
	for (i = 0; i < MESHCKTBL_FACE_MAX; i++) 
	{
		if (i == 0) 
		{
			// 最初の配列（全体を取り囲む、正立方体）
			fx = (float)fabs(vMax.x - vMin.x);
			fy = (float)fabs(vMax.y - vMin.y);
			fz = (float)fabs(vMax.z - vMin.z);
			if (fx < fy) fx = fy;    // 全ての辺の長さは同じ(正立方体)
			if (fx < fz) fx = fz;    // 全ての辺の長さは同じ(正立方体)
			fx *= fSpare;	// 余裕を持つ
			m_ChkColMesh[n].ChkBlkArray[i].vBlksize.x = m_ChkColMesh[n].ChkBlkArray[i].vBlksize.y = m_ChkColMesh[n].ChkBlkArray[i].vBlksize.z = fx;
		}
		else {
			// MESHCKTBL_LOWLIMIT(３．０ｍ)以下の配列は作成しない
			if (m_ChkColMesh[n].ChkBlkArray[i - 1].vBlksize.x <= MESHCKTBL_LOWLIMIT &&
				m_ChkColMesh[n].ChkBlkArray[i - 1].vBlksize.y <= MESHCKTBL_LOWLIMIT &&
				m_ChkColMesh[n].ChkBlkArray[i - 1].vBlksize.z <= MESHCKTBL_LOWLIMIT) 
			{
				for (; i < MESHCKTBL_FACE_MAX; i++)  // 残りの配列の内容は全て０とする
				{
					m_ChkColMesh[n].ChkBlkArray[i].ppChkFace = nullptr;   // 作成しない位置のポインタはnullptr
					m_ChkColMesh[n].ChkBlkArray[i].vBlksize.x = 0;
					m_ChkColMesh[n].ChkBlkArray[i].vBlksize.y = 0;
					m_ChkColMesh[n].ChkBlkArray[i].vBlksize.z = 0;
					m_ChkColMesh[n].ChkBlkArray[i].dwNumX = 0;
					m_ChkColMesh[n].ChkBlkArray[i].dwNumY = 0;
					m_ChkColMesh[n].ChkBlkArray[i].dwNumZ = 0;
				}
				break;  // ループを抜け出す
			}
			else {
				// １／２、１／４、１／８・・・・・の配列
				m_ChkColMesh[n].ChkBlkArray[i].vBlksize.x = m_ChkColMesh[n].ChkBlkArray[i - 1].vBlksize.x / 2;
				m_ChkColMesh[n].ChkBlkArray[i].vBlksize.y = m_ChkColMesh[n].ChkBlkArray[i - 1].vBlksize.y / 2;
				m_ChkColMesh[n].ChkBlkArray[i].vBlksize.z = m_ChkColMesh[n].ChkBlkArray[i - 1].vBlksize.z / 2;
			}
		}

		// 新規生成する配列の要素数を計算する
		// dwNumは、生成したvBlksizeが本来のメッシュ全体の大きさに対して何個必要なのかを求める
		m_ChkColMesh[n].ChkBlkArray[i].dwNumX = (DWORD)(fabs(vMax.x - vMin.x) / m_ChkColMesh[n].ChkBlkArray[i].vBlksize.x + 1);
		m_ChkColMesh[n].ChkBlkArray[i].dwNumY = (DWORD)(fabs(vMax.y - vMin.y) / m_ChkColMesh[n].ChkBlkArray[i].vBlksize.y + 1);
		m_ChkColMesh[n].ChkBlkArray[i].dwNumZ = (DWORD)(fabs(vMax.z - vMin.z) / m_ChkColMesh[n].ChkBlkArray[i].vBlksize.z + 1);

		// 配列を新規生成する。なお、ppChkFaceは構造体ポインタ配列である
		// 要素数は、全必要数なのでdwNumX*dwNumY*dwNumZ個である
		m_ChkColMesh[n].ChkBlkArray[i].ppChkFace = new struct ChkFace*[m_ChkColMesh[n].ChkBlkArray[i].dwNumX * m_ChkColMesh[n].ChkBlkArray[i].dwNumY * m_ChkColMesh[n].ChkBlkArray[i].dwNumZ];
		if (m_ChkColMesh[n].ChkBlkArray[i].ppChkFace == nullptr) 
		{
			MessageBox(0, _T("Collision.cpp : MakeChkColMesh() : m_ChkColMesh[n].ChkBlkArray[i].ppChkFace　メモリー割り当てエラー"), nullptr, MB_OK);
			return;
		}

		// 生成したポインタ配列をゼロクリヤーする
		ZeroMemory((BYTE**)m_ChkColMesh[n].ChkBlkArray[i].ppChkFace, sizeof(struct ChkFace*) * (m_ChkColMesh[n].ChkBlkArray[i].dwNumX * m_ChkColMesh[n].ChkBlkArray[i].dwNumY * m_ChkColMesh[n].ChkBlkArray[i].dwNumZ));
	}

	// メッシュの面配列から三角形ポリゴンデータを取得し、そのアドレスを
	// 複数分割度マップに格納していく
	// なお、１ポリゴンのアドレス格納場所は、リスト構造となっていて、
	// 複数個のポリゴンアドレスを格納できるようになっている。
	for (i = 0; i < m_ColArray[n].nNumFace; i++)
	{
		// 三角形ポリゴンのＡＡＢＢの大きさを求める
		fx = fabsf(m_ColArray[n].pFace[i].AABB.m_vMax.x - m_ColArray[n].pFace[i].AABB.m_vMin.x);
		fy = fabsf(m_ColArray[n].pFace[i].AABB.m_vMax.y - m_ColArray[n].pFace[i].AABB.m_vMin.y);
		fz = fabsf(m_ColArray[n].pFace[i].AABB.m_vMax.z - m_ColArray[n].pFace[i].AABB.m_vMin.z);

		// 三角形ポリゴンの大きさから、最適な大きさの格納配列を決定する
		for (j = 0; j < MESHCKTBL_FACE_MAX - 1; j++) 
		{
			if (m_ChkColMesh[n].ChkBlkArray[j+1].ppChkFace == nullptr) break;
			// 一つ小さい[j+1]の大きさの配列に入りきらないときは、この配列に格納する大きさであるとする
			if (m_ChkColMesh[n].ChkBlkArray[j+1].vBlksize.x < fx ||
				m_ChkColMesh[n].ChkBlkArray[j+1].vBlksize.y < fy ||
				m_ChkColMesh[n].ChkBlkArray[j+1].vBlksize.z < fz) break;
		}

		// 同じ大きさの配列の中のどこの位置に格納するかを決定する
		getChkArrayIdx(n, j, m_ColArray[n].pFace[i].AABB, ChkIdx, ChkIMax);

		// 格納配列に格納する（リスト構造である）
		for (int k = 0; k < ChkIMax; k++)
		{
			setChkArray(n, j, ChkIdx[k], &m_ColArray[n].pFace[i]);
		}
	}
	
}
//-----------------------------------------------------------------------------
// 同じ大きさの配列の中のどこの位置に格納するかを決定する
//																	2019.8.6
//  
//  三角形ポリゴンのＡＡＢＢの頂点８つのそれぞれがどの位置に格納されるかを調べる
//  
//  うまく、格納位置に納まる場合には1カ所ですむが、格納場所をまたいで存在する時は、
//  最大で左右上下前後の8カ所にまたがる場合がある。
//  なお、格納ポリゴンは格納場所の大きさより小さいため、２つ以上隣にまたがることはない。
//  
//	const int&   nNum       メッシュ配列添字
//  const int&   nNo        最適な大きさの格納配列ChkBlkArray添字
//  CAABB AABB              三角形ポリゴンのＡＡＢＢ
//  int   nIdx[8]           格納場所配列。最大８カ所に分かれる可能性有り(Out)
//  int&  nIMax             格納場所配列の格納数。最大８(Out)
//  
//  戻り値　なし
//-----------------------------------------------------------------------------
void  CCollision::getChkArrayIdx(const int& nNum, const int& nNo, CAABB AABB, int nIdx[], int& nIMax)
{
	VECTOR3 vPos;
	int i, j, x, y, z, m;
	int n = nNum;

	// 格納場所配列クリヤー
	nIMax = 0;
	for ( i = 0; i < 8; i++) nIdx[i] = 0;

	// 格納場所配列の設定
	for ( i = 0; i < 8; i++ )
	{
		vPos = AABB.GetVecPos(i);  // ＡＡＢＢの８頂点を順番に得る

		// 頂点から格納場所ブロックのxyz方向の添字番号を得る
		x = (int)((vPos.x - m_ChkColMesh[n].vMin.x) / m_ChkColMesh[n].ChkBlkArray[nNo].vBlksize.x);
		y = (int)((vPos.y - m_ChkColMesh[n].vMin.y) / m_ChkColMesh[n].ChkBlkArray[nNo].vBlksize.y);
		z = (int)((vPos.z - m_ChkColMesh[n].vMin.z) / m_ChkColMesh[n].ChkBlkArray[nNo].vBlksize.z);
		if (x >= (int)m_ChkColMesh[n].ChkBlkArray[nNo].dwNumX || y >= (int)m_ChkColMesh[n].ChkBlkArray[nNo].dwNumY
			|| z >= (int)m_ChkColMesh[n].ChkBlkArray[nNo].dwNumZ ||	x < 0 || y < 0 || z < 0) {
			MessageBox(0, _T("Collision.cpp : GetChkArrayIdx() : 配列オーバーエラー"), nullptr, MB_OK);
			return;
		}

		// 格納する位置を確定する
		// xyzの添字番号から、格納位置の添字を計算で得る
		// １次元目がＸ方向、２次元目がＺ方向、３次元目がＹ方向である
		m = y * m_ChkColMesh[n].ChkBlkArray[nNo].dwNumX * m_ChkColMesh[n].ChkBlkArray[nNo].dwNumZ
												+ z * m_ChkColMesh[n].ChkBlkArray[nNo].dwNumX + x;

		// 決定した位置を格納場所配列に書き込む
		if (nIMax == 0)  // 最初の格納
		{
			nIdx[nIMax] = m;
			nIMax++;
		}
		else {
			for (j = 0; j < nIMax; j++)  // 同じ格納場所がないかどうか調べる
			{
				if (nIdx[j] == m) break;  // 同じ格納場所が見つかったので読み飛ばす
			}
			if (j == nIMax)   // 異なる格納場所なので書き込む
			{
				nIdx[nIMax] = m;
				nIMax++;
			}
		}
	}
}
//-----------------------------------------------------------------------------
// 格納配列の指定位置に格納する関数
//																	2019.8.6
//  
//  格納配列の指定位置はリスト構造のためその最後尾に追加する
//  なお、格納するデータは面（三角形ポリゴン）のアドレスである。
//  
//	const int&     nNum    メッシュ配列添字
//  const int&     nNo     最適な大きさの格納配列ChkBlkArray添字
//  const int&     nIdx    格納場所配列添字
//  ColFace* pFace         面のアドレス
//  
//  戻り値　なし
//-----------------------------------------------------------------------------
void  CCollision::setChkArray(const int& nNum, const int& nNo, const int& nIdx, ColFace* pFace)
{
	struct ChkFace* p;
	int n = nNum;

	if (m_ChkColMesh[n].ChkBlkArray[nNo].ppChkFace[nIdx] == nullptr)
	{
		// リスト構造の先頭ポインタに追加するとき
		m_ChkColMesh[n].ChkBlkArray[nNo].ppChkFace[nIdx] = new struct ChkFace;
		m_ChkColMesh[n].ChkBlkArray[nNo].ppChkFace[nIdx]->pFace = pFace;
		m_ChkColMesh[n].ChkBlkArray[nNo].ppChkFace[nIdx]->pNext = nullptr;
	}
	else {
		// リスト構造の最後に追加するとき
		p = m_ChkColMesh[n].ChkBlkArray[nNo].ppChkFace[nIdx];
		while (1)
		{
			if (p->pNext == nullptr)
			{
				// リスト構造の最後であるので、データを追加する
				p->pNext = new struct ChkFace;
				p = p->pNext;
				p->pFace = pFace;
				p->pNext = nullptr;
				break;
			}
			// 次ポインタをたどって行く
			p = p->pNext;
		}
	}
}
// -----------------------------------------------------------------------------------------------------------
// 配列の対象とするブロック番号の範囲を求める
//																						2019.8.6
//
//		const int&		nNum		メッシュ配列添字
//		const int&		nNo			最適な大きさの格納配列ChkBlkArray添字
//		const VECTOR3&	vNow		移動後の位置
//		const VECTOR3&	vOld		移動前位置
//		const float&	fRadius		オブジェクトの半径
//		int&			nStatrX		ブロック番号開始Ｘ(out)
//		int&			nEndX		ブロック番号終了Ｘ(out)
//		int&			nStatrY		ブロック番号開始Ｙ(out)
//		int&			nEndY		ブロック番号終了Ｙ(out)
//		int&			nStatrZ		ブロック番号開始Ｚ(out)
//		int&			nEndZ		ブロック番号終了Ｚ(out)
//
//	戻り値 
//		なし
// -----------------------------------------------------------------------------------------------------------
void	CCollision::getMeshLimit(const int& nNum, const int& nNo, const VECTOR3& vNow, const VECTOR3& vOld, const float& fRadius,
									int& nStatrX, int& nEndX, int& nStatrY, int& nEndY, int& nStatrZ, int& nEndZ)
{
	int n = nNum;
	int i = nNo;
	int x, y, z;

	// 移動直線を囲むAABBを作成する。余裕をfRadiusとる。
	CAABB AABB;
	AABB.MakeAABB(vNow, vOld, fRadius);

	// 配列の対象とする開始点のブロック番号を求める
	x = (int)((AABB.m_vMin.x - m_ChkColMesh[n].vMin.x) / m_ChkColMesh[n].ChkBlkArray[i].vBlksize.x);
	y = (int)((AABB.m_vMin.y - m_ChkColMesh[n].vMin.y) / m_ChkColMesh[n].ChkBlkArray[i].vBlksize.y);
	z = (int)((AABB.m_vMin.z - m_ChkColMesh[n].vMin.z) / m_ChkColMesh[n].ChkBlkArray[i].vBlksize.z);
	if (x < 0)				x = 0;
	if (x >= (int)m_ChkColMesh[n].ChkBlkArray[i].dwNumX)	x = m_ChkColMesh[n].ChkBlkArray[i].dwNumX - 1;
	if (y < 0)				y = 0;
	if (y >= (int)m_ChkColMesh[n].ChkBlkArray[i].dwNumY)	y = m_ChkColMesh[n].ChkBlkArray[i].dwNumY - 1;
	if (z < 0)				z = 0;
	if (z >= (int)m_ChkColMesh[n].ChkBlkArray[i].dwNumZ)	z = m_ChkColMesh[n].ChkBlkArray[i].dwNumZ - 1;
	nStatrX = x;
	nStatrY = y;
	nStatrZ = z;

	// 配列の対象とする終了点のブロック番号を求める
	x = (int)((AABB.m_vMax.x - m_ChkColMesh[n].vMin.x) / m_ChkColMesh[n].ChkBlkArray[i].vBlksize.x);
	y = (int)((AABB.m_vMax.y - m_ChkColMesh[n].vMin.y) / m_ChkColMesh[n].ChkBlkArray[i].vBlksize.y);
	z = (int)((AABB.m_vMax.z - m_ChkColMesh[n].vMin.z) / m_ChkColMesh[n].ChkBlkArray[i].vBlksize.z);
	if (x < 0)				x = 0;
	if (x >= (int)m_ChkColMesh[n].ChkBlkArray[i].dwNumX)	x = m_ChkColMesh[n].ChkBlkArray[i].dwNumX - 1;
	if (y < 0)				y = 0;
	if (y >= (int)m_ChkColMesh[n].ChkBlkArray[i].dwNumY)	y = m_ChkColMesh[n].ChkBlkArray[i].dwNumY - 1;
	if (z < 0)				z = 0;
	if (z >= (int)m_ChkColMesh[n].ChkBlkArray[i].dwNumZ)	z = m_ChkColMesh[n].ChkBlkArray[i].dwNumZ - 1;

	if (nStatrX <= x) {
		nEndX = x;
	}
	else {
		nEndX = nStatrX;
		nStatrX = x;
	}
	if (nStatrY <= y) {
		nEndY = y;
	}
	else {
		nEndY = nStatrY;
		nStatrY = y;
	}
	if (nStatrZ <= z) {
		nEndZ = z;
	}
	else {
		nEndZ = nStatrZ;
		nStatrZ = z;
	}
}

//-----------------------------------------------------------------------------
// オブジェクトのレイとメッシュ接触判定用配列との接触判定
//																		2019.8.6
//  
//	const VECTOR3&　startIn		移動開始位置の座標
//	const VECTOR3&　endIn		移動終了位置の座標
//  VECTOR3 &vHit				接触点の座標（出力）
//  VECTOR3 &vNormal			接触点の法線ベクトル（出力）
//  
//  戻り値　bool bRet
//		接触したとき　		true
//		接触していないとき  false
//-----------------------------------------------------------------------------
bool  CCollision::IsCollisionLay(const VECTOR3& startIn, const VECTOR3& endIn, VECTOR3& vHit, VECTOR3& vNormal)
{
	bool     bRet = false;
	int      n, i;
	int      x, y, z, m;
	int      nStartX, nEndX, nStartY, nEndY, nStartZ, nEndZ;
	ChkFace* p;

	VECTOR3  vOld, vNow;
	VECTOR3  vVert[3], vFaceNorm, vInsPt;
	float    fNowDist, fOldDist, fLayDist;
	float    fLenMin = 9999999.0f;				// 交点と移動前点との距離の最小値

	// 移動マップの処理を行う
	if (m_bMoveFlag)  // コリジョンマップが移動しているかどうか
	{
		// コリジョンマップが移動しているときは、キャラクターが逆に移動していると見なして判定をする
		// このために、キャラクターの位置にマップ移動の逆行列を掛けて判定を行う
		vOld = XMVector3TransformCoord(startIn, m_mWorldInv);
		vNow = XMVector3TransformCoord(endIn, m_mWorldInv);
	}
	else {
		// コリジョンマップが移動していないときは、キャラクターの位置をそのまま使用する
		vOld = startIn;
		vNow = endIn;
	}

	// レイとメッシュとの衝突判定を行う
	for (n = 0; n<m_nNum; n++)
	{
		// 複数分割度配列の検索
		for (i = 0; i < MESHCKTBL_FACE_MAX; i++)
		{
			if (m_ChkColMesh[n].ChkBlkArray[i].ppChkFace == nullptr) break;	// 配列にデータがないとき

			// 配列の対象とする開始点と終了点のブロック番号を求める
			getMeshLimit(n, i, vNow, vOld, 0, nStartX, nEndX, nStartY, nEndY, nStartZ, nEndZ);

			// 配列の検索を開始する
			for (y = nStartY; y <= nEndY; y++) {
				for (z = nStartZ; z <= nEndZ; z++) {
					for (x = nStartX; x <= nEndX; x++) {
						m = y * m_ChkColMesh[n].ChkBlkArray[i].dwNumX * m_ChkColMesh[n].ChkBlkArray[i].dwNumZ
							+ z * m_ChkColMesh[n].ChkBlkArray[i].dwNumX + x;

						p = m_ChkColMesh[n].ChkBlkArray[i].ppChkFace[m];

						while (p != nullptr)  // リスト構造の最後まで
						{

							// ３角形ポリゴンの値を得る
							vVert[0] = m_ColArray[n].pVert[p->pFace->dwIdx[0]];
							vVert[1] = m_ColArray[n].pVert[p->pFace->dwIdx[1]];
							vVert[2] = m_ColArray[n].pVert[p->pFace->dwIdx[2]];
							vFaceNorm = p->pFace->vNormal;

							// 直線の３角形ポリゴン法線方向の距離を求める
							getDistNormal(vVert, vNow, vOld, vFaceNorm, fNowDist, fOldDist, fLayDist);
							// ３角形ポリゴンと直線（レイ）との接触判定を行う
							if (checkLay(vVert, vNow, vOld, vFaceNorm, fNowDist, fOldDist, fLayDist, vInsPt) == 1)
							{
								bRet = true;   // 接触している
								float len = magnitude(vOld - vInsPt);
								if (fLenMin > len)   // より近い交点を探す
								{
									fLenMin = len;
									if (m_bMoveFlag)  // コリジョンマップが移動しているかどうか
									{
										// コリジョンマップが移動しているときは、実際の描画位置に戻して出力する
										vHit = XMVector3TransformCoord(vInsPt, m_mWorld);	   // -- 2024.9.10
										vNormal = XMVector3TransformCoord(vFaceNorm, GetRotateMatrix(m_mWorld));
										vNormal = normalize(vNormal);
									}
									else {
										vHit = vInsPt;
										vNormal = vFaceNorm;
									}
								}
							}
							p = p->pNext;   // リスト構造をたどって次のポリゴンを探す
						}
					}
				}
			}
		}
	}
	return  bRet;
}
//-----------------------------------------------------------------------------
// オブジェクトの球とメッシュ接触判定用配列との接触判定
//																				// -- 2020.12.14
//  
//	const VECTOR3&　startIn		移動開始位置の座標
//	const VECTOR3&　endIn		移動終了位置の座標
//	const float&　fRadius		球の半径
//  VECTOR3 &vHit				接触時のオブジェクト中心位置の座標（出力）
//  VECTOR3 &vNormal			接触点の法線ベクトル（出力）
//  
//  戻り値　book bRet
//		接触したとき　		true
//		接触していないとき	false
//-----------------------------------------------------------------------------
bool  CCollision::IsCollisionSphere(const VECTOR3& startIn, const VECTOR3& endIn, const float& fRadius, VECTOR3& vHit, VECTOR3& vNormal)
{
	bool     bRet = false;
	int      n, i;
	int      x, y, z, m;
	int      nStartX, nEndX, nStartY, nEndY, nStartZ, nEndZ;
	ChkFace* p;

	VECTOR3  vNow, vOld;
	VECTOR3  vVert[3], vFaceNorm, vInsPt;
	float    fNowDist, fOldDist, fLayDist;
	float    fLenMin = 9999999.0f;				// 交点と移動前点との距離の最小値

	// 移動マップの処理を行う
	if (m_bMoveFlag)  // コリジョンマップが移動しているかどうか
	{
		// コリジョンマップが移動しているときは、キャラクターが逆に移動していると見なして判定をする
		// このために、キャラクターの位置マトリックスにマップ移動の逆行列を掛けて判定を行う
		vOld = XMVector3TransformCoord(startIn, m_mWorldInv);
		vNow = XMVector3TransformCoord(endIn, m_mWorldInv);
	}
	else {
		// コリジョンマップが移動していないときは、キャラクターの位置をそのまま使用する
		vOld = startIn;
		vNow = endIn;
	}

	// レイとメッシュとの衝突判定を行う
	for (n = 0; n < m_nNum; n++)
	{
		// 複数分割度配列の検索
		for (i = 0; i < MESHCKTBL_FACE_MAX; i++)
		{
			if (m_ChkColMesh[n].ChkBlkArray[i].ppChkFace == nullptr) break;	// 配列にデータがないとき

			// 配列の対象とする開始点と終了点のブロック番号を求める
			// ・半径分移動させるため、判定範囲を半径の２倍とする
			getMeshLimit(n, i, vNow, vOld, fRadius*2, nStartX, nEndX, nStartY, nEndY, nStartZ, nEndZ);

			// 配列の検索を開始する
			for (y = nStartY; y <= nEndY; y++) {
				for (z = nStartZ; z <= nEndZ; z++) {
					for (x = nStartX; x <= nEndX; x++) {
						m = y * m_ChkColMesh[n].ChkBlkArray[i].dwNumX * m_ChkColMesh[n].ChkBlkArray[i].dwNumZ
							+ z * m_ChkColMesh[n].ChkBlkArray[i].dwNumX + x;

						p = m_ChkColMesh[n].ChkBlkArray[i].ppChkFace[m];

						while (p != nullptr)  // リスト構造の最後まで
						{

							// ３角形ポリゴンの値を得る
							vVert[0] = m_ColArray[n].pVert[p->pFace->dwIdx[0]];
							vVert[1] = m_ColArray[n].pVert[p->pFace->dwIdx[1]];
							vVert[2] = m_ColArray[n].pVert[p->pFace->dwIdx[2]];
							vFaceNorm = p->pFace->vNormal;

							// 法線方向に半径分移動するベクトルを求める
							VECTOR3 vNormalRadius = vFaceNorm * fRadius;

							// 直線の３角形ポリゴン法線方向の距離を求める
							// ・オブジェクトの中心点から法線方向の逆方向に半径分移動させた点で判定する
							getDistNormal(vVert, vNow - vNormalRadius, vOld - vNormalRadius, vFaceNorm, fNowDist, fOldDist, fLayDist);

                            // ３角形ポリゴンと直線（レイ）との接触判定を行う
							// ・オブジェクトの中心点から法線方向の逆方向に半径分移動させた点で判定する
							if (checkLay(vVert, vNow - vNormalRadius, vOld - vNormalRadius, vFaceNorm, fNowDist, fOldDist, fLayDist, vInsPt) == 1)
							{
								bRet = true;   // 接触している
								float len = magnitude(vOld - vInsPt);
								if (fLenMin > len)   // より近い交点を探す
								{
									fLenMin = len;
									if (m_bMoveFlag)  // コリジョンマップが移動しているかどうか
									{
										// コリジョンマップが移動しているときは、実際の描画位置に戻して出力する
										vHit = XMVector3TransformCoord(vInsPt + vNormalRadius, m_mWorld); // 接触位置を法線方向に半径分戻してやる	  // -- 2024.9.10
										vNormal = XMVector3TransformCoord(vFaceNorm, GetRotateMatrix(m_mWorld));
										vNormal = normalize(vNormal);
									}
									else {
										vHit = vInsPt + vNormalRadius; // 接触位置を法線方向に半径分戻してやる
										vNormal = vFaceNorm;
									}
								}
							}
							else {
								// 接触していないときは、ポリゴンを法線方向に持ち上げた事により、
								// ポリゴンの切れ目ですり抜けている可能性があるので、
								// 再度、直線の中心点で接触判定を行う
								getDistNormal(vVert, vNow, vOld, vFaceNorm, fNowDist, fOldDist, fLayDist);

								// ３角形ポリゴンと直線（レイ）との接触判定を行う
								if (checkLay(vVert, vNow, vOld, vFaceNorm, fNowDist, fOldDist, fLayDist, vInsPt) == 1)
								{
									bRet = true;   // 接触している
									float len = magnitude(vOld - vInsPt);
									if (fLenMin > len)   // より近い交点を探す
									{
										fLenMin = len;
										if (m_bMoveFlag)  // コリジョンマップが移動しているかどうか
										{
											// コリジョンマップが移動しているときは、実際の描画位置に戻して出力する
											vHit = XMVector3TransformCoord(vInsPt - normalize(vNow - vOld)*fRadius, m_mWorld); // 接触位置を進行方向の逆方向に半径分戻してやる	 // -- 2024.9.10
											vNormal = XMVector3TransformCoord(vFaceNorm, GetRotateMatrix(m_mWorld));
											vNormal = normalize(vNormal);
										}
										else {
											vHit = vInsPt - normalize(vNow - vOld)*fRadius; // 接触位置を進行方向の逆方向に半径分戻してやる
											vNormal = vFaceNorm;
										}
									}
								}
							}
							p = p->pNext;   // リスト構造をたどって次のポリゴンを探す
						}
					}
				}
			}
		}
	}

	return  bRet;
}

//-----------------------------------------------------------------------------
// オブジェクトのレイとメッシュ接触判定用配列との接触判定とスリスリ動かす制御
//																		// -- 2024.9.10
//	重力を考えず、接触判定と移動を行う
//  また、判定は２度行う
//  
//  const VECTOR3& positionOld     オブジェクトの1つ前の位置
//  VECTOR3& position              オブジェクトの現在位置(in/out)
//  float fRadius                  オブジェクトの半径（省略値は0.2）
//  
//  戻り値　bool bRet
//		接触したとき　		true
//		接触していないとき	false
//-----------------------------------------------------------------------------
bool CCollision::IsCollisionMove(const VECTOR3& positionOld, VECTOR3& position, float fRadius)
{
	VECTOR3 vHit, vNormal;
	return IsCollisionMove( positionOld, position, vHit, vNormal, fRadius);
}
//-----------------------------------------------------------------------------
// オブジェクトのレイとメッシュ接触判定用配列との接触判定とスリスリ動かす制御
//																		// -- 2024.9.10
//	重力を考えず、接触判定と移動を行う
//  また、判定は２度行う
//  
//  const VECTOR3& positionOld     オブジェクトの1つ前の位置
//  VECTOR3& position              オブジェクトの現在位置(in/out)
//  VECTOR3 &vHit					接触点の座標（出力）
//  VECTOR3 &vNormal				接触点の法線ベクトル（出力）
//  float fRadius					オブジェクトの半径（省略値は0.2）
// 
//  戻り値　bool bRet
//		接触したとき　		true
//		接触していないとき	false
//-----------------------------------------------------------------------------
bool CCollision::IsCollisionMove(const VECTOR3& positionOld, VECTOR3& position, VECTOR3& vHit, VECTOR3& vNormal, float fRadius)
{

	bool bRet = false;
	int  nRet = 0;

	VECTOR3 vOld = positionOld;
	VECTOR3 vNow = position;

	// 移動マップの事前処理を行う
	if (m_bMoveFlag)  // コリジョンマップが移動しているかどうか
	{
		// コリジョンマップが移動しているときは、キャラクターが逆に移動していると見なして判定をする
		// このために、キャラクターの位置マトリックスにマップ移動の逆行列を掛けて判定を行う
		vOld = XMVector3TransformCoord( vOld, m_mWorldInv);
		vNow = XMVector3TransformCoord( vNow, m_mWorldInv);
	}
	else {
		// コリジョンマップが移動していないときは、キャラクターの位置をそのまま使用する
	}

	// 接触判定とスリスリ動かす制御を行う
	nRet = checkCollisionMove(vOld, vNow, vHit, vNormal, fRadius);    // -- 2022.11.14
	if ( nRet != 0)   // 接触して移動したときは、2度目の接触判定と移動を行う
	{
		checkCollisionMove(vOld, vNow, vHit, vNormal, fRadius);    // -- 2022.11.14
	}

	// 移動マップの事後処理を行う
	if (m_bMoveFlag)  // コリジョンマップが移動しているかどうか
	{
		// コリジョンマップが移動しているときは、マップ移動の行列を掛けて元の値に戻す
		position = XMVector3TransformCoord(vNow, m_mWorld);
	}
	else {
		// コリジョンマップが移動していないときは、そのまま使用する
		position = vNow;
	}
	
	if (nRet != 0) bRet = true;

	return bRet;
}

//-----------------------------------------------------------------------------
// オブジェクトのレイとメッシュ接触判定用配列との接触判定とスリスリ動かす制御
//																		2024.9.10
//  
//	重力を考えず、接触判定と適切な位置への移動を行う
//  
//  const VECTOR3& positionOld   オブジェクトの1つ前の位置
//  VECTOR3& position            オブジェクトの現在位置(in/out)
//  VECTOR3 &vHit				 接触点の座標（出力）
//  VECTOR3 &vNormal			 接触点の法線ベクトル（出力）
//  float fRadius                オブジェクトの半径
//  
//  戻り値　int nRet
//		接触したとき　		１
//		接触していないとき	０
//		なお、内部で近接のとき　２
//-----------------------------------------------------------------------------
int CCollision::checkCollisionMove(const VECTOR3& positionOld, VECTOR3& position, VECTOR3& vHit, VECTOR3& vNormal, float fRadius)
{
	int      nRet = 0;
	int      n, i;
	int      x, y, z, m;
	int      nStartX, nEndX, nStartY, nEndY, nStartZ, nEndZ;
	ChkFace* p;

	VECTOR3  vNow, vOld;

	// 位置を得る
	vOld = positionOld;
	vNow = position;

	VECTOR3  vVert[3], vFaceNorm, vInsPt, vMove;
	float    fNowDist, fOldDist, fLayDist;

	float    fLenMin = 9999999.0f;				// 交点と移動前点との距離の最小値
	float    fNowDistSave;
	VECTOR3  vFaceNormSave;

	float    fNearMin = 9999999.0f;				// 食い込み距離最小値
	float    fNowDistNearSave;
	VECTOR3  vFaceNormNearSave;

	// レイとメッシュとの衝突判定を行う
	for (n = 0; n<m_nNum; n++)
	{
		// 複数分割度配列の検索
		for (i = 0; i < MESHCKTBL_FACE_MAX; i++)
		{
			if (m_ChkColMesh[n].ChkBlkArray[i].ppChkFace == nullptr) break;	// 配列にデータがないとき

			// 配列の対象とする開始点と終了点のブロック番号を求める
			getMeshLimit(n, i, vNow, vOld, fRadius, nStartX, nEndX, nStartY, nEndY, nStartZ, nEndZ);

			// 配列の検索を開始する
			for (y = nStartY; y <= nEndY; y++) {
				for (z = nStartZ; z <= nEndZ; z++) {
					for (x = nStartX; x <= nEndX; x++) {
						m = y * m_ChkColMesh[n].ChkBlkArray[i].dwNumX * m_ChkColMesh[n].ChkBlkArray[i].dwNumZ
							+ z * m_ChkColMesh[n].ChkBlkArray[i].dwNumX + x;

						p = m_ChkColMesh[n].ChkBlkArray[i].ppChkFace[m];

						while (p != nullptr)  // リスト構造の最後まで
						{
							// ３角形ポリゴンの値を得る
							vVert[0] = m_ColArray[n].pVert[p->pFace->dwIdx[0]];
							vVert[1] = m_ColArray[n].pVert[p->pFace->dwIdx[1]];
							vVert[2] = m_ColArray[n].pVert[p->pFace->dwIdx[2]];
							vFaceNorm = p->pFace->vNormal;

							// 直線の３角形ポリゴン法線方向の距離を求める
							getDistNormal(vVert, vNow, vOld, vFaceNorm, fNowDist, fOldDist, fLayDist);

							// ３角形平面と移動後点が近接しているかのチェックを行う
							if (checkNear(vVert, vNow, vFaceNorm, fNowDist, fRadius, vInsPt) == 1)
							{
								nRet = 2;   // 近接している

								if (fNearMin > fNowDist)		// より食い込んでいる距離を探す
								{
									fNearMin = fNowDist;
									fNowDistNearSave = fNowDist;
									vFaceNormNearSave = vFaceNorm;

									if (m_bMoveFlag)  // コリジョンマップが移動しているかどうか    // -- 2022.11.14
									{
										// コリジョンマップが移動しているときは、実際の描画位置に戻して出力する
										vHit = XMVector3TransformCoord(vInsPt, m_mWorld);
										vNormal = XMVector3TransformCoord(vFaceNorm, GetRotateMatrix(m_mWorld)); // -- 2022.11.14
										vNormal = normalize(vNormal);
									}
									else {
										vHit = vInsPt;
										vNormal = vFaceNorm;
									}
								}
							}
							else {
								// ３角形ポリゴンと直線（レイ）との接触判定を行う
								if (checkLay(vVert, vNow, vOld, vFaceNorm, fNowDist, fOldDist, fLayDist, vInsPt) == 1)
								{
									nRet = 1;   // 接触している

									float len = magnitude(vOld - vInsPt);
									if (fLenMin > len)		// より近い交点を探す
									{
										fLenMin = len;
										fNowDistSave = fNowDist;
										vFaceNormSave = vFaceNorm;

										if (m_bMoveFlag)  // コリジョンマップが移動しているかどうか    // -- 2022.11.14
										{
											// コリジョンマップが移動しているときは、実際の描画位置に戻して出力する
											vHit = XMVector3TransformCoord(vInsPt, m_mWorld);
											vNormal = XMVector3TransformCoord(vFaceNorm, GetRotateMatrix(m_mWorld)); // -- 2022.11.14
											vNormal = normalize(vNormal);
										}
										else {
											vHit = vInsPt;
											vNormal = vFaceNorm;
										}
									}
								}
							}
							p = p->pNext;     // リスト構造をたどって次のポリゴンを探す
						}
					}
				}
			}
		}
	}

	// 接触判定後の移動処理
	//  0:接触・近接していない  1:接触している  2:近接している
	// （接触・近接しているときは、平面よりfRadiusだけ外側に移動させる）
	if (nRet != 0)
	{
		if (nRet == 1)
		{
			// 接触している
			vMove = vNow + vFaceNormSave * (-fNowDistSave + fRadius);
		}
		else {
			// 近接している
			vMove = vNow + vFaceNormNearSave * (-fNowDistNearSave + fRadius);
		}

		// 判定後の移動処理
		position = vMove;

		nRet = 1;   // 全て接触とする
	}
	return  nRet;

}

//-----------------------------------------------------------------------------
// オブジェクトのレイとメッシュ接触判定用配列との接触判定とスリスリ動かす制御
//																		2024.9.10
//	高低差と重力を考慮した、接触判定と移動を行う
//  先ず①壁の判定（２度行う）を行い、次に②高さ判定、最後に③床判定を行う
//  
//  const VECTOR3& positionOld   オブジェクトの1つ前の位置
//  VECTOR3& position            オブジェクトの現在位置(in/out)
//  float fRadius                オブジェクトの半径（省略値は0.2）
//  
//  戻り値　CollRet  RetFloor
//		エラー		= clError
//		面上を移動	= clMove
//		着地		= clLand
//		落下中		= clFall
//-----------------------------------------------------------------------------
CollRet CCollision::IsCollisionMoveGravity(const VECTOR3& positionOld, VECTOR3& position, float fRadius)
{
	VECTOR3 vHit, vNormal;
	return IsCollisionMoveGravity(positionOld, position, vHit, vNormal, fRadius);
}
//-----------------------------------------------------------------------------
// オブジェクトのレイとメッシュ接触判定用配列との接触判定とスリスリ動かす制御
//																		2024.9.10
//	高低差と重力を考慮した、接触判定と移動を行う
//  先ず①壁の判定（２度行う）を行い、次に②高さ判定、最後に③床判定を行う
//  
//  const VECTOR3& positionOld     オブジェクトの1つ前の位置
//  VECTOR3& position              オブジェクトの現在位置(in/out)
//  VECTOR3 &vHit				   接触点の座標（出力）
//  VECTOR3 &vNormal			   接触点の法線ベクトル（出力）
//  float fRadius                  オブジェクトの半径（省略値は0.2）
//  
//  戻り値　CollRet  RetFloor
//		エラー		= clError
//		面上を移動	= clMove
//		着地		= clLand
//		落下中		= clFall
//-----------------------------------------------------------------------------
CollRet CCollision::IsCollisionMoveGravity(const VECTOR3& positionOld, VECTOR3& position, VECTOR3& vHit, VECTOR3& vNormal, float fRadius)
{
	CollRet  RetFloor = clError;
	int nRetWall = 0;

	VECTOR3 vOld = positionOld;
	VECTOR3 vNow = position;

	// 移動マップの事前処理を行う
	if (m_bMoveFlag)  // コリジョンマップが移動しているかどうか
	{
		// コリジョンマップが移動しているときは、キャラクターが逆に移動していると見なして判定をする
		// このために、キャラクターの位置マトリックスにマップ移動の逆行列を掛けて判定を行う
		vOld = XMVector3TransformCoord(vOld, m_mWorldInv);
		vNow = XMVector3TransformCoord(vNow, m_mWorldInv);
	}
	else {
		// コリジョンマップが移動していないときは、キャラクターの位置をそのまま使用する
	}

	// 高さ判定変数の初期化
	initHeightCheck();

	//	メッシュの壁との接触判定と適切な位置への移動
	nRetWall = checkWallMove(vOld, vNow, vHit, vNormal, fRadius);    // -- 2022.11.14

	if (nRetWall != 0)   // 接触して移動したときは、2度目の接触判定を行う
	{
		checkWallMove(vOld, vNow, vHit, vNormal, fRadius);    // -- 2022.11.14
	}
	
	// 床との接触判定と上下移動
	// なお、UNDERFOOTLIMIT（0.05f)は、床からの高さ
	checkHeight(vOld, vNow, UNDERFOOTLIMIT);	//	現在位置より低い面の中で一番高い面を探す
	RetFloor = checkFloorMove(vOld, vNow);	//	床を判定し床に沿って移動や、着地をさせる。戻り値 clError:-1エラー clMove:面上を移動1 clLand:着地2 clFall:落下中3
	
	// 移動マップの事後処理を行う
	if (m_bMoveFlag)  // コリジョンマップが移動しているかどうか
	{
		// コリジョンマップが移動しているときは、マップ移動の行列を掛けて元の値に戻す
		vNow = XMVector3TransformCoord(vNow, m_mWorld);
		if (nRetWall == 0 && (RetFloor == clMove || RetFloor == clLand))  // 壁に接触していなくて、移動マップに着地しているとき
		{
			// 移動マップの水平移動の処理
			// 水平方向に移動増分だけ移動させる
			VECTOR3 vMM = GetPositionVector(m_mWorld) - GetPositionVector(m_mWorldOld);
			vNow.x += vMM.x;
			vNow.z += vMM.z;
		}
		position = vNow;
	}
	else {
		// コリジョンマップが移動していないときは、そのまま使用する
		position = vNow;
	}

	return RetFloor;
}

//-----------------------------------------------------------------------------
// オブジェクトのレイと壁メッシュとの接触判定とスリスリ動かす制御
//																		2022.11.14
// 壁と判定された三角形ポリゴンのみ接触判定を行う
// 接触判定と適切な位置への移動（ＸＺ方向のみ）を行う
//  
//  const VECTOR3& positionOld   オブジェクトの1つ前の位置
//  VECTOR3& position            オブジェクトの現在位置(in/out)
//  VECTOR3 &vHit				 接触点の座標（出力）
//  VECTOR3 &vNormal			 接触点の法線ベクトル（出力）
//  float fRadius                オブジェクトの半径
//  
//  戻り値　int nRet
//		接触したとき　		１
//		接触していないとき	０
//-----------------------------------------------------------------------------
int CCollision::checkWallMove(const VECTOR3& positionOld, VECTOR3& position, VECTOR3& vHit, VECTOR3& vNormal, float fRadius)
{
	int      nRet = 0;
	int      n, i;
	int      x, y, z, m;
	int      nStartX, nEndX, nStartY, nEndY, nStartZ, nEndZ;
	ChkFace* p;

	// 位置を得る
	VECTOR3  vOld = positionOld;
	VECTOR3  vNow = position;

	VECTOR3  vVert[3], vFaceNorm, vInsPt, vMove;
	float    fNowDist, fOldDist, fLayDist;

	float    fLenMin = 9999999.0f;				// 交点と移動前点との距離の最小値
	float    fNowDistSave;
	VECTOR3  vFaceNormSave;

	float    fNearMin = 9999999.0f;				// 食い込み距離最小値
	float    fNowDistNearSave;
	VECTOR3  vFaceNormNearSave;

	// ジャンプで上昇中かどうか判定する。(現在高さより上昇しているか)	// -- 2019.9.3
	// （誤差を考慮して0.0001fを調整する）
	bool bJumpUp = (vNow.y - 0.0001f > vOld.y);

	// レイとメッシュ（壁）との衝突判定を行う
	for (n = 0; n<m_nNum; n++)
	{
		// 複数分割度配列の検索
		for (i = 0; i < MESHCKTBL_FACE_MAX; i++)
		{
			if (m_ChkColMesh[n].ChkBlkArray[i].ppChkFace == nullptr) break;	// 配列にデータがないとき

			// 配列の対象とする開始点と終了点のブロック番号を求める
			getMeshLimit(n, i, vNow, vOld, fRadius, nStartX, nEndX, nStartY, nEndY, nStartZ, nEndZ);

			// 配列の検索を開始する
			for (y = nStartY; y <= nEndY; y++) {
				for (z = nStartZ; z <= nEndZ; z++) {
					for (x = nStartX; x <= nEndX; x++) {
						m = y * m_ChkColMesh[n].ChkBlkArray[i].dwNumX * m_ChkColMesh[n].ChkBlkArray[i].dwNumZ
							+ z * m_ChkColMesh[n].ChkBlkArray[i].dwNumX + x;

						p = m_ChkColMesh[n].ChkBlkArray[i].ppChkFace[m];

						while (p != nullptr)  // リスト構造の最後まで
						{
							// 壁か床かを判定し壁の場合のみ判定を行う(床の時は読み飛ばす)
							// ただし、ジャンプ上昇中は全て壁と見なす  // -- 2019.9.3
							if (!bJumpUp && (p->pFace->vNormal.y > GROUND || p->pFace->vNormal.y < -GROUND))  // -- 2019.9.3
							{
								;
							}
							else {
								// ３角形ポリゴンの値を得る
								vVert[0] = m_ColArray[n].pVert[p->pFace->dwIdx[0]];
								vVert[1] = m_ColArray[n].pVert[p->pFace->dwIdx[1]];
								vVert[2] = m_ColArray[n].pVert[p->pFace->dwIdx[2]];
								vFaceNorm = p->pFace->vNormal;

								// 直線の３角形ポリゴン法線方向の距離を求める
								getDistNormal(vVert, vNow, vOld, vFaceNorm, fNowDist, fOldDist, fLayDist);

								// ３角形平面と移動後点が近接しているかのチェックを行う
								if (checkNear(vVert, vNow, vFaceNorm, fNowDist, fRadius, vInsPt) == 1)
								{
									nRet = 2;   // 近接している

									if (fNearMin > fNowDist)		// より食い込んでいる距離を探す
									{
										fNearMin = fNowDist;
										fNowDistNearSave = fNowDist;
										vFaceNormNearSave = vFaceNorm;

										if (m_bMoveFlag)  // コリジョンマップが移動しているかどうか    // -- 2022.11.14
										{
											// コリジョンマップが移動しているときは、実際の描画位置に戻して出力する
											vHit = GetPositionVector(XMMatrixTranslation(vInsPt.x, vInsPt.y, vInsPt.z) * m_mWorld);
											vNormal = GetPositionVector(XMMatrixTranslation(vFaceNorm.x, vFaceNorm.y, vFaceNorm.z) * GetRotateMatrix(m_mWorld)); // -- 2022.11.14
											vNormal = normalize(vNormal);
										}
										else {
											vHit = vInsPt;
											vNormal = vFaceNorm;
										}
									}
								}
								else {
									// ３角形ポリゴンと直線（レイ）との接触判定を行う
									if (checkLay(vVert, vNow, vOld, vFaceNorm, fNowDist, fOldDist, fLayDist, vInsPt) == 1)
									{
										nRet = 1;   // 接触している

										float len = magnitude(vOld - vInsPt);
										if (fLenMin > len)		// より近い交点を探す
										{
											fLenMin = len;
											fNowDistSave = fNowDist;
											vFaceNormSave = vFaceNorm;

											if (m_bMoveFlag)  // コリジョンマップが移動しているかどうか    // -- 2022.11.14
											{
												// コリジョンマップが移動しているときは、実際の描画位置に戻して出力する
												vHit = GetPositionVector(XMMatrixTranslation(vInsPt.x, vInsPt.y, vInsPt.z) * m_mWorld);
												vNormal = GetPositionVector(XMMatrixTranslation(vFaceNorm.x, vFaceNorm.y, vFaceNorm.z) * GetRotateMatrix(m_mWorld)); // -- 2022.11.14
												vNormal = normalize(vNormal);
											}
											else {
												vHit = vInsPt;
												vNormal = vFaceNorm;
											}
										}
									}
								}
							}
							p = p->pNext;   // リスト構造をたどって次のポリゴンを探す
						}
					}
				}
			}
		}
	}

	// 接触判定後の移動処理
	//  0:接触・近接していない  1:接触している  2:近接している
	// （接触・近接しているときは、平面よりfRadiusだけ外側に移動させる）
	if (nRet != 0)
	{
		if (nRet == 1)
		{
			// 接触している
			vMove = vNow + vFaceNormSave * (-fNowDistSave + fRadius);
		}
		else {
			// 近接している
			vMove = vNow + vFaceNormNearSave * (-fNowDistNearSave + fRadius);
		}

		// 判定後の移動処理。ＸＺ方向のみ移動し、Ｙ方向には移動させない
		vNow.x = vMove.x;
		vNow.z = vMove.z;
		position = vNow;

		nRet = 1;   // 全て接触とする
	}

	return  nRet;

}


//----------------------------------------------------------------------------
//	高さ判定変数の初期化
//																		2019.8.6
//
//	引数・戻り値　なし
//----------------------------------------------------------------------------
void CCollision::initHeightCheck()
{
	m_vVertexH[0] = VECTOR3(0.0f, 0.0f, 0.0f);
	m_vVertexH[2] = m_vVertexH[1] = m_vVertexH[0];
	m_pIndex = nullptr;
	m_vNormalH = VECTOR3(0.0f, 1.0f, 0.0f);
	m_fHeight = -100000000.0f;    // 最低値
}


//----------------------------------------------------------------------------
//	現在位置より低い面の中で一番高い面を探す
//																			2024.9.10
//
//	＊この関数の目的は、床が交差しているとき、適切な床に着地させるために必要である
//	　実行後、「高さ判定変数」に値が設定される。
//
//  const VECTOR3& positionOld   オブジェクトの1つ前の位置
//  VECTOR3& position            オブジェクトの現在位置(in/out)
//	const float& fObjheight      オブジェクトの中心の高さUNDERFOOTLIMIT
//
//	戻り値 bool bRet
//		true  = より高い面を発見した
//		false = 発見してない
//----------------------------------------------------------------------------
bool CCollision::checkHeight(const VECTOR3& positionOld, VECTOR3& position, const float fObjheight)
{
	bool     bRet = false;
	int      n, i;
	int      x, y, z, m;
	int      nStartX, nEndX, nStartY, nEndY, nStartZ, nEndZ;
	ChkFace* p;
	const    float  fHeightRadius = 1.0f;  // 高さ判定時のキャラクタの半径を最低1.0とする

	// ワールドマトリックスから位置を得る
	VECTOR3  vOld = positionOld;
	VECTOR3  vNow = position;
	VECTOR3  vVert[3], vFaceNorm;

	struct ColFace*  pWIndex = nullptr;
	VECTOR3 vXP(vNow.x, 0, vNow.z);     // 移動後点のY方向ベクトルを０にして平面上の位置を得る
	FLOAT wkHeight = m_fHeight;
	FLOAT MaxY = max(vOld.y, vNow.y);
	FLOAT MinY = min(vOld.y, vNow.y);

	float fRadius = (fObjheight < fHeightRadius) ? fHeightRadius : fObjheight;  // キャラクタの半径を最低fHeightRadiusとする
	
	CAABB     NowAABB;							// 移動後点vNowのAABB
	NowAABB.MakeAABB(vOld, vNow, fRadius);		// 移動後点vNowのAABBを作成する。最低幅半径をfRadiusとする。

	// レイとメッシュ（床）との衝突判定を行う
	for (n = 0; n<m_nNum; n++)
	{
		// 複数分割度配列の検索
		for (i = 0; i < MESHCKTBL_FACE_MAX; i++)
		{
			if (m_ChkColMesh[n].ChkBlkArray[i].ppChkFace == nullptr) break;	// 配列にデータがないとき

			// 配列の対象とする開始点と終了点のブロック番号を求める
			getMeshLimit(n, i, vNow, vOld, fRadius, nStartX, nEndX, nStartY, nEndY, nStartZ, nEndZ);

			// 配列の検索を開始する
			for (y = nStartY; y <= nEndY; y++)
			{
				for (z = nStartZ; z <= nEndZ; z++)
				{
					for (x = nStartX; x <= nEndX; x++)
					{
						m = y * m_ChkColMesh[n].ChkBlkArray[i].dwNumX * m_ChkColMesh[n].ChkBlkArray[i].dwNumZ
							+ z * m_ChkColMesh[n].ChkBlkArray[i].dwNumX + x;

						p = m_ChkColMesh[n].ChkBlkArray[i].ppChkFace[m];

						while (p != nullptr)  // リスト構造の最後まで
						{
							// 壁か床かを判定し、床の場合のみチェックする(壁の時は読み飛ばす)
							if (p->pFace->vNormal.y <= GROUND)
							{
								;
							}
							else {
								if (p->pFace->AABB.HitcheckXZ(NowAABB))  // まず、AABBが平面上で接触しているかのチェックをする
								{
									// ３角形ポリゴンの値を得る
									vVert[0] = m_ColArray[n].pVert[p->pFace->dwIdx[0]];
									vVert[1] = m_ColArray[n].pVert[p->pFace->dwIdx[1]];
									vVert[2] = m_ColArray[n].pVert[p->pFace->dwIdx[2]];
									vFaceNorm = p->pFace->vNormal;

									// Y方向ベクトルを０にして平面上の３角形の頂点位置を得る
									VECTOR3 vX0(vVert[0].x, 0, vVert[0].z);
									VECTOR3 vX1(vVert[1].x, 0, vVert[1].z);
									VECTOR3 vX2(vVert[2].x, 0, vVert[2].z);

									// 移動後点の平面上の位置が三角形ポリゴンの平面上の位置の中にあるかどうかの判定
									//   交点を原点としたとき、３角形ポリゴンの各頂点と原点との角度を合計する関数AddAngleである
									//   この関数の戻り値が３６０°（２π）の時は、接触点が３つの３Ｄベクトルの中にある
									//   ただし、誤差を考慮し２πではなくADDANGLELIMIT(1.99f) * πで判断している。
									if (AddAngle(vX0 - vXP, vX1 - vXP, vX2 - vXP) >= ADDANGLELIMIT * XM_PI)
									{
										// 移動後点が三角形ポリゴンの中にあるとき
										float FaceDist = dot(vFaceNorm, vVert[0]);	// 原点から三角形平面までの距離

										// 移動後点が三角形ポリゴンと交差する点の高さ
										float wh = -(vFaceNorm.x*vXP.x + vFaceNorm.z*vXP.z - FaceDist) / vFaceNorm.y;

										wh += fObjheight;	// 高さにオブジェクトの床からの高さを加味する

										if (wh < MaxY+UPHILLLIMIT && wh > wkHeight)	// 現在の高さより低くて最大の高さであるか？余裕をUPHILLLIMITとる
										{
											wkHeight = wh;
											pWIndex = p->pFace;
											m_vVertexH[0] = vVert[0];
											m_vVertexH[1] = vVert[1];
											m_vVertexH[2] = vVert[2];
										}
									}
								}
							}
							p = p->pNext;   // リスト構造をたどって次のポリゴンを探す
						}
					}
				}
			}
		}
	}

	if (pWIndex != nullptr)
	{
		//	見つかった現在の高さより低くて最大高さの面を記憶する
		m_pIndex = pWIndex;			// 面データのアドレス
		m_vNormalH = m_pIndex->vNormal;
		//m_fHeight = (wkHeight == m_fHeight) ? m_fHeight : wkHeight;  // 最大高さを記憶する
		m_fHeight = wkHeight;  // 最大高さを記憶する  // -- 2019.9.3

		bRet = true;	// 最大高さが見つかった
	}

	return  bRet;
}

//----------------------------------------------------------------------------
//	床を判定し床に沿って移動や、着地をさせる
//																		2019.8.6
//　（注意）この関数は、HeightCheck実行後に呼び出すこと！！
//				
//  const VECTOR3& positionOld   オブジェクトの1つ前の位置
//  VECTOR3& position            オブジェクトの現在位置(in/out)
//				
//  戻り値　CollRet  RetFloor
//		エラー		= clError
//		面上を移動	= clMove
//		着地		= clLand
//		落下中		= clFall
//----------------------------------------------------------------------------
CollRet CCollision::checkFloorMove(const VECTOR3& positionOld, VECTOR3& position)
{
	CollRet  Ret = clError;

	// 位置を得る
	VECTOR3  vOld = positionOld;
	VECTOR3  vNow = position;

	// ジャンプで上昇中かどうか判定する。(現在高さより上昇しているか)
	// （誤差を考慮して0.0001fを調整する）
	bool bJumpUp = (vNow.y - 0.0001f > vOld.y);	

	FLOAT MaxY = max(vOld.y, vNow.y);
	FLOAT MinY = min(vOld.y, vNow.y);

	//vOld = vNow;   // ?????

	if (m_pIndex && m_fHeight <= vNow.y + 0.000001f && m_fHeight >= vNow.y - 0.000001f) {		// 面上を水平に移動中のとき（誤差を考慮し0.000001fを調整する）// -- 2025.1.6
		Ret = clMove;
	}
	else if (!bJumpUp && m_pIndex && m_fHeight >= vNow.y) {
		// ジャンプ上昇中でなくて、床が移動後の高さより高いとき（着地したとき）
		if (m_vNormalH.y > GROUND) {
			// 床に着地した
			// （誤差は考慮しない）
			vNow.y = m_fHeight;
			Ret = clLand;	 // 着地
		}
		else {
			// 壁に着地した
			// （誤差を考慮し0.000001fを調整する）
			vNow.y = m_fHeight + 0.000001f;
			Ret = clFall;	// 落下中
		}
	}
	else {
		if (!bJumpUp && m_pIndex && m_vNormalH.y > GROUND && m_fHeight <= MaxY && m_fHeight >= MinY + LOWFLOORLIMIT)
		{
			// ジャンプ上昇中でなくて、床が移動前と移動後の高さの間にあるとき。または、床が足下LOWFLOORLIMIT(-0.2m)以内の時
			vNow.y = m_fHeight;	// 移動後の高さを床の高さに補正する
			Ret = clLand; 	 // 着地
			//MessageBox(0, _T("CheckFloorMove()   Ret=2 clLand") , nullptr, MB_OK);
		}
		else {
			// ジャンプ上昇中または、落下中のとき
			Ret = clFall;	// 落下中
			//MessageBox(0, _T("CheckFloorMove()   Ret=3 clFall") , nullptr, MB_OK);
		}
	}

	// 床に合わせた上下移動の処理
	if (Ret >= clMove)	 // エラーではないとき
	{
		position = vNow;
	}

	return Ret;
}


//-----------------------------------------------------------------------------
// 移動ベクトルの３角形ポリゴン法線方向の距離を求める関数
//																	2019.9.6
//  
//  const VECTOR3  vVec[3]		三角形ベクトル
//  const VECTOR3& vNow			直線の終点
//  const VECTOR3& vOld			直線の始点
//  const VECTOR3& vFaceNorm	平面の法線
//  float& fNowDist				移動後点と三角形平面との距離（出力）
//  float& fOldDist				移動前点と三角形平面との距離（出力）
//  float& fLayDist				移動ベクトルLayの法線方向の距離（出力）
//  
//  戻り値
//      なし
//-----------------------------------------------------------------------------
void  CCollision::getDistNormal(const VECTOR3 vVec[], const VECTOR3& vNow, const VECTOR3& vOld, const VECTOR3& vFaceNorm, float& fNowDist, float& fOldDist, float& fLayDist)
{
	float   fFaceDist = dot(vFaceNorm, vVec[0]);		// 原点から三角形平面までの距離
	fNowDist = dot(vFaceNorm, vNow) - fFaceDist;		// 移動後点と三角形平面との距離。正の時は表側、負の時は裏側
	fOldDist = dot(vFaceNorm, vOld) - fFaceDist;		// 移動前点と三角形平面との距離。正の時は表側、負の時は裏側
	fLayDist = fOldDist - fNowDist;						// 移動ベクトルLayの法線方向の距離
}

//-----------------------------------------------------------------------------
// レイと３角形ポリゴンとの接触判定
//																	2019.8.6
//  
//  const VECTOR3  vVec[3]		三角形ベクトル
//  const VECTOR3& vNow			直線の終点
//  const VECTOR3& vOld			直線の始点
//  const VECTOR3& vFaceNorm	平面の法線
//  const float& fNowDist		移動後点と三角形平面との距離
//  const float& fOldDist		移動前点と三角形平面との距離
//  const float& fLayDist		移動ベクトルLayの法線方向の距離
//  VECTOR3 &vHit				接触点の座標（出力）
//  
//  戻り値　int
//		接触したとき　		１
//		接触していないとき	０
//-----------------------------------------------------------------------------
int  CCollision::checkLay(const VECTOR3 vVec[], const VECTOR3& vNow, const VECTOR3& vOld, const VECTOR3& vFaceNorm, const float& fNowDist, const float& fOldDist, const float& fLayDist, VECTOR3& vHit)
{
	int nRet = 0;

	if (fNowDist <= 0.0f && fOldDist >= 0.0f)	// 移動前点が三角形平面の表で、移動後点が平面の裏側の時のみ判定
	{
		if (fLayDist != 0.0f)		// 移動ベクトルの法線方向の距離が０のときは、線分と三角形平面が平行なので対象外
		{
			// 平面との交点vInsPtを求める
			//   ①　移動ベクトルの交点から移動後点までの距離の比率を求める　　　　　fNowDist / fLayDist
			//   ②　交点から移動後点までの移動ベクトルを求める　　　　　　　　　　　(vOld - vNow) * fNowDist / fLayDist
			//   ③　上記②を移動後点から引くことによって、交点のベクトルを求める　　vNow - ( (vOld - vNow) * fNowDist / fLayDist)
			VECTOR3 vInsPt = vNow - ((vOld - vNow) * fNowDist / fLayDist);

			// ３角形ポリゴンと接触しているかどうかの判定
			//   交点を原点としたとき、３角形ポリゴンの各頂点と原点との角度を合計する関数AddAngleである
			//   この関数の戻り値が３６０°（２π）の時は、接触点が３つの３Ｄベクトルの中にある
			//   ただし、誤差を考慮し２πではなくADDANGLELIMIT(1.99f) * πで判断している。
			if (AddAngle(vVec[0] - vInsPt, vVec[1] - vInsPt, vVec[2] - vInsPt) >= ADDANGLELIMIT * XM_PI)
			{
				nRet = 1;         // 交点が三角形の内にあるので接触している
				vHit = vInsPt;    // 交点を保存
			}
		}
	}

	return nRet;
}


//-----------------------------------------------------------------------------
// レイと３角形ポリゴンとの近接判定
//																	2019.8.6
//  
//  const VECTOR3  vVec[3]		三角形ベクトル
//  const VECTOR3& vNow			直線の終点
//  const VECTOR3& vFaceNorm	平面の法線
//  const float& fNowDist		移動後点と三角形平面との距離
//  float fRadius				オブジェクトの半径
//  VECTOR3 &vHit				接触点の座標（出力）
//  
//  戻り値　int
//		接触したとき　		１
//		接触していないとき	０
//-----------------------------------------------------------------------------
int  CCollision::checkNear(const VECTOR3 vVec[], const VECTOR3& vNow, const VECTOR3& vFaceNorm, const float& fNowDist, float fRadius, VECTOR3& vHit)
{
	int nRet = 0;

	// 移動後点が平面から+-fRadius以内のときのみ判定
	// ２回目の近接チェック時の誤差を考慮して0.00001f狭めて判定する
	if (fNowDist > -fRadius + 0.00001f && fNowDist < fRadius - 0.00001f)
	{

		// 平面との交点vInsPtを求める
		//   ①　移動後点から平面までの法線方向のベクトルを求める　　　　　　　vFaceNorm * -fNowDist
		//   ②　上記②を移動後点に足すことによって、交点のベクトルを求める　　vNow + vFaceNorm * -fNowDist
		VECTOR3 vInsPt = vNow + vFaceNorm * -fNowDist;

		// ３角形ポリゴンと接触しているかどうかの判定
		//   交点を原点としたとき、３角形ポリゴンの各頂点と原点との角度を合計する関数AddAngleである
		//   この関数の戻り値が３６０°（２π）の時は、接触点が３つの３Ｄベクトルの中にある
		//   ただし、誤差を考慮し２πではなくADDANGLELIMIT(1.99f) * πで判断している。
		if (AddAngle(vVec[0] - vInsPt, vVec[1] - vInsPt, vVec[2] - vInsPt) >= ADDANGLELIMIT * XM_PI)
		{
			nRet = 1;         // 交点が三角形の内にあるので接触している
			vHit = vInsPt;    // 交点を保存
		}
	}

	return nRet;
}

//------------------------------------------------------------------------
//	ワールドマトリックスを初期化する
//																			2022.11.14
//  引数
//      const MATRIX4X4& mWorld
//
//	戻り値 
//------------------------------------------------------------------------
void CCollision::InitWorldMatrix(const MATRIX4X4& mWorld)
{
	m_mWorldOld  = mWorld;
	m_mWorld     = mWorld;
	m_mWorldInv  = XMMatrixInverse(nullptr, m_mWorld);
	m_bMoveFlag  = true;
}

//------------------------------------------------------------------------
//	ワールドマトリックスを設定する
//																			2019.8.6
//  引数
//      const MATRIX4X4& mWorld
//
//	戻り値 
//------------------------------------------------------------------------
void CCollision::SetWorldMatrix(const MATRIX4X4& mWorld)
{
	m_mWorld = mWorld;
	m_mWorldInv = XMMatrixInverse(nullptr, m_mWorld);
	m_bMoveFlag = true;
}


//------------------------------------------------------------------------  // -- 2020.12.3
//	複数分割度マップからコリジョンマップ全体を囲むAABBを求める
//		
//	・この処理はCollision.cppの中では使用しない	
//	　外部でコリジョンマップの大きさを知りたいときに使用する
//		
//  引数
//      VECTOR3& vMin  バウンディングボックスの最小値(Out)
//      VECTOR3& vMax  バウンディングボックスの最大値(Out)
//
//------------------------------------------------------------------------
void CCollision::GetChkAABB(VECTOR3& vMin, VECTOR3& vMax)
{
	if (m_nNum == 0) return;

	vMin = m_ChkColMesh[0].vMin;
	vMax = m_ChkColMesh[0].vMax;

	for (int i = 1; i < m_nNum; i++)
	{
		if (vMin.x > m_ChkColMesh[i].vMin.x) vMin.x = m_ChkColMesh[i].vMin.x;
		if (vMin.y > m_ChkColMesh[i].vMin.y) vMin.y = m_ChkColMesh[i].vMin.y;
		if (vMin.z > m_ChkColMesh[i].vMin.z) vMin.z = m_ChkColMesh[i].vMin.z;
		if (vMax.x < m_ChkColMesh[i].vMax.x) vMax.x = m_ChkColMesh[i].vMax.x;
		if (vMax.y < m_ChkColMesh[i].vMax.y) vMax.y = m_ChkColMesh[i].vMax.y;
		if (vMax.z < m_ChkColMesh[i].vMax.z) vMax.z = m_ChkColMesh[i].vMax.z;
	}
}


//------------------------------------------------------------------------  // 2019.8.6
//
// AABB（軸並行境界ボックス：Axis - Aligned Bounding Box）
//
//------------------------------------------------------------------------

//------------------------------------------------------------------------
//
// 三角ポリゴンを囲むAABBを作成する
//
//------------------------------------------------------------------------
void CAABB::MakeAABB(const VECTOR3& v1, const VECTOR3& v2, const VECTOR3& v3)
{
	if (v1.x > v2.x) {
		m_vMin.x = v2.x;
	}
	else {
		m_vMin.x = v1.x;
	}
	if (m_vMin.x > v3.x) m_vMin.x = v3.x;

	if (v1.y > v2.y) {
		m_vMin.y = v2.y;
	}
	else {
		m_vMin.y = v1.y;
	}
	if (m_vMin.y > v3.y) m_vMin.y = v3.y;

	if (v1.z > v2.z) {
		m_vMin.z = v2.z;
	}
	else {
		m_vMin.z = v1.z;
	}
	if (m_vMin.z > v3.z) m_vMin.z = v3.z;


	if (v1.x < v2.x) {
		m_vMax.x = v2.x;
	}
	else {
		m_vMax.x = v1.x;
	}
	if (m_vMax.x < v3.x) m_vMax.x = v3.x;

	if (v1.y < v2.y) {
		m_vMax.y = v2.y;
	}
	else {
		m_vMax.y = v1.y;
	}
	if (m_vMax.y < v3.y) m_vMax.y = v3.y;

	if (v1.z < v2.z) {
		m_vMax.z = v2.z;
	}
	else {
		m_vMax.z = v1.z;
	}
	if (m_vMax.z < v3.z) m_vMax.z = v3.z;

}
//------------------------------------------------------------------------
//
// v1～v2直線の長さで、半径rの幅のAABBを作成する
//
//------------------------------------------------------------------------
void CAABB::MakeAABB(const VECTOR3& v1, const VECTOR3& v2, const FLOAT& r)
{
	if (v1.x > v2.x) {
		m_vMin.x = v2.x;
	}
	else {
		m_vMin.x = v1.x;
	}
	m_vMin.x -= r;

	if (v1.y > v2.y) {
		m_vMin.y = v2.y;
	}
	else {
		m_vMin.y = v1.y;
	}
	m_vMin.y -= r;

	if (v1.z > v2.z) {
		m_vMin.z = v2.z;
	}
	else {
		m_vMin.z = v1.z;
	}
	m_vMin.z -= r;

	if (v1.x < v2.x) {
		m_vMax.x = v2.x;
	}
	else {
		m_vMax.x = v1.x;
	}
	m_vMax.x += r;

	if (v1.y < v2.y) {
		m_vMax.y = v2.y;
	}
	else {
		m_vMax.y = v1.y;
	}
	m_vMax.y += r;

	if (v1.z < v2.z) {
		m_vMax.z = v2.z;
	}
	else {
		m_vMax.z = v1.z;
	}
	m_vMax.z += r;
}
//------------------------------------------------------------------------
//
// ２つのAABBの接触判定を行う
//
// 引数
//    AABB other
//
// 戻り値
//    true:接触している    false:接触していない 
//
//------------------------------------------------------------------------
bool CAABB::Hitcheck(const CAABB& other)
{
	if (m_vMax.x < other.m_vMin.x) return false;
	if (m_vMax.y < other.m_vMin.y) return false;
	if (m_vMax.z < other.m_vMin.z) return false;
	if (m_vMin.x > other.m_vMax.x) return false;
	if (m_vMin.y > other.m_vMax.y) return false;
	if (m_vMin.z > other.m_vMax.z) return false;

	return true;
}
//------------------------------------------------------------------------
//
// ２つのAABBのＸＺ方向のみ（水平平面）接触判定を行う
//
// 引数
//    AABB other
//
// 戻り値
//    true:接触している    false:接触していない 
//
//------------------------------------------------------------------------
bool CAABB::HitcheckXZ(const CAABB& other)
{
	if (m_vMax.x < other.m_vMin.x) return false;
	if (m_vMax.z < other.m_vMin.z) return false;
	if (m_vMin.x > other.m_vMax.x) return false;
	if (m_vMin.z > other.m_vMax.z) return false;

	return true;
}
//------------------------------------------------------------------------
//
// AABBの8頂点の引数の添字位置の座標値を取得する
//
// 引数
//    int nIdx   0～7の整数値
//
// 戻り値
//    VECTOR3  座標値
//
//------------------------------------------------------------------------
VECTOR3 CAABB::GetVecPos(const int& nIdx)
{
	VECTOR3 vPos;

	switch (nIdx)
	{
	case 0:
		vPos = m_vMin;
		break;
	case 1:
		vPos = VECTOR3(m_vMin.x, m_vMin.y, m_vMax.z);
		break;
	case 2:
		vPos = VECTOR3(m_vMin.x, m_vMax.y, m_vMax.z);
		break;
	case 3:
		vPos = VECTOR3(m_vMin.x, m_vMax.y, m_vMin.z);
		break;
	case 4:
		vPos = VECTOR3(m_vMax.x, m_vMin.y, m_vMin.z);
		break;
	case 5:
		vPos = VECTOR3(m_vMax.x, m_vMin.y, m_vMax.z);
		break;
	case 6:
		vPos = VECTOR3(m_vMax.x, m_vMax.y, m_vMin.z);
		break;
	case 7:
		vPos = m_vMax;
		break;
	}
	return vPos;
}
