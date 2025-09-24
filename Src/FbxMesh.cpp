//=============================================================================
//		メッシュの読み込みと描画のプログラム
//　                                                  ver 4.0        2024.10.7
//
//		メッシュ処理
//
//      (メッシュコントロールクラスでテクスチャの総合管理を行う)
//
//																	FbxMesh.cpp
//=============================================================================
#include "FbxMesh.h"
#include "GameMain.h"
#include "Animator.h"

//------------------------------------------------------------------------
//
//	CFbxMesh  コンストラクタ	
//
//------------------------------------------------------------------------
CFbxMesh::CFbxMesh() : CFbxMesh(GameDevice()->m_pFbxMeshCtrl)
{
}
CFbxMesh::CFbxMesh(CFbxMeshCtrl* pFbxMeshCtrl)
{
	ZeroMemory(this, sizeof(this));
	m_pD3D = pFbxMeshCtrl->m_pD3D;
	m_pShader = pFbxMeshCtrl->m_pShader;
	m_pFbxMeshCtrl = pFbxMeshCtrl;                    // -- 2021.2.4

	m_fHeightMax = 0.01f;                // ディスプレースメントマッピングの高さ     // -- 2020.1.24
	m_iMaxDevide = 1;                   // ディスプレースメントマッピングの分割数   // -- 2020.1.24
	m_vDiffuse = VECTOR4(1, 1, 1, 1);    // ディフューズ色                           // -- 2020.1.24
}
CFbxMesh::CFbxMesh(CFbxMeshCtrl* pFbxMeshCtrl, const TCHAR* FName)
{
	ZeroMemory(this, sizeof(this));
	m_pD3D = pFbxMeshCtrl->m_pD3D;
	m_pShader = pFbxMeshCtrl->m_pShader;
	m_pFbxMeshCtrl = pFbxMeshCtrl;                    // -- 2021.2.4
	Load(FName);

	m_fHeightMax = 0.01f;                // ディスプレースメントマッピングの高さ     // -- 2020.1.24
	m_iMaxDevide = 1;                   // ディスプレースメントマッピングの分割数   // -- 2020.1.24
	m_vDiffuse = VECTOR4(1, 1, 1, 1);  // ディフューズ色                           // -- 2020.1.24
}
//------------------------------------------------------------------------
//
//	CFbxMesh  デストラクタ	
//
//------------------------------------------------------------------------
CFbxMesh::~CFbxMesh()
{
	DestroyD3D();
}
//------------------------------------------------------------------------
//
//	メッシュの終了処理
//
// 引数　なし
//
//	戻り値 なし
//
//------------------------------------------------------------------------
void CFbxMesh::DestroyD3D()
{
	for (DWORD i = 0; i < m_dwMeshNum; i++)
	{
		SAFE_RELEASE(m_pMeshArray[i].m_pIndexBuffer);
		SAFE_RELEASE(m_pMeshArray[i].m_pVertexBuffer);
		SAFE_DELETE_ARRAY(m_pMeshArray[i].m_vStaticVerticesNormal);
		SAFE_DELETE_ARRAY(m_pMeshArray[i].m_vSkinVerticesNormal);
		SAFE_DELETE_ARRAY(m_pMeshArray[i].m_nIndices);
		for (int j = 0; j < ANIMATION_MAX; j++)
		{
			SAFE_DELETE_ARRAY(m_pMeshArray[i].m_pBoneShader[j]);
		}
	}
	SAFE_DELETE_ARRAY(m_pMeshArray);
	for (DWORD i = 0; i < m_dwTextureNum; i++)
	{
		m_pFbxMeshCtrl->DeleteTextureList(m_pTextureArray[i]);                   // -- 2021.2.4
		m_pFbxMeshCtrl->DeleteTextureList(m_pTextureNormalArray[i]);
		m_pFbxMeshCtrl->DeleteTextureList(m_pTextureHeightArray[i]);
		m_pFbxMeshCtrl->DeleteTextureList(m_pTextureSpecularArray[i]);
	}
	SAFE_DELETE_ARRAY(m_pMaterialDiffuseArray);      // -- 2020.12.15
	SAFE_DELETE_ARRAY(m_pMaterialSpecularArray);     // -- 2020.12.15

	SAFE_DELETE_ARRAY(m_pTextureArray);
	SAFE_DELETE_ARRAY(m_pTextureNormalArray);
	SAFE_DELETE_ARRAY(m_pTextureHeightArray);
	SAFE_DELETE_ARRAY(m_pTextureSpecularArray);

	SAFE_DELETE_ARRAY(m_dwRenderIdxArray);
	SAFE_DELETE_ARRAY(m_fRenderDistanceArray);
}

//------------------------------------------------------------------------
//
//	メッシュ(バイナリ)ファイルの読み込み処理
//
// 引数
//  const TCHAR*         FName          Fbxファイル名
//
//	戻り値 bool
//         true:正常   false:異常
//
//------------------------------------------------------------------------
bool CFbxMesh::Load(const TCHAR* FName)
{
	timeBeginPeriod(1);
	DWORD startTime = timeGetTime();

	HANDLE	hFile;
	DWORD	dwLen, dwBufSize;
	TCHAR  PathName[256], PathName2[256], FullName[256];
	BYTE  *pBuf = nullptr;
	BYTE  *p;
	int   mi, ti, tnum, n;
	int   xi;                              // -- 2021.2.4

	StaticVertex*     staticvertices = nullptr;  // スタティックメッシュ頂点データ
	SkinVertex*       skinvertices = nullptr;    // スキンメッシュ頂点データ

	// メッシュファイル名からパス名を得てPathNameに格納しておく
	// （後で、テクスチャファイル名のパスとして使うため）
	_tsplitpath_s(FName, PathName, sizeof(PathName) / sizeof(TCHAR), PathName2, sizeof(PathName2) / sizeof(TCHAR),
		NULL, NULL, NULL, NULL);
	_tcscat_s(PathName, PathName2);

	// ファイルのオープン処理
	hFile = CreateFile(FName, GENERIC_READ,
		FILE_SHARE_READ, nullptr, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, nullptr);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		// 最大値、最小値の初期設定                      // -- 2021.2.4
		m_vMax = VECTOR3(-999999, -999999, -999999);
		m_vMin = VECTOR3(999999, 999999, 999999);

		dwBufSize = GetFileSize(hFile, nullptr);     // 入力ファイルのサイズを得る。
		pBuf = new BYTE[dwBufSize];				  // 入力バッファを確保する 

		ReadFile(hFile, pBuf, dwBufSize, &dwLen, nullptr);		// ファイルをバッファに読み込む

		p = pBuf;	// バッファの先頭ポインタをセット

		// -----------------------------------------------------------------
		// ファイルヘッダ(WCHARの8文字)のチェック
		WCHAR Head[8];
		memcpy_s(Head, sizeof(Head), p, sizeof(Head));
		if (Head[0] == L'M' && Head[1] == L'E' && Head[2] == L'S' && Head[3] == L'H')
		{
			;
		}
		else {
			MessageBox(nullptr, FName, _T("■□■ メッシュファイル('MESH')ではありません ■□■"), MB_OK);
			return false;
		}
		// バージョンのチェックは行わない	 		// -- 2024.3.23
		if (Head[4] < L'2')
		{
			;
		}

		// メッシュタイプのチェック
		if (Head[6] == L'1')
		{
			// スタティックメッシュ
			m_nMeshType = 1;
		}
		else if (Head[6] == L'2')
		{
			// スキンメッシュ
			m_nMeshType = 2;
		}
		else {
			m_nMeshType = 0;
			MessageBox(nullptr, FName, _T("■□■ メッシュタイプが異なります（1,2以外不可） ■□■"), MB_OK);
			return false;
		}
		p += sizeof(Head);

		// -----------------------------------------------------------------------------------
		// テクスチャーに関する処理
		memcpy_s(&m_dwTextureNum, sizeof(DWORD), p, sizeof(int));         // テクスチャー数を得る

		m_pMaterialDiffuseArray = new VECTOR4[m_dwTextureNum];            // マテリアルディフューズカラー配列     // -- 2020.12.15
		m_pMaterialSpecularArray = new VECTOR4[m_dwTextureNum];           // マテリアルスペキュラ－カラー配列     // -- 2020.12.15
		m_pTextureArray = new ID3D11ShaderResourceView*[m_dwTextureNum];  // テクスチャー配列を生成する
		m_pTextureNormalArray = new ID3D11ShaderResourceView*[m_dwTextureNum];  // テクスチャーNormal配列を生成する
		m_pTextureHeightArray = new ID3D11ShaderResourceView*[m_dwTextureNum];  // テクスチャーHeight配列を生成する   // -- 2020.1.15
		m_pTextureSpecularArray = new ID3D11ShaderResourceView*[m_dwTextureNum];  // テクスチャーSpecular配列を生成する
		p += sizeof(int);

		for (ti = 0; ti < m_dwTextureNum; ti++)   // テクスチャー数だけ繰り返し
		{
			WCHAR WName[128];
			TCHAR TTName[256] = { 0x00 };   // マルチバイトのときは、変換後に文字列最後の\0が作成されないのでまず受取側をゼロクリヤーしておく;

			memcpy_s(WName, sizeof(WName), p, sizeof(WCHAR) * 128);	// テクスチャー名(WCHAR型)を得る

			if (WName[0] == _T('\0'))     // -- 2020.12.15
			{
				// マテリアルカラーを使用するとき
#if _UNICODE
				SetMaterial(ti, WName);   // -- 2020.12.15
#else
				WideCharToMultiByte(CP_ACP, 0, WName, -1, TTName, sizeof(TTName), nullptr, nullptr);
				SetMaterial(ti, TTName); // マルチバイトに対応しました
#endif
			}
			else {
				// テクスチャを使用するとき

#if _UNICODE
				// unicodeの場合、WName(WCHAR)をそのままTTNameにコピー
				_tcscpy_s(TTName, WName);
#else
				// マルチバイト文字指定の場合、WName(WCHAR)をマルチバイトに変換してコピー
				// Unicode 文字コード(WName)を第一引数で指定した文字コードに変換する( CP_ACP は日本語WindowsではシフトJISコード )
				WideCharToMultiByte(CP_ACP, 0, WName, -1, TTName, sizeof(TTName), nullptr, nullptr);
#endif
				_tcscpy_s(FullName, 256, PathName);
				_tcscat_s(FullName, TTName);		// パス名の後ろにテクスチャー名を連結する
				SetTexture(ti, FullName);			// テクスチャーをテクスチャー配列にセットする
			}

			p += sizeof(WCHAR) * 128;
		}

		// -------------------------------------------------------------------------------------
		// メッシュに関する処理
		// メッシュカウントデータ
		memcpy_s(&m_dwMeshNum, sizeof(DWORD), p, sizeof(int));       // メッシュ数を得る
		m_pMeshArray = new CFbxMeshArray[m_dwMeshNum];               // メッシュ配列を生成する
		p += sizeof(int);
		for (mi = 0; mi < m_dwMeshNum; mi++)   // メッシュ数だけ繰り返し
		{
			// メッシュ名を得る
			WCHAR TMeshName[128];
			memcpy_s(TMeshName, sizeof(TMeshName), p, sizeof(WCHAR) * 128);	// メッシュ名(WCHAR型)を得る
																			//  現在、メッシュ名は使用していない
			p += sizeof(WCHAR) * 128;

			// メッシュタイプごとの処理
			if (m_nMeshType == 1)     // スタティックメッシュ
			{
				// 頂点数データ
				memcpy_s(&m_pMeshArray[mi].m_dwVerticesNum, sizeof(DWORD), p, sizeof(int));       // 頂点数を得る
				staticvertices = new StaticVertex[m_pMeshArray[mi].m_dwVerticesNum];  // 頂点配列を生成する
				m_pMeshArray[mi].m_vStaticVerticesNormal = new StaticVertexNormal[m_pMeshArray[mi].m_dwVerticesNum];  // 頂点配列バンプマップ用を生成する
				p += sizeof(int);

				// 頂点データ
				memcpy_s(staticvertices, sizeof(StaticVertex)*m_pMeshArray[mi].m_dwVerticesNum, p, sizeof(StaticVertex)*m_pMeshArray[mi].m_dwVerticesNum);       // 頂点配列に頂点を得る
				p += sizeof(StaticVertex)*m_pMeshArray[mi].m_dwVerticesNum;

				// 頂点の最大値と最小値を求める                             // -- 2021.2.4
				for (xi = 0; xi < m_pMeshArray[mi].m_dwVerticesNum; xi++)
				{
					if (m_vMax.x < (staticvertices + xi)->Pos.x) m_vMax.x = (staticvertices + xi)->Pos.x;
					if (m_vMax.y < (staticvertices + xi)->Pos.y) m_vMax.y = (staticvertices + xi)->Pos.y;
					if (m_vMax.z < (staticvertices + xi)->Pos.z) m_vMax.z = (staticvertices + xi)->Pos.z;
					if (m_vMin.x > (staticvertices + xi)->Pos.x) m_vMin.x = (staticvertices + xi)->Pos.x;
					if (m_vMin.y > (staticvertices + xi)->Pos.y) m_vMin.y = (staticvertices + xi)->Pos.y;
					if (m_vMin.z > (staticvertices + xi)->Pos.z) m_vMin.z = (staticvertices + xi)->Pos.z;
				}

				// インデックス数データ
				memcpy_s(&m_pMeshArray[mi].m_dwIndicesNum, sizeof(DWORD), p, sizeof(int));       // インデックス数を得る
				m_pMeshArray[mi].m_nIndices = new DWORD[m_pMeshArray[mi].m_dwIndicesNum];      // インデックス配列を生成する
				p += sizeof(int);

				memcpy_s(m_pMeshArray[mi].m_nIndices, sizeof(DWORD)*m_pMeshArray[mi].m_dwIndicesNum, p, sizeof(DWORD)*m_pMeshArray[mi].m_dwIndicesNum);  // インデックス配列にインデックスを得る
				if (Head[4] < L'2') ConvIndicesData(mi);	// 旧バージョンのときは右回り表に変換する   // -- 2024.3.23
				p += sizeof(DWORD)*m_pMeshArray[mi].m_dwIndicesNum;

				// 頂点のレイアウトを変換し、接線と従法線を計算
				ChangeStaticVertexLayout(staticvertices, m_pMeshArray[mi].m_nIndices, m_pMeshArray[mi].m_dwIndicesNum, m_pMeshArray[mi].m_vStaticVerticesNormal);

				// バーテックスバッファとインデックスバッファをセット
				SetStaticVIBuffer(mi, m_pMeshArray[mi].m_vStaticVerticesNormal, m_pMeshArray[mi].m_nIndices);

				// 一つのメッシュのテクスチャー数（現バージョンでは必ず１のはず）
				memcpy_s(&tnum, sizeof(DWORD), p, sizeof(int));       // テクスチャー数
				p += sizeof(int);
				// テクスチャー番号のテクスチャを設定する
				for (int i = 0; i < tnum; i++)
				{
					memcpy_s(&n, sizeof(DWORD), p, sizeof(int));       // テクスチャー番号を得る
					m_pMeshArray[mi].m_pTexture = m_pTextureArray[n];   // テクスチャー配列のアドレスをセット
					m_pMeshArray[mi].m_pTextureNormal = m_pTextureNormalArray[n];   // テクスチャーNormal配列のアドレスをセット
					m_pMeshArray[mi].m_pTextureHeight = m_pTextureHeightArray[n];   // テクスチャーHeight配列のアドレスをセット
					m_pMeshArray[mi].m_pTextureSpecular = m_pTextureSpecularArray[n];   // テクスチャーSpecular配列のアドレスをセット
					m_pMeshArray[mi].m_pMaterialDiffuse = m_pMaterialDiffuseArray[n];   // マテリアル・ディフューズ色   // -- 2020.12.15
					m_pMeshArray[mi].m_pMaterialSpecular = m_pMaterialSpecularArray[n]; // マテリアル・スペキュラー色   // -- 2020.12.15
					p += sizeof(int);
				}

				// メッシュの中心点の座標を求める                                      // -- 2018.8.1
				m_pMeshArray[mi].m_vCenterPos = GetStaticCenterPos(staticvertices, m_pMeshArray[mi].m_dwVerticesNum);

				// 不要になった一時　頂点配列を削除する
				SAFE_DELETE_ARRAY(staticvertices);

			}
			else if (m_nMeshType == 2)    // スキンメッシュ
			{

				// 頂点数データ
				memcpy_s(&m_pMeshArray[mi].m_dwVerticesNum, sizeof(DWORD), p, sizeof(int));       // 頂点数を得る
				skinvertices = new SkinVertex[m_pMeshArray[mi].m_dwVerticesNum];  // 頂点配列を生成する
				m_pMeshArray[mi].m_vSkinVerticesNormal = new SkinVertexNormal[m_pMeshArray[mi].m_dwVerticesNum];  // 頂点配列バンプマップ用を生成する
				p += sizeof(int);

				// 頂点データ
				memcpy_s(skinvertices, sizeof(SkinVertex)*m_pMeshArray[mi].m_dwVerticesNum, p, sizeof(SkinVertex)*m_pMeshArray[mi].m_dwVerticesNum);       // 頂点配列に頂点を得る
				p += sizeof(SkinVertex)*m_pMeshArray[mi].m_dwVerticesNum;

				// 頂点の最大値と最小値を求める                             // -- 2021.2.4
				for (xi = 0; xi < m_pMeshArray[mi].m_dwVerticesNum; xi++)
				{
					if (m_vMax.x < (skinvertices + xi)->Pos.x) m_vMax.x = (skinvertices + xi)->Pos.x;
					if (m_vMax.y < (skinvertices + xi)->Pos.y) m_vMax.y = (skinvertices + xi)->Pos.y;
					if (m_vMax.z < (skinvertices + xi)->Pos.z) m_vMax.z = (skinvertices + xi)->Pos.z;
					if (m_vMin.x > (skinvertices + xi)->Pos.x) m_vMin.x = (skinvertices + xi)->Pos.x;
					if (m_vMin.y > (skinvertices + xi)->Pos.y) m_vMin.y = (skinvertices + xi)->Pos.y;
					if (m_vMin.z > (skinvertices + xi)->Pos.z) m_vMin.z = (skinvertices + xi)->Pos.z;
				}

				// インデックス数データ
				memcpy_s(&m_pMeshArray[mi].m_dwIndicesNum, sizeof(DWORD), p, sizeof(int));       // インデックス数を得る
				m_pMeshArray[mi].m_nIndices = new DWORD[m_pMeshArray[mi].m_dwIndicesNum];      // インデックス配列を生成する
				p += sizeof(int);

				memcpy_s(m_pMeshArray[mi].m_nIndices, sizeof(DWORD)*m_pMeshArray[mi].m_dwIndicesNum, p, sizeof(DWORD)*m_pMeshArray[mi].m_dwIndicesNum);  // インデックス配列にインデックスを得る
				if (Head[4] < L'2') ConvIndicesData(mi);	// 旧バージョンのときは右回り表に変換する   // -- 2024.3.23
				p += sizeof(DWORD)*m_pMeshArray[mi].m_dwIndicesNum;

				// 頂点のレイアウトを変換し、接線と従法線を計算
				ChangeSkinVertexLayout(skinvertices, m_pMeshArray[mi].m_nIndices, m_pMeshArray[mi].m_dwIndicesNum, m_pMeshArray[mi].m_vSkinVerticesNormal);

				// バーテックスバッファとインデックスバッファをセット
				SetSkinVIBuffer(mi, m_pMeshArray[mi].m_vSkinVerticesNormal, m_pMeshArray[mi].m_nIndices);

				// 一つのメッシュのテクスチャー数（現バージョンでは必ず１のはず）
				memcpy_s(&tnum, sizeof(DWORD), p, sizeof(int));       // テクスチャー数
				p += sizeof(int);
				// テクスチャー番号のテクスチャを設定する
				for (int i = 0; i < tnum; i++)
				{
					memcpy_s(&n, sizeof(DWORD), p, sizeof(int));       // テクスチャー番号を得る
					m_pMeshArray[mi].m_pTexture = m_pTextureArray[n];   // テクスチャー配列のアドレスをセット
					m_pMeshArray[mi].m_pTextureNormal = m_pTextureNormalArray[n];   // ノーマルテクスチャー配列のアドレスをセット
					m_pMeshArray[mi].m_pTextureHeight = m_pTextureHeightArray[n];   // ハイトテクスチャー配列のアドレスをセット
					m_pMeshArray[mi].m_pTextureSpecular = m_pTextureSpecularArray[n];   // Specularテクスチャー配列のアドレスをセット
					m_pMeshArray[mi].m_pMaterialDiffuse = m_pMaterialDiffuseArray[n];    // マテリアル・ディフューズ色  // -- 2020.12.15
					m_pMeshArray[mi].m_pMaterialSpecular = m_pMaterialSpecularArray[n];  // マテリアル・スペキュラー色  // -- 2020.12.15
					p += sizeof(int);
				}

				// メッシュの中心点の座標を求める                                      // -- 2018.8.1
				m_pMeshArray[mi].m_vCenterPos = GetSkinCenterPos(skinvertices, m_pMeshArray[mi].m_dwVerticesNum);

				// 不要になった一時　頂点配列、インデックス配列を削除する
				SAFE_DELETE_ARRAY(skinvertices);

			}
		}

		// レンダリング順の添字配列の領域を確保          // -- 2018.8.1
		m_dwRenderIdxArray = new DWORD[m_dwMeshNum];
		// レンダリング順を決めるための距離配列の領域を確保  // -- 2018.8.1
		m_fRenderDistanceArray = new float[m_dwMeshNum];

		// ----------------------------------------------------------------------------------------------
		CloseHandle(hFile);
		SAFE_DELETE_ARRAY(pBuf);  // 入力バッファを削除する

		m_dwLoadTime = timeGetTime() - startTime;
		timeEndPeriod(1);

		return true;

	}
	else {
		MessageBox(nullptr, FName, _T("■□■ メッシュファイル(.mesh)がありません ■□■"), MB_OK);
	}

	return false;
}

