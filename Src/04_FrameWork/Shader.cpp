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
//                                                                              Shader.cpp
// ========================================================================================

#include "Shader.h"
#include "../06_GameLib/FbxMesh.h"

//------------------------------------------------------------------------
//
//	�V�F�[�_�[�̃R���X�g���N�^	
//
//  �����@CDirect3D* pD3D
//
//------------------------------------------------------------------------
CShader::CShader(CDirect3D* pD3D)
{
	ZeroMemory(this, sizeof(CShader));
	m_pD3D = pD3D;
}
//------------------------------------------------------------------------
//
//	�V�F�[�_�[�̃f�X�g���N�^	
//
//------------------------------------------------------------------------
CShader::~CShader()
{
	// �V�F�[�_�[�̉��  ------------------------------------------

	// �ʏ�p�̃V���v���ȃV�F�[�_�[
	SAFE_RELEASE(m_pSimple_VertexLayout); 
	SAFE_RELEASE(m_pSimple_VS);
	SAFE_RELEASE(m_pSimple_PS);

	// 3D�X�v���C�g�p�̃V�F�[�_�[
	SAFE_RELEASE(m_pSprite3D_VertexLayout);
	SAFE_RELEASE(m_pSprite3D_VS);
	SAFE_RELEASE(m_pSprite3D_PS);
	SAFE_RELEASE(m_pSprite3D_VS_BILL);

	// �f�B�X�v���[�X�����g�}�b�s���O(�g)�p�̃V�F�[�_�[
	SAFE_RELEASE(m_pDisplaceWave_VertexLayout);
	SAFE_RELEASE(m_pDisplaceWave_VS);
	SAFE_RELEASE(m_pDisplaceWave_HS);
	SAFE_RELEASE(m_pDisplaceWave_DS);
	SAFE_RELEASE(m_pDisplaceWave_PS);

	// �f�B�X�v���[�X�����g�}�b�s���O(���b�V��)�p�̃V�F�[�_�[
	SAFE_RELEASE(m_pDisplaceSkinMesh_VS);
	SAFE_RELEASE(m_pDisplaceSkinMesh_HS);
	SAFE_RELEASE(m_pDisplaceSkinMesh_DS);
	SAFE_RELEASE(m_pDisplaceSkinMesh_PS);

	SAFE_RELEASE(m_pDisplaceStaticMesh_VS);
	SAFE_RELEASE(m_pDisplaceStaticMesh_HS);
	SAFE_RELEASE(m_pDisplaceStaticMesh_DS);
	SAFE_RELEASE(m_pDisplaceStaticMesh_PS);

	// �G�t�F�N�g�p�̃V�F�[�_�[
	SAFE_RELEASE(m_pEffect3D_VertexLayout);
	SAFE_RELEASE(m_pEffect3D_VS_POINT);
	SAFE_RELEASE(m_pEffect3D_GS_POINT);
	SAFE_RELEASE(m_pEffect3D_PS);
	SAFE_RELEASE(m_pEffect3D_VertexLayout_BILL);
	SAFE_RELEASE(m_pEffect3D_VS_BILL);
	SAFE_RELEASE(m_pEffect3D_VS_BILLMESH);              // -- 2019.7.17

	// Fbx���f���@�X�^�e�B�c�N���b�V���p�̃V�F�[�_�[
	// �iNormal�}�b�s���O�j
	SAFE_RELEASE(m_pFbxStaticMesh_VertexLayout);
	SAFE_RELEASE(m_pFbxStaticMesh_VS);
	SAFE_RELEASE(m_pFbxStaticMesh_PS);

	// Fbx���f���@�X�L�����b�V���p�̃V�F�[�_�[
	// �iNormal�}�b�s���O�j
	SAFE_RELEASE(m_pFbxSkinMesh_VertexLayout);
	SAFE_RELEASE(m_pFbxSkinMesh_VS);
	SAFE_RELEASE(m_pFbxSkinMesh_PS);


	// �R���X�^���g�o�b�t�@�̉�� ---------------------------

	SAFE_RELEASE(m_pConstantBufferDisplace);
	SAFE_RELEASE(m_pConstantBufferEffect);
	SAFE_RELEASE(m_pConstantBufferSprite3D);

	SAFE_RELEASE(m_pConstantBufferWVLED);
	SAFE_RELEASE(m_pConstantBufferBone2);
	SAFE_RELEASE(m_pConstantBufferMaterial);      // -- 2020.12.15

}

