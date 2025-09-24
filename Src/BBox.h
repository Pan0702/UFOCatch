//=============================================================================
//
//  バウンディングボックス                             ver 3.3      2024.3.23
//
//   ポリゴンの表面判定を変更。左回りを表とする
//									                                 BBox.h
//=============================================================================

#pragma once

#include "Direct3D.h"
#include "Shader.h"

// ---------------------------------------------------------
// 
// OBB (有向境界ボックス:Oriented Bounding Box)
// 
// ---------------------------------------------------------
struct BBOX_VERTEX
{
	VECTOR3 vPos;
	VECTOR3 vNorm;
	VECTOR2 vTex;       // テクスチャ座標(使用していない)
};

// 
class CDirect3D;
class CShader;
class CBBox
{
public:
	// Direct3D11
	CDirect3D*  m_pD3D;
	CShader*    m_pShader;

	// OBB
	VECTOR3    m_vMax;
	VECTOR3    m_vMin;
	VECTOR3    m_vAxisX;
	VECTOR3    m_vAxisY;
	VECTOR3    m_vAxisZ;
	FLOAT      m_fLengthX;  // ボックスの半径（辺の長さの1/2）
	FLOAT      m_fLengthY;  // ボックスの半径（辺の長さの1/2）
	FLOAT      m_fLengthZ;  // ボックスの半径（辺の長さの1/2）
	MATRIX4X4  m_mWorld;

	VECTOR4    m_vDiffuse;  // ディフューズカラー  // -- 2020.1.24

	// メッシュ
	ID3D11Buffer*   m_pVertexBuffer;
	ID3D11Buffer*   m_pIndexBuffer;

public:
	// 
	CBBox();                                                 // -- 2024.3.23
	CBBox(const VECTOR3& vMin, const VECTOR3& vMax);         // -- 2024.3.23
	CBBox(CShader* pShader);
	CBBox(CShader* pShader, const VECTOR3& vMin, const VECTOR3& vMax);
	~CBBox();

	void InitBBox(const VECTOR3& vMin, const VECTOR3& vMax);
	HRESULT InitMesh();
	void MakeVertexIndex(VECTOR3* pVertex, DWORD* pIndex);
	bool OBBCollisionDetection(CBBox* pOtherBBox, VECTOR3* vHit, VECTOR3* vNrm);
	bool CompareLength(const CBBox* pBBoxA, const CBBox* pBBoxB, const VECTOR3* pvSeparate, const VECTOR3* pvDistance);
	bool OBBCollisionLay(const MATRIX4X4& mLay, const MATRIX4X4& mLayOld, VECTOR3* vHit, VECTOR3* vNrm);
	bool OBBCollisionLay(const VECTOR3& vLay, const VECTOR3& vLayOld, VECTOR3* vHit, VECTOR3* vNrm);
	bool OBBCollisionTri(const VECTOR3* pTri, VECTOR3* vHit);
	bool OBBCollisionTri(const VECTOR3* pTri, const MATRIX4X4& mWorld, VECTOR3* vhit);
	float LenSegOnSeparateAxis(const VECTOR3 *Sep, const VECTOR3 *e1, const VECTOR3 *e2, const VECTOR3 *e3);

	void Render();            // -- 2024.3.23
	void Render(const MATRIX4X4& mWorld, const MATRIX4X4& mView, const MATRIX4X4& mProj, const VECTOR3& vLight, const VECTOR3& vEye);  // -- 2021.2.4
	void Render(const MATRIX4X4& mView, const MATRIX4X4& mProj, const VECTOR3& vLight, const VECTOR3& vEye);

};