//------------------------------------------------------------------------  // -- 2024.3.23
//	 バージョンが2.0.0以前のときはポリゴンの左回りが表になっているので
//   右回り表に変更する
//------------------------------------------------------------------------
void CFbxMesh::ConvIndicesData(int mi)
{
	for (int i = 0; i < m_pMeshArray[mi].m_dwIndicesNum; i += 3)
	{
		int w;
		w = m_pMeshArray[mi].m_nIndices[i + 1];
		m_pMeshArray[mi].m_nIndices[i + 1] = m_pMeshArray[mi].m_nIndices[i + 2];
		m_pMeshArray[mi].m_nIndices[i + 2] = w;
	}
}

//------------------------------------------------------------------------
//
// スタティックメッシュの中心点の座標を求める関数
// 
// 引数
//   const StaticVertex*  vertex   頂点配列;
//   const DWORD&         Num      頂点数;
// 
// 戻り値
//   VECTOR3  メッシュの中心点の座標
// 
//------------------------------------------------------------------------
VECTOR3 CFbxMesh::GetStaticCenterPos(const StaticVertex* vertex, const DWORD& Num)
{
	VECTOR3 vMax = VECTOR3(-999999, -999999, -999999);
	VECTOR3 vMin = VECTOR3(999999, 999999, 999999);
	VECTOR3 vCenter = VECTOR3(0, 0, 0);

	for (DWORD i = 0; i < Num; i++)
	{
		if (vMax.x < vertex[i].Pos.x) vMax.x = vertex[i].Pos.x;
		if (vMax.y < vertex[i].Pos.y) vMax.y = vertex[i].Pos.y;
		if (vMax.z < vertex[i].Pos.z) vMax.z = vertex[i].Pos.z;
		if (vMin.x > vertex[i].Pos.x) vMin.x = vertex[i].Pos.x;
		if (vMin.y > vertex[i].Pos.y) vMin.y = vertex[i].Pos.y;
		if (vMin.z > vertex[i].Pos.z) vMin.z = vertex[i].Pos.z;
	}

	vCenter.x = (vMax.x + vMin.x) / 2;
	vCenter.y = (vMax.y + vMin.y) / 2;
	vCenter.z = (vMax.z + vMin.z) / 2;

	return vCenter;
}

//------------------------------------------------------------------------
//
// スタティックメッシュ　頂点のレイアウトをノーマルマップ用に変換し、接線と従法線を計算し追加する
// 
// 引数
//   const StaticVertex*       vertices             頂点配列
//   const DWORD*              indices              インデックス配列
//   const DWORD&              IndicesNum           インデックス数
//   StaticVertexNormal*       verticesNormal(OUT)  ノーマルマップ用頂点配列（出力）
// 
// 戻り値
//   bool  true
// 
//------------------------------------------------------------------------
bool CFbxMesh::ChangeStaticVertexLayout(const StaticVertex* vertices, const DWORD* indices, const DWORD& IndicesNum, StaticVertexNormal* verticesNormal)
{

	// ３角形ポリゴン毎に、全てのポリゴンの処理を行う
	for (int i = 0; i < IndicesNum / 3; i++) {
		// 頂点、法線、テクスチャ座標をノーマルマップ用の頂点レイアウトにコピーする
		verticesNormal[indices[i * 3]].Pos = vertices[indices[i * 3]].Pos;
		verticesNormal[indices[i * 3 + 1]].Pos = vertices[indices[i * 3 + 1]].Pos;
		verticesNormal[indices[i * 3 + 2]].Pos = vertices[indices[i * 3 + 2]].Pos;
		verticesNormal[indices[i * 3]].Normal = vertices[indices[i * 3]].Normal;
		verticesNormal[indices[i * 3 + 1]].Normal = vertices[indices[i * 3 + 1]].Normal;
		verticesNormal[indices[i * 3 + 2]].Normal = vertices[indices[i * 3 + 2]].Normal;
		verticesNormal[indices[i * 3]].vTex = vertices[indices[i * 3]].vTex;
		verticesNormal[indices[i * 3 + 1]].vTex = vertices[indices[i * 3 + 1]].vTex;
		verticesNormal[indices[i * 3 + 2]].vTex = vertices[indices[i * 3 + 2]].vTex;

		//	頂点3つとUV座標３つをもとに接線と従法線を計算する
		CalcTangentSub(verticesNormal[indices[i * 3]].Pos, verticesNormal[indices[i * 3 + 1]].Pos, verticesNormal[indices[i * 3 + 2]].Pos,
			verticesNormal[indices[i * 3]].vTex, verticesNormal[indices[i * 3 + 1]].vTex, verticesNormal[indices[i * 3 + 2]].vTex,
			verticesNormal[indices[i * 3]].Tangent, verticesNormal[indices[i * 3]].Binormal);

		//	得られた接線と従法線を、残りの２つの頂点にもコピーする
		verticesNormal[indices[i * 3 + 1]].Tangent = verticesNormal[indices[i * 3]].Tangent;
		verticesNormal[indices[i * 3 + 2]].Tangent = verticesNormal[indices[i * 3]].Tangent;
		verticesNormal[indices[i * 3 + 1]].Binormal = verticesNormal[indices[i * 3]].Binormal;
		verticesNormal[indices[i * 3 + 2]].Binormal = verticesNormal[indices[i * 3]].Binormal;
	}

	return true;
}

//------------------------------------------------------------------------
//
//	スタティックメッシュ　バーテックスバッファとインデックスバッファをセットする処理
//
// 引数
//   const DWORD&        mi        セットするメッシュ配列の添字
//   const SimpleVertex* vertices  頂点配列
//   const DWORD*        indices   インデックス配列
//
//	戻り値 
//
//------------------------------------------------------------------------
HRESULT  CFbxMesh::SetStaticVIBuffer(const DWORD& mi, const StaticVertexNormal* vertices, const DWORD* indices)
{
	D3D11_BUFFER_DESC bd;
	D3D11_SUBRESOURCE_DATA InitData;

	//バーテックスバッファー作成
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(StaticVertexNormal) * m_pMeshArray[mi].m_dwVerticesNum;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	InitData.pSysMem = vertices;
	if (FAILED(m_pD3D->m_pDevice->CreateBuffer(&bd, &InitData, &m_pMeshArray[mi].m_pVertexBuffer)))
	{
		return E_FAIL;
	}

	//インデックスバッファー作成
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(DWORD) * m_pMeshArray[mi].m_dwIndicesNum;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	InitData.pSysMem = indices;
	if (FAILED(m_pD3D->m_pDevice->CreateBuffer(&bd, &InitData, &m_pMeshArray[mi].m_pIndexBuffer)))
	{
		return E_FAIL;
	}

	return S_OK;
}