//------------------------------------------------------------------------
//
//	�e��V�F�[�_�[�̍쐬	
//
//  �E�V�F�[�_�[�ƃR���X�^���g�o�b�t�@���쐬����
//  �E�e�N�X�`���[�T���v���[�ƃu�����h�X�e�[�g���쐬����
//
//  �����@�Ȃ�
//
//	�߂�l HRESULT
//		S_OK	= ����
//		E_FAIL	= �ُ�
//
//------------------------------------------------------------------------
HRESULT CShader::InitShader()
{
	//  �ʏ탁�b�V���p�̃V�F�[�_�[�쐬
	InitShaderSimple();

	//  Fbx�X�^�e�B�b�N�E�X�L�����b�V���p�̃V�F�[�_�[�쐬
	InitShaderFbx();

	//  �f�B�X�v���[�X�����g�}�b�s���O�p�̃V�F�[�_�[�쐬
	InitShaderDisplace();

	//  �G�t�F�N�g�p�̃V�F�[�_�[�쐬
	InitShaderEffect();

	//  �X�v���C�g�p�̃V�F�[�_�[�쐬
	InitShaderSprite();

	//  �R���X�^���g�o�b�t�@�쐬
	InitShaderConstant();

	return S_OK;
}
//------------------------------------------------------------------------
//
//  �ʏ�p(Simple Shader)�̃V�F�[�_�[�쐬
//
//  �����@�Ȃ�
//
//	�߂�l HRESULT
//		S_OK	= ����
//		E_FAIL	= �ُ�
//
//------------------------------------------------------------------------
HRESULT CShader::InitShaderSimple()
{

	// ���_�C���v�b�g���C�A�E�g���`
	UINT numElements = 0;
	D3D11_INPUT_ELEMENT_DESC Simplelayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	numElements = sizeof(Simplelayout) / sizeof(Simplelayout[0]);

	// �o�[�e�b�N�X�V�F�[�_�E�s�N�Z���V�F�[�_�E���_�C���v�b�g���C�A�E�g�̍쐬
	MakeShader(_T("VS"), _T("Simple_VS.cso"), (void**)&m_pSimple_VS, Simplelayout, numElements, &m_pSimple_VertexLayout);
	MakeShader(_T("PS"), _T("Simple_PS.cso"), (void**)&m_pSimple_PS);

	return S_OK;
}

