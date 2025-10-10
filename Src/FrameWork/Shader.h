// ========================================================================================
//
// �V�F�[�_�[�̏���                                               ver 3.0        2021.1.11
//
//   �V�F�[�_�[�O�����̓ǂݍ��݂ƃV�F�[�_�[�̍쐬
//   �C���v�b�g���C�A�E�g�̍쐬
//   �R���X�^���g�o�b�t�@�̍쐬
//
//
//    �o�^����Ă���V�F�[�_�[
//
//		�E�V���v���ȃV�F�[�_�[                          InitShaderSimple();
//		�E�X�v���C�g�p�̃V�F�[�_�[                      InitShaderSprite();
//		�EFBXStaticMesh/FBXSkinMesh�p�̃V�F�[�_�[       InitShaderFbx();
//		�E�f�B�X�v���C�X�����g�}�b�s���O�p�̃V�F�[�_�[  InitShaderDisplace();
//		�E�G�t�F�N�g�p�̃V�F�[�_�[                      InitShaderEffect();
//
//                                                                              Shader.h
// ========================================================================================
#pragma once

//�w�b�_�[�t�@�C���̃C���N���[�h
#include <stdio.h>
#include <windows.h>

#include "../WinMain/Main.h"
#include "../DirectX/Direct3D.h"

//�V�F�[�_�[�̃o�b�t�@�\���̒�`

// ���b�V���V�F�[�_�[�p�̃R���X�^���g�o�b�t�@�[�̃A�v�����\���́B
// �i���[���h�s�񂩂�ˉe�s��A���C�g�A�J���[�j  // -- 2020.1.24
struct CONSTANT_BUFFER_WVLED
{
	MATRIX4X4  mW;             // ���[���h�s��
	MATRIX4X4  mWVP;           // ���[���h����ˉe�܂ł̕ϊ��s��
	VECTOR4    vLightDir;      // ���C�g����
	VECTOR4    vEyePos;        // ���_
	VECTOR4    vDiffuse;       // �f�B�t���[�Y�F	
	VECTOR4    vDrawInfo;      // �`��֘A���(�g�p���Ă��Ȃ�)   // -- 2020.12.15
	CONSTANT_BUFFER_WVLED()
	{
		ZeroMemory(this, sizeof(CONSTANT_BUFFER_WVLED));
	}
};

// �f�B�X�v���[�X�����g�}�b�s���O�p�̊e��f�[�^��n��  // -- 2020.1.24
struct CONSTANT_BUFFER_DISPLACE
{
	VECTOR3    vEyePosInv;    // �e���_���猩���A���_�̈ʒu
	float      fMinDistance;  // �|���S�������̍ŏ�����
	float      fMaxDistance;  // �|���S�������̍ő召����
	int        iMaxDevide;    // �����ő吔
	VECTOR2    vHeight;       // �f�B�X�v���[�X�����g�}�b�s���O���̐���グ����
	VECTOR4    vWaveMove;     // �g�̈ړ���(�g�̏������̂�)
	VECTOR4    vSpecular;     // ���ʔ���(�g�̏������̂�)
	CONSTANT_BUFFER_DISPLACE()
	{
		ZeroMemory(this, sizeof(CONSTANT_BUFFER_DISPLACE));
	}
};

//  �G�t�F�N�g�p�̃R���X�^���g�o�b�t�@�̃A�v�����\����   //  2017.8.25
struct CONSTANT_BUFFER_EFFECT
{
	MATRIX4X4  mWVP;       // ���[���h����ˉe�܂ł̕ϊ��s��
	MATRIX4X4  mW;         // ���[���h
	MATRIX4X4  mV;         // �r���[
	MATRIX4X4  mP;         // �ˉe
	VECTOR2    vUVOffset;  // �e�N�X�`�����W�̃I�t�Z�b�g
	VECTOR2    vUVScale;   // �e�N�X�`�����W�̊g�k  // -- 2019.7.17
	float      fAlpha;
	float      fSize;      // �p�[�e�B�N���̑傫��  // -- 2018.8.23
	VECTOR2    Dummy;                               // -- 2019.7.17
	CONSTANT_BUFFER_EFFECT()
	{
		ZeroMemory(this, sizeof(CONSTANT_BUFFER_EFFECT));
	}
};


//�@3D�X�v���C�g�V�F�[�_�[�p�̃R���X�^���g�o�b�t�@�[�̃A�v�����\���� 
struct CONSTANT_BUFFER_SPRITE
{
	MATRIX4X4  mWVP;
	MATRIX4X4  mW;
	float      ViewPortWidth;
	float      ViewPortHeight;
	VECTOR2    vUVOffset;
	VECTOR4    vColor;         // �J���[���B�������̊������w�肷��
	VECTOR4    vMatInfo;       // �}�e���A���֘A���@x:�e�N�X�`���L�薳���BDrawRect()�ADrawLine()�Ŏg�p�B
	CONSTANT_BUFFER_SPRITE()
	{
		ZeroMemory(this, sizeof(CONSTANT_BUFFER_SPRITE));
	}
};

// �}�e���A�����                      // -- 2020.12.15
struct CONSTANT_BUFFER_MATERIAL
{
	VECTOR4    vMatDuffuse;
	VECTOR4    vMatSpecular;
};