//------------------------------------------------------------------------
//
// スキンメッシュの中心点の座標を求める関数
// 
// 引数
//   const SkinVertex*    vertex   頂点配列;
//   const DWORD&          Num      頂点数;
// 
// 戻り値
//   VECTOR3  メッシュの中心点の座標
// 
//------------------------------------------------------------------------
VECTOR3 CFbxMesh::GetSkinCenterPos(const SkinVertex* vertex, const DWORD& Num)
{
	VECTOR3 vMax = VECTOR3(-999999, -999999, -999999);
	VECTOR3 vMin = VECTOR3(999999, 999999, 999999);
	VECTOR3 vCenter = VECTOR3(0, 0, 0);

	for (DWORD i = 0; i < Num; i++)
	{
		if (vMax.x < vertex[i].Pos.x) vMax.x = vertex[i].Pos.x;
		if (vMax.y < vertex[i].Pos.y) vMax.y = vertex[i].Pos.y;
		if (vMax.z < vertex[i].Pos.z) vMax.z = vertex[i].Pos.z;
		if (vMin.x > vertex[i].Pos.x) vMin.x = vertex[i].Pos.x;
		if (vMin.y > vertex[i].Pos.y) vMin.y = vertex[i].Pos.y;
		if (vMin.z > vertex[i].Pos.z) vMin.z = vertex[i].Pos.z;
	}

	vCenter.x = (vMax.x + vMin.x) / 2;
	vCenter.y = (vMax.y + vMin.y) / 2;
	vCenter.z = (vMax.z + vMin.z) / 2;

	return vCenter;
}

//------------------------------------------------------------------------
//
// スキンメッシュ　頂点のレイアウトをノーマルマップ用に変換し、接線と従法線を計算し追加する
// 
// 引数
//   const SkinVertex*    vertices             頂点配列
//   const DWORD*         indices              インデックス配列
//   const DWORD&         IndicesNum           インデックス数
//   SkinVertexNormal*    verticesNormal(OUT)  ノーマルマップ用頂点配列（出力）
// 
// 戻り値
//   bool  true
// 
//------------------------------------------------------------------------
bool CFbxMesh::ChangeSkinVertexLayout(const SkinVertex* vertices, const DWORD* indices, const DWORD& IndicesNum, SkinVertexNormal* verticesNormal)
{

	// ３角形ポリゴン毎に、全てのポリゴンの処理を行う
	for (int i = 0; i < IndicesNum / 3; i++) {
		// 頂点、法線、テクスチャ座標、クラスター(ボーン)インデックス、ウェイトをノーマルマップ用の頂点レイアウトにコピーする
		verticesNormal[indices[i * 3]].Pos = vertices[indices[i * 3]].Pos;
		verticesNormal[indices[i * 3 + 1]].Pos = vertices[indices[i * 3 + 1]].Pos;
		verticesNormal[indices[i * 3 + 2]].Pos = vertices[indices[i * 3 + 2]].Pos;
		verticesNormal[indices[i * 3]].Normal = vertices[indices[i * 3]].Normal;
		verticesNormal[indices[i * 3 + 1]].Normal = vertices[indices[i * 3 + 1]].Normal;
		verticesNormal[indices[i * 3 + 2]].Normal = vertices[indices[i * 3 + 2]].Normal;
		verticesNormal[indices[i * 3]].vTex = vertices[indices[i * 3]].vTex;
		verticesNormal[indices[i * 3 + 1]].vTex = vertices[indices[i * 3 + 1]].vTex;
		verticesNormal[indices[i * 3 + 2]].vTex = vertices[indices[i * 3 + 2]].vTex;
		verticesNormal[indices[i * 3]].ClusterNum[0] = vertices[indices[i * 3]].ClusterNum[0];
		verticesNormal[indices[i * 3]].ClusterNum[1] = vertices[indices[i * 3]].ClusterNum[1];
		verticesNormal[indices[i * 3]].ClusterNum[2] = vertices[indices[i * 3]].ClusterNum[2];
		verticesNormal[indices[i * 3]].ClusterNum[3] = vertices[indices[i * 3]].ClusterNum[3];
		verticesNormal[indices[i * 3 + 1]].ClusterNum[0] = vertices[indices[i * 3 + 1]].ClusterNum[0];
		verticesNormal[indices[i * 3 + 1]].ClusterNum[1] = vertices[indices[i * 3 + 1]].ClusterNum[1];
		verticesNormal[indices[i * 3 + 1]].ClusterNum[2] = vertices[indices[i * 3 + 1]].ClusterNum[2];
		verticesNormal[indices[i * 3 + 1]].ClusterNum[3] = vertices[indices[i * 3 + 1]].ClusterNum[3];
		verticesNormal[indices[i * 3 + 2]].ClusterNum[0] = vertices[indices[i * 3 + 2]].ClusterNum[0];
		verticesNormal[indices[i * 3 + 2]].ClusterNum[1] = vertices[indices[i * 3 + 2]].ClusterNum[1];
		verticesNormal[indices[i * 3 + 2]].ClusterNum[2] = vertices[indices[i * 3 + 2]].ClusterNum[2];
		verticesNormal[indices[i * 3 + 2]].ClusterNum[3] = vertices[indices[i * 3 + 2]].ClusterNum[3];
		verticesNormal[indices[i * 3]].Weits = vertices[indices[i * 3]].Weits;
		verticesNormal[indices[i * 3 + 1]].Weits = vertices[indices[i * 3 + 1]].Weits;
		verticesNormal[indices[i * 3 + 2]].Weits = vertices[indices[i * 3 + 2]].Weits;

		//	頂点3つとUV座標３つをもとに接線と従法線を計算する
		CalcTangentSub(verticesNormal[indices[i * 3]].Pos, verticesNormal[indices[i * 3 + 1]].Pos, verticesNormal[indices[i * 3 + 2]].Pos,
			verticesNormal[indices[i * 3]].vTex, verticesNormal[indices[i * 3 + 1]].vTex, verticesNormal[indices[i * 3 + 2]].vTex,
			verticesNormal[indices[i * 3]].Tangent, verticesNormal[indices[i * 3]].Binormal);

		//	得られた接線と従法線を、残りの２つの頂点にもコピーする
		verticesNormal[indices[i * 3 + 1]].Tangent = verticesNormal[indices[i * 3]].Tangent;
		verticesNormal[indices[i * 3 + 2]].Tangent = verticesNormal[indices[i * 3]].Tangent;
		verticesNormal[indices[i * 3 + 1]].Binormal = verticesNormal[indices[i * 3]].Binormal;
		verticesNormal[indices[i * 3 + 2]].Binormal = verticesNormal[indices[i * 3]].Binormal;
	}

	return true;
}

//------------------------------------------------------------------------
//
//	スキンメッシュ　バーテックスバッファとインデックスバッファをセットする処理
//
// 引数
//   const DWORD&            mi        セットするメッシュ配列の添字
//   const SkinVertexNormal* vertices  頂点配列
//   const DWORD*            indices   インデックス配列
//
//	戻り値 
//
//------------------------------------------------------------------------
HRESULT CFbxMesh::SetSkinVIBuffer(const DWORD& mi, const SkinVertexNormal* vertices, const DWORD* indices)
{
	D3D11_BUFFER_DESC bd;
	D3D11_SUBRESOURCE_DATA InitData;

	//バーテックスバッファー作成
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SkinVertexNormal) * m_pMeshArray[mi].m_dwVerticesNum;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	InitData.pSysMem = vertices;
	if (FAILED(m_pD3D->m_pDevice->CreateBuffer(&bd, &InitData, &m_pMeshArray[mi].m_pVertexBuffer)))
	{
		return E_FAIL;
	}

	//インデックスバッファー作成
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(DWORD) * m_pMeshArray[mi].m_dwIndicesNum;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	InitData.pSysMem = indices;
	if (FAILED(m_pD3D->m_pDevice->CreateBuffer(&bd, &InitData, &m_pMeshArray[mi].m_pIndexBuffer)))
	{
		return E_FAIL;
	}

	return S_OK;
}


//------------------------------------------------------------------------  // -- 2018.8.1
//
// 	メッシュの描画順を決定し、m_dwRenderIdxArrayに設定する関数
// 
//	・視点からメッシュの中心点までの距離を元に並び替え、
//    描画順をm_dwRenderIdxArrayに設定する
// 
// 引数
//   const MATRIX4X4& mWorld   メッシュのワールドマトリックス
//   const VECTOR3& vEye    視点座標
// 
// 戻り値
//   なし
// 
//------------------------------------------------------------------------
void CFbxMesh::SetRenderIdxArray(const MATRIX4X4& mWorld, const VECTOR3& vEye)
{
	DWORD widx;
	float wdis;
	MATRIX4X4 mPos;

	// 視点からメッシュの中心点までの距離を設定する
	for (DWORD i = 0; i < m_dwMeshNum; i++)
	{
		MATRIX4X4 mTemp;
		VECTOR3   vLen;
		m_dwRenderIdxArray[i] = i;
		mPos = XMMatrixTranslation(m_pMeshArray[i].m_vCenterPos.x, m_pMeshArray[i].m_vCenterPos.y, m_pMeshArray[i].m_vCenterPos.z),

		mTemp = mPos * mWorld;
		vLen = GetPositionVector(mTemp) - vEye;
		m_fRenderDistanceArray[i] = magnitude(vLen);
	}

	// 距離の降順に並べ替える（交換法）
	if (m_dwMeshNum <= 1) return;
	for (DWORD i = m_dwMeshNum - 1; i > 0; i--)
	{
		for (DWORD j = 0; j < i; j++)
		{
			if (m_fRenderDistanceArray[j] < m_fRenderDistanceArray[j + 1])
			{
				widx = m_dwRenderIdxArray[j];
				m_dwRenderIdxArray[j] = m_dwRenderIdxArray[j + 1];
				m_dwRenderIdxArray[j + 1] = widx;

				wdis = m_fRenderDistanceArray[j];
				m_fRenderDistanceArray[j] = m_fRenderDistanceArray[j + 1];
				m_fRenderDistanceArray[j + 1] = wdis;
			}
		}
	}
}


//------------------------------------------------------------------------
//
//	頂点3つとUV座標３つをもとに接線と従法線を計算する。
//
//  const VECTOR3& v1          頂点１
//  const VECTOR3& v2          頂点２
//  const VECTOR3& v3          頂点３
//  const VECTOR2& uv1         UV座標１
//  const VECTOR2& uv2         UV座標２
//  const VECTOR2& uv3         UV座標３
//	VECTOR3 &Tangent           接線(OUT)
//	VECTOR3 &Binormal          従法線(OUT)
//
//	戻り値 bool
//
//------------------------------------------------------------------------
bool CFbxMesh::CalcTangentSub(const VECTOR3& v1, const VECTOR3& v2, const VECTOR3& v3,
	const VECTOR2& uv1, const VECTOR2& uv2, const VECTOR2& uv3, VECTOR3 &Tangent, VECTOR3 &Binormal)
{
	//
	// 1頂点5成分(x,y,z,u,v)を1頂点3成分(x,u,v)に変換
	// 3頂点から平面を作る
	VECTOR3 CP0[3] = {
		VECTOR3(v1.x, uv1.x, uv1.y),
		VECTOR3(v1.y, uv1.x, uv1.y),
		VECTOR3(v1.z, uv1.x, uv1.y),
	};
	VECTOR3 CP1[3] = {
		VECTOR3(v2.x, uv2.x, uv2.y),
		VECTOR3(v2.y, uv2.x, uv2.y),
		VECTOR3(v2.z, uv2.x, uv2.y),
	};
	VECTOR3 CP2[3] = {
		VECTOR3(v3.x, uv3.x, uv3.y),
		VECTOR3(v3.y, uv3.x, uv3.y),
		VECTOR3(v3.z, uv3.x, uv3.y),
	};

	// 平面パラメータからUV軸座標算出
	float U[3], V[3];
	for (int i = 0; i < 3; ++i) {
		VECTOR3 V1 = CP1[i] - CP0[i];
		VECTOR3 V2 = CP2[i] - CP1[i];
		VECTOR3 abc;
		abc = cross(V1, V2);

		if (abc.x == 0.0f) {
			// ポリゴンかUV上のポリゴンが縮退していて、算出不可
			//MessageBox(0, _T("CalcTangentSub() : 座標、UVが同一の重複頂点を検出しました"), _T("エラー"), MB_OK);
			Tangent = VECTOR3(1, 0, 0);   // エラーのため０にする
			Binormal = VECTOR3(0, 1, 0);
			return false;
		}
		U[i] = -abc.y / abc.x;
		V[i] = -abc.z / abc.x;
	}

	// 接ベクトルをコピー、正規化
	Tangent = VECTOR3(U[0], U[1], U[2]);
	Tangent = normalize(Tangent);

	// 従法線ベクトルをコピー、正規化
	Binormal = VECTOR3(V[0], V[1], V[2]);
	Binormal = normalize(Binormal);

	return true;
}


//------------------------------------------------------------------------
//
//	テクスチャをセットする処理
//
// 引数
//  const DWORD& ti        セットするテクスチャー配列の添字
//  const TCHAR* TexName   テクスチャー名
//  
//	戻り値 
//
//------------------------------------------------------------------------
HRESULT CFbxMesh::SetTexture(const DWORD& ti, const TCHAR* TexName)
{

	// マテリアルカラーのクリヤー
	m_pMaterialDiffuseArray[ti] = VECTOR4(0, 0, 0, 0);     // -- 2020.12.15
	m_pMaterialSpecularArray[ti] = VECTOR4(0, 0, 0, 0);     // -- 2020.12.15

	//テクスチャー作成
	m_pTextureArray[ti] = nullptr;
	m_pTextureArray[ti] = m_pFbxMeshCtrl->SetTextureList(TexName);                     // -- 2021.2.4
	if (m_pTextureArray[ti] == nullptr)
	{
		MessageBox(nullptr, TexName, _T("■□■ テクスチャファイルがありません ■□■"), MB_OK);
		return E_FAIL;
	}

	// ノーマルテクスチャ作成
	TCHAR fullname[256], drv[256], path[256], fname[256], ext[256];
	_tsplitpath_s(TexName, drv, sizeof(drv) / sizeof(TCHAR), path, sizeof(path) / sizeof(TCHAR),
		fname, sizeof(fname) / sizeof(TCHAR), ext, sizeof(ext) / sizeof(TCHAR));

	_tcscpy_s(fullname, drv);
	_tcscat_s(fullname, path);
	_tcscat_s(fullname, fname);
	_tcscat_s(fullname, _T("_NM"));
	_tcscat_s(fullname, ext);

	m_pTextureNormalArray[ti] = nullptr;
	m_pTextureNormalArray[ti] = m_pFbxMeshCtrl->SetTextureList(fullname);                     // -- 2021.2.4

	// ハイトテクスチャ作成                                                       // -- 2020.1.15
	_tcscpy_s(fullname, drv);
	_tcscat_s(fullname, path);
	_tcscat_s(fullname, fname);
	_tcscat_s(fullname, _T("_HM"));
	_tcscat_s(fullname, ext);

	m_pTextureHeightArray[ti] = nullptr;
	m_pTextureHeightArray[ti] = m_pFbxMeshCtrl->SetTextureList(fullname);                     // -- 2021.2.4

	// スペキュラーテクスチャ作成
	_tcscpy_s(fullname, drv);
	_tcscat_s(fullname, path);
	_tcscat_s(fullname, fname);
	_tcscat_s(fullname, _T("_SM"));
	_tcscat_s(fullname, ext);

	m_pTextureSpecularArray[ti] = nullptr;
	m_pTextureSpecularArray[ti] = m_pFbxMeshCtrl->SetTextureList(fullname);                     // -- 2021.2.4


	return S_OK;

}