//------------------------------------------------------------------------
//
//  Fbx���f���@�X�^�e�B�c�N���X�L�����b�V���p�̃V�F�[�_�[�쐬
//
//  �����@�Ȃ�
//
//	�߂�l HRESULT
//		S_OK	= ����
//		E_FAIL	= �ُ�
//
//------------------------------------------------------------------------
HRESULT CShader::InitShaderFbx()
{

	// -------------------------------------------------------------------
	// 
	// �X�^�e�B�b�N���b�V��  FbxStaticMesh
	// 
	// -------------------------------------------------------------------
	// ���_�C���v�b�g���C�A�E�g���`
	UINT numElements = 0;
	D3D11_INPUT_ELEMENT_DESC FbxStaticNM_layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },		// �v32byte
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT,0,32, D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT,0,44, D3D11_INPUT_PER_VERTEX_DATA,0 }, // �v56byte
	};
	numElements = sizeof(FbxStaticNM_layout) / sizeof(FbxStaticNM_layout[0]);

	// �o�[�e�b�N�X�V�F�[�_�E�s�N�Z���V�F�[�_�E���_�C���v�b�g���C�A�E�g�̍쐬
	MakeShader(_T("VS"), _T("FbxStaticMesh_VS.cso"), (void**)&m_pFbxStaticMesh_VS, FbxStaticNM_layout, numElements, &m_pFbxStaticMesh_VertexLayout);
	MakeShader(_T("PS"), _T("FbxStaticMesh_PS.cso"), (void**)&m_pFbxStaticMesh_PS);


	// -------------------------------------------------------------------
	// 
	// �X�L�����b�V��  FbxSkinMesh
	// 
	// -------------------------------------------------------------------
	// ���_�C���v�b�g���C�A�E�g���`
	numElements = 0;
	D3D11_INPUT_ELEMENT_DESC FbxSkinNM_layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BONE_INDEX", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BONE_WEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 64, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 76, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // �v76Byte
	};
	numElements = sizeof(FbxSkinNM_layout) / sizeof(FbxSkinNM_layout[0]);

	// �o�[�e�b�N�X�V�F�[�_�E�s�N�Z���V�F�[�_�E���_�C���v�b�g���C�A�E�g�̍쐬
	MakeShader(_T("VS"), _T("FbxSkinMesh_VS.cso"), (void**)&m_pFbxSkinMesh_VS, FbxSkinNM_layout, numElements, &m_pFbxSkinMesh_VertexLayout);
	MakeShader(_T("PS"), _T("FbxSkinMesh_PS.cso"), (void**)&m_pFbxSkinMesh_PS);

	return S_OK;
}
//------------------------------------------------------------------------
//
//  �f�B�X�v���[�X�����g�}�b�s���O�p�̃V�F�[�_�[�쐬
//
//  �����@�Ȃ�
//
//	�߂�l HRESULT
//		S_OK	= ����
//		E_FAIL	= �ُ�
//
//------------------------------------------------------------------------
HRESULT CShader::InitShaderDisplace()
{

	// -----------------------------------------------------------------------------------------------
	// �g�̃f�B�X�v���C�X�}�b�s���O DisplaceWave
	// -----------------------------------------------------------------------------------------------
	// ���_�C���v�b�g���C�A�E�g���`
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);

	// �o�[�e�b�N�X�V�F�[�_�E�s�N�Z���V�F�[�_�E���_�C���v�b�g���C�A�E�g�̍쐬
	MakeShader(_T("VS"), _T("DisplaceWave_VS.cso"), (void**)&m_pDisplaceWave_VS, layout, numElements, &m_pDisplaceWave_VertexLayout);
	MakeShader(_T("HS"), _T("DisplaceWave_HS.cso"), (void**)&m_pDisplaceWave_HS);
	MakeShader(_T("DS"), _T("DisplaceWave_DS.cso"), (void**)&m_pDisplaceWave_DS);
	MakeShader(_T("PS"), _T("DisplaceWave_PS.cso"), (void**)&m_pDisplaceWave_PS);


	// -----------------------------------------------------------------------------------------------
	// �X�^�e�B�b�N���b�V���̃f�B�X�v���C�X�����g�}�b�s���O DisplaceStaticMesh
	// -----------------------------------------------------------------------------------------------
	// �o�[�e�b�N�X�V�F�[�_�E�s�N�Z���V�F�[�_�E�n���V�F�[�_�E�h���C���V�F�[�_�̍쐬
	//  !!!!! ���_�C���v�b�g���C�A�E�g�́A�X�^�e�B�b�N���b�V���̃��C�A�E�g���g�p	
	MakeShader(_T("VS"), _T("DisplaceStaticMesh_VS.cso"), (void**)&m_pDisplaceStaticMesh_VS);
	MakeShader(_T("HS"), _T("DisplaceStaticMesh_HS.cso"), (void**)&m_pDisplaceStaticMesh_HS);
	MakeShader(_T("DS"), _T("DisplaceStaticMesh_DS.cso"), (void**)&m_pDisplaceStaticMesh_DS);
	MakeShader(_T("PS"), _T("DisplaceStaticMesh_PS.cso"), (void**)&m_pDisplaceStaticMesh_PS);

	// -----------------------------------------------------------------------------------------------
	// �X�L�����b�V���̃f�B�X�v���C�X�����g�}�b�s���O DisplaceSkinMesh
	// -----------------------------------------------------------------------------------------------
	// �o�[�e�b�N�X�V�F�[�_�E�s�N�Z���V�F�[�_�E�n���V�F�[�_�E�h���C���V�F�[�_�̍쐬
	//  !!!!! ���_�C���v�b�g���C�A�E�g�́A�X�L�����b�V���̃��C�A�E�g���g�p	
	MakeShader(_T("VS"), _T("DisplaceSkinMesh_VS.cso"), (void**)&m_pDisplaceSkinMesh_VS);
	MakeShader(_T("HS"), _T("DisplaceSkinMesh_HS.cso"), (void**)&m_pDisplaceSkinMesh_HS);
	MakeShader(_T("DS"), _T("DisplaceSkinMesh_DS.cso"), (void**)&m_pDisplaceSkinMesh_DS);
	MakeShader(_T("PS"), _T("DisplaceSkinMesh_PS.cso"), (void**)&m_pDisplaceSkinMesh_PS);


	return S_OK;
}