//
// CShader�N���X
//
class CShader
{
public:
	// Direct3D11
	CDirect3D*              m_pD3D;

	// �V�F�[�_�[
	// �ʏ�p�̃V���v���ȃV�F�[�_�[
	ID3D11InputLayout*      m_pSimple_VertexLayout;
	ID3D11VertexShader*     m_pSimple_VS;
	ID3D11PixelShader*      m_pSimple_PS;

	// 3D�X�v���C�g�p�̃V�F�[�_�[
	ID3D11InputLayout*      m_pSprite3D_VertexLayout;
	ID3D11VertexShader*     m_pSprite3D_VS;
	ID3D11PixelShader*      m_pSprite3D_PS;
	ID3D11VertexShader*     m_pSprite3D_VS_BILL;

	// �f�B�X�v���[�X�����g�}�b�s���O(�g)�p�̃V�F�[�_�[
	ID3D11InputLayout*      m_pDisplaceWave_VertexLayout;
	ID3D11VertexShader*     m_pDisplaceWave_VS;
	ID3D11HullShader*       m_pDisplaceWave_HS;
	ID3D11DomainShader*     m_pDisplaceWave_DS;
	ID3D11PixelShader*      m_pDisplaceWave_PS;

	// �f�B�X�v���[�X�����g�}�b�s���O(�X�L�����b�V��)�p�̃V�F�[�_�[
	ID3D11VertexShader*     m_pDisplaceSkinMesh_VS;
	ID3D11HullShader*       m_pDisplaceSkinMesh_HS;
	ID3D11DomainShader*     m_pDisplaceSkinMesh_DS;
	ID3D11PixelShader*      m_pDisplaceSkinMesh_PS;

	// �f�B�X�v���[�X�����g�}�b�s���O(�X�^�e�B�b�N���b�V��)�p�̃V�F�[�_�[
	ID3D11VertexShader*     m_pDisplaceStaticMesh_VS;
	ID3D11HullShader*       m_pDisplaceStaticMesh_HS;
	ID3D11DomainShader*     m_pDisplaceStaticMesh_DS;
	ID3D11PixelShader*      m_pDisplaceStaticMesh_PS;

	// �G�t�F�N�g�p�̃V�F�[�_�[
	ID3D11InputLayout*      m_pEffect3D_VertexLayout;
	ID3D11VertexShader*     m_pEffect3D_VS_POINT;
	ID3D11GeometryShader*   m_pEffect3D_GS_POINT;
	ID3D11PixelShader*      m_pEffect3D_PS;
	ID3D11InputLayout*      m_pEffect3D_VertexLayout_BILL;
	ID3D11VertexShader*     m_pEffect3D_VS_BILL;
	ID3D11VertexShader*     m_pEffect3D_VS_BILLMESH;  // -- 2019.7.17

	// Fbx���f���@�X�^�e�B�c�N���b�V���p�̃V�F�[�_�[
	// �iNormal�}�b�s���O�j
	ID3D11InputLayout*      m_pFbxStaticMesh_VertexLayout;
	ID3D11VertexShader*     m_pFbxStaticMesh_VS;
	ID3D11PixelShader*      m_pFbxStaticMesh_PS;

	// Fbx���f���@�X�L�����b�V���p�̃V�F�[�_�[
	// �iNormal�}�b�s���O�j
	ID3D11InputLayout*      m_pFbxSkinMesh_VertexLayout;
	ID3D11VertexShader*     m_pFbxSkinMesh_VS;
	ID3D11PixelShader*      m_pFbxSkinMesh_PS;


	// �R���X�^���g�o�b�t�@  ------------------------------------------

	// �R���X�^���g�o�b�t�@�[�f�B�X�v���C�X�����g�}�b�s���O�p   // -- 2020.1.24
	ID3D11Buffer*           m_pConstantBufferDisplace;

	// �R���X�^���g�o�b�t�@�[�G�t�F�N�g�p
	ID3D11Buffer*           m_pConstantBufferEffect;

	// �R���X�^���g�o�b�t�@�[ 3D�X�v���C�g�p
	ID3D11Buffer*			m_pConstantBufferSprite3D;

	// �R���X�^���g�o�b�t�@�[�@���b�V�� �ϊ��s��E�J���[�n���p
	ID3D11Buffer*           m_pConstantBufferWVLED;    // -- 2020.1.24

	// �R���X�^���g�o�b�t�@�[�@�{�[���s��n���p
	ID3D11Buffer*           m_pConstantBufferBone2;

	// �}�e���A�����@�n���p
	ID3D11Buffer*           m_pConstantBufferMaterial;    // -- 2020.12.15


public:
	HRESULT InitShader();
	HRESULT InitShaderSimple();
	HRESULT InitShaderSprite();

	HRESULT InitShaderFbx();
	HRESULT InitShaderDisplace();
	HRESULT InitShaderEffect();
	HRESULT InitShaderConstant();

	HRESULT MakeShader(const TCHAR ProfileName[], const TCHAR FileName[], void** ppShader, D3D11_INPUT_ELEMENT_DESC Fluid_layout[] = nullptr, UINT numElements = 0, ID3D11InputLayout** ppInputLayout = nullptr);
	HRESULT MakeConstantBuffer(UINT size, ID3D11Buffer**  ppConstantBuffer);

	CShader(CDirect3D* pD3D);
	~CShader();

};