//------------------------------------------------------------------------   // -- 2020.12.15
//
//	マテリアルカラーをセットする処理
//
// 引数
//  const DWORD& ti        セットするテクスチャー配列の添字
//  const TCHAR* Color     カラー
//  
//	戻り値 
//
//------------------------------------------------------------------------
bool  CFbxMesh::SetMaterial(const DWORD& ti, const TCHAR* Color)
{

	VECTOR4 vDiffuse, vSpecular;
	memcpy((void*)&vDiffuse, (void*)(Color + 1), sizeof(vDiffuse));
	memcpy((void*)&vSpecular, (void*)(Color + 1 + sizeof(vDiffuse) / sizeof(TCHAR)), sizeof(vSpecular));

	m_pMaterialDiffuseArray[ti] = vDiffuse;
	m_pMaterialSpecularArray[ti] = vSpecular;

	m_pTextureArray[ti] = nullptr;
	m_pTextureNormalArray[ti] = nullptr;
	m_pTextureHeightArray[ti] = nullptr;
	m_pTextureSpecularArray[ti] = nullptr;

	return true;
}

//------------------------------------------------------------------------
//
//	アニメーションファイルの読み込み処理
//
// 引数
//  const TCHAR*         FName          アニメファイル名
//  const ROOTANIMTYPE&  RAType         ルートボーンアニメタイプ（省略値:eRootAnimNone）
//
//	戻り値 bool
//         true:正常   false:異常
//
//------------------------------------------------------------------------
bool CFbxMesh::LoadAnimation(int id, const TCHAR* FName, bool loopFlag, const ROOTANIMTYPE& RAType)
{
	// メッシュファイルが読み込まれていないとエラー
	if (m_pMeshArray == nullptr)
	{
		MessageBox(nullptr, FName, _T("■□■ 対応するスキンメッシュファイル(.mesh)が読み込まれていません ■□■"), MB_OK);
		return false;
	}
	// スタティックメッシュファイルのときはエラー
	if (m_nMeshType != 2 )
	{
		MessageBox(nullptr, FName, _T("■□■ スタティックメッシュにはアニメーションは設定できません ■□■"), MB_OK);
		return false;
	}

	timeBeginPeriod(1);
	DWORD startTime = timeGetTime();

	HANDLE	hFile;
	DWORD	dwLen, dwBufSize;
	BYTE  *pBuf = nullptr;
	BYTE  *p;
	int   mi, bi, n;

	// ファイルのオープン処理
	hFile = CreateFile(FName, GENERIC_READ,
		FILE_SHARE_READ, nullptr, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, nullptr);
	if (hFile != INVALID_HANDLE_VALUE)
	{

		dwBufSize = GetFileSize(hFile, nullptr);     // 入力ファイルのサイズを得る。
		pBuf = new BYTE[dwBufSize];				  // 入力バッファを確保する 

		ReadFile(hFile, pBuf, dwBufSize, &dwLen, nullptr);		// ファイルをバッファに読み込む

		p = pBuf;	// バッファの先頭ポインタをセット

		// -----------------------------------------------------------------    // -- 2020.12.15 -- 3
		// ファイルヘッダ(WCHARの8文字)のチェック
		WCHAR Head[8];
		memcpy_s(Head, sizeof(Head), p, sizeof(Head));
		if (Head[0] == L'A' && Head[1] == L'N' && Head[2] == L'M' && Head[3] == L'X')
		{
			;
		}
		else {
			MessageBox(nullptr, FName, _T("■□■ アニメーションファイル('ANMX')ではありません ■□■"), MB_OK);
			return false;
		}
		// バージョンのチェックは行わない
		if (Head[4] == L'1')
		{
			;
		}
		p += sizeof(Head);

		// ------------------------------------------------------------------------------------------------
		// アニメーションに関するデータ
		ANIMATION* pAnim = &m_Animation[id];
		pAnim->used = true;
		pAnim->loop = loopFlag;

		memcpy_s(&pAnim->startFrame, sizeof(int), p, sizeof(int));       // 開始フレームを得る
		p += sizeof(int);
		memcpy_s(&pAnim->endFrame, sizeof(int), p, sizeof(int));         // 終了フレームを得る
		p += sizeof(int);

		// ルートボーンに関するデータ
		memcpy_s(&pAnim->RootBoneMesh, sizeof(int), p, sizeof(int));       // ルートボーンメッシュ番号を得る    // -- 2020.12.15 -- 3
		p += sizeof(int);
		memcpy_s(&pAnim->RootBone, sizeof(int), p, sizeof(int));           // ルートボーン番号を得る            // -- 2020.12.15 -- 3
		p += sizeof(int);

		// ------------------------------------------------------------------------------------------------
		// メッシュ毎の処理
		for (mi = 0; mi < m_dwMeshNum; mi++)   // メッシュ数だけ繰り返し
		{
			// ボーン数データ
			memcpy_s(&m_pMeshArray[mi].m_NumBones, sizeof(DWORD), p, sizeof(int));       // ボーン数を得る
			p += sizeof(int);

			// 各ボーン毎の処理
			for (bi = 0; bi < m_pMeshArray[mi].m_NumBones; bi++)
			{
				BONE* pBone = &m_pMeshArray[mi].m_BoneArray[id][bi];
				// バインドポーズの取り出し
				// （バインドポーズは、逆行列となっている）
				memcpy_s(&pBone->bindPose, sizeof(MATRIX4X4), p, sizeof(MATRIX4X4));  // バインドポーズ行列の転送
				p += sizeof(MATRIX4X4);

				// フレームポーズ数の取り出し
				memcpy_s(&pBone->framePoseNum, sizeof(int), p, sizeof(int));
				p += sizeof(int);
				SAFE_DELETE_ARRAY(pBone->framePose);
				pBone->framePose = new MATRIX4X4[pBone->framePoseNum];  // フレームポーズ毎のボーン配列を生成する

				// フレームポーズの取り出し
				for (n = 0; n < pBone->framePoseNum; n++)
				{
					memcpy_s(&pBone->framePose[n], sizeof(MATRIX4X4), p, sizeof(MATRIX4X4));  // フレームポーズ行列の転送
					p += sizeof(MATRIX4X4);
				}
			}
		}

		// -------------------------------------------------------------------------------
		CloseHandle(hFile);
		SAFE_DELETE_ARRAY(pBuf);  // 入力バッファを削除する

		// -------------------------------------------------------------------------------
		// ルートボーンアニメーションとシェーダーマトリックスの作成                // -- 2020.12.15 -- 3
		MakeRootAnimAndShaderMatrix(id, RAType);

		// -------------------------------------------------------------------------------
		m_dwLoadTime = timeGetTime() - startTime;
		timeEndPeriod(1);

		return true;

	}
	else {
		MessageBox(nullptr, FName, _T("■□■ アニメーションファイル(.anmx)がありません ■□■"), MB_OK);
	}

	return false;
}

//==========================================================================================================================================================
//
// ルートボーンアニメーションとシェーダーマトリックスの作成                                                                           // -- 2020.12.15 -- 3
// 
// const int& animNum  : セットするアニメーション番号
// ROOTANIMTYPE RAType : ルートボーンアニメタイプ ( eRootAnimNone:ルートアニメなし  eRootAnimXZ:ルートアニメXZ  eRootAnim:ルートアニメ)
//
//==========================================================================================================================================================
void CFbxMesh::MakeRootAnimAndShaderMatrix(const int& animNum, ROOTANIMTYPE RAType)
{
	int RootMeshNo;
	int RootBoneNo;

	if (m_nMeshType != 2)
	{
		MessageBox(nullptr, _T("MakeRootAnimAndShaderMatrix()"), _T("■□■ スタティックメッシュにルートアニメーションの設定はできません ■□■"), MB_OK);
		return;
	}
	if (!m_Animation[animNum].used)
	{
		MessageBox(nullptr, _T("■□■ MakeRootAnimAndShaderMatrix() ■□■"), _T("■□■ スキンメッシュに、指定されたアニメーションが読み込まれていません ■□■"), MB_OK);
		return;
	}

	if (m_Animation[animNum].RootBoneMesh == -1)  // ルートボーンがないとき(通常はあり得ない)
	{
		if (RAType == eRootAnimNone)  // ルートアニメなしのとき
		{
			RootMeshNo = 0;
			RootBoneNo = 0;
		}
		else {
			// ルートアニメ指定ができないのでエラー
			MessageBox(nullptr, _T("MakeRootAnimAndShaderMatrix()"), _T("■□■ ルートボーンが無いため、ルートアニメーションはできません ■□■"), MB_OK);
			return;
		}
	}
	else {
		// ルートボーンのメッシュ番号とボーン番号を設定
		RootMeshNo = m_Animation[animNum].RootBoneMesh;
		RootBoneNo = m_Animation[animNum].RootBone;
	}

	// ルートボーン関係の変数を設定
	m_RootBoneArray[animNum].bindPose = m_pMeshArray[RootMeshNo].m_BoneArray[animNum][RootBoneNo].bindPose;            // ルートボーンのバインドポーズ
	m_RootBoneArray[animNum].framePoseNum = m_pMeshArray[RootMeshNo].m_BoneArray[animNum][RootBoneNo].framePoseNum;    // ルートボーンのフレーム数
	SAFE_DELETE_ARRAY(m_RootBoneArray[animNum].framePose);                                              // フレーム行列を初期クリヤーする
	m_RootBoneArray[animNum].framePose = new MATRIX4X4[m_RootBoneArray[animNum].framePoseNum];          // フレーム行列を生成する
	m_RootAnimType[animNum] = RAType;																	// ルートアニメタイプを保存する

	// ルートボーン配列の作成
	for (int n = 0; n < m_RootBoneArray[animNum].framePoseNum; n++)
	{
		// ルートボーンアニメタイプごとの処理
		switch (RAType)
		{
		case eRootAnimNone:
			// ルートボーンアニメを行わないので初期値を設定する
			m_RootBoneArray[animNum].framePose[n] = XMMatrixIdentity();
			break;

		case eRootAnimXZ:
			// ルートボーンのバインドポーズからの差分（ＸＺ移動のみ）の行列を作成する
		{
			VECTOR3   vVec;
			m_RootBoneArray[animNum].framePose[n] =
				m_RootBoneArray[animNum].bindPose * m_pMeshArray[RootMeshNo].m_BoneArray[animNum][RootBoneNo].framePose[n];
			vVec = GetPositionVector(m_RootBoneArray[animNum].framePose[n]);
			vVec.y = 0;
			m_RootBoneArray[animNum].framePose[n] = XMMatrixTranslationFromVector(vVec);
		}
		break;

		case eRootAnim:
		{
			// ルートボーンのバインドポーズからの差分の行列を作成する
			m_RootBoneArray[animNum].framePose[n] = m_RootBoneArray[animNum].bindPose *
				m_pMeshArray[RootMeshNo].m_BoneArray[animNum][RootBoneNo].framePose[n];
		}
		break;
		}
	}

	// シェーダーへ渡すマトリックスの作成
	for (int mi = 0; mi < m_dwMeshNum; mi++)   // メッシュ数だけ繰り返し
	{
		// シェーダーへ渡す形式の行列を作成
		// （シェーダーへ渡すときは、①各フレーム毎の②ボーン毎に並べておく必要がある）
		// （フレームポーズ数は、一つのメッシュのボーンでは全て同じなので、m_BoneArray[animNum][0].framePoseNumを使用する）
		m_pMeshArray[mi].m_pBoneShader[animNum] = new BONESHADER[m_pMeshArray[mi].m_BoneArray[animNum][0].framePoseNum];  // シェーダーボーン配列を生成する

		for (int i = 0; i < m_pMeshArray[mi].m_BoneArray[animNum][0].framePoseNum; i++)  // 行列の初期化
		{
			for (int j = 0; j < MAX_BONES; j++)
			{
				m_pMeshArray[mi].m_pBoneShader[animNum][i].shaderFramePose[j] = XMMatrixIdentity();
				m_pMeshArray[mi].m_pBoneShader[animNum][i].shaderFramePose[j] =
					XMMatrixTranspose(m_pMeshArray[mi].m_pBoneShader[animNum][i].shaderFramePose[j]);  // 転置行列初期値
			}
		}

		for (int n = 0; n < m_pMeshArray[mi].m_BoneArray[animNum][0].framePoseNum; n++)  // 各フレームポーズ毎の処理
		{
			for (int bi = 0; bi < m_pMeshArray[mi].m_NumBones; bi++)  // 各ボーン毎の処理
			{
				MATRIX4X4 mFramePose;

				if (RAType == eRootAnimNone)
				{
					// ルートボーンアニメをしないときは、バインドボーズの逆行列とフレームポーズを掛け合わせ、バインドボーズからの差分の行列を作成する
					mFramePose = m_pMeshArray[mi].m_BoneArray[animNum][bi].bindPose * m_pMeshArray[mi].m_BoneArray[animNum][bi].framePose[n];
				}
				else {
					// ルートボーンアニメをするとき
					mFramePose = m_pMeshArray[mi].m_BoneArray[animNum][bi].framePose[n] *
									XMMatrixInverse(nullptr, m_RootBoneArray[animNum].framePose[n]);      // 各フレームポーズ行列をルートボーン行列との差分行列にする            // -- 2020.12.15 -- 3
					mFramePose = m_pMeshArray[mi].m_BoneArray[animNum][bi].bindPose * mFramePose;      // バインドボーズの逆行列とフレームポーズを掛け合わせ、バインドボーズからの差分の行列を作成する            // -- 2020.12.15 -- 3
				}
				// フレームポーズを転置行列にしてそのままシェーダーに渡せるようにする
				m_pMeshArray[mi].m_pBoneShader[animNum][n].shaderFramePose[bi] = XMMatrixTranspose(mFramePose);  // 転置行列
			}
		}
	}
}

