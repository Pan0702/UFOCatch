//=============================================================================
//
//  �o�E���f�B���O�{�b�N�X                             ver 3.3      2024.3.23
//
//   �|���S���̕\�ʔ����ύX�B�E����\�Ƃ���
//									                                 BBox.cpp
//=============================================================================
#include "BBox.h"
#include "../GameMain/GameMain.h"
#pragma warning(disable : 6387)


// -----------------------------------------------------------------------
// 
// OBB (�L�����E�{�b�N�X:Oriented Bounding Box)
// 
// -----------------------------------------------------------------------
//------------------------------------------------------------------------ // -- 2024.3.23
//
// �R���X�g���N�^
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
// �R���X�g���N�^
//
//  CShader* pShader         �V�F�[�_�[
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

	m_mWorld = XMMatrixIdentity();		// ���[���h�}�g���N�X�̏�����

	m_vDiffuse = VECTOR4(1.0f, 1.0f, 1.0f, 0.5f);    // �f�B�t���[�Y�F   // -- 2021.1.11

}
//------------------------------------------------------------------------
//
// �R���X�g���N�^
//
//  CShader* pShader         �V�F�[�_�[
//  const VECTOR3& vMin      �o�E���f�B���O�{�b�N�X�̍ŏ��l
//  const VECTOR3& vMax      �o�E���f�B���O�{�b�N�X�̍ő�l
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

	m_mWorld = XMMatrixIdentity();		// ���[���h�}�g���N�X�̏�����

	InitBBox(vMin, vMax);

	m_vDiffuse = VECTOR4(1.0f, 1.0f, 1.0f, 0.5f);    // �f�B�t���[�Y�F   // -- 2021.1.11

}
//------------------------------------------------------------------------
//
// �f�X�g���N�^
//
//------------------------------------------------------------------------
CBBox::~CBBox()
{
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pIndexBuffer);
	SAFE_DELETE_ARRAY(m_pIndexBuffer);
}

//------------------------------------------------------------------------
//	�o�E���f�B���O�{�b�N�X�̏������Ɛ���
// 
//  const VECTOR3& vMin      �o�E���f�B���O�{�b�N�X�̍ŏ��l
//  const VECTOR3& vMax      �o�E���f�B���O�{�b�N�X�̍ő�l
// 
//  �ŏ��l�`�ő�l�̑傫���̃o�E���f�B���O�{�b�N�X���쐬����
// 
//  �߂�l�@�Ȃ�
//------------------------------------------------------------------------
//BBox�̏�����
void CBBox::InitBBox(const VECTOR3& vMin, const VECTOR3& vMax)
{

	m_vMax = vMax;
	m_vMin = vMin;

	//���x�N�g�� ���̒����i���̏ꍇ�{�b�N�X�̊e���a�j������������
	m_fLengthX = (vMax.x - vMin.x) / 2;
	m_fLengthY = (vMax.y - vMin.y) / 2;
	m_fLengthZ = (vMax.z - vMin.z) / 2;

	// �\���p�̃{�b�N�X���b�V���̐���
	InitMesh();

}