//------------------------------------------------------------------------
//
//  �G�t�F�N�g�p�̃V�F�[�_�[�쐬
//
//  �����@�Ȃ�
//
//	�߂�l HRESULT
//		S_OK	= ����
//		E_FAIL	= �ُ�
//
//------------------------------------------------------------------------
HRESULT CShader::InitShaderEffect()
{

	// -------------------------------------------------------------------
	// 
	// �p�[�e�B�N���̃V�F�[�_�[
	// 
	// -------------------------------------------------------------------
	// ���_�C���v�b�g���C�A�E�g���`
	UINT numElements = 0;
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	numElements = sizeof(layout) / sizeof(layout[0]);

	// �o�[�e�b�N�X�V�F�[�_�E�W�I���g���V�F�[�_�E���_�C���v�b�g���C�A�E�g�̍쐬
	MakeShader(_T("VS"), _T("Effect3D_VS_POINT.cso"), (void**)&m_pEffect3D_VS_POINT, layout, numElements, &m_pEffect3D_VertexLayout);
	MakeShader(_T("GS"), _T("Effect3D_GS_POINT.cso"), (void**)&m_pEffect3D_GS_POINT);


	// -----------------------------------------------------------------------------------------------------
	// 
	// �r���{�[�h�̃V�F�[�_�[
	// 
	// -----------------------------------------------------------------------------------------------------
	//���_�C���v�b�g���C�A�E�g���`
	D3D11_INPUT_ELEMENT_DESC layoutbill[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	numElements = sizeof(layoutbill) / sizeof(layoutbill[0]);

	// �o�[�e�b�N�X�V�F�[�_�E���_�C���v�b�g���C�A�E�g�̍쐬
	MakeShader(_T("VS"), _T("Effect3D_VS_BILL.cso"), (void**)&m_pEffect3D_VS_BILL, layoutbill, numElements, &m_pEffect3D_VertexLayout_BILL);

	// -----------------------------------------------------------------------------------------------------
	// 
	// �r���{�[�h���b�V���̃V�F�[�_�[
	// 
	// -----------------------------------------------------------------------------------------------------
	// �o�[�e�b�N�X�V�F�[�_�̍쐬
	MakeShader(_T("VS"), _T("Effect3D_VS_BILLMESH.cso"), (void**)&m_pEffect3D_VS_BILLMESH);

	// -----------------------------------------------------------------------------------------------------
	// 
	// ���ʂ̃V�F�[�_�[
	// 
	// -----------------------------------------------------------------------------------------------------
	// �s�N�Z���V�F�[�_�̍쐬
	MakeShader(_T("PS"), _T("Effect3D_PS.cso"), (void**)&m_pEffect3D_PS);

	return S_OK;
}

//------------------------------------------------------------------------
//
//  �X�v���C�g�p�̃V�F�[�_�[�쐬
//
//  �����@�Ȃ�
//
//	�߂�l HRESULT
//		S_OK	= ����
//		E_FAIL	= �ُ�
//
//------------------------------------------------------------------------
HRESULT CShader::InitShaderSprite()
{
	// ���_�C���v�b�g���C�A�E�g���`	
	D3D11_INPUT_ELEMENT_DESC layout_sprite[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof(layout_sprite) / sizeof(layout_sprite[0]);

	// �o�[�e�b�N�X�V�F�[�_�E�s�N�Z���V�F�[�_�E���_�C���v�b�g���C�A�E�g�̍쐬
	MakeShader(_T("VS"), _T("Sprite3D_VS.cso"), (void**)&m_pSprite3D_VS, layout_sprite, numElements, &m_pSprite3D_VertexLayout);
	MakeShader(_T("VS"), _T("Sprite3D_VS_BILL.cso"), (void**)&m_pSprite3D_VS_BILL);
	MakeShader(_T("PS"), _T("Sprite3D_PS.cso"), (void**)&m_pSprite3D_PS);

	return S_OK;
}

//------------------------------------------------------------------------
//
//  �V�F�[�_�[�̍쐬�֐�
//
//  ����	TCHAR ProfileName[]	�쐬����V�F�[�_�[���
//								(VS,PS,GS,HS,DS,CS)
//			TCHAR FileName[]	�g�k�r�k�t�@�C����
//			void** ppShader		�쐬����V�F�[�_�[(OUT)
//			D3D11_INPUT_ELEMENT_DESC Fluid_layout[]	���_���C�A�E�g��`(�ȗ���)
//			UINT numElements						���_���C�A�E�g�G�������g��(�ȗ���)
//			ID3D11InputLayout** ppInputLayout		�쐬���钸�_���C�A�E�g(OUT)(�ȗ���)
//
//	�߂�l HRESULT
//		S_OK	= ����
//		E_FAIL	= �ُ�
//
//------------------------------------------------------------------------
HRESULT CShader::MakeShader(const TCHAR ProfileName[], const TCHAR FileName[], void** ppShader, D3D11_INPUT_ELEMENT_DESC Fluid_layout[], UINT numElements, ID3D11InputLayout** ppInputLayout)
{
	// �R���p�C���ς݃V�F�[�_�̓ǂݍ��ݔz��
	BYTE* pCso = nullptr;
	DWORD dwCsoSize = 0;

	// �R���p�C���ς݃V�F�[�_�̓ǂݍ��݂�����
	m_pD3D->ReadCso(FileName, &pCso, &dwCsoSize);

	// �V�F�[�_�[��ނ��Ƃ̍쐬����
	if (_tcscmp(ProfileName, _T("VS")) == 0)	// �o�[�e�b�N�X�V�F�[�_�[
	{
		if (FAILED(m_pD3D->m_pDevice->CreateVertexShader(pCso, dwCsoSize, nullptr, (ID3D11VertexShader**)ppShader)))
		{
			SAFE_DELETE_ARRAY(pCso);
			MessageBox(0, _T("�o�[�e�b�N�X�V�F�[�_�[�쐬���s"), FileName, MB_OK);
			return E_FAIL;
		}
		if (ppInputLayout)	// ���_�C���v�b�g���C�A�E�g���쐬����Ƃ�
		{
			// ���_�C���v�b�g���C�A�E�g���쐬
			if (FAILED(m_pD3D->m_pDevice->CreateInputLayout(Fluid_layout, numElements, pCso, dwCsoSize, ppInputLayout)))
			{
				MessageBox(0, _T("�C���v�b�g ���C�A�E�g�쐬���s"), FileName, MB_OK);
				return E_FAIL;
			}
		}
	}
	else if (_tcscmp(ProfileName, _T("PS")) == 0)	// �s�N�Z���V�F�[�_�[
	{
		if (FAILED(m_pD3D->m_pDevice->CreatePixelShader(pCso, dwCsoSize, nullptr, (ID3D11PixelShader**)ppShader)))
		{
			SAFE_DELETE_ARRAY(pCso);
			MessageBox(0, _T("�s�N�Z���V�F�[�_�[�쐬���s"), FileName, MB_OK);
			return E_FAIL;
		}
	}
	else if (_tcscmp(ProfileName, _T("GS")) == 0)	// �W�I���g���V�F�[�_�[
	{
		if (FAILED(m_pD3D->m_pDevice->CreateGeometryShader(pCso, dwCsoSize, nullptr, (ID3D11GeometryShader**)ppShader)))
		{
			SAFE_DELETE_ARRAY(pCso);
			MessageBox(0, _T("�W�I���g���V�F�[�_�[�쐬���s"), FileName, MB_OK);
			return E_FAIL;
		}
	}
	else if (_tcscmp(ProfileName, _T("HS")) == 0)	// �n���V�F�[�_�[
	{
		if (FAILED(m_pD3D->m_pDevice->CreateHullShader(pCso, dwCsoSize, nullptr, (ID3D11HullShader**)ppShader)))
		{
			SAFE_DELETE_ARRAY(pCso);
			MessageBox(0, _T("�n���V�F�[�_�[�쐬���s"), FileName, MB_OK);
			return E_FAIL;
		}
	}
	else if (_tcscmp(ProfileName, _T("DS")) == 0)	// �h���C���V�F�[�_�[
	{
		if (FAILED(m_pD3D->m_pDevice->CreateDomainShader(pCso, dwCsoSize, nullptr, (ID3D11DomainShader**)ppShader)))
		{
			SAFE_DELETE_ARRAY(pCso);
			MessageBox(0, _T("�h���C���V�F�[�_�[�쐬���s"), FileName, MB_OK);
			return E_FAIL;
		}
	}
	else if (_tcscmp(ProfileName, _T("CS")) == 0)	// �R���s���[�g�V�F�[�_
	{
		if (FAILED(m_pD3D->m_pDevice->CreateComputeShader(pCso, dwCsoSize, nullptr, (ID3D11ComputeShader**)ppShader)))
		{
			SAFE_DELETE_ARRAY(pCso);
			MessageBox(0, _T("�R���s���[�g�V�F�[�_�쐬���s"), FileName, MB_OK);
			return E_FAIL;
		}
	}
	else {
		SAFE_DELETE_ARRAY(pCso);
		MessageBox(0, _T("�V�F�[�_��ގw��G���["), ProfileName, MB_OK);
		return E_FAIL;
	}

	SAFE_DELETE_ARRAY(pCso);
	return S_OK;

}

//------------------------------------------------------------------------
//
//  �e��R���X�^���g�o�b�t�@�[�쐬
//
//  �����@�Ȃ�
//
//	�߂�l HRESULT
//		S_OK	= ����
//		E_FAIL	= �ُ�
//
//------------------------------------------------------------------------
HRESULT CShader::InitShaderConstant()
{

	// �f�B�X�v���C�X�����g�}�b�s���O�p �R���X�^���g�o�b�t�@�[�쐬
	MakeConstantBuffer(sizeof(CONSTANT_BUFFER_DISPLACE), &m_pConstantBufferDisplace);

	// �G�t�F�N�g�p �R���X�^���g�o�b�t�@�[�쐬
	MakeConstantBuffer(sizeof(CONSTANT_BUFFER_EFFECT), &m_pConstantBufferEffect);

	// �X�v���C�g�p �R���X�^���g�o�b�t�@�[�쐬�@�����ł͕ϊ��s��n���p
	MakeConstantBuffer(sizeof(CONSTANT_BUFFER_SPRITE), &m_pConstantBufferSprite3D);

	// Fbx�R���X�^���g�o�b�t�@�[�쐬�@�����ł͕ϊ��s��n���p
	MakeConstantBuffer(sizeof(CONSTANT_BUFFER_WVLED), &m_pConstantBufferWVLED);

	// Fbx�R���X�^���g�o�b�t�@�[�쐬�@�����ł̓{�[���s��n���p
	MakeConstantBuffer(sizeof(MATRIX4X4) * MAX_BONES, &m_pConstantBufferBone2);

	// �R���X�^���g�o�b�t�@�[�쐬�@�}�e���A���n���p                                    // -- 2020.12.15
	MakeConstantBuffer(sizeof(CONSTANT_BUFFER_MATERIAL), &m_pConstantBufferMaterial);

	return S_OK;
}

//------------------------------------------------------------------------
//
//  �R���X�^���g�o�b�t�@�[�̍쐬�֐�
//
//  ����	UINT	size						�쐬����R���X�^���g�o�b�t�@�[�̃T�C�Y
//			ID3D11Buffer**  pppConstantBuffer	�쐬����R���X�^���g�o�b�t�@�[(OUT)
//
//	�߂�l HRESULT
//		S_OK	= ����
//		E_FAIL	= �ُ�
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
		MessageBox(0, _T("�R���X�^���g�o�b�t�@�[ �쐬���s"), nullptr, MB_OK);
		return E_FAIL;
	}
	return S_OK;
}