//==========================================================================================================================================================
//
// アニメーションの最終フレーム番号を取得 
// (endFrame-startFrameを返す) 
//
//==========================================================================================================================================================
int CFbxMesh::GetEndFrame(const int& animID)
{
	if (m_nMeshType != 2)
	{
		MessageBox(nullptr, _T("■□■ GetEndFrame() ■□■"), _T("■□■ スキンメッシュではありません ■□■"), MB_OK);
		return 0;
	}
	if (!m_Animation[animID].used)				  // -- 2024.9.5
	{
		MessageBox(nullptr, _T("■□■ GetEndFrame() ■□■"), _T("■□■ スキンメッシュに、指定されたアニメーションが読み込まれていません ■□■"), MB_OK);
		return 0;
	}
	return m_Animation[animID].endFrame - m_Animation[animID].startFrame;			 // -- 2024.9.5
}

//==========================================================================================================================================================
// 指定ボーンのマトリックスを取得																						    // -- 2024.9.5
// （そのままの相対マトリックスを取得）
// 
// 引数：
// 	  Animator* animStatus          アニメーター
//    const DWORD& nBone            ボーン番号
//    const DWORD& nMesh            メッシュ番号(省略値０)
// 戻り値：
//    MATRIX4X4                     指定ボーンのマトリックス（そのままの相対マトリックス）
//==========================================================================================================================================================
MATRIX4X4 CFbxMesh::GetFrameMatrices(Animator* animStatus, const DWORD& nBone, const DWORD& nMesh)
{
	MATRIX4X4 mBoneWorld;

	if (m_nMeshType == 1)
	{
		MessageBox(nullptr, _T("■□■ GetFrameMatrices() ■□■"), _T("■□■ スタティックメッシュには使用できません ■□■"), MB_OK);
		return XMMatrixIdentity();
	}
	if (nMesh >= m_dwMeshNum || nBone >= m_pMeshArray[nMesh].m_NumBones)
	{
		MessageBox(nullptr, _T("■□■ GetFrameMatrices() ■□■"), _T("■□■ メッシュ番号不正　又は　ボーン番号不正。 ■□■"), MB_OK);
		OutputDebugString(_T("■□■ GetFrameMatrices() メッシュ番号不正　又は　ボーン番号不正 ■□■\n"));
		return XMMatrixIdentity();
	}
	if (animStatus == nullptr || !m_Animation[animStatus->PlayingID()].used)		   // -- 2024.9.5
	{
		MessageBox(nullptr, _T("■□■ GetFrameMatrices() ■□■"), _T("■□■ スキンメッシュに、指定されたアニメーションが読み込まれていません ■□■"), MB_OK);
		return XMMatrixIdentity();
	}

	//  アニメーションが変わっていた場合、作業領域上でフレームを最初に戻す --------------------------------- // -- 2018.8.28
	// なお、最初とはanimFrameを０フレームにすること。startFrameではない。理由はanimFrameは添字番号だからである。
	int  animFrameW;
	animFrameW = animStatus->CurrentFrame();		// -- 2024.9.5
	//if (animStatus.animNum != animStatus.animNumOld) animFrameW = 0;

	// 指定メッシュの指定アニメーションの指定ボーンの指定フレームのマトリックスを取得
	//mBoneWorld = m_pMeshArray[nMesh].m_BoneArray[animStatus->PlayingID()][nBone].framePose[animFrameW]; // -- 2018.8.28
	mBoneWorld = GetMixBoneMatrix(animStatus, nBone, nMesh);											  // -- 2024.10.7

	// ルートボーンアニメをするときの処理                                   // -- 2020.12.15
	if( m_RootAnimType[animStatus->PlayingID()] != eRootAnimNone)	  	// -- 2024.9.5
	{
		// ルートボーンアニメ行列の逆行列を掛け合わせてマトリックスを作成する
		mBoneWorld =
			mBoneWorld * XMMatrixInverse(nullptr, m_RootBoneArray[animStatus->PlayingID()].framePose[animFrameW]);
	}

	// マトリックスを右手座標系から左手座標系に変換(Z軸を反転)
	mBoneWorld = XMMatrixScaling(1, 1, -1) * mBoneWorld;    // -- 2020.7.15

	return mBoneWorld;
}

//==========================================================================================================================================================
// 指定ボーンのマトリックスを取得																					     // -- 2024.9.5
// （ワールドマトリックスと掛け合わせた結果を取得）
// 
// 引数：
// 	  Animator* animStatus          アニメーター
//    const MATRIX4X4& mWorld       ワールドマトリックス
//    const DWORD& nBone            ボーン番号
//    const DWORD& nMesh            メッシュ番号(省略値０)
// 戻り値：
//    MATRIX4X4                    指定ボーンのマトリックス（ワールドマトリックスと掛け合わせたマトリックス）
//==========================================================================================================================================================
MATRIX4X4 CFbxMesh::GetFrameMatrices(Animator* animStatus, const MATRIX4X4& mWorld, const DWORD& nBone, const DWORD& nMesh)
{
	MATRIX4X4 mBoneWorld;

	if (m_nMeshType == 1)
	{
		MessageBox(nullptr, _T("■□■ GetFrameMatrices() ■□■"), _T("■□■ スタティックメッシュには使用できません ■□■"), MB_OK);
		return mWorld;
	}
	if (nMesh >= m_dwMeshNum || nBone >= m_pMeshArray[nMesh].m_NumBones)
	{
		MessageBox(nullptr, _T("■□■ GetFrameMatrices() ■□■"), _T("■□■ メッシュ番号不正　又は　ボーン番号不正。 ■□■"), MB_OK);
		OutputDebugString(_T("■□■ GetFrameMatrices() メッシュ番号不正　又は　ボーン番号不正 ■□■\n"));
		return mWorld;
	}
	if (animStatus == nullptr ||  !m_Animation[animStatus->PlayingID()].used)						  	// -- 2024.9.5
	{
		MessageBox(nullptr, _T("■□■ GetFrameMatrices() ■□■"), _T("■□■ スキンメッシュに、指定されたアニメーションが読み込まれていません ■□■"), MB_OK);
		return mWorld;
	}

	//  アニメーションが変わっていた場合、作業領域上でフレームを最初に戻す --------------------------------- // -- 2018.8.28
	// なお、最初とはanimFrameを０フレームにすること。startFrameではない。理由はanimFrameは添字番号だからである。
	int  animFrameW;
	animFrameW = animStatus->CurrentFrame();
	//if (animStatus.animNum != animStatus.animNumOld) animFrameW = 0;

	// 指定メッシュの指定アニメーションの指定ボーンの指定フレームのマトリックスを取得
	//mBoneWorld = m_pMeshArray[nMesh].m_BoneArray[animStatus->PlayingID()][nBone].framePose[animFrameW];      // -- 2024.9.5
	mBoneWorld = GetMixBoneMatrix(animStatus, nBone, nMesh);											       // -- 2024.10.7

	// ルートボーンアニメをするときの処理                                   // -- 2020.12.15
	if (m_RootAnimType[animStatus->PlayingID()] != eRootAnimNone)						   // -- 2024.9.5
	{
		// ルートボーンアニメ行列の逆行列を掛け合わせてマトリックスを作成する
		mBoneWorld =
			mBoneWorld * XMMatrixInverse(nullptr, m_RootBoneArray[animStatus->PlayingID()].framePose[animFrameW]);
	}

	// マトリックスを右手座標系から左手座標系に変換(Z軸を反転)
	mBoneWorld = XMMatrixScaling(1, 1, -1) * mBoneWorld;    // -- 2020.7.15

	return mBoneWorld * mWorld;  // ワールドマトリックスと掛け合わせる
}

// ボーン配列からミックスしたボーンマトリックスを得る
MATRIX4X4  CFbxMesh::GetMixBoneMatrix(Animator* animStatus, const DWORD& nBone, const DWORD& nMesh)
{
	MATRIX4X4 mat = XMMatrixIdentity();

	if (animStatus == nullptr) return mat;

	int animNum = animStatus->PlayingID();			// -- 2024.9.5
	int frint = (int)animStatus->CurrentFrame();
	int frnext = frint + 1;
	if (frnext > m_Animation[animNum].endFrame) {
		if (m_Animation[animNum].loop)
			frnext -= m_Animation[animNum].endFrame + m_Animation[animNum].startFrame;
		else
			frnext = m_Animation[animNum].endFrame;
	}
	float rate = animStatus->CurrentFrame() - (float)frint;
	if (animStatus->Rate() >= 1.0f) { // ブレンドする必要なし
		VECTOR4* org1 = (VECTOR4*)&m_pMeshArray[nMesh].m_BoneArray[animNum][nBone].framePose[frint];
		VECTOR4* org2 = (VECTOR4*)&m_pMeshArray[nMesh].m_BoneArray[animNum][nBone].framePose[frnext];
		VECTOR4* dst = (VECTOR4*)&mat;
		for (DWORD b = 0; b < 4; b++) {
			*dst++ = *org1++ * (1.0f - rate) + *org2++ * rate;
		}
	}
	else {
		int animNum2 = animStatus->SubID();
		int frint2 = (int)animStatus->SubFrame();
		int frnext2 = frint2 + 1;
		if (frnext2 > m_Animation[animNum2].endFrame) {
			if (m_Animation[animNum2].loop)
				frnext2 -= m_Animation[animNum2].endFrame + m_Animation[animNum2].startFrame;
			else
				frnext2 = m_Animation[animNum2].endFrame;
		}
		float rate2 = animStatus->SubFrame() - (float)frint2;

		VECTOR4* org11 = (VECTOR4*)&m_pMeshArray[nMesh].m_BoneArray[animNum][nBone].framePose[frint];
		VECTOR4* org12 = (VECTOR4*)&m_pMeshArray[nMesh].m_BoneArray[animNum][nBone].framePose[frnext];
		VECTOR4* org21 = (VECTOR4*)&m_pMeshArray[nMesh].m_BoneArray[animNum2][nBone].framePose[frint2];
		VECTOR4* org22 = (VECTOR4*)&m_pMeshArray[nMesh].m_BoneArray[animNum2][nBone].framePose[frnext2];

		VECTOR4* dst = (VECTOR4*)&mat;
		for (DWORD b = 0; b < 4; b++) {
			*dst++ = *org11++ * (1.0f - rate) * animStatus->Rate()
				+ *org12++ * rate * animStatus->Rate()
				+ *org21++ * (1.0f - rate2) * (1.0f - animStatus->Rate())
				+ *org22++ * rate2 * (1.0f - animStatus->Rate());
		}
	}
	return mat;
}


//==========================================================================================================================================================
// ルートボーンアニメーションのマトリックスを取得                                                                                    // -- 2024.9.5
// 
// 引数：
//    const int& UpFrame            アニメーションフレームの現在値からの増分（省略値：０）
//
// 戻り値：
//    MATRIX4X4                    ルートボーンのマトリックス
//==========================================================================================================================================================
MATRIX4X4 CFbxMesh::GetRootAnimMatrices(Animator* animStatus, const int& UpFrame)
{
	MATRIX4X4 mBoneWorld;

	if (m_nMeshType != 2)
	{
		MessageBox(nullptr, _T("■□■ GetRootAnimMatrices() ■□■"), _T("■□■ スタティックメッシュには使用できません ■□■"), MB_OK);
		return XMMatrixIdentity();
	}
	if (animStatus == nullptr || !m_Animation[animStatus->PlayingID()].used)	   // -- 2024.9.5
	{
		MessageBox(nullptr, _T("■□■ GetRootAnimMatrices() ■□■"), _T("■□■ スキンメッシュに、指定されたアニメーションが読み込まれていません ■□■"), MB_OK);
		return XMMatrixIdentity();
	}

	// アニメーションフレームに増分値を加味して現在値とする。
	// 上限・下限値を超えていた場合は適切な位置に訂正する。
	// なお、最初とはanimFrameを０フレームにすること。startFrameではない。理由はanimFrameは添字番号だからである。
	int  animFrameW;
	animFrameW = animStatus->CurrentFrame() + UpFrame;			   // -- 2024.9.5
	if (animFrameW < 0)
	{
		animFrameW = animFrameW + (m_Animation[animStatus->PlayingID()].endFrame - m_Animation[animStatus->PlayingID()].startFrame);
	}
	if (animFrameW > m_Animation[animStatus->PlayingID()].endFrame - m_Animation[animStatus->PlayingID()].startFrame)
	{
		animFrameW = animFrameW - (m_Animation[animStatus->PlayingID()].endFrame - m_Animation[animStatus->PlayingID()].startFrame);
	}
	//  アニメーションが変わっていた場合、作業領域上でフレームを最初に戻す
	// なお、最初とはanimFrameを０フレームにすること。startFrameではない。理由はanimFrameは添字番号だからである。
	//if (animStatus.animNum != animStatus.animNumOld) animFrameW = 0;

	// ルートボーンアニメの現在フレームのマトリックスを取得
	mBoneWorld = m_RootBoneArray[animStatus->PlayingID()].framePose[animFrameW];

	return mBoneWorld;
}

//==========================================================================================================================================================
// ルートボーンアニメーションの現在値フレームの一つ前のフレーム位置からの増分マトリックスを取得する                                      // -- 2024.9.5
// 
// 引数：
//    const int& UpFrame            アニメーションフレームの現在値からの増分（省略値：０）
//    const int& StartFrameUp       アニメフレームが開始フレーム（フレーム０）のときの増分値の処理方法（省略値：１）
//                                    0:XMMatrixIdentity()にする   1:０～１の増分値を使用する
//
// 戻り値：
//    MATRIX4X4                     ルートボーンの一つ前の位置からの増分マトリックス
//==========================================================================================================================================================
MATRIX4X4 CFbxMesh::GetRootAnimUpMatrices(Animator* animStatus, const int& UpFrame, const int& StartFrameUp)
{
	MATRIX4X4 mBoneWorld, mBoneWorldOld;

	if (m_nMeshType != 2)
	{
		MessageBox(nullptr, _T("■□■ GetRootAnimUpMatrices() ■□■"), _T("■□■ スタティックメッシュには使用できません ■□■"), MB_OK);
		return XMMatrixIdentity();
	}
	if (animStatus == nullptr || !m_Animation[animStatus->PlayingID()].used)		   // -- 2024.9.5
	{
		MessageBox(nullptr, _T("■□■ GetRootAnimUpMatrices() ■□■"), _T("■□■ スキンメッシュに、指定されたアニメーションが読み込まれていません ■□■"), MB_OK);
		return XMMatrixIdentity();
	}

	// アニメーションフレームに増分値を加味して現在値とする。
	// 上限・下限値を超えていた場合は適切な位置に訂正する。
	// なお、最初とはanimFrameを０フレームにすること。startFrameではない。理由はanimFrameは添字番号だからである。
	int  animFrameW;
	animFrameW = animStatus->CurrentFrame() + UpFrame;				// -- 2024.9.5
	if (animFrameW < 0)
	{
		animFrameW = animFrameW + (m_Animation[animStatus->PlayingID()].endFrame - m_Animation[animStatus->PlayingID()].startFrame);	// -- 2024.9.5
	}
	if (animFrameW > m_Animation[animStatus->PlayingID()].endFrame - m_Animation[animStatus->PlayingID()].startFrame)
	{
		animFrameW = animFrameW - (m_Animation[animStatus->PlayingID()].endFrame - m_Animation[animStatus->PlayingID()].startFrame);
	}
	//  アニメーションが変わっていた場合、作業領域上でフレームを最初に戻す
	// なお、最初とはanimFrameを０フレームにすること。startFrameではない。理由はanimFrameは添字番号だからである。
	//if (animStatus.animNum != animStatus.animNumOld) animFrameW = 0;

	// アニメーションフレームの現在値が０の場合
	if (animFrameW <= 0)
	{
		if (StartFrameUp == 0) return XMMatrixIdentity();  // 増分値は０となる
		animFrameW = 1;                                    // ０～１の増分値を使用する
	}

	// ルートボーンアニメの現在フレームのマトリックスを取得
	mBoneWorld = m_RootBoneArray[animStatus->PlayingID()].framePose[animFrameW];

	// ルートボーンアニメの現在値より一つ前のフレームのマトリックスを取得
	mBoneWorldOld = m_RootBoneArray[animStatus->PlayingID()].framePose[animFrameW-1];

	// 現在値フレームマトリックスの一つ前のフレームマトリックスからの増分マトリックスを得る
	mBoneWorld = XMMatrixInverse(nullptr, mBoneWorldOld) * mBoneWorld;

	return mBoneWorld;
}