//------------------------------------------------------------------------
//	�o�E���f�B���O�{�b�N�X�̒��_�ƃC���f�b�N�X�f�[�^���쐬����
// 
//  VECTOR3* pVertex      ���_�f�[�^(OUT)�@        VECTOR3�~8
//  DWORD*   pIndex       �C���f�b�N�X�f�[�^(OUT)�@DWORD�~36
// 
//  �߂�l�@�Ȃ�
//------------------------------------------------------------------------
void CBBox::MakeVertexIndex(VECTOR3* pVertex, DWORD* pIndex)
{
	//�o�[�e�b�N�X�f�[�^�쐬
	pVertex[0] = VECTOR3(m_vMin.x, m_vMin.y, m_vMin.z);    //���_0  ��ʁ@���O
	pVertex[1] = VECTOR3(m_vMin.x, m_vMin.y, m_vMax.z);    //���_1  ��ʁ@����
	pVertex[2] = VECTOR3(m_vMax.x, m_vMin.y, m_vMax.z);    //���_2  ��ʁ@�E��
	pVertex[3] = VECTOR3(m_vMax.x, m_vMin.y, m_vMin.z);    //���_3  ��ʁ@�E�O
	pVertex[4] = VECTOR3(m_vMin.x, m_vMax.y, m_vMin.z);    //���_4  ��ʁ@���O
	pVertex[5] = VECTOR3(m_vMin.x, m_vMax.y, m_vMax.z);    //���_5  ��ʁ@����
	pVertex[6] = VECTOR3(m_vMax.x, m_vMax.y, m_vMax.z);    //���_6  ��ʁ@�E��
	pVertex[7] = VECTOR3(m_vMax.x, m_vMax.y, m_vMin.z);    //���_7  ��ʁ@�E�O

	// �C���f�b�N�X�f�[�^�쐬�@3�p�`���X�g�~2�~6
	DWORD pIndexConst[] = {


		//	���_�E����\�ʂƂ���			      // -- 2024.3.23
		0, 2, 1,   0, 3, 2,       // ���
		4, 5, 6,   4, 6, 7,       // ���
		0, 4, 7,   0, 7, 3,       // �O��
		2, 6, 5,   2, 5, 1,       // �w��
		1, 5, 4,   1, 4, 0,       // ����
		3, 7, 6,   3, 6, 2,       // �E��

	};
	memcpy_s(pIndex, sizeof(DWORD) * 36, pIndexConst, sizeof(DWORD) * 36);

}
//------------------------------------------------------------------------
//	�\���p�̃{�b�N�X���b�V�����쐬����
// 
//  �߂�l�@HRESULT	����:S_OK�@�@�ُ�:E_FAIL
//------------------------------------------------------------------------
// �\���p�̃{�b�N�X���b�V�����쐬����
HRESULT CBBox::InitMesh()
{

	// ���_�f�[�^
	VECTOR3 pVertexConst[8];
	// ���_�@���f�[�^
	VECTOR3 pNormalConst[] =
	{
		VECTOR3(0, -1, 0),    //���_0  ��ʁ@���O
		VECTOR3(0, -1, 0),    //���_1  ��ʁ@����
		VECTOR3(0, -1, 0),    //���_2  ��ʁ@�E��
		VECTOR3(0, -1, 0),    //���_3  ��ʁ@�E�O
		VECTOR3(0,  1, 0),    //���_4  ��ʁ@���O
		VECTOR3(0,  1, 0),    //���_5  ��ʁ@����
		VECTOR3(0,  1, 0),    //���_6  ��ʁ@�E��
		VECTOR3(0,  1, 0),    //���_7  ��ʁ@�E�O
	};

	// �e�N�X�`�����W�f�[�^
	VECTOR2 pTexConst[] =
	{
		VECTOR2(0, 1),    //���_0  ��ʁ@���O
		VECTOR2(1, 1),    //���_1  ��ʁ@����
		VECTOR2(0, 1),    //���_2  ��ʁ@�E��
		VECTOR2(1, 1),    //���_3  ��ʁ@�E�O
		VECTOR2(0, 0),    //���_4  ��ʁ@���O
		VECTOR2(1, 0),    //���_5  ��ʁ@����
		VECTOR2(0, 0),    //���_6  ��ʁ@�E��
		VECTOR2(1, 0),    //���_7  ��ʁ@�E�O
	};

	// �C���f�b�N�X�f�[�^�@3�p�`���X�g�~2�~6
	DWORD pIndex[36];

	// �{�b�N�X�̒��_�ƃC���f�b�N�X�f�[�^���쐬
	MakeVertexIndex(pVertexConst, pIndex);

	// �K�v���̒��_�f�[�^���i�[����z����m�ۂ���
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

	// �o�[�e�b�N�X�o�b�t�@�����łɍ쐬�ς݂��ǂ����`�F�b�N����
	if (m_pVertexBuffer == nullptr)
	{
		//�o�[�e�b�N�X�o�b�t�@�[���쐬
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
			MessageBox(0, _T("�a�a�����@�o�[�e�b�N�X�o�b�t�@ �쐬���s"), nullptr, MB_OK);
			return E_FAIL;
		}
	}
	else {
		// ���łɍ쐬�ς݂̂��߁A�o�[�e�b�N�X�o�b�t�@�̏�������������
		if (SUCCEEDED(m_pD3D->m_pDeviceContext->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr)))
		{
			memcpy(msr.pData, pVertices, sizeof(BBOX_VERTEX) * 8); // 8���_���R�s�[
			m_pD3D->m_pDeviceContext->Unmap(m_pVertexBuffer, 0);
		}
		else {
			MessageBox(0, _T("�a�a�����@�o�[�e�b�N�X�o�b�t�@ �����������s"), nullptr, MB_OK);
			return E_FAIL;

		}
	}

	// �C���f�b�N�X�o�b�t�@�����łɍ쐬�ς݂��ǂ����`�F�b�N����
	// ���łɍ쐬�ς݂̂Ƃ��A���������̂̂��߃C���f�b�N�X�o�b�t�@�̏��������͕s�v
	if (m_pIndexBuffer == nullptr)
	{
		//�C���f�b�N�X�o�b�t�@�[���쐬
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
			MessageBox(0, _T("�a�a�����@�C���f�b�N�X�o�b�t�@ �쐬���s"), nullptr, MB_OK);
			return E_FAIL;
		}
	}

	return S_OK;

}

