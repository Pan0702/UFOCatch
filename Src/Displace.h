// ========================================================================================
//
//   ディスプレイスメントマッピングを利用するライブラリ
//                                                                 ver 3.3
//
//	 ・波の効果を設定
//
//      (波に加え、溶岩等の処理に対応する)
//
//																				 2024.3.23
// ========================================================================================

#pragma once

#include "Direct3D.h"
#include "Shader.h"
//
// 頂点の構造体
//
struct DISPLACE_VERTEX
{
	VECTOR3 vPos;
	VECTOR2 vTex;
	VECTOR3 vNorm;
};

// 
class CDirect3D;
class CShader;
//
// ディスプレースメント波のクラス
//
class CWave
{
public:
	// Direct3D11
	CDirect3D*					m_pD3D;
	CShader*					m_pShader;

	// 
	ID3D11Buffer*             m_pVertexBuffer;
	ID3D11Buffer*             m_pIndexBuffer;
	ID3D11ShaderResourceView* m_pTexture;//テクスチャー
	ID3D11ShaderResourceView* m_pNormalTexture;//テクスチャー
	ID3D11ShaderResourceView* m_pSpecularTexture;  // スペキュラテクスチャー    // -- 2021.2.4

	DWORD                     m_dwNumIndex;
	VECTOR4                   m_vWaveMove;
	VECTOR2                   m_vWaveSpeed;
	FLOAT                     m_fWaveHeight;
	VECTOR4                   m_vDiffuse;
	VECTOR4                   m_vSpecular;

	FLOAT                     m_fMinDistance; //最小距離
	FLOAT                     m_fMaxDistance; //最大距離
	int                       m_iMaxDevide;   //最大分割数

public:
	// 
	CWave();								// -- 2024.3.23
	CWave(CShader* pShader);
	CWave(const int& iWidth, const int& iHeight, const TCHAR* FName, const TCHAR* TexName = nullptr, const TCHAR* SpecName = nullptr);  // -- 2024.3.23
	CWave(CShader* pShader, const int& iWidth, const int& iHeight, const TCHAR* FName, const TCHAR* TexName = nullptr, const TCHAR* SpecName = nullptr);  // -- 2021.2.4
	~CWave();

	void Init();
	void DeleteAll();
	HRESULT SetSrc(const int& iWidth, const int& iHeight, const TCHAR* FName, const TCHAR* TexName, const TCHAR* SpecName);  // -- 2021.2.4
	void Render(const MATRIX4X4& mWorld);																					 // -- 2024.3.23
	void Render(const MATRIX4X4& mWorld, const MATRIX4X4& mView, const MATRIX4X4& mProj, const VECTOR3& vLight, const VECTOR3& vEye);


};