//------------------------------------------------------------------------
//																			 // -- 2024.9.5
//	メッシュを画面にレンダリングする処理
//
//	(全てのレンダリングの呼びだしを行う処理)
//
//------------------------------------------------------------------------
void CFbxMesh::Render( const MATRIX4X4& mWorld)
{
	Render(mWorld, GameDevice()->m_mView, GameDevice()->m_mProj, GameDevice()->m_vLightDir, GameDevice()->m_vEyePt);
}
void CFbxMesh::RenderDisplace(const MATRIX4X4& mWorld)
{
	RenderDisplace(mWorld, GameDevice()->m_mView, GameDevice()->m_mProj, GameDevice()->m_vLightDir, GameDevice()->m_vEyePt);
}

void CFbxMesh::Render(Animator* animStatus, const MATRIX4X4& mWorld)
{
	Render(animStatus, mWorld, GameDevice()->m_mView, GameDevice()->m_mProj, GameDevice()->m_vLightDir, GameDevice()->m_vEyePt);
}
void CFbxMesh::RenderDisplace(Animator* animStatus, const MATRIX4X4& mWorld)
{
	RenderDisplace(animStatus, mWorld, GameDevice()->m_mView, GameDevice()->m_mProj, GameDevice()->m_vLightDir, GameDevice()->m_vEyePt);
}


//------------------------------------------------------------------------
//																			 // -- 2024.9.5
//	メッシュを画面にレンダリングする処理
//
// 引数
//      const MATRIX4X4& mWorld              ワールドマトリックス
//      const MATRIX4X4& mView               ビューマトリックス
//      const MATRIX4X4& mProj               プロジェクションマトリックス
//      const VECTOR3&   vLight              光源ベクトル
//      const VECTOR3&   vEye                視点ベクトル
//
//	戻り値 HRESULT
//		S_OK	= 正常
//		E_FAIL	= 異常
//
//------------------------------------------------------------------------
void CFbxMesh::Render(const MATRIX4X4& mWorld, const MATRIX4X4& mView, const MATRIX4X4& mProj, const VECTOR3& vLight, const VECTOR3& vEye)
{
	if (m_nMeshType == 1)
	{
		RenderStatic(mWorld, mView, mProj, vLight, vEye);
	}
	else  if (m_nMeshType == 2) {
		MessageBox(nullptr, _T("■□■ Render() ■□■"), _T("■□■ スキンメッシュなのにアニメーションAnimatorの指定がありません ■□■"), MB_OK);
		return;
	}
}
void CFbxMesh::RenderDisplace( const MATRIX4X4& mWorld, const MATRIX4X4& mView, const MATRIX4X4& mProj, const VECTOR3& vLight, const VECTOR3& vEye)
{
	if (m_nMeshType == 1)
	{
		RenderDisplaceStatic(mWorld, mView, mProj, vLight, vEye);
	}
	else  if (m_nMeshType == 2) {
		MessageBox(nullptr, _T("■□■ RenderDisplace() ■□■"), _T("■□■ スキンメッシュなのにアニメーションAnimatorの指定がありません ■□■"), MB_OK);
		return;
	}
}

//------------------------------------------------------------------------
//
//	スタティックメッシュを画面にレンダリングする処理
//
// 引数
//      const MATRIX4X4& mWorld              ワールドマトリックス
//      const MATRIX4X4& mView               ビューマトリックス
//      const MATRIX4X4& mProj               プロジェクションマトリックス
//      const VECTOR3&   vLight              光源ベクトル
//      const VECTOR3&   vEye                視点ベクトル
//
//	戻り値 HRESULT
//		S_OK	= 正常
//		E_FAIL	= 異常
//
//------------------------------------------------------------------------
void CFbxMesh::RenderStatic(const MATRIX4X4& mWorld, const MATRIX4X4& mView, const MATRIX4X4& mProj, const VECTOR3& vLight, const VECTOR3& vEye)
{

	//使用するシェーダーの登録	
	m_pD3D->m_pDeviceContext->VSSetShader(m_pShader->m_pFbxStaticMesh_VS, nullptr, 0);
	m_pD3D->m_pDeviceContext->PSSetShader(m_pShader->m_pFbxStaticMesh_PS, nullptr, 0);

	//シェーダーのコンスタントバッファーに各種データを渡す	
	D3D11_MAPPED_SUBRESOURCE pData;
	CONSTANT_BUFFER_WVLED cb;
	if (SUCCEEDED(m_pD3D->m_pDeviceContext->Map(m_pShader->m_pConstantBufferWVLED, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		//ワールド行列を渡す
		cb.mW = XMMatrixTranspose(mWorld);

		//ワールド、カメラ、射影行列を渡す
		cb.mWVP = XMMatrixTranspose(mWorld * mView * mProj);

		//ライト方向を渡す
		cb.vLightDir = VECTOR4(vLight.x, vLight.y, vLight.z, 0);

		// 視点を渡す
		cb.vEyePos = VECTOR4(vEye.x, vEye.y, vEye.z, 1);

		//カラーを渡す
		cb.vDiffuse = m_vDiffuse;

		// 各種情報を渡す。(使っていない)
		cb.vDrawInfo = VECTOR4(0, 0, 0, 0);

		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		m_pD3D->m_pDeviceContext->Unmap(m_pShader->m_pConstantBufferWVLED, 0);
	}

	//このコンスタントバッファーを使うシェーダーの登録
	m_pD3D->m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pShader->m_pConstantBufferWVLED);
	m_pD3D->m_pDeviceContext->PSSetConstantBuffers(0, 1, &m_pShader->m_pConstantBufferWVLED);


	//頂点インプットレイアウトをセット
	m_pD3D->m_pDeviceContext->IASetInputLayout(m_pShader->m_pFbxStaticMesh_VertexLayout);

	//プリミティブ・トポロジーをセット
	m_pD3D->m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//テクスチャーサンプラーをシェーダーに渡す
	m_pD3D->m_pDeviceContext->PSSetSamplers(0, 1, &m_pD3D->m_pSampleLinear);

	// メッシュの描画順を決定するm_dwRenderIdxArrayの設定   // -- 2018.8.1
	SetRenderIdxArray(mWorld, vEye);

	// メッシュの数だけテクスチャー、バーテックスバッファ、インデックスバッファをセットして、レンダリングする
	for (DWORD mi = 0; mi < m_dwMeshNum; mi++)   // -- 2018.8.1
	{
		DWORD i = m_dwRenderIdxArray[mi];   // -- 2018.8.1

		// テクスチャ（ディフューズ、ノーマル、スペキュラ）をピクセルシェーダーに渡す。スロット番号は0,1,3
		SetShaderTexture(_T("PS"), 0, &m_pMeshArray[i].m_pTexture);             // ディフューズテクスチャ
		SetShaderTexture(_T("PS"), 1, &m_pMeshArray[i].m_pTextureNormal);       // ノーマルテクスチャ
		SetShaderTexture(_T("PS"), 3, &m_pMeshArray[i].m_pTextureSpecular);     // スペキュラテクスチャ

		// ディフューズテクスチャがないときのみシェーダーのコンスタントバッファーにマテリアルカラーを渡す        // -- 2020.12.15
		if (m_pMeshArray[i].m_pTexture == nullptr)
		{
			SetShaderMatColor(_T("PS"), 3, i);   // マテリアルカラーをPSに渡す
		}

		// バーテックスバッファーをセット
		UINT stride = sizeof(StaticVertexNormal);
		UINT offset = 0;
		m_pD3D->m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pMeshArray[i].m_pVertexBuffer, &stride, &offset);

		//インデックスバッファーをセット
		m_pD3D->m_pDeviceContext->IASetIndexBuffer(m_pMeshArray[i].m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		//プリミティブをレンダリング
		// （インデックスの数を指定してレンダリング）
		m_pD3D->m_pDeviceContext->DrawIndexed(m_pMeshArray[i].m_dwIndicesNum, 0, 0);
	}

}

//------------------------------------------------------------------------
//
//	スタティックメッシュを画面にレンダリングする処理(ディスプレースメントマッピング)
//
// 引数
//      const MATRIX4X4&  mWorld              ワールドマトリックス
//      const MATRIX4X4&4 mView               ビューマトリックス
//      const MATRIX4X4&  mProj               プロジェクションマトリックス
//      const VECTOR3&    vLight              光源ベクトル
//      const VECTOR3&    vEye                視点ベクトル
//
//	戻り値 HRESULT
//		S_OK	= 正常
//		E_FAIL	= 異常
//
//------------------------------------------------------------------------
void CFbxMesh::RenderDisplaceStatic(const MATRIX4X4& mWorld, const MATRIX4X4& mView, const MATRIX4X4& mProj, const VECTOR3& vLight, const VECTOR3& vEye)
{

	//m_pD3D->m_pDeviceContext->RSSetState(m_pD3D->m_pRStateLW);  // 左回り、ワイヤーフレーム表示

	//使用するシェーダーの登録	
	m_pD3D->m_pDeviceContext->VSSetShader(m_pShader->m_pDisplaceStaticMesh_VS, nullptr, 0);
	m_pD3D->m_pDeviceContext->HSSetShader(m_pShader->m_pDisplaceStaticMesh_HS, nullptr, 0);
	m_pD3D->m_pDeviceContext->DSSetShader(m_pShader->m_pDisplaceStaticMesh_DS, nullptr, 0);
	m_pD3D->m_pDeviceContext->PSSetShader(m_pShader->m_pDisplaceStaticMesh_PS, nullptr, 0);


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
		cb.vDiffuse = m_vDiffuse;

		// 各種情報を渡す。(使っていない)       // -- 2020.12.15
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

		// 最小距離(1.0以上)、最大距離
		sg.fMinDistance = 5.0f;
		sg.fMaxDistance = 40.0f;
		// 最大分割数
		sg.iMaxDevide = m_iMaxDevide;

		// 高さを渡す
		sg.vHeight = VECTOR2(0.0f, m_fHeightMax);

		// ダミー
		sg.vWaveMove = VECTOR4(0.0f, 0.0f, 0.0f, 0.0f);
		sg.vSpecular = VECTOR4(0.0f, 0.0f, 0.0f, 0.0f);

		memcpy_s(pData.pData, pData.RowPitch, (void*)&sg, sizeof(CONSTANT_BUFFER_DISPLACE));
		m_pD3D->m_pDeviceContext->Unmap(m_pShader->m_pConstantBufferDisplace, 0);
	}
	// このコンスタントバッファーを使うシェーダーの登録
	m_pD3D->m_pDeviceContext->VSSetConstantBuffers(1, 1, &m_pShader->m_pConstantBufferDisplace); // バーテックスシェーダーで使う
	m_pD3D->m_pDeviceContext->DSSetConstantBuffers(1, 1, &m_pShader->m_pConstantBufferDisplace); // ドメインシェーダーで使う
	m_pD3D->m_pDeviceContext->HSSetConstantBuffers(1, 1, &m_pShader->m_pConstantBufferDisplace); // ハルシェーダーで使う
	m_pD3D->m_pDeviceContext->PSSetConstantBuffers(1, 1, &m_pShader->m_pConstantBufferDisplace); // ピクセルシェーダーで使う


	// 頂点インプットレイアウトをセット
	//  (ディスプレイスメントマッピングも、頂点レイアウトは、スタティックメッシュと共用)
	m_pD3D->m_pDeviceContext->IASetInputLayout(m_pShader->m_pFbxStaticMesh_VertexLayout);

	// プリミティブ・トポロジーをセット(ディスプレイスメントマッピング用)
	m_pD3D->m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);


	// テクスチャーサンプラーをシェーダーに渡す
	// m_pD3D->m_pDeviceContext->DSSetSamplers(0, 1, &m_pD3D->m_pSampleLinear);
	m_pD3D->m_pDeviceContext->PSSetSamplers(0, 1, &m_pD3D->m_pSampleLinear);

	// メッシュの描画順を決定するm_dwRenderIdxArrayの設定   // -- 2018.8.1
	SetRenderIdxArray(mWorld, vEye);

	// メッシュの数だけテクスチャー、バーテックスバッファ、インデックスバッファをセットして、レンダリングする
	for (DWORD mi = 0; mi < m_dwMeshNum; mi++)   // -- 2018.8.1
	{
		DWORD i = m_dwRenderIdxArray[mi];   // -- 2018.8.1

		// テクスチャ（ディフューズ、ノーマル、ハイト、スペキュラ）をドメインシェーダーとピクセルシェーダーに渡す。スロット番号は0～3
		SetShaderTexture(_T("DSPS"), 0, &m_pMeshArray[i].m_pTexture);             // ディフューズテクスチャ
		SetShaderTexture(_T("DSPS"), 1, &m_pMeshArray[i].m_pTextureNormal);       // ノーマルテクスチャ
		SetShaderTexture(_T("DSPS"), 2, &m_pMeshArray[i].m_pTextureHeight);       // ハイトテクスチャ
		SetShaderTexture(_T("DSPS"), 3, &m_pMeshArray[i].m_pTextureSpecular);     // スペキュラテクスチャ

		// ディフューズテクスチャがないときのみシェーダーのコンスタントバッファーにマテリアルカラーを渡す        // -- 2020.12.15
		if (m_pMeshArray[i].m_pTexture == nullptr)
		{
			SetShaderMatColor(_T("DSPS"), 3, i);   // マテリアルカラーをDSPSに渡す
		}

		// バーテックスバッファーをセット
		UINT stride = sizeof(StaticVertexNormal);
		UINT offset = 0;
		m_pD3D->m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pMeshArray[i].m_pVertexBuffer, &stride, &offset);

		//インデックスバッファーをセット
		m_pD3D->m_pDeviceContext->IASetIndexBuffer(m_pMeshArray[i].m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		//プリミティブをレンダリング
		// （インデックスの数を指定してレンダリング）
		m_pD3D->m_pDeviceContext->DrawIndexed(m_pMeshArray[i].m_dwIndicesNum, 0, 0);

	}

	// ハルシェーダーとドメインシェーダーをリセットする
	m_pD3D->m_pDeviceContext->HSSetShader(nullptr, nullptr, 0);
	m_pD3D->m_pDeviceContext->DSSetShader(nullptr, nullptr, 0);

	//m_pD3D->m_pDeviceContext->RSSetState(m_pD3D->m_pRStateL);  // 左回り通常表示

}

