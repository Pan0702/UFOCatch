// ========================================================================================
//
//   �f�B�X�v���C�X�����g�}�b�s���O�𗘗p���郉�C�u����
//                                                                 ver 3.3
//
//	 �E�g�̌��ʂ�ݒ�
//
//      (�g�ɉ����A�n�ⓙ�̏����ɑΉ�����)
//
//																				 2024.3.23
// ========================================================================================

#pragma once

#include "../DirectX/Direct3D.h"
#include "../GameMain/GameMain.h"
//
// ���_�̍\����
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
// �f�B�X�v���[�X�����g�g�̃N���X
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
	ID3D11ShaderResourceView* m_pTexture;//�e�N�X�`���[
	ID3D11ShaderResourceView* m_pNormalTexture;//�e�N�X�`���[
	ID3D11ShaderResourceView* m_pSpecularTexture;  // �X�y�L�����e�N�X�`���[    // -- 2021.2.4

	DWORD                     m_dwNumIndex;
	VECTOR4                   m_vWaveMove;
	VECTOR2                   m_vWaveSpeed;
	FLOAT                     m_fWaveHeight;
	VECTOR4                   m_vDiffuse;
	VECTOR4                   m_vSpecular;

	FLOAT                     m_fMinDistance; //�ŏ�����
	FLOAT                     m_fMaxDistance; //�ő勗��
	int                       m_iMaxDevide;   //�ő啪����

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