//------------------------------------------------------------------------
//	�o�E���f�B���O�{�b�N�X�Ƒ���̃o�E���f�B���O�{�b�N�X�Ƃ̐ڐG����
// 
//	CBBox* pOtherBBox      ����̃o�E���f�B���O�{�b�N�X(IN/OUT)
//  VECTOR3* vHit          �q�b�g�����Ƃ��̃q�b�g�ʒu(OUT)
//  VECTOR3* vNrm          �q�b�g�����Ƃ��̖@�����W(OUT)
//  
//  �߂�l�@bool	ret�@  false:�q�b�g���Ȃ��Ƃ��@true:�q�b�g�����Ƃ�
//------------------------------------------------------------------------
bool CBBox::OBBCollisionDetection( CBBox* pOtherBBox, VECTOR3* vHit, VECTOR3* vNrm)
{
	MATRIX4X4 mOtherWorld = pOtherBBox->m_mWorld;

	//�{�b�N�X�̋����x�N�g���i���S�ƒ��S�����񂾃x�N�g���j�����߂�
	// ��BBOX(BBoxA)�̒��S�_�܂ł̈ړ��}�g���b�N�X���쐬����
	MATRIX4X4 mWorldCenterA;
	mWorldCenterA = XMMatrixTranslation( m_fLengthX + m_vMin.x, m_fLengthY + m_vMin.y, m_fLengthZ + m_vMin.z);
	mWorldCenterA = mWorldCenterA * m_mWorld;		// ���S�_�̃��[���h�}�g���b�N�X�����߂�
	// ��BBOX(BBoxB)�̒��S�_�܂ł̈ړ��}�g���b�N�X���쐬����
	MATRIX4X4 mWorldCenterB;
	mWorldCenterB = XMMatrixTranslation( pOtherBBox->m_fLengthX + pOtherBBox->m_vMin.x, 
							pOtherBBox->m_fLengthY + pOtherBBox->m_vMin.y, pOtherBBox->m_fLengthZ + pOtherBBox->m_vMin.z);
	mWorldCenterB = mWorldCenterB * mOtherWorld;	// ���S�_�̃��[���h�}�g���b�N�X�����߂�
	// �{�b�N�X�̒��S�_�Ԃ̋����x�N�g�������߂�
	VECTOR3 vDistance = VECTOR3(mWorldCenterB._41, mWorldCenterB._42, mWorldCenterB._43)
	                      - VECTOR3(mWorldCenterA._41, mWorldCenterA._42, mWorldCenterA._43);

	// ��]�s��(�ʒu��r�����A��]�݂̂̍s��)�����߂�
	MATRIX4X4 mTrans;
	mTrans = XMMatrixTranslation(-m_mWorld._41, -m_mWorld._42, -m_mWorld._43);
	MATRIX4X4 mRot = m_mWorld * mTrans;

	mTrans = XMMatrixTranslation( -mOtherWorld._41, -mOtherWorld._42, -mOtherWorld._43);
	MATRIX4X4 mOtherRot = mOtherWorld * mTrans;

	//������
	VECTOR3 vSeparate;
	//���ꂼ��̃��[�J����ꎲ�x�N�g���ɁA���ꂼ��̉�]�𔽉f������
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

	//�{�b�N�XA(��BBOX)�̃��[�J����ꎲ����ɂ����A�h�e�h�̎Z�o�i3�p�^�[���j
	{
		//X���𕪗����Ƃ����ꍇ
		if (!CompareLength(this, pOtherBBox, &m_vAxisX, &vDistance)) return false;
		//Y���𕪗����Ƃ����ꍇ
		if (!CompareLength(this, pOtherBBox, &m_vAxisY, &vDistance)) return false;
		//Z���𕪗����Ƃ����ꍇ
		if (!CompareLength(this, pOtherBBox, &m_vAxisZ, &vDistance)) return false;
	}
	//�{�b�N�XB(��BBOX)�̃��[�J����ꎲ����ɂ����A�h�e�h�̎Z�o�i3�p�^�[���j
	{
		//X���𕪗����Ƃ����ꍇ
		if (!CompareLength(this, pOtherBBox, &pOtherBBox->m_vAxisX, &vDistance)) return false;
		//Y���𕪗����Ƃ����ꍇ
		if (!CompareLength(this, pOtherBBox, &pOtherBBox->m_vAxisY, &vDistance)) return false;
		//Z���𕪗����Ƃ����ꍇ
		if (!CompareLength(this, pOtherBBox, &pOtherBBox->m_vAxisZ, &vDistance)) return false;
	}
	//���݂��̊�ꎲ���m�̊O�σx�N�g������ɂ����A�h�e�h�̎Z�o�i9�p�^�[���j
	{
		//�{�b�N�XA(��BBOX)��X��
		{
			//�Ɓ@�{�b�N�XB(��BBOX)��X���Ƃ̊O�σx�N�g���𕪗����Ƃ����ꍇ
			vSeparate = cross(m_vAxisX, pOtherBBox->m_vAxisX);
			if (!CompareLength(this, pOtherBBox, &vSeparate, &vDistance)) return false;
			//�Ɓ@�{�b�N�XB(��BBOX)��Y���Ƃ̊O�σx�N�g���𕪗����Ƃ����ꍇ
			vSeparate = cross(m_vAxisX, pOtherBBox->m_vAxisY);
			if (!CompareLength(this, pOtherBBox, &vSeparate, &vDistance)) return false;
			//�Ɓ@�{�b�N�XB(��BBOX)��Z���Ƃ̊O�σx�N�g���𕪗����Ƃ����ꍇ
			vSeparate = cross(m_vAxisX, pOtherBBox->m_vAxisZ);
			if (!CompareLength(this, pOtherBBox, &vSeparate, &vDistance)) return false;
		}
		//�{�b�N�XA(��BBOX)��Y��
		{
			//�Ɓ@�{�b�N�XB(��BBOX)��X���Ƃ̊O�σx�N�g���𕪗����Ƃ����ꍇ
			vSeparate = cross( m_vAxisY, pOtherBBox->m_vAxisX);
			if (!CompareLength(this, pOtherBBox, &vSeparate, &vDistance)) return false;
			//�Ɓ@�{�b�N�XB(��BBOX)��Y���Ƃ̊O�σx�N�g���𕪗����Ƃ����ꍇ
			vSeparate = cross( m_vAxisY, pOtherBBox->m_vAxisY);
			if (!CompareLength(this, pOtherBBox, &vSeparate, &vDistance)) return false;
			//�Ɓ@�{�b�N�XB(��BBOX)��Z���Ƃ̊O�σx�N�g���𕪗����Ƃ����ꍇ
			vSeparate = cross( m_vAxisY, pOtherBBox->m_vAxisZ);
			if (!CompareLength(this, pOtherBBox, &vSeparate, &vDistance)) return false;
		}
		//�{�b�N�XA(��BBOX)��Z��
		{
			//�Ɓ@�{�b�N�XB(��BBOX)��X���Ƃ̊O�σx�N�g���𕪗����Ƃ����ꍇ
			vSeparate = cross( m_vAxisZ, pOtherBBox->m_vAxisX);
			if (!CompareLength(this, pOtherBBox, &vSeparate, &vDistance)) return false;
			//�Ɓ@�{�b�N�XB(��BBOX)��Y���Ƃ̊O�σx�N�g���𕪗����Ƃ����ꍇ
			vSeparate = cross( m_vAxisZ, pOtherBBox->m_vAxisY);
			if (!CompareLength(this, pOtherBBox, &vSeparate, &vDistance)) return false;
			//�Ɓ@�{�b�N�XB(��BBOX)��Z���Ƃ̊O�σx�N�g���𕪗����Ƃ����ꍇ
			vSeparate = cross( m_vAxisZ, pOtherBBox->m_vAxisZ);
			if (!CompareLength(this, pOtherBBox, &vSeparate, &vDistance)) return false;
		}
	}

	// �q�b�g�����Ƃ�
	MATRIX4X4 mHitWorld;
	// BBox���S�_�̍��W���q�b�g�ʒu�Ƃ���
	mHitWorld = XMMatrixTranslation((pOtherBBox->m_vMax.x + pOtherBBox->m_vMin.x) / 2, 
									(pOtherBBox->m_vMax.y + pOtherBBox->m_vMin.y) / 2, 
									(pOtherBBox->m_vMax.z + pOtherBBox->m_vMin.z) / 2);
	mHitWorld = mHitWorld * pOtherBBox->m_mWorld;
	*vHit = GetPositionVector(mHitWorld);
	*vNrm = VECTOR3(0.0f, 1.0f, 0.0f);

	return true;
}
//------------------------------------------------------------------------
//	�����x�N�g���𕪗�����ɓ��e���A2�̉e������Ă��邩�ǂ������ׂ鏈��
// 
//  const CBBox* pBBoxA            �o�E���f�B���O�{�b�N�X�`
//  const CBBox* pBBoxB            �o�E���f�B���O�{�b�N�X�a
//  const VECTOR3* pvSeparate      ������
//  const VECTOR3* pvDistance      �{�b�N�X�̒��S�_�Ԃ̋���
//  
//  �߂�l�@bool	ret�@  false:����Ă���Ƃ��@true:�ڐG���Ă���Ƃ�
//------------------------------------------------------------------------
bool CBBox::CompareLength(const CBBox* pBBoxA, const CBBox* pBBoxB, const VECTOR3* pvSeparate, const VECTOR3* pvDistance)
{
	//�h��������ł́h�{�b�N�X�`�̒��S����{�b�N�X�a�̒��S�܂ł̋���
	FLOAT fDistance = fabsf(dot(*pvDistance, *pvSeparate));
	//��������Ń{�b�N�XA�̒��S����ł������{�b�N�XA�̒��_�܂ł̋���
	FLOAT fShadowA = 0;
	//��������Ń{�b�N�XB�̒��S����ł������{�b�N�XB�̒��_�܂ł̋���
	FLOAT fShadowB = 0;
	//���ꂼ��̃{�b�N�X�́h�e�h���Z�o
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
//	�o�E���f�B���O�{�b�N�X�ƈړ������i���C�j�Ƃ̐ڐG����
// 
//	const MATRIX4X4&  mLay       �����ړ���̃��[���h�}�g���b�N�X
//  const MATRIX4X4&  mLayOld    �����ړ��O�̃��[���h�}�g���b�N�X
//  VECTOR3* vHit                �q�b�g�����Ƃ��̃q�b�g�ʒu(OUT)
//  VECTOR3* vNrm                �q�b�g�����Ƃ��̖@�����W(OUT)
//  
//  �߂�l�@bool	ret�@  false:�q�b�g���Ȃ��Ƃ��@true:�q�b�g�����Ƃ�
//------------------------------------------------------------------------
bool CBBox::OBBCollisionLay( const MATRIX4X4& mLay, const MATRIX4X4& mLayOld, VECTOR3* vHit, VECTOR3* vNrm)
{
	return OBBCollisionLay( GetPositionVector(mLay), GetPositionVector(mLayOld), vHit, vNrm );
}

//------------------------------------------------------------------------  // -- 2019.10.8
//	�o�E���f�B���O�{�b�N�X�ƈړ������i���C�j�Ƃ̐ڐG����
// 
//	const VECTOR3&  vNow      �����ړ���̍��W
//  const VECTOR3&  vOld      �����ړ��O�̍��W
//  VECTOR3* vHit             �q�b�g�����Ƃ��̃q�b�g�ʒu(OUT)
//  VECTOR3* vNrm             �q�b�g�����Ƃ��̖@�����W(OUT)
//  
//  �߂�l�@bool	ret�@  false:�q�b�g���Ȃ��Ƃ��@true:�q�b�g�����Ƃ�
//------------------------------------------------------------------------
bool CBBox::OBBCollisionLay( const VECTOR3&  vNow, const VECTOR3& vOld, VECTOR3* vHit, VECTOR3* vNrm)
{

	bool	ret = false;
	int		i;

	VECTOR3 vFaceNorm;
	float    fNowDist, fOldDist, fLayDist;
	float    fLenMin = 9999999.0f;				// ��_�ƈړ��O�_�Ƃ̋����̍ŏ��l

	// �߂�l�̃N�����[
	vHit->x = 0.0f;
	vHit->y = 0.0f;
	vHit->z = 0.0f;

	// ���_�f�[�^
	VECTOR3 pVertex[8];
	// �C���f�b�N�X�f�[�^�@3�p�`���X�g�~2�~6
	DWORD pIndex[36];
	// �o�E���f�B���O�{�b�N�X�̒��_�ƃC���f�b�N�X�f�[�^���쐬
	MakeVertexIndex(pVertex, pIndex);

	// ���_�f�[�^�����[���h���W�ɕϊ�����
	for (i = 0; i < 8; i++)
	{
		MATRIX4X4 mTemp;
		mTemp = XMMatrixTranslation(pVertex[i].x, pVertex[i].y, pVertex[i].z);
		mTemp = mTemp * m_mWorld;
		pVertex[i].x = mTemp._41;
		pVertex[i].y = mTemp._42;
		pVertex[i].z = mTemp._43;
	}

	// �o�E���f�B���O�{�b�N�X�̂U�ʂP�Q�|���S���ƒ����Ƃ̌���������s��
	for (i = 0; i<12; i++)
	{
		VECTOR3 vVert[3] = { VECTOR3(0,0,0) };
		// �R�p�`�|���S���̒��_�̒l�𓾂�
		vVert[0] = pVertex[pIndex[i * 3 + 0]];
		vVert[1] = pVertex[pIndex[i * 3 + 1]];
		vVert[2] = pVertex[pIndex[i * 3 + 2]];
		// �ʖ@�����쐬����
		vFaceNorm = normalize(cross(vVert[1] - vVert[0], vVert[2] - vVert[0]));   // �E����\�Ƃ���	    // -- 2024.3.23
		//vFaceNorm = normalize(cross(vVert[2] - vVert[0], vVert[1] - vVert[0]));     // ������\�Ƃ���

		// �����̂R�p�`�|���S���@�������̋��������߂�
		float   fFaceDist = dot(vFaceNorm, vVert[0]);		// ���_����O�p�`���ʂ܂ł̋���
		fNowDist = dot(vFaceNorm, vNow) - fFaceDist;		// �����ړ���_�ƎO�p�`���ʂƂ̋����B���̎��͕\���A���̎��͗���
		fOldDist = dot(vFaceNorm, vOld) - fFaceDist;		// �����ړ��O�_�ƎO�p�`���ʂƂ̋����B���̎��͕\���A���̎��͗���
		fLayDist = fOldDist - fNowDist;						// �����ړ��x�N�g��Lay�̖@�������̋���

		// �R�p�`�|���S���ƒ����i���C�j�Ƃ̐ڐG������s��
		if (fNowDist <= 0.0f && fOldDist >= 0.0f)	// �����ړ��O�_���O�p�`���ʂ̕\�ŁA�ړ���_�����ʂ̗����̎��̂ݔ���
		{
			if (fLayDist != 0.0f)		// �����ړ��x�N�g���̖@�������̋������O�̂Ƃ��́A�����ƎO�p�`���ʂ����s�Ȃ̂őΏۊO
			{
				// ���ʂƂ̌�_vInsPt�����߂�
				//   �@�@�ړ��x�N�g���̌�_����ړ���_�܂ł̋����̔䗦�����߂�@�@�@�@�@fNowDist / fLayDist
				//   �A�@��_����ړ���_�܂ł̈ړ��x�N�g�������߂�@�@�@�@�@�@�@�@�@�@�@(vOld - vNow) * fNowDist / fLayDist
				//   �B�@��L�A���ړ���_����������Ƃɂ���āA��_�̃x�N�g�������߂�@�@vNow - ( (vOld - vNow) * fNowDist / fLayDist)
				VECTOR3 vInsPt = vNow - ((vOld - vNow) * fNowDist / fLayDist);

				// �R�p�`�|���S���ƐڐG���Ă��邩�ǂ����̔���
				//   ��_�����_�Ƃ����Ƃ��A�R�p�`�|���S���̊e���_�ƌ��_�Ƃ̊p�x�����v����֐�AddAngle�ł���
				//   ���̊֐��̖߂�l���R�U�O���i�Q�΁j�̎��́A�ڐG�_���R�̂R�c�x�N�g���̒��ɂ���
				//   �������A�덷���l�����Q�΂ł͂Ȃ�1.99f * �΂Ŕ��f���Ă���B
				if (AddAngle(vVert[0] - vInsPt, vVert[1] - vInsPt, vVert[2] - vInsPt) >= 1.99f * XM_PI)
				{
					ret = true;
					if (fLenMin > magnitude(vOld - vInsPt))   // ���߂���_��T��
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
//	�o�E���f�B���O�{�b�N�X�ƎO�p�`�Ƃ̐ڐG����
// 
//	const VECTOR3* pTri      ����̎O�p�`
//  const MATRIX4X4& mWorld  ����̎O�p�`�̃��[���h�}�g���b�N�X
//  VECTOR3* vHit            �q�b�g�����Ƃ��̃q�b�g�ʒu(OUT)
//  
//  �߂�l�@bool	ret�@  false:�q�b�g���Ȃ��Ƃ��@true:�q�b�g�����Ƃ�
//------------------------------------------------------------------------
bool CBBox::OBBCollisionTri(const VECTOR3* pTri, const MATRIX4X4& mWorld, VECTOR3* vhit)
{
	VECTOR3 vTri[3] = { VECTOR3(0,0,0) };

	// �O�p�`�̒��_�����[���h���W�ϊ�����
	vTri[0] = XMVector3TransformCoord(*(pTri + 0), mWorld);
	vTri[1] = XMVector3TransformCoord(*(pTri + 1), mWorld);
	vTri[2] = XMVector3TransformCoord(*(pTri + 2), mWorld);

	//	�o�E���f�B���O�{�b�N�X�ƎO�p�`�Ƃ̐ڐG����
	return OBBCollisionTri(vTri, vhit);
}
//------------------------------------------------------------------------  // -- 2019.10.7
//	�o�E���f�B���O�{�b�N�X�ƎO�p�`�Ƃ̐ڐG����
// 
//	const VECTOR3* pTri      ����̎O�p�`
//  VECTOR3* vHit            �q�b�g�����Ƃ��̃q�b�g�ʒu(OUT)
//  
//  �߂�l�@bool	ret�@  false:�q�b�g���Ȃ��Ƃ��@true:�q�b�g�����Ƃ�
//------------------------------------------------------------------------
bool CBBox::OBBCollisionTri(const VECTOR3* pTri, VECTOR3* vhit)
{
	float  p0, p1, p2, r;

	// �n�a�a�e�����̒����i���a�j�x�N�g�������߂�
	VECTOR3 AeX = VECTOR3(m_fLengthX, 0, 0);
	VECTOR3 AeY = VECTOR3(0, m_fLengthY, 0);
	VECTOR3 AeZ = VECTOR3(0, 0, m_fLengthZ);

	// �n�a�a�e�����̒����i���a�j�����߂�
	float eX = m_fLengthX;
	float eY = m_fLengthY;
	float eZ = m_fLengthZ;

	// OBB�̃��[���h�}�g���b�N�X�̋t�s������߂�
	// �i���̒i�K�ŁA�n�a�a�͂��̃��[�J�����W�����[���h���W�n�ƈ�v����`�`�a�a�Ɠ��l�ɂȂ�j
	MATRIX4X4 mWorldInv = XMMatrixInverse(nullptr, m_mWorld);

	// �O�p�`��OBB�̃��[���h�}�g���b�N�X�̋t�s����|�����킹�AOBB�̃��[�J�����W�n�ɕϊ�����
	VECTOR3 vT[3] = { VECTOR3(0,0,0) };
	vT[0] = XMVector3TransformCoord(*(pTri + 0), mWorldInv);
	vT[1] = XMVector3TransformCoord(*(pTri + 1), mWorldInv);
	vT[2] = XMVector3TransformCoord(*(pTri + 2), mWorldInv);

	// �O�p�`���n�a�a�̒��S�ʒu�����_�Ƃ�����W�n�Ɉړ�����
	VECTOR3 vOffset = VECTOR3(m_fLengthX + m_vMin.x, m_fLengthY + m_vMin.y, m_fLengthZ + m_vMin.z);
	vT[0] -= vOffset;
	vT[1] -= vOffset;
	vT[2] -= vOffset;

	// �O�p�`�ɑ΂��ĕӂ̃x�N�g�����v�Z
	VECTOR3  f0 = vT[1] - vT[0];
	VECTOR3  f1 = vT[2] - vT[1];
	VECTOR3  f2 = vT[0] - vT[2];

	// �n�a�a�̊e�ӂƎO�p�`�̊e�ӂ̑g�ݍ��킹�ɑ΂��Đ��������O�ς𕪗����Ƃ��Ĕ��肷��
	// �i�������A�n�a�a�͌��_�ɂ����ă��[���h���W���Ɉ�v����`�`�a�a�ƂȂ��Ă��邽�߁A�v�Z���ȗ����ł���j

	// �n�a�a�̂w���i���[���h���W�̂w���j�ƎO�p�`�̒��_�O�|���_�Q���Ő��������O�ς𕪗����Ƃ��锻��ia00�j
	p0 = vT[0].z * vT[1].y - vT[0].y * vT[1].z;
	p2 = vT[2].z * (vT[1].y - vT[0].y) - vT[2].y * (vT[1].z - vT[0].z);
	r = eY * fabsf(f0.z) + eZ * fabsf(f0.y);
	if (max(-max(p0, p2), min(p0, p2)) > r)
		return  false;

	// �n�a�a�̂w���i���[���h���W�̂w���j�ƎO�p�`�̒��_�O�|���_�P���Ő��������O�ς𕪗����Ƃ��锻��ia01�j
	p0 = vT[0].z * (vT[2].y - vT[1].y) - vT[0].y * (vT[2].z - vT[1].z);
	p1 = vT[1].z * vT[2].y - vT[1].y * vT[2].z;
	r = eY * fabsf(f1.z) + eZ * fabsf(f1.y);
	if (max(-max(p0, p1), min(p0, p1)) > r)
		return  false;

	// �n�a�a�̂w���i���[���h���W�̂w���j�ƎO�p�`�̒��_�P�|���_�Q���Ő��������O�ς𕪗����Ƃ��锻��ia02�j
	p1 = vT[1].z * (vT[0].y - vT[2].y) - vT[1].y * (vT[0].z - vT[2].z);
	p2 = vT[2].z * vT[0].y - vT[2].y * vT[0].z;
	r = eY * fabsf(f2.z) + eZ * fabsf(f2.y);
	if (max(-max(p1, p2), min(p1, p2)) > r)
		return  false;

	// �n�a�a�̂x���i���[���h���W�̂x���j�ƎO�p�`�̒��_�O�|���_�Q���Ő��������O�ς𕪗����Ƃ��锻��ia10�j
	p0 = vT[0].x * vT[1].z - vT[0].z * vT[1].x;
	p2 = vT[2].x * (vT[1].z - vT[0].z) - vT[2].z * (vT[1].x - vT[0].x);
	r = eZ * fabsf(f0.x) + eX * fabsf(f0.z);
	if (max(-max(p0, p2), min(p0, p2)) > r)
		return  false;

	// �n�a�a�̂x���i���[���h���W�̂x���j�ƎO�p�`�̒��_�O�|���_�P���Ő��������O�ς𕪗����Ƃ��锻��ia11�j
	p0 = vT[0].x * (vT[2].z - vT[1].z) - vT[0].z * (vT[2].x - vT[1].x);
	p1 = vT[1].x * vT[2].z - vT[1].z * vT[2].x;
	r = eZ * fabsf(f1.x) + eX * fabsf(f1.z);
	if (max(-max(p0, p1), min(p0, p1)) > r)
		return  false;

	// �n�a�a�̂x���i���[���h���W�̂x���j�ƎO�p�`�̒��_�P�|���_�Q���Ő��������O�ς𕪗����Ƃ��锻��ia12�j
	p1 = vT[1].x * (vT[0].z - vT[2].z) - vT[1].z * (vT[0].x - vT[2].x);
	p2 = vT[2].x * vT[0].z - vT[2].z * vT[0].x;
	r = eZ * fabsf(f2.x) + eX * fabsf(f2.z);
	if (max(-max(p1, p2), min(p1, p2)) > r)
		return  false;

	// �n�a�a�̂y���i���[���h���W�̂y���j�ƎO�p�`�̒��_�O�|���_�Q���Ő��������O�ς𕪗����Ƃ��锻��ia20�j
	p0 = vT[0].y * vT[1].x - vT[0].x * vT[1].y;
	p2 = vT[2].y * (vT[1].x - vT[0].x) - vT[2].x * (vT[1].y - vT[0].y);
	r = eX * fabsf(f0.y) + eY * fabsf(f0.x);
	if (max(-max(p0, p2), min(p0, p2)) > r)
		return  false;

	// �n�a�a�̂y���i���[���h���W�̂y���j�ƎO�p�`�̒��_�O�|���_�P���Ő��������O�ς𕪗����Ƃ��锻��ia21�j
	p0 = vT[0].y * (vT[2].x - vT[1].x) - vT[0].x * (vT[2].y - vT[1].y);
	p1 = vT[1].y * vT[2].x - vT[1].x * vT[2].y;
	r = eX * fabsf(f1.y) + eY * fabsf(f1.x);
	if (max(-max(p0, p1), min(p0, p1)) > r)
		return  false;

	// �n�a�a�̂y���i���[���h���W�̂y���j�ƎO�p�`�̒��_�P�|���_�Q���Ő��������O�ς𕪗����Ƃ��锻��ia22�j
	p1 = vT[1].y * (vT[0].x - vT[2].x) - vT[1].x * (vT[0].y - vT[2].y);
	p2 = vT[2].y * vT[0].x - vT[2].x * vT[0].y;
	r = eX * fabsf(f2.y) + eY * fabsf(f2.x);
	if (max(-max(p1, p2), min(p1, p2)) > r)
		return  false;

	// �n�a�a�̖ʖ@���Ɉ�v���镪�����̔���
	// �w��
	if (max(vT[0].x, max(vT[1].x, vT[2].x)) < -eX || min(vT[0].x, min(vT[1].x, vT[2].x)) > eX)
		return  false;
	// �x��
	if (max(vT[0].y, max(vT[1].y, vT[2].y)) < -eY || min(vT[0].y, min(vT[1].y, vT[2].y)) > eY)
		return  false;
	// �y��
	if (max(vT[0].z, max(vT[1].z, vT[2].z)) < -eZ || min(vT[0].z, min(vT[1].z, vT[2].z)) > eZ)
		return  false;

	// �O�p�`�̖ʂ̖@���Ɉ�v���镪�����̔���	
	VECTOR3 vNormal;
	vNormal = cross(f0, f1);

	p0 = LenSegOnSeparateAxis(&vNormal, &AeX, &AeY, &AeZ);
	r = fabsf(dot(vNormal, vT[0]));

	if (r > p0)
		return  false;

	// �������ʂ����݂��Ȃ��̂Łu�Փ˂��Ă���v

	// �Փˍ��W�̐ݒ�@�O�p�`�̒��S���q�b�g�ʒu�Ƃ���
	*vhit = (*(pTri + 0) + *(pTri + 1) + *(pTri + 2)) / 3;

	// �Փˍ��W�̐ݒ�@�O�p�`�̒��S�Ƃn�a�a���S�̒��_���q�b�g�ʒu�Ƃ���
	//*vhit = ( GetPositionVector(m_mWorld) + ( *(pTri+0) + *(pTri+1) + *(pTri+2) ) / 3 ) / 2;

	return true;
}

//------------------------------------------------------------------------
// �������ɓ��e���ꂽ���������瓊�e���������Z�o����
// �i3�̓��ς̐�Βl�̘a�œ��e���������v�Z�j
// �i������Sep�͕W��������Ă��邱�Ɓj
// 
// VECTOR3 *Sep : ������
// VECTOR3 *e1  : �����P
// VECTOR3 *e2  : �����Q 
// VECTOR3 *e3  : �����R 
// 
// �߂�l�@float ���e������
//------------------------------------------------------------------------
float CBBox::LenSegOnSeparateAxis(const VECTOR3 *Sep, const VECTOR3 *e1, const VECTOR3 *e2, const VECTOR3 *e3)
{
	float r1 = fabsf(dot(*Sep, *e1));
	float r2 = fabsf(dot(*Sep, *e2));
	float r3 = e3 ? (fabsf(dot(*Sep, *e3))) : 0;
	return r1 + r2 + r3;
}

//------------------------------------------------------------------------  // -- 2021.2.4
// �\���p�̃o�E���f�B���O�{�b�N�X�������_�����O����
// 
// �߂�l�@�Ȃ�
//------------------------------------------------------------------------
void CBBox::Render()
{
	Render(m_mWorld, GameDevice()->m_mView, GameDevice()->m_mProj, GameDevice()->m_vLightDir, GameDevice()->m_vEyePt);
}
//------------------------------------------------------------------------  // -- 2021.2.4
// �\���p�̃o�E���f�B���O�{�b�N�X�������_�����O����
// 
// (Simple.hlsli�̃V�F�[�_�[���g�p����)
// 
// const MATRIX4X4& mView    �r���[�}�g���b�N�X
// const MATRIX4X4& mProj    �v���W�F�N�V�����}�g���b�N�X
// const VECTOR3& vLight     ���C�g�̕���
// const VECTOR3& vEye       ���_�ʒu
// 
// �߂�l�@�Ȃ�
//------------------------------------------------------------------------
void CBBox::Render(const MATRIX4X4& mView, const MATRIX4X4& mProj, const VECTOR3& vLight, const VECTOR3& vEye)
{
	Render(m_mWorld, mView, mProj, vLight, vEye);
}
//------------------------------------------------------------------------  // -- 2021.2.4
// �\���p�̃o�E���f�B���O�{�b�N�X�������_�����O����
// 
// (Simple.hlsli�̃V�F�[�_�[���g�p����)
// 
// const MATRIX4X4& mWorld   ���[���h�}�g���b�N�X
// const MATRIX4X4& mView    �r���[�}�g���b�N�X
// const MATRIX4X4& mProj    �v���W�F�N�V�����}�g���b�N�X
// const VECTOR3& vLight     ���C�g�̕���
// const VECTOR3& vEye       ���_�ʒu
// 
// �߂�l�@�Ȃ�
//------------------------------------------------------------------------
void CBBox::Render(const MATRIX4X4& mWorld, const MATRIX4X4& mView, const MATRIX4X4& mProj, const VECTOR3& vLight, const VECTOR3& vEye)
{

	// �g�p����V�F�[�_�[�̓o�^	
	m_pD3D->m_pDeviceContext->VSSetShader(m_pShader->m_pSimple_VS, nullptr, 0);
	m_pD3D->m_pDeviceContext->PSSetShader(m_pShader->m_pSimple_PS, nullptr, 0);

	// �V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��	
	D3D11_MAPPED_SUBRESOURCE pData;
	CONSTANT_BUFFER_WVLED cb;
	if (SUCCEEDED(m_pD3D->m_pDeviceContext->Map(m_pShader->m_pConstantBufferWVLED, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		// ���[���h�s���n��
		cb.mW = XMMatrixTranspose(mWorld);                        // -- 2021.2.4

		// ���[���h�A�J�����A�ˉe�s���n��
		cb.mWVP = XMMatrixTranspose(mWorld * mView * mProj);      // -- 2021.2.4

		//���C�g������n��
		cb.vLightDir = VECTOR4(vLight.x, vLight.y, vLight.z, 0);

		// ���_��n��
		cb.vEyePos = VECTOR4(vEye.x, vEye.y, vEye.z, 1);

		//�J���[��n��
		cb.vDiffuse = m_vDiffuse;

		// �e�����n���B(�g���Ă��Ȃ�)    // -- 2020.12.15
		cb.vDrawInfo = VECTOR4(0, 0, 0, 0);

		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		m_pD3D->m_pDeviceContext->Unmap(m_pShader->m_pConstantBufferWVLED, 0);
	}

	// ���̃R���X�^���g�o�b�t�@�[���g���V�F�[�_�[�̓o�^
	m_pD3D->m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pShader->m_pConstantBufferWVLED);
	m_pD3D->m_pDeviceContext->PSSetConstantBuffers(0, 1, &m_pShader->m_pConstantBufferWVLED);


	//�v���~�e�B�u�E�g�|���W�[���Z�b�g
	m_pD3D->m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	// �o�[�e�b�N�X�o�b�t�@�[���Z�b�g
	UINT stride = sizeof(BBOX_VERTEX);
	UINT offset = 0;
	m_pD3D->m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	// �C���f�b�N�X�o�b�t�@�[���Z�b�g
	m_pD3D->m_pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);


	// ���_�C���v�b�g���C�A�E�g���Z�b�g
	m_pD3D->m_pDeviceContext->IASetInputLayout(m_pShader->m_pSimple_VertexLayout);


	// �e�N�X�`���[�T���v���[���V�F�[�_�[�ɓn��
	m_pD3D->m_pDeviceContext->PSSetSamplers(0, 1, &m_pD3D->m_pSampleLinear);

	// �e�N�X�`���[�Ȃ�
	ID3D11ShaderResourceView* Nothing[1] = { 0 };
	m_pD3D->m_pDeviceContext->PSSetShaderResources(0, 1, Nothing);

	// �v���~�e�B�u�������_�����O
	m_pD3D->m_pDeviceContext->DrawIndexed(2 * 3 * 6, 0, 0);


}