//------------------------------------------------------------------------
//																		  // -- 2024.9.5
//	メッシュを画面にレンダリングする処理
//
// 引数
//      Animator* animStatus    アニメーションステータス
//      const MATRIX4X4& mWorld         ワールドマトリックス
//      const MATRIX4X4& mView          ビューマトリックス
//      const MATRIX4X4& mProj          プロジェクションマトリックス
//      const VECTOR3&   vLight         光源ベクトル
//      const VECTOR3&   vEye           視点ベクトル
//
//	戻り値 
//		なし
//
//------------------------------------------------------------------------
void CFbxMesh::Render(Animator* animStatus, const MATRIX4X4& mWorld, const MATRIX4X4& mView, const MATRIX4X4& mProj, const VECTOR3& vLight, const VECTOR3& vEye)
{
	if (m_nMeshType == 1)
	{
		RenderStatic(mWorld, mView, mProj, vLight, vEye);
	}
	else if (m_nMeshType == 2) {
		RenderSkin(animStatus, mWorld, mView, mProj, vLight, vEye);
	}
}
void CFbxMesh::RenderDisplace(Animator* animStatus, const MATRIX4X4& mWorld, const MATRIX4X4& mView, const MATRIX4X4& mProj, const VECTOR3& vLight, const VECTOR3& vEye)
{
	if (m_nMeshType == 1)
	{
		RenderDisplaceStatic(mWorld, mView, mProj, vLight, vEye);
	}
	else if (m_nMeshType == 2) {
		RenderDisplaceSkin(animStatus, mWorld, mView, mProj, vLight, vEye);
	}
}
// シェーダーボーン配列のボーンマトリックスをミックスする
// なお、シェーダーボーンマトリックスは転置行列になっている
void CFbxMesh::MakeBoneMatrix(Animator* animStatus, MATRIX4X4* mat, const CFbxMeshArray& _mesh)
{
	if (animStatus == nullptr) return;

	int animNum = animStatus->PlayingID();			// -- 2024.9.5
	int frint = (int)animStatus->CurrentFrame();
	int frnext = frint + 1;
	if (frnext > m_Animation[animNum].endFrame) {
		if (m_Animation[animNum].loop)
			frnext -= m_Animation[animNum].endFrame + m_Animation[animNum].startFrame;
		else
			frnext = m_Animation[animNum].endFrame;
	}
	float rate = animStatus->CurrentFrame() - (float)frint;
	if (animStatus->Rate() >= 1.0f) { // ブレンドする必要なし
		VECTOR4* org1 = (VECTOR4*)&_mesh.m_pBoneShader[animNum][frint].shaderFramePose;
		VECTOR4* org2 = (VECTOR4*)&_mesh.m_pBoneShader[animNum][frnext].shaderFramePose;
		VECTOR4* dst = (VECTOR4*)mat;
		for (DWORD b = 0; b < _mesh.m_NumBones * 4; b++) {
			*dst++ = *org1++ * (1.0f - rate) + *org2++ * rate;
		}
	}
	else {
		int animNum2 = animStatus->SubID();
		int frint2 = (int)animStatus->SubFrame();
		int frnext2 = frint2 + 1;
		if (frnext2 > m_Animation[animNum2].endFrame) {
			if (m_Animation[animNum2].loop)
				frnext2 -= m_Animation[animNum2].endFrame + m_Animation[animNum2].startFrame;
			else
				frnext2 = m_Animation[animNum2].endFrame;
		}
		float rate2 = animStatus->SubFrame() - (float)frint2;
		VECTOR4* org11 = (VECTOR4*)&_mesh.m_pBoneShader[animNum][frint].shaderFramePose;
		VECTOR4* org12 = (VECTOR4*)&_mesh.m_pBoneShader[animNum][frnext].shaderFramePose;
		VECTOR4* org21 = (VECTOR4*)&_mesh.m_pBoneShader[animNum2][frint2].shaderFramePose;
		VECTOR4* org22 = (VECTOR4*)&_mesh.m_pBoneShader[animNum2][frnext2].shaderFramePose;
		VECTOR4* dst = (VECTOR4*)mat;
		for (DWORD b = 0; b < _mesh.m_NumBones * 4; b++) {
			*dst++ = *org11++ * (1.0f - rate) * animStatus->Rate()
				+ *org12++ * rate * animStatus->Rate()
				+ *org21++ * (1.0f - rate2) * (1.0f - animStatus->Rate())
				+ *org22++ * rate2 * (1.0f - animStatus->Rate());
		}
	}
}


//------------------------------------------------------------------------
//
//	スキンメッシュを画面にレンダリングする処理
//
// 引数
//      ANIMATION_STATUS& animStatus    アニメーションステータス
//      const MATRIX4X4& mWorld         ワールドマトリックス
//      const MATRIX4X4& mView          ビューマトリックス
//      const MATRIX4X4& mProj          プロジェクションマトリックス
//      const VECTOR3&   vLight         光源ベクトル
//      const VECTOR3&   vEye           視点ベクトル
//
//	戻り値 
//		なし
//
//------------------------------------------------------------------------
void CFbxMesh::RenderSkin(Animator* animStatus, const MATRIX4X4& mWorld, const MATRIX4X4& mView, const MATRIX4X4& mProj, const VECTOR3& vLight, const VECTOR3& vEye)
{
	// メッシュの描画
	Draw(animStatus, mWorld, mView, mProj, vLight, vEye);				   // -- 2024.9.5
}

//------------------------------------------------------------------------
//																		   // -- 2024.9.5
//	スキンメッシュを画面に描画する処理
//
// 引数
//      const int&       animNum   アニメーション番号 
//      const int&       frame     アニメーションフレーム
//      const MATRIX4X4& mWorld    ワールドマトリックス
//      const MATRIX4X4& mView     ビューマトリックス
//      const MATRIX4X4& mProj     プロジェクションマトリックス
//      const VECTOR3&   vLight    光源ベクトル
//      const VECTOR3&   vEye      視点ベクトル
//
//	戻り値 
//		なし
//
//------------------------------------------------------------------------
void CFbxMesh::Draw(Animator* animStatus, const MATRIX4X4& mWorld, const MATRIX4X4& mView, const MATRIX4X4& mProj, const VECTOR3& vLight, const VECTOR3& vEye)
{
	if (animStatus == nullptr) return;    // -- 2024.9.5

	//使用するシェーダーの登録	
	m_pD3D->m_pDeviceContext->VSSetShader(m_pShader->m_pFbxSkinMesh_VS, nullptr, 0);
	m_pD3D->m_pDeviceContext->PSSetShader(m_pShader->m_pFbxSkinMesh_PS, nullptr, 0);

	//シェーダーのコンスタントバッファーに各種データを渡す	
	D3D11_MAPPED_SUBRESOURCE pData;
	CONSTANT_BUFFER_WVLED cb;
	if (SUCCEEDED(m_pD3D->m_pDeviceContext->Map(m_pShader->m_pConstantBufferWVLED, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		//ワールド行列を渡す
		cb.mW = XMMatrixTranspose(mWorld);

		//ワールド、カメラ、射影行列を渡す
		cb.mWVP = XMMatrixTranspose(mWorld * mView * mProj);

		//ライト方向を渡す
		cb.vLightDir = VECTOR4(vLight.x, vLight.y, vLight.z, 0);

		// 視点を渡す
		cb.vEyePos = VECTOR4(vEye.x, vEye.y, vEye.z, 1);

		//カラーを渡す
		cb.vDiffuse = m_vDiffuse;

		// 各種情報を渡す。(使っていない)       // -- 2020.12.15
		cb.vDrawInfo = VECTOR4(0, 0, 0, 0);

		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		m_pD3D->m_pDeviceContext->Unmap(m_pShader->m_pConstantBufferWVLED, 0);
	}

	//このコンスタントバッファーを使うシェーダーの登録
	m_pD3D->m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pShader->m_pConstantBufferWVLED);
	m_pD3D->m_pDeviceContext->PSSetConstantBuffers(0, 1, &m_pShader->m_pConstantBufferWVLED);


	//このコンスタントバッファーを使うシェーダーの登録
	m_pD3D->m_pDeviceContext->VSSetConstantBuffers(1, 1, &m_pShader->m_pConstantBufferBone2);
	m_pD3D->m_pDeviceContext->PSSetConstantBuffers(1, 1, &m_pShader->m_pConstantBufferBone2);

	//頂点インプットレイアウトをセット
	m_pD3D->m_pDeviceContext->IASetInputLayout(m_pShader->m_pFbxSkinMesh_VertexLayout);

	//プリミティブ・トポロジーをセット
	m_pD3D->m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//テクスチャーサンプラーをシェーダーに渡す
	m_pD3D->m_pDeviceContext->PSSetSamplers(0, 1, &m_pD3D->m_pSampleLinear);

	// メッシュの描画順を決定するm_dwRenderIdxArrayの設定   // -- 2018.8.1
	SetRenderIdxArray(mWorld, vEye);

	// メッシュの数だけテクスチャー、バーテックスバッファ、インデックスバッファ、ボーン行列をセットして、レンダリングする
	for (DWORD mi = 0; mi < m_dwMeshNum; mi++)   // -- 2018.8.1
	{
		DWORD i = m_dwRenderIdxArray[mi];   // -- 2018.8.1

		// テクスチャ（ディフューズ、ノーマル、スペキュラ）をピクセルシェーダーに渡す。スロット番号は0,1,3
		SetShaderTexture(_T("PS"), 0, &m_pMeshArray[i].m_pTexture);             // ディフューズテクスチャ
		SetShaderTexture(_T("PS"), 1, &m_pMeshArray[i].m_pTextureNormal);       // ノーマルテクスチャ
		SetShaderTexture(_T("PS"), 3, &m_pMeshArray[i].m_pTextureSpecular);     // スペキュラテクスチャ

		// ディフューズテクスチャがないときのみシェーダーのコンスタントバッファーにマテリアルカラーを渡す        // -- 2020.12.15
		if (m_pMeshArray[i].m_pTexture == nullptr)
		{
			SetShaderMatColor(_T("PS"), 3, i);   // マテリアルカラーをPSに渡す
		}

		// バーテックスバッファーをセット
		UINT stride = sizeof(SkinVertexNormal);
		UINT offset = 0;
		m_pD3D->m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pMeshArray[i].m_pVertexBuffer, &stride, &offset);

		//インデックスバッファーをセット
		m_pD3D->m_pDeviceContext->IASetIndexBuffer(m_pMeshArray[i].m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		// ボーン行列をボーンのコンスタントバッファにセット
		if (SUCCEEDED(m_pD3D->m_pDeviceContext->Map(
			m_pShader->m_pConstantBufferBone2,   // マップするリソース・ボーン行列用コンスタントバッファ
			0,                               // サブリソースのインデックス番号
			D3D11_MAP_WRITE_DISCARD,         // 書き込みアクセス
			0,                               //
			&pData)))                        // データの書き込み先ポインタ
		{
			// このメッシュのシェーダーボーン配列のうち、指定されたアニメーション番号（animNum）、フレーム番号(frame)のボーン行列配列を先頭からボーン数だけ転送する
			MakeBoneMatrix(animStatus, (MATRIX4X4*)pData.pData, m_pMeshArray[i]);
//			memcpy_s(pData.pData, sizeof(MATRIX4X4)*m_pMeshArray[i].m_NumBones, &m_pMeshArray[i].m_pBoneShader[animNum][frame].shaderFramePose, sizeof(MATRIX4X4)*m_pMeshArray[i].m_NumBones);		// cbBonesの書き込み 全ボーンのフレームポーズ行列を渡す
			m_pD3D->m_pDeviceContext->Unmap(m_pShader->m_pConstantBufferBone2, 0);
		}

		//プリミティブをレンダリング
		// （インデックスの数を指定してレンダリング）
		m_pD3D->m_pDeviceContext->DrawIndexed(m_pMeshArray[i].m_dwIndicesNum, 0, 0);
	}

}

//------------------------------------------------------------------------    // -- 2024.9.5
//
//	スキンメッシュを画面にレンダリングする処理(ディスプレースメントマッピング)
//
// 引数
//      ANIMATION_STATUS& animStatus    アニメーションステータス
//      const MATRIX4X4& mWorld         ワールドマトリックス
//      const MATRIX4X4& mView          ビューマトリックス
//      const MATRIX4X4& mProj          プロジェクションマトリックス
//      const VECTOR3&   vLight         光源ベクトル
//      const VECTOR3&   vEye           視点ベクトル
//
//	戻り値 
//		なし
//
//------------------------------------------------------------------------
void CFbxMesh::RenderDisplaceSkin(Animator* animStatus, const MATRIX4X4& mWorld, const MATRIX4X4& mView, const MATRIX4X4& mProj, const VECTOR3& vLight, const VECTOR3& vEye)
{
	// メッシュの描画
	DrawDisplace(animStatus, mWorld, mView, mProj, vLight, vEye);		  // -- 2024.9.5
}

//------------------------------------------------------------------------  // -- 2020.1.15
//
//	スキンメッシュを画面に描画する処理(ディスプレースメントマッピング)
//
// 引数
//      const int&       animNum   アニメーション番号 
//      const int&       frame     アニメーションフレーム
//      const MATRIX4X4& mWorld    ワールドマトリックス
//      const MATRIX4X4& mView     ビューマトリックス
//      const MATRIX4X4& mProj     プロジェクションマトリックス
//      const VECTOR3&   vLight    光源ベクトル
//      const VECTOR3&   vEye      視点ベクトル
//
//	戻り値 
//		なし
//
//------------------------------------------------------------------------
void CFbxMesh::DrawDisplace(Animator* animStatus, const MATRIX4X4& mWorld, const MATRIX4X4& mView, const MATRIX4X4& mProj, const VECTOR3& vLight, const VECTOR3& vEye)
{
	if (animStatus == nullptr) return;    // -- 2024.9.5

	//使用するシェーダーの登録	
	m_pD3D->m_pDeviceContext->VSSetShader(m_pShader->m_pDisplaceSkinMesh_VS, nullptr, 0);
	m_pD3D->m_pDeviceContext->HSSetShader(m_pShader->m_pDisplaceSkinMesh_HS, nullptr, 0);
	m_pD3D->m_pDeviceContext->DSSetShader(m_pShader->m_pDisplaceSkinMesh_DS, nullptr, 0);
	m_pD3D->m_pDeviceContext->PSSetShader(m_pShader->m_pDisplaceSkinMesh_PS, nullptr, 0);


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
		cb.vDiffuse = m_vDiffuse;

		// 各種情報を渡す。(使っていない)       // -- 2020.12.15
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

		// 最小距離(1.0以上)、最大距離
		sg.fMinDistance = 3.0f;
		sg.fMaxDistance = 30.0f;
		// 最大分割数
		sg.iMaxDevide = m_iMaxDevide;

		// 高さを渡す
		sg.vHeight = VECTOR2(0.0f, m_fHeightMax);

		// ダミー
		sg.vWaveMove = VECTOR4(0.0f, 0.0f, 0.0f, 0.0f);
		sg.vSpecular = VECTOR4(0.0f, 0.0f, 0.0f, 0.0f);

		memcpy_s(pData.pData, pData.RowPitch, (void*)&sg, sizeof(CONSTANT_BUFFER_DISPLACE));
		m_pD3D->m_pDeviceContext->Unmap(m_pShader->m_pConstantBufferDisplace, 0);
	}
	// このコンスタントバッファーを使うシェーダーの登録
	m_pD3D->m_pDeviceContext->VSSetConstantBuffers(1, 1, &m_pShader->m_pConstantBufferDisplace);
	m_pD3D->m_pDeviceContext->DSSetConstantBuffers(1, 1, &m_pShader->m_pConstantBufferDisplace);
	m_pD3D->m_pDeviceContext->HSSetConstantBuffers(1, 1, &m_pShader->m_pConstantBufferDisplace);
	m_pD3D->m_pDeviceContext->PSSetConstantBuffers(1, 1, &m_pShader->m_pConstantBufferDisplace);


	//このコンスタントバッファーを使うシェーダーの登録
	m_pD3D->m_pDeviceContext->VSSetConstantBuffers(2, 1, &m_pShader->m_pConstantBufferBone2);
	m_pD3D->m_pDeviceContext->PSSetConstantBuffers(2, 1, &m_pShader->m_pConstantBufferBone2);


	// 頂点インプットレイアウトをセット
	//  (ディスプレイスメントマッピングも、頂点レイアウトは、スキンメッシュと共用)
	m_pD3D->m_pDeviceContext->IASetInputLayout(m_pShader->m_pFbxSkinMesh_VertexLayout);

	// プリミティブ・トポロジーをセット(ディスプレイスメントマッピング用)
	m_pD3D->m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);


	// テクスチャーサンプラーをシェーダーに渡す
	// m_pD3D->m_pDeviceContext->DSSetSamplers(0, 1, &m_pD3D->m_pSampleLinear);
	m_pD3D->m_pDeviceContext->PSSetSamplers(0, 1, &m_pD3D->m_pSampleLinear);

	// メッシュの描画順を決定するm_dwRenderIdxArrayの設定 
	SetRenderIdxArray(mWorld, vEye);

	// メッシュの数だけテクスチャー、バーテックスバッファ、インデックスバッファをセットして、レンダリングする
	for (DWORD mi = 0; mi < m_dwMeshNum; mi++)
	{
		DWORD i = m_dwRenderIdxArray[mi];

		// テクスチャ（ディフューズ、ノーマル、ハイト、スペキュラ）をドメインシェーダーとピクセルシェーダーに渡す。スロット番号は0～3
		SetShaderTexture(_T("DSPS"), 0, &m_pMeshArray[i].m_pTexture);             // ディフューズテクスチャ
		SetShaderTexture(_T("DSPS"), 1, &m_pMeshArray[i].m_pTextureNormal);       // ノーマルテクスチャ
		SetShaderTexture(_T("DSPS"), 2, &m_pMeshArray[i].m_pTextureHeight);       // ハイトテクスチャ
		SetShaderTexture(_T("DSPS"), 3, &m_pMeshArray[i].m_pTextureSpecular);     // スペキュラテクスチャ

		// ディフューズテクスチャがないときのみシェーダーのコンスタントバッファーにマテリアルカラーを渡す        // -- 2020.12.15
		if (m_pMeshArray[i].m_pTexture == nullptr)
		{
			SetShaderMatColor(_T("DSPS"), 3, i);   // マテリアルカラーをDSPSに渡す
		}

		// バーテックスバッファーをセット
		UINT stride = sizeof(SkinVertexNormal);
		UINT offset = 0;
		m_pD3D->m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pMeshArray[i].m_pVertexBuffer, &stride, &offset);

		//インデックスバッファーをセット
		m_pD3D->m_pDeviceContext->IASetIndexBuffer(m_pMeshArray[i].m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		// ボーン行列をボーンのコンスタントバッファにセット
		if (SUCCEEDED(m_pD3D->m_pDeviceContext->Map(
			m_pShader->m_pConstantBufferBone2,   // マップするリソース・ボーン行列用コンスタントバッファ
			0,                               // サブリソースのインデックス番号
			D3D11_MAP_WRITE_DISCARD,         // 書き込みアクセス
			0,                               //
			&pData)))                        // データの書き込み先ポインタ
		{
			// このメッシュのシェーダーボーン配列のうち、指定されたアニメーション番号（animNum）、フレーム番号(frame)のボーン行列配列を先頭からボーン数だけ転送する
//			memcpy_s(pData.pData, sizeof(MATRIX4X4)*m_pMeshArray[i].m_NumBones, &m_pMeshArray[i].m_pBoneShader[animNum][frame].shaderFramePose, sizeof(MATRIX4X4)*m_pMeshArray[i].m_NumBones);		// cbBonesの書き込み 全ボーンのフレームポーズ行列を渡す
			MakeBoneMatrix(animStatus, (MATRIX4X4*)pData.pData, m_pMeshArray[i]);
			m_pD3D->m_pDeviceContext->Unmap(m_pShader->m_pConstantBufferBone2, 0);
		}

		//プリミティブをレンダリング
		// （インデックスの数を指定してレンダリング）
		m_pD3D->m_pDeviceContext->DrawIndexed(m_pMeshArray[i].m_dwIndicesNum, 0, 0);

	}

	// ハルシェーダーとドメインシェーダーをリセットする
	m_pD3D->m_pDeviceContext->HSSetShader(nullptr, nullptr, 0);
	m_pD3D->m_pDeviceContext->DSSetShader(nullptr, nullptr, 0);

}

//------------------------------------------------------------------------
//
//	テクスチャをシェーダーに渡す処理
//
// 引数
//  TCHAR ProfileName[]        : 設定するシェーダーの種類（"PS" or "DSPS"）
//  int   SlotNo               : 設定するスロット番号
//  ID3D11ShaderResourceView** : 渡すテクスチャ
//  
//  戻り値 
//
//------------------------------------------------------------------------
void  CFbxMesh::SetShaderTexture(const TCHAR ProfileName[], int SlotNo, ID3D11ShaderResourceView** ppTexture)
{
	int prof = _tcscmp(_T("PS"), ProfileName);  // "PS"のみのときは0, "DSPS"のときは0以外

	if (ppTexture && *ppTexture)
	{
		// テクスチャがあるとき
		if (prof != 0) m_pD3D->m_pDeviceContext->DSSetShaderResources(SlotNo, 1, ppTexture);  // テクスチャーをドメインシェーダーに渡す
		m_pD3D->m_pDeviceContext->PSSetShaderResources(SlotNo, 1, ppTexture);  // テクスチャーをピクセルシェーダーに渡す
	}
	else {
		// テクスチャが無いとき
		ID3D11ShaderResourceView* Nothing[1] = { 0 };
		if (prof != 0) m_pD3D->m_pDeviceContext->DSSetShaderResources(SlotNo, 1, Nothing);
		m_pD3D->m_pDeviceContext->PSSetShaderResources(SlotNo, 1, Nothing);
	}
}

//------------------------------------------------------------------------
//
//	マテリアルカラーをシェーダーに渡す処理
//
// 引数
//  TCHAR ProfileName[]        : 設定するシェーダーの種類（"PS" or "DSPS"）
//  int   SlotNo               : 設定するスロット番号
//  int   i                    : メッシュ番号
//  
//  戻り値 
//
//------------------------------------------------------------------------
void  CFbxMesh::SetShaderMatColor(const TCHAR ProfileName[], int SlotNo, int i )
{
	int prof = _tcscmp(_T("PS"), ProfileName);  // "PS"のみのときは0, "DSPS"のときは0以外

	//シェーダーのコンスタントバッファーに各種データを渡す
	D3D11_MAPPED_SUBRESOURCE pData;
	CONSTANT_BUFFER_MATERIAL cb = {};
	if (SUCCEEDED(m_pD3D->m_pDeviceContext->Map(m_pShader->m_pConstantBufferMaterial, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		// マテリアルカラーを渡す
		cb.vMatDuffuse = m_pMeshArray[i].m_pMaterialDiffuse;
		cb.vMatSpecular = m_pMeshArray[i].m_pMaterialSpecular;
		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		m_pD3D->m_pDeviceContext->Unmap(m_pShader->m_pConstantBufferMaterial, 0);
	}

	//このコンスタントバッファーを使うシェーダーの登録
	if (prof != 0) m_pD3D->m_pDeviceContext->DSSetConstantBuffers(SlotNo, 1, &m_pShader->m_pConstantBufferMaterial);
	m_pD3D->m_pDeviceContext->PSSetConstantBuffers(SlotNo, 1, &m_pShader->m_pConstantBufferMaterial);
}

// ----------------------------------------------------------------------------------------------------------------
// 
// FbxMeshCtrl  メッシュコントロールクラス                                              // -- 2021.2.4
// 
// メッシュの総合的な管理を行うクラス
// 主にテクスチャの管理を行う。
// 複数のメッシュ間で同一のテクスチャが使われている場合、重複する読み込みを排除する
// 
// -----------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------
//
//	CFbxMeshCtrl  コンストラクタ	
//
//------------------------------------------------------------------------
CFbxMeshCtrl::CFbxMeshCtrl(CShader* pShader)
{
	m_pD3D = pShader->m_pD3D;
	m_pShader = pShader;
}
//------------------------------------------------------------------------
//
//	CFbxMeshCtrl  デストラクタ	
//
//------------------------------------------------------------------------
CFbxMeshCtrl::~CFbxMeshCtrl()
{
	std::list<TextureList>::iterator itr;    // リストのイテレータ
	for (itr = m_TexList.begin(); itr != m_TexList.end(); itr++)
	{
		// 実際は事前にリリースされてここには来ないはずだが、念のためリリースする
		SAFE_RELEASE(itr->m_pTexture);   // テクスチャをリリース
	}
	m_TexList.clear();		// リスト削除
}

//------------------------------------------------------------------------
//
//	テクスチャを読み込む処理	
//
//	同じテクスチャを重複して読み込まないようにリストで管理する	
//	同一名称のテクスチャは、重複して読み込まず、リスト上のテクスチャアドレスを返す
//	リスト上にないテクスチャ（新規のテクスチャ）は、読み込んでリストの末尾に登録する。そのアドレスを返す。
//	リストのカウンタ(m_nCnt)で、同一テクスチャを使用しているメッシュの数を数えておく。
//
//	引数
//	  const TCHAR FName[] : テクスチャ名
//
//	戻り値
//	  ID3D11ShaderResourceView*	テクスチャ
//------------------------------------------------------------------------
ID3D11ShaderResourceView* CFbxMeshCtrl::SetTextureList(const TCHAR FName[])
{
	TextureList tl;
	std::list<TextureList>::iterator itr;    // リストのイテレータ

	for (itr = m_TexList.begin(); itr != m_TexList.end(); itr++)
	{
		if (_tcscmp(itr->m_FName, FName) == 0)  // リストの中に一致するテクスチャがあったとき
		{
			itr->m_nCnt++;            // カウントに１をたす（これは使用している箇所のカウントになる）
			return itr->m_pTexture;   // リストの中のテクスチャアドレスを返す
		}
	}

	// リストの中に一致するテクスチャがなかったときはテクスチャを読み込んでリストに追加する
	if (FAILED(m_pD3D->CreateShaderResourceViewFromFile(FName, &tl.m_pTexture, 3)))
	{
		// テクスチャがなかったとき
		//MessageBox(nullptr, FName, _T("■□■ SetTextureList テクスチャファイルがありません ■□■"), MB_OK);
		return nullptr;
	}
	// テクスチャがあったとき
	_tcscpy_s(tl.m_FName, FName);
	tl.m_nCnt = 1;				// カウントを１にする
	m_TexList.push_back(tl);  // リストの末尾に追加する

	return tl.m_pTexture;	// 読み込んだテクスチャアドレスを返す
}

//------------------------------------------------------------------------
//
//	指定のテクスチャをリストから削除する	
//
//	リストをたどり指定のテクスチャ（アドレス）を検索する
//	同一テクスチャが見つかったら、カウンタ(m_nCnt)から１を引く
//	カウンタが０になったら、もうそのテクスチャを使用しているメッシュが
//	一つもなくなったと言うことなので、テクスチャを解放しそのリストも削除する。
//
//	引数
//	  ID3D11ShaderResourceView* pTexture :	テクスチャアドレス
//
//------------------------------------------------------------------------
void CFbxMeshCtrl::DeleteTextureList(ID3D11ShaderResourceView* pTexture)
{
	std::list<TextureList>::iterator itr;    // リストのイテレータ

	for (itr = m_TexList.begin(); itr != m_TexList.end(); itr++)
	{
		if (itr->m_pTexture == pTexture)  // 指定のテクスチャがあったとき
		{
			itr->m_nCnt--;              // カウンタから１を引く
			if (itr->m_nCnt <= 0)
			{
				// カウンタが０になったので、どこでも使われていないテクスチャのため削除する
				SAFE_RELEASE(itr->m_pTexture);
				m_TexList.erase(itr);
			}
			break;
		}
	}
}


