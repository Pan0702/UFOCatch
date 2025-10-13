//------------------------------------------------------------------------
//    ���b�V���ڐG����p�̃��C�u����
//	  								             ver 4.0        2025.1.3
//	                                                      Collision.cpp
//------------------------------------------------------------------------
#include "Collision.h"
#include "../03_GameMain/GameMain.h"

//------------------------------------------------------------------------
//	�R���X�g���N�^
//
//	CDirect3D*	  pD3D        Direct3D
//
//------------------------------------------------------------------------
CCollision::CCollision() : CCollision(GameDevice()->m_pFbxMeshCtrl)	   		   // -- 2024.3.13
{
}
//------------------------------------------------------------------------
//	�R���X�g���N�^
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
//	�f�X�g���N�^
//
//------------------------------------------------------------------------
CCollision::~CCollision()
{
	deleteAll();
};

//------------------------------------------------------------------------
//	�S�Ẵ����o�ϐ��̏�����
//															2019.8.6
//
//------------------------------------------------------------------------
void CCollision::clearAll(void)
{

	// ��������p�̕ϐ�
	m_pIndex = nullptr;
	m_vNormalH = VECTOR3(0,0,0);
	m_fHeight = 0;
	m_vVertexH[0] = VECTOR3(0,0,0);
	m_vVertexH[1] = VECTOR3(0,0,0);
	m_vVertexH[2] = VECTOR3(0,0,0);

	// ���b�V���ڐG����z��Y��
	m_nNum = 0;

	// �R���W�����ړ��p
	m_bMoveFlag = false;				// �ړ����邩�ǂ��� �ړ��̎� �^
	m_mWorldOld  = XMMatrixIdentity();	// �ړ��}�g���b�N�X��O
	m_mWorld     = XMMatrixIdentity();	// �ړ��}�g���b�N�X
	m_mWorldInv  = XMMatrixIdentity();	// �ړ��}�g���b�N�X�̋t�}�g���b�N�X

}
//------------------------------------------------------------------------
//	�S�Ă̔z��̍폜�Ə�����
//															2019.8.6
//
//------------------------------------------------------------------------
void CCollision::deleteAll(void)
{
	// �������̉��
	for (int n = 0; n<m_nNum; n++)
	{
		// ���b�V���z��̉��
		SAFE_DELETE_ARRAY(m_ColArray[n].pFace);
		SAFE_DELETE_ARRAY(m_ColArray[n].pVert);

		// ���������x�}�b�v�̉��
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

	// �[���N�����[�i�Ďg�p���̑΍�j
	clearAll();
}

//------------------------------------------------------------------------
//
//	Fbx���b�V���t�@�C�����烁�b�V����ǂݍ���Ŕ���p�z��ɒǉ�����
//																	2022.11.14
//
//	Fbx�t�@�C������ɂ��ă��b�V���ڐG����p�z��m_ColArray���쐬����
//	������Fbx�t�@�C����ǂݍ��ނƁA���b�V���ڐG����p�z��ɒǉ����Ă���
//	���Ƃ��ł���
//
//	const TCHAR* pFileName			���b�V���t�@�C���̃t�@�C�����@
//  
//  �߂�l�@�@�@bool  true�@����  false  ���b�V����������Ȃ�
//  
//------------------------------------------------------------------------
bool CCollision::AddFbxLoad(const TCHAR* pFileName)
{
	bool bRet;

	CFbxMesh* pFbxMesh;
	pFbxMesh = new CFbxMesh(m_pFbxMeshCtrl);					// -- 2021.2.4
	if (pFbxMesh->Load(pFileName) == false)  // Fbx�t�@�C����ǂݍ���
	{
		SAFE_DELETE(pFbxMesh);
		return false;
	}

	// ���b�V���I�u�W�F�N�g����ڐG����z��̍쐬
	bRet = AddFbxLoad(pFbxMesh, XMMatrixIdentity());  // ���b�V���I�u�W�F�N�g�����[�h����  // -- 2022.11.14

	SAFE_DELETE(pFbxMesh);  // �ǂݍ��񂾈ꎞ���b�V�����폜����

	return bRet;

}
//------------------------------------------------------------------------
//
//	Fbx���b�V���t�@�C�����烁�b�V����ǂݍ���Ŕ���p�z��ɒǉ�����
//																	2019.8.6
//
//	Fbx�t�@�C������ɂ��ă��b�V���ڐG����p�z��m_ColArray���쐬����
//	������Fbx�t�@�C����ǂݍ��ނƁA���b�V���ڐG����p�z��ɒǉ����Ă���
//	���Ƃ��ł���
//
//	const TCHAR* pFileName			���b�V���t�@�C���̃t�@�C�����@
//  const VECTOR3& vOffset			�z�u�ʒu�̍��W
//  
//  �߂�l�@�@�@bool  true�@����  false  ���b�V����������Ȃ�
//  
//------------------------------------------------------------------------
bool CCollision::AddFbxLoad(const TCHAR* pFileName, const VECTOR3& vOffset)
{
	bool bRet;

	CFbxMesh* pFbxMesh;
	pFbxMesh = new CFbxMesh(m_pFbxMeshCtrl);					// -- 2021.2.4
	if (pFbxMesh->Load(pFileName) == false)  // Fbx�t�@�C����ǂݍ���
	{
		SAFE_DELETE(pFbxMesh);
		return false;
	}

	// ���b�V���I�u�W�F�N�g����ڐG����z��̍쐬
	bRet = AddFbxLoad(pFbxMesh, vOffset);  // ���b�V���I�u�W�F�N�g�����[�h����

	SAFE_DELETE(pFbxMesh);  // �ǂݍ��񂾈ꎞ���b�V�����폜����

	return bRet;

}

//------------------------------------------------------------------------
//
//	Fbx���b�V���t�@�C�����烁�b�V����ǂݍ���Ŕ���p�z��ɒǉ�����
//																	2022.11.14
//
//	Fbx�t�@�C������ɂ��ă��b�V���ڐG����p�z��m_ColArray���쐬����
//	������Fbx�t�@�C����ǂݍ��ނƁA���b�V���ڐG����p�z��ɒǉ����Ă���
//	���Ƃ��ł���
//
//	const TCHAR* pFileName			���b�V���t�@�C���̃t�@�C�����@
//  const MATRIX4X4& mOffset		�z�u�ʒu�̃��[���h�}�g���b�N�X
//  
//  �߂�l�@�@�@bool  true�@����  false  ���b�V����������Ȃ�
//  
//------------------------------------------------------------------------
bool CCollision::AddFbxLoad(const TCHAR* pFileName, const MATRIX4X4& mOffset)
{
	bool bRet;

	CFbxMesh* pFbxMesh;
	pFbxMesh = new CFbxMesh(m_pFbxMeshCtrl);					// -- 2021.2.4
	if (pFbxMesh->Load(pFileName) == false)  // Fbx�t�@�C����ǂݍ���
	{
		SAFE_DELETE(pFbxMesh);
		return false;
	}

	// ���b�V���I�u�W�F�N�g����ڐG����z��̍쐬
	bRet = AddFbxLoad(pFbxMesh, mOffset);  // ���b�V���I�u�W�F�N�g�����[�h����   // -- 2022.11.14

	SAFE_DELETE(pFbxMesh);  // �ǂݍ��񂾈ꎞ���b�V�����폜����

	return bRet;

}

//  -------------------------------------------------------------------
//
//	Fbx���b�V���I�u�W�F�N�g���烁�b�V�������[�h���Ĕ���p�z��ɒǉ�����
//																		2020.12.15
//
//	���b�V���I�u�W�F�N�g����ɂ��ă��b�V���ڐG����p�z��m_ColArray��
//	�쐬����
//	�����̃X�^�e�B�b�N���b�V���I�u�W�F�N�g��ǂݍ��ނƁA���b�V���ڐG����p�z���
//	�ǉ����Ă������Ƃ��ł���
//
//  const CFbxMesh* pFbxMesh		���b�V���I�u�W�F�N�g
//
//  �߂�l�@bool  true�@����   false  �G���[
//  -------------------------------------------------------------------
bool CCollision::AddFbxLoad(const CFbxMesh* pFbxMesh)
{
	return AddFbxLoad(pFbxMesh, XMMatrixIdentity());    // -- 2022.11.14
}
//  -------------------------------------------------------------------
//
//	Fbx���b�V���I�u�W�F�N�g���烁�b�V�������[�h���Ĕ���p�z��ɒǉ�����
//																		2022.11.14
//
//	���b�V���I�u�W�F�N�g����ɂ��ă��b�V���ڐG����p�z��m_ColArray��
//	�쐬����
//	�����̃X�^�e�B�b�N���b�V���I�u�W�F�N�g��ǂݍ��ނƁA���b�V���ڐG����p�z���
//	�ǉ����Ă������Ƃ��ł���
//
//  const CFbxMesh* pFbxMesh		���b�V���I�u�W�F�N�g
//  const VECTOR3& vOffset			�z�u�ʒu�̍��W
//
//  �߂�l�@bool  true�@����   false  �G���[
//  -------------------------------------------------------------------
bool CCollision::AddFbxLoad(const CFbxMesh* pFbxMesh, const VECTOR3& vOffset)
{
	return AddFbxLoad(pFbxMesh, XMMatrixTranslationFromVector(vOffset));    // -- 2022.11.14
}
//  -------------------------------------------------------------------
//
//	Fbx���b�V���I�u�W�F�N�g���烁�b�V�������[�h���Ĕ���p�z��ɒǉ�����
//																		2022.11.14
//
//	���b�V���I�u�W�F�N�g����ɂ��ă��b�V���ڐG����p�z��m_ColArray��
//	�쐬����
//	�����̃X�^�e�B�b�N���b�V���I�u�W�F�N�g��ǂݍ��ނƁA���b�V���ڐG����p�z���
//	�ǉ����Ă������Ƃ��ł���
//
//  const CFbxMesh* pFbxMesh		���b�V���I�u�W�F�N�g
//  const MATRIX4X4& mOffset		�z�u�ʒu�̃��[���h�}�g���b�N�X
//
//  �߂�l�@bool  true�@����   false  �G���[
//  -------------------------------------------------------------------
bool CCollision::AddFbxLoad(const CFbxMesh* pFbxMesh, const MATRIX4X4& mOffset)
{
	if (m_nNum >= MCKTBL_MAX)
	{
		MessageBox(0, _T("Collision.cpp : AddFbxLoad() : ���b�V���ڐG����p�z�� MCKTBL_MAX���I�[�o�[���܂���"), nullptr, MB_OK);
		return  false;
	}

	DWORD  i, j, nNumvert = 0, nNumidx = 0, nVertoffset = 0, f0, f1, f2;
	int n = m_nNum;
	VECTOR3 vMin = VECTOR3(9999999, 9999999, 9999999);
	VECTOR3 vMax = VECTOR3(-9999999, -9999999, -9999999);

	// �S�Ă̎q�f�[�^�̍��v�Ƃ��Ă̒��_�f�[�^���ƃC���f�b�N�X�f�[�^���̎擾
	for (i = 0; i < pFbxMesh->m_dwMeshNum; i++)
	{
		// ���_�f�[�^���̍��v
		nNumvert += pFbxMesh->m_pMeshArray[i].m_dwVerticesNum;
		// �C���f�b�N�X�f�[�^���̍��v
		nNumidx += pFbxMesh->m_pMeshArray[i].m_dwIndicesNum;
	}

	// �S�Ă̎q�f�[�^�̍��v�Ƃ��Ă̒��_�z��ƃC���f�b�N�X�z��𐶐�
	m_ColArray[n].pFace = new ColFace[nNumidx / 3];
	m_ColArray[n].pVert = new VECTOR3[nNumvert];
	m_ColArray[n].nNumFace = nNumidx / 3;
	m_ColArray[n].nNumVert = nNumvert;

	nNumvert = 0;
	nNumidx = 0;

	// �q�f�[�^���̒��_�f�[�^�ƃC���f�b�N�X�f�[�^�����ׂĈ�̒��_�E�C���f�b�N�X�Ƃ��Đݒ肷��
	for (i = 0; i < pFbxMesh->m_dwMeshNum; i++)
	{
		// ���_�f�[�^�̐ݒ�
		// Offset�������āA���ʒu�ɂ���
		for (j = 0; j < pFbxMesh->m_pMeshArray[i].m_dwVerticesNum; j++, nNumvert++)
		{
			// ���_�f�[�^�̐ݒ�
			// mOffset�̃}�g���b�N�X�ɂ���Ē��_�����[���h�ϊ�����   // -- 2022.11.14
			if (pFbxMesh->m_nMeshType == 1)   // ���b�V���^�C�v�Œ��_�t�H�[�}�b�g���ς��    // -- 2020.12.15
			{
				m_ColArray[n].pVert[nNumvert] = XMVector3TransformCoord( pFbxMesh->m_pMeshArray[i].m_vStaticVerticesNormal[j].Pos, mOffset);    // -- 2022.11.14
			}
			else {
				m_ColArray[n].pVert[nNumvert] = XMVector3TransformCoord( pFbxMesh->m_pMeshArray[i].m_vSkinVerticesNormal[j].Pos, mOffset);      // -- 2022.11.14
			}

			// �S���_�̍ő�l�ƍŏ��l�����߂�
			if (vMin.x > m_ColArray[n].pVert[nNumvert].x) vMin.x = m_ColArray[n].pVert[nNumvert].x;
			if (vMin.y > m_ColArray[n].pVert[nNumvert].y) vMin.y = m_ColArray[n].pVert[nNumvert].y;
			if (vMin.z > m_ColArray[n].pVert[nNumvert].z) vMin.z = m_ColArray[n].pVert[nNumvert].z;
			if (vMax.x < m_ColArray[n].pVert[nNumvert].x) vMax.x = m_ColArray[n].pVert[nNumvert].x;
			if (vMax.y < m_ColArray[n].pVert[nNumvert].y) vMax.y = m_ColArray[n].pVert[nNumvert].y;
			if (vMax.z < m_ColArray[n].pVert[nNumvert].z) vMax.z = m_ColArray[n].pVert[nNumvert].z;
		}

		// �C���f�b�N�X�f�[�^�Ɩʖ@���Ƃ`�`�a�a�̐ݒ�
		for (j = 0; j < pFbxMesh->m_pMeshArray[i].m_dwIndicesNum; j+=3, nNumidx+=3)
		{
			f0 = m_ColArray[n].pFace[nNumidx/3].dwIdx[0] = pFbxMesh->m_pMeshArray[i].m_nIndices[j+0] + nVertoffset;  // �S�̂̒��ł̃C���f�b�N�X�l�ɕϊ����邽�߂ɃI�t�Z�b�g��������
			f1 = m_ColArray[n].pFace[nNumidx/3].dwIdx[1] = pFbxMesh->m_pMeshArray[i].m_nIndices[j+1] + nVertoffset;  // �S�̂̒��ł̃C���f�b�N�X�l�ɕϊ����邽�߂ɃI�t�Z�b�g��������
			f2 = m_ColArray[n].pFace[nNumidx/3].dwIdx[2] = pFbxMesh->m_pMeshArray[i].m_nIndices[j+2] + nVertoffset;  // �S�̂̒��ł̃C���f�b�N�X�l�ɕϊ����邽�߂ɃI�t�Z�b�g��������
			//m_ColArray[n].pFace[nNumidx / 3].vNormal = normalize(cross(m_ColArray[n].pVert[f2] - m_ColArray[n].pVert[f0],		// �ʖ@�����쐬����		  // -- 2024.3.23
			//																m_ColArray[n].pVert[f1] - m_ColArray[n].pVert[f0]));
			m_ColArray[n].pFace[nNumidx / 3].vNormal = normalize(cross(m_ColArray[n].pVert[f1] - m_ColArray[n].pVert[f0],		// �ʖ@�����쐬����		  // -- 2024.3.23
																			m_ColArray[n].pVert[f2] - m_ColArray[n].pVert[f0]));
			m_ColArray[n].pFace[nNumidx / 3].AABB.MakeAABB(m_ColArray[n].pVert[f0], m_ColArray[n].pVert[f1], m_ColArray[n].pVert[f2]);  // �|���S����AABB���쐬����
		}

		nVertoffset = nNumvert;  // �q���̃C���f�b�N�X�f�[�^��S�̂̒��ł̃C���f�b�N�X�l�ɕϊ����邽�߂̃I�t�Z�b�g
	}

	// ���������x�}�b�v���쐬����
	makeChkColMesh(m_nNum, vMin, vMax);

	m_nNum++;	// ���b�V���ڐG����z��Y�����P���₷

	return true;
}

//  -------------------------------------------------------------------
//
//	���������x�}�b�v���쐬����
//
//	���������x�}�b�vm_ChkColMesh���쐬����ړI�́A�}�b�v���\������
//	�O�p�|���S�������̑傫���ɉ������A�ו������ꂽ�i�[�ꏊ�ɒu�����ƂŁA
//	�ڐG����̍ۂɁA����ɕK�v�Ȉʒu�̎O�p�|���S���݂̂�ΏۂƂ���������
//	�s�Ȃ��A�������Ԃ̑啝�ȒZ�k���s�����Ƃ��ł���_�ɂ���B
//	�Ȃ��A���������x�}�b�v�ɂ́A�����܂ł��ʁi�O�p�|���S���j�f�[�^��
//	�A�h���X�݂̂��i�[���A�f�[�^���̂��̂́A���b�V���ڐG����z��
//	m_ColArray�ɒu���Ă���B
//
//	����
//		const int&		nNum   ���b�V���z��Y��
//		const VECTOR3&	vMin   ���b�V���̑S���_�̒��̍ŏ��l
//		const VECTOR3&	vMax   ���b�V���̑S���_�̒��̍ő�l
//
//  �߂�l�@�Ȃ�
//  -------------------------------------------------------------------
void CCollision::makeChkColMesh( const int& nNum, const VECTOR3& vMin, const VECTOR3& vMax)
{
	const float fSpare = 1.05f;  // �������̂̑傫���T���̗]�T

	int     n = nNum;
	int     i, j;
	float   fx, fy, fz;

	int ChkIdx[8], ChkIMax;

	// �S���b�V���̒��_�̍ŏ��l�̎擾
	m_ChkColMesh[n].vMin = vMin;
	m_ChkColMesh[n].vMax = vMax;					// -- 2020.12.3

	// ���������x�}�b�vm_ChkColMesh�z��̐V�K�쐬
	// �S�̂�\���z�񂩂�A�P�^�Q�Â̑傫���̔z��������쐬���Ă���
	for (i = 0; i < MESHCKTBL_FACE_MAX; i++) 
	{
		if (i == 0) 
		{
			// �ŏ��̔z��i�S�̂����͂ށA�������́j
			fx = (float)fabs(vMax.x - vMin.x);
			fy = (float)fabs(vMax.y - vMin.y);
			fz = (float)fabs(vMax.z - vMin.z);
			if (fx < fy) fx = fy;    // �S�Ă̕ӂ̒����͓���(��������)
			if (fx < fz) fx = fz;    // �S�Ă̕ӂ̒����͓���(��������)
			fx *= fSpare;	// �]�T������
			m_ChkColMesh[n].ChkBlkArray[i].vBlksize.x = m_ChkColMesh[n].ChkBlkArray[i].vBlksize.y = m_ChkColMesh[n].ChkBlkArray[i].vBlksize.z = fx;
		}
		else {
			// MESHCKTBL_LOWLIMIT(�R�D�O��)�ȉ��̔z��͍쐬���Ȃ�
			if (m_ChkColMesh[n].ChkBlkArray[i - 1].vBlksize.x <= MESHCKTBL_LOWLIMIT &&
				m_ChkColMesh[n].ChkBlkArray[i - 1].vBlksize.y <= MESHCKTBL_LOWLIMIT &&
				m_ChkColMesh[n].ChkBlkArray[i - 1].vBlksize.z <= MESHCKTBL_LOWLIMIT) 
			{
				for (; i < MESHCKTBL_FACE_MAX; i++)  // �c��̔z��̓��e�͑S�ĂO�Ƃ���
				{
					m_ChkColMesh[n].ChkBlkArray[i].ppChkFace = nullptr;   // �쐬���Ȃ��ʒu�̃|�C���^��nullptr
					m_ChkColMesh[n].ChkBlkArray[i].vBlksize.x = 0;
					m_ChkColMesh[n].ChkBlkArray[i].vBlksize.y = 0;
					m_ChkColMesh[n].ChkBlkArray[i].vBlksize.z = 0;
					m_ChkColMesh[n].ChkBlkArray[i].dwNumX = 0;
					m_ChkColMesh[n].ChkBlkArray[i].dwNumY = 0;
					m_ChkColMesh[n].ChkBlkArray[i].dwNumZ = 0;
				}
				break;  // ���[�v�𔲂��o��
			}
			else {
				// �P�^�Q�A�P�^�S�A�P�^�W�E�E�E�E�E�̔z��
				m_ChkColMesh[n].ChkBlkArray[i].vBlksize.x = m_ChkColMesh[n].ChkBlkArray[i - 1].vBlksize.x / 2;
				m_ChkColMesh[n].ChkBlkArray[i].vBlksize.y = m_ChkColMesh[n].ChkBlkArray[i - 1].vBlksize.y / 2;
				m_ChkColMesh[n].ChkBlkArray[i].vBlksize.z = m_ChkColMesh[n].ChkBlkArray[i - 1].vBlksize.z / 2;
			}
		}

		// �V�K��������z��̗v�f�����v�Z����
		// dwNum�́A��������vBlksize���{���̃��b�V���S�̂̑傫���ɑ΂��ĉ��K�v�Ȃ̂������߂�
		m_ChkColMesh[n].ChkBlkArray[i].dwNumX = (DWORD)(fabs(vMax.x - vMin.x) / m_ChkColMesh[n].ChkBlkArray[i].vBlksize.x + 1);
		m_ChkColMesh[n].ChkBlkArray[i].dwNumY = (DWORD)(fabs(vMax.y - vMin.y) / m_ChkColMesh[n].ChkBlkArray[i].vBlksize.y + 1);
		m_ChkColMesh[n].ChkBlkArray[i].dwNumZ = (DWORD)(fabs(vMax.z - vMin.z) / m_ChkColMesh[n].ChkBlkArray[i].vBlksize.z + 1);

		// �z���V�K��������B�Ȃ��AppChkFace�͍\���̃|�C���^�z��ł���
		// �v�f���́A�S�K�v���Ȃ̂�dwNumX*dwNumY*dwNumZ�ł���
		m_ChkColMesh[n].ChkBlkArray[i].ppChkFace = new struct ChkFace*[m_ChkColMesh[n].ChkBlkArray[i].dwNumX * m_ChkColMesh[n].ChkBlkArray[i].dwNumY * m_ChkColMesh[n].ChkBlkArray[i].dwNumZ];
		if (m_ChkColMesh[n].ChkBlkArray[i].ppChkFace == nullptr) 
		{
			MessageBox(0, _T("Collision.cpp : MakeChkColMesh() : m_ChkColMesh[n].ChkBlkArray[i].ppChkFace�@�������[���蓖�ăG���["), nullptr, MB_OK);
			return;
		}

		// ���������|�C���^�z����[���N�����[����
		ZeroMemory((BYTE**)m_ChkColMesh[n].ChkBlkArray[i].ppChkFace, sizeof(struct ChkFace*) * (m_ChkColMesh[n].ChkBlkArray[i].dwNumX * m_ChkColMesh[n].ChkBlkArray[i].dwNumY * m_ChkColMesh[n].ChkBlkArray[i].dwNumZ));
	}

	// ���b�V���̖ʔz�񂩂�O�p�`�|���S���f�[�^���擾���A���̃A�h���X��
	// ���������x�}�b�v�Ɋi�[���Ă���
	// �Ȃ��A�P�|���S���̃A�h���X�i�[�ꏊ�́A���X�g�\���ƂȂ��Ă��āA
	// �����̃|���S���A�h���X���i�[�ł���悤�ɂȂ��Ă���B
	for (i = 0; i < m_ColArray[n].nNumFace; i++)
	{
		// �O�p�`�|���S���̂`�`�a�a�̑傫�������߂�
		fx = fabsf(m_ColArray[n].pFace[i].AABB.m_vMax.x - m_ColArray[n].pFace[i].AABB.m_vMin.x);
		fy = fabsf(m_ColArray[n].pFace[i].AABB.m_vMax.y - m_ColArray[n].pFace[i].AABB.m_vMin.y);
		fz = fabsf(m_ColArray[n].pFace[i].AABB.m_vMax.z - m_ColArray[n].pFace[i].AABB.m_vMin.z);

		// �O�p�`�|���S���̑傫������A�œK�ȑ傫���̊i�[�z������肷��
		for (j = 0; j < MESHCKTBL_FACE_MAX - 1; j++) 
		{
			if (m_ChkColMesh[n].ChkBlkArray[j+1].ppChkFace == nullptr) break;
			// �������[j+1]�̑傫���̔z��ɓ��肫��Ȃ��Ƃ��́A���̔z��Ɋi�[����傫���ł���Ƃ���
			if (m_ChkColMesh[n].ChkBlkArray[j+1].vBlksize.x < fx ||
				m_ChkColMesh[n].ChkBlkArray[j+1].vBlksize.y < fy ||
				m_ChkColMesh[n].ChkBlkArray[j+1].vBlksize.z < fz) break;
		}

		// �����傫���̔z��̒��̂ǂ��̈ʒu�Ɋi�[���邩�����肷��
		getChkArrayIdx(n, j, m_ColArray[n].pFace[i].AABB, ChkIdx, ChkIMax);

		// �i�[�z��Ɋi�[����i���X�g�\���ł���j
		for (int k = 0; k < ChkIMax; k++)
		{
			setChkArray(n, j, ChkIdx[k], &m_ColArray[n].pFace[i]);
		}
	}
	
}
//-----------------------------------------------------------------------------
// �����傫���̔z��̒��̂ǂ��̈ʒu�Ɋi�[���邩�����肷��
//																	2019.8.6
//  
//  �O�p�`�|���S���̂`�`�a�a�̒��_�W�̂��ꂼ�ꂪ�ǂ̈ʒu�Ɋi�[����邩�𒲂ׂ�
//  
//  ���܂��A�i�[�ʒu�ɔ[�܂�ꍇ�ɂ�1�J���ł��ނ��A�i�[�ꏊ���܂����ő��݂��鎞�́A
//  �ő�ō��E�㉺�O���8�J���ɂ܂�����ꍇ������B
//  �Ȃ��A�i�[�|���S���͊i�[�ꏊ�̑傫����菬�������߁A�Q�ȏ�ׂɂ܂����邱�Ƃ͂Ȃ��B
//  
//	const int&   nNum       ���b�V���z��Y��
//  const int&   nNo        �œK�ȑ傫���̊i�[�z��ChkBlkArray�Y��
//  CAABB AABB              �O�p�`�|���S���̂`�`�a�a
//  int   nIdx[8]           �i�[�ꏊ�z��B�ő�W�J���ɕ������\���L��(Out)
//  int&  nIMax             �i�[�ꏊ�z��̊i�[���B�ő�W(Out)
//  
//  �߂�l�@�Ȃ�
//-----------------------------------------------------------------------------
void  CCollision::getChkArrayIdx(const int& nNum, const int& nNo, CAABB AABB, int nIdx[], int& nIMax)
{
	VECTOR3 vPos;
	int i, j, x, y, z, m;
	int n = nNum;

	// �i�[�ꏊ�z��N�����[
	nIMax = 0;
	for ( i = 0; i < 8; i++) nIdx[i] = 0;

	// �i�[�ꏊ�z��̐ݒ�
	for ( i = 0; i < 8; i++ )
	{
		vPos = AABB.GetVecPos(i);  // �`�`�a�a�̂W���_�����Ԃɓ���

		// ���_����i�[�ꏊ�u���b�N��xyz�����̓Y���ԍ��𓾂�
		x = (int)((vPos.x - m_ChkColMesh[n].vMin.x) / m_ChkColMesh[n].ChkBlkArray[nNo].vBlksize.x);
		y = (int)((vPos.y - m_ChkColMesh[n].vMin.y) / m_ChkColMesh[n].ChkBlkArray[nNo].vBlksize.y);
		z = (int)((vPos.z - m_ChkColMesh[n].vMin.z) / m_ChkColMesh[n].ChkBlkArray[nNo].vBlksize.z);
		if (x >= (int)m_ChkColMesh[n].ChkBlkArray[nNo].dwNumX || y >= (int)m_ChkColMesh[n].ChkBlkArray[nNo].dwNumY
			|| z >= (int)m_ChkColMesh[n].ChkBlkArray[nNo].dwNumZ ||	x < 0 || y < 0 || z < 0) {
			MessageBox(0, _T("Collision.cpp : GetChkArrayIdx() : �z��I�[�o�[�G���["), nullptr, MB_OK);
			return;
		}

		// �i�[����ʒu���m�肷��
		// xyz�̓Y���ԍ�����A�i�[�ʒu�̓Y�����v�Z�œ���
		// �P�����ڂ��w�����A�Q�����ڂ��y�����A�R�����ڂ��x�����ł���
		m = y * m_ChkColMesh[n].ChkBlkArray[nNo].dwNumX * m_ChkColMesh[n].ChkBlkArray[nNo].dwNumZ
												+ z * m_ChkColMesh[n].ChkBlkArray[nNo].dwNumX + x;

		// ���肵���ʒu���i�[�ꏊ�z��ɏ�������
		if (nIMax == 0)  // �ŏ��̊i�[
		{
			nIdx[nIMax] = m;
			nIMax++;
		}
		else {
			for (j = 0; j < nIMax; j++)  // �����i�[�ꏊ���Ȃ����ǂ������ׂ�
			{
				if (nIdx[j] == m) break;  // �����i�[�ꏊ�����������̂œǂݔ�΂�
			}
			if (j == nIMax)   // �قȂ�i�[�ꏊ�Ȃ̂ŏ�������
			{
				nIdx[nIMax] = m;
				nIMax++;
			}
		}
	}
}
//-----------------------------------------------------------------------------
// �i�[�z��̎w��ʒu�Ɋi�[����֐�
//																	2019.8.6
//  
//  �i�[�z��̎w��ʒu�̓��X�g�\���̂��߂��̍Ō���ɒǉ�����
//  �Ȃ��A�i�[����f�[�^�͖ʁi�O�p�`�|���S���j�̃A�h���X�ł���B
//  
//	const int&     nNum    ���b�V���z��Y��
//  const int&     nNo     �œK�ȑ傫���̊i�[�z��ChkBlkArray�Y��
//  const int&     nIdx    �i�[�ꏊ�z��Y��
//  ColFace* pFace         �ʂ̃A�h���X
//  
//  �߂�l�@�Ȃ�
//-----------------------------------------------------------------------------
void  CCollision::setChkArray(const int& nNum, const int& nNo, const int& nIdx, ColFace* pFace)
{
	struct ChkFace* p;
	int n = nNum;

	if (m_ChkColMesh[n].ChkBlkArray[nNo].ppChkFace[nIdx] == nullptr)
	{
		// ���X�g�\���̐擪�|�C���^�ɒǉ�����Ƃ�
		m_ChkColMesh[n].ChkBlkArray[nNo].ppChkFace[nIdx] = new struct ChkFace;
		m_ChkColMesh[n].ChkBlkArray[nNo].ppChkFace[nIdx]->pFace = pFace;
		m_ChkColMesh[n].ChkBlkArray[nNo].ppChkFace[nIdx]->pNext = nullptr;
	}
	else {
		// ���X�g�\���̍Ō�ɒǉ�����Ƃ�
		p = m_ChkColMesh[n].ChkBlkArray[nNo].ppChkFace[nIdx];
		while (1)
		{
			if (p->pNext == nullptr)
			{
				// ���X�g�\���̍Ō�ł���̂ŁA�f�[�^��ǉ�����
				p->pNext = new struct ChkFace;
				p = p->pNext;
				p->pFace = pFace;
				p->pNext = nullptr;
				break;
			}
			// ���|�C���^�����ǂ��čs��
			p = p->pNext;
		}
	}
}
// -----------------------------------------------------------------------------------------------------------
// �z��̑ΏۂƂ���u���b�N�ԍ��͈̔͂����߂�
//																						2019.8.6
//
//		const int&		nNum		���b�V���z��Y��
//		const int&		nNo			�œK�ȑ傫���̊i�[�z��ChkBlkArray�Y��
//		const VECTOR3&	vNow		�ړ���̈ʒu
//		const VECTOR3&	vOld		�ړ��O�ʒu
//		const float&	fRadius		�I�u�W�F�N�g�̔��a
//		int&			nStatrX		�u���b�N�ԍ��J�n�w(out)
//		int&			nEndX		�u���b�N�ԍ��I���w(out)
//		int&			nStatrY		�u���b�N�ԍ��J�n�x(out)
//		int&			nEndY		�u���b�N�ԍ��I���x(out)
//		int&			nStatrZ		�u���b�N�ԍ��J�n�y(out)
//		int&			nEndZ		�u���b�N�ԍ��I���y(out)
//
//	�߂�l 
//		�Ȃ�
// -----------------------------------------------------------------------------------------------------------
void	CCollision::getMeshLimit(const int& nNum, const int& nNo, const VECTOR3& vNow, const VECTOR3& vOld, const float& fRadius,
									int& nStatrX, int& nEndX, int& nStatrY, int& nEndY, int& nStatrZ, int& nEndZ)
{
	int n = nNum;
	int i = nNo;
	int x, y, z;

	// �ړ��������͂�AABB���쐬����B�]�T��fRadius�Ƃ�B
	CAABB AABB;
	AABB.MakeAABB(vNow, vOld, fRadius);

	// �z��̑ΏۂƂ���J�n�_�̃u���b�N�ԍ������߂�
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

	// �z��̑ΏۂƂ���I���_�̃u���b�N�ԍ������߂�
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
// �I�u�W�F�N�g�̃��C�ƃ��b�V���ڐG����p�z��Ƃ̐ڐG����
//																		2019.8.6
//  
//	const VECTOR3&�@startIn		�ړ��J�n�ʒu�̍��W
//	const VECTOR3&�@endIn		�ړ��I���ʒu�̍��W
//  VECTOR3 &vHit				�ڐG�_�̍��W�i�o�́j
//  VECTOR3 &vNormal			�ڐG�_�̖@���x�N�g���i�o�́j
//  
//  �߂�l�@bool bRet
//		�ڐG�����Ƃ��@		true
//		�ڐG���Ă��Ȃ��Ƃ�  false
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
	float    fLenMin = 9999999.0f;				// ��_�ƈړ��O�_�Ƃ̋����̍ŏ��l

	// �ړ��}�b�v�̏������s��
	if (m_bMoveFlag)  // �R���W�����}�b�v���ړ����Ă��邩�ǂ���
	{
		// �R���W�����}�b�v���ړ����Ă���Ƃ��́A�L�����N�^�[���t�Ɉړ����Ă���ƌ��Ȃ��Ĕ��������
		// ���̂��߂ɁA�L�����N�^�[�̈ʒu�Ƀ}�b�v�ړ��̋t�s����|���Ĕ�����s��
		vOld = XMVector3TransformCoord(startIn, m_mWorldInv);
		vNow = XMVector3TransformCoord(endIn, m_mWorldInv);
	}
	else {
		// �R���W�����}�b�v���ړ����Ă��Ȃ��Ƃ��́A�L�����N�^�[�̈ʒu�����̂܂܎g�p����
		vOld = startIn;
		vNow = endIn;
	}

	// ���C�ƃ��b�V���Ƃ̏Փ˔�����s��
	for (n = 0; n<m_nNum; n++)
	{
		// ���������x�z��̌���
		for (i = 0; i < MESHCKTBL_FACE_MAX; i++)
		{
			if (m_ChkColMesh[n].ChkBlkArray[i].ppChkFace == nullptr) break;	// �z��Ƀf�[�^���Ȃ��Ƃ�

			// �z��̑ΏۂƂ���J�n�_�ƏI���_�̃u���b�N�ԍ������߂�
			getMeshLimit(n, i, vNow, vOld, 0, nStartX, nEndX, nStartY, nEndY, nStartZ, nEndZ);

			// �z��̌������J�n����
			for (y = nStartY; y <= nEndY; y++) {
				for (z = nStartZ; z <= nEndZ; z++) {
					for (x = nStartX; x <= nEndX; x++) {
						m = y * m_ChkColMesh[n].ChkBlkArray[i].dwNumX * m_ChkColMesh[n].ChkBlkArray[i].dwNumZ
							+ z * m_ChkColMesh[n].ChkBlkArray[i].dwNumX + x;

						p = m_ChkColMesh[n].ChkBlkArray[i].ppChkFace[m];

						while (p != nullptr)  // ���X�g�\���̍Ō�܂�
						{

							// �R�p�`�|���S���̒l�𓾂�
							vVert[0] = m_ColArray[n].pVert[p->pFace->dwIdx[0]];
							vVert[1] = m_ColArray[n].pVert[p->pFace->dwIdx[1]];
							vVert[2] = m_ColArray[n].pVert[p->pFace->dwIdx[2]];
							vFaceNorm = p->pFace->vNormal;

							// �����̂R�p�`�|���S���@�������̋��������߂�
							getDistNormal(vVert, vNow, vOld, vFaceNorm, fNowDist, fOldDist, fLayDist);
							// �R�p�`�|���S���ƒ����i���C�j�Ƃ̐ڐG������s��
							if (checkLay(vVert, vNow, vOld, vFaceNorm, fNowDist, fOldDist, fLayDist, vInsPt) == 1)
							{
								bRet = true;   // �ڐG���Ă���
								float len = magnitude(vOld - vInsPt);
								if (fLenMin > len)   // ���߂���_��T��
								{
									fLenMin = len;
									if (m_bMoveFlag)  // �R���W�����}�b�v���ړ����Ă��邩�ǂ���
									{
										// �R���W�����}�b�v���ړ����Ă���Ƃ��́A���ۂ̕`��ʒu�ɖ߂��ďo�͂���
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
							p = p->pNext;   // ���X�g�\�������ǂ��Ď��̃|���S����T��
						}
					}
				}
			}
		}
	}
	return  bRet;
}
//-----------------------------------------------------------------------------
// �I�u�W�F�N�g�̋��ƃ��b�V���ڐG����p�z��Ƃ̐ڐG����
//																				// -- 2020.12.14
//  
//	const VECTOR3&�@startIn		�ړ��J�n�ʒu�̍��W
//	const VECTOR3&�@endIn		�ړ��I���ʒu�̍��W
//	const float&�@fRadius		���̔��a
//  VECTOR3 &vHit				�ڐG���̃I�u�W�F�N�g���S�ʒu�̍��W�i�o�́j
//  VECTOR3 &vNormal			�ڐG�_�̖@���x�N�g���i�o�́j
//  
//  �߂�l�@book bRet
//		�ڐG�����Ƃ��@		true
//		�ڐG���Ă��Ȃ��Ƃ�	false
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
	float    fLenMin = 9999999.0f;				// ��_�ƈړ��O�_�Ƃ̋����̍ŏ��l

	// �ړ��}�b�v�̏������s��
	if (m_bMoveFlag)  // �R���W�����}�b�v���ړ����Ă��邩�ǂ���
	{
		// �R���W�����}�b�v���ړ����Ă���Ƃ��́A�L�����N�^�[���t�Ɉړ����Ă���ƌ��Ȃ��Ĕ��������
		// ���̂��߂ɁA�L�����N�^�[�̈ʒu�}�g���b�N�X�Ƀ}�b�v�ړ��̋t�s����|���Ĕ�����s��
		vOld = XMVector3TransformCoord(startIn, m_mWorldInv);
		vNow = XMVector3TransformCoord(endIn, m_mWorldInv);
	}
	else {
		// �R���W�����}�b�v���ړ����Ă��Ȃ��Ƃ��́A�L�����N�^�[�̈ʒu�����̂܂܎g�p����
		vOld = startIn;
		vNow = endIn;
	}

	// ���C�ƃ��b�V���Ƃ̏Փ˔�����s��
	for (n = 0; n < m_nNum; n++)
	{
		// ���������x�z��̌���
		for (i = 0; i < MESHCKTBL_FACE_MAX; i++)
		{
			if (m_ChkColMesh[n].ChkBlkArray[i].ppChkFace == nullptr) break;	// �z��Ƀf�[�^���Ȃ��Ƃ�

			// �z��̑ΏۂƂ���J�n�_�ƏI���_�̃u���b�N�ԍ������߂�
			// �E���a���ړ������邽�߁A����͈͂𔼌a�̂Q�{�Ƃ���
			getMeshLimit(n, i, vNow, vOld, fRadius*2, nStartX, nEndX, nStartY, nEndY, nStartZ, nEndZ);

			// �z��̌������J�n����
			for (y = nStartY; y <= nEndY; y++) {
				for (z = nStartZ; z <= nEndZ; z++) {
					for (x = nStartX; x <= nEndX; x++) {
						m = y * m_ChkColMesh[n].ChkBlkArray[i].dwNumX * m_ChkColMesh[n].ChkBlkArray[i].dwNumZ
							+ z * m_ChkColMesh[n].ChkBlkArray[i].dwNumX + x;

						p = m_ChkColMesh[n].ChkBlkArray[i].ppChkFace[m];

						while (p != nullptr)  // ���X�g�\���̍Ō�܂�
						{

							// �R�p�`�|���S���̒l�𓾂�
							vVert[0] = m_ColArray[n].pVert[p->pFace->dwIdx[0]];
							vVert[1] = m_ColArray[n].pVert[p->pFace->dwIdx[1]];
							vVert[2] = m_ColArray[n].pVert[p->pFace->dwIdx[2]];
							vFaceNorm = p->pFace->vNormal;

							// �@�������ɔ��a���ړ�����x�N�g�������߂�
							VECTOR3 vNormalRadius = vFaceNorm * fRadius;

							// �����̂R�p�`�|���S���@�������̋��������߂�
							// �E�I�u�W�F�N�g�̒��S�_����@�������̋t�����ɔ��a���ړ��������_�Ŕ��肷��
							getDistNormal(vVert, vNow - vNormalRadius, vOld - vNormalRadius, vFaceNorm, fNowDist, fOldDist, fLayDist);

                            // �R�p�`�|���S���ƒ����i���C�j�Ƃ̐ڐG������s��
							// �E�I�u�W�F�N�g�̒��S�_����@�������̋t�����ɔ��a���ړ��������_�Ŕ��肷��
							if (checkLay(vVert, vNow - vNormalRadius, vOld - vNormalRadius, vFaceNorm, fNowDist, fOldDist, fLayDist, vInsPt) == 1)
							{
								bRet = true;   // �ڐG���Ă���
								float len = magnitude(vOld - vInsPt);
								if (fLenMin > len)   // ���߂���_��T��
								{
									fLenMin = len;
									if (m_bMoveFlag)  // �R���W�����}�b�v���ړ����Ă��邩�ǂ���
									{
										// �R���W�����}�b�v���ړ����Ă���Ƃ��́A���ۂ̕`��ʒu�ɖ߂��ďo�͂���
										vHit = XMVector3TransformCoord(vInsPt + vNormalRadius, m_mWorld); // �ڐG�ʒu��@�������ɔ��a���߂��Ă��	  // -- 2024.9.10
										vNormal = XMVector3TransformCoord(vFaceNorm, GetRotateMatrix(m_mWorld));
										vNormal = normalize(vNormal);
									}
									else {
										vHit = vInsPt + vNormalRadius; // �ڐG�ʒu��@�������ɔ��a���߂��Ă��
										vNormal = vFaceNorm;
									}
								}
							}
							else {
								// �ڐG���Ă��Ȃ��Ƃ��́A�|���S����@�������Ɏ����グ�����ɂ��A
								// �|���S���̐؂�ڂł��蔲���Ă���\��������̂ŁA
								// �ēx�A�����̒��S�_�ŐڐG������s��
								getDistNormal(vVert, vNow, vOld, vFaceNorm, fNowDist, fOldDist, fLayDist);

								// �R�p�`�|���S���ƒ����i���C�j�Ƃ̐ڐG������s��
								if (checkLay(vVert, vNow, vOld, vFaceNorm, fNowDist, fOldDist, fLayDist, vInsPt) == 1)
								{
									bRet = true;   // �ڐG���Ă���
									float len = magnitude(vOld - vInsPt);
									if (fLenMin > len)   // ���߂���_��T��
									{
										fLenMin = len;
										if (m_bMoveFlag)  // �R���W�����}�b�v���ړ����Ă��邩�ǂ���
										{
											// �R���W�����}�b�v���ړ����Ă���Ƃ��́A���ۂ̕`��ʒu�ɖ߂��ďo�͂���
											vHit = XMVector3TransformCoord(vInsPt - normalize(vNow - vOld)*fRadius, m_mWorld); // �ڐG�ʒu��i�s�����̋t�����ɔ��a���߂��Ă��	 // -- 2024.9.10
											vNormal = XMVector3TransformCoord(vFaceNorm, GetRotateMatrix(m_mWorld));
											vNormal = normalize(vNormal);
										}
										else {
											vHit = vInsPt - normalize(vNow - vOld)*fRadius; // �ڐG�ʒu��i�s�����̋t�����ɔ��a���߂��Ă��
											vNormal = vFaceNorm;
										}
									}
								}
							}
							p = p->pNext;   // ���X�g�\�������ǂ��Ď��̃|���S����T��
						}
					}
				}
			}
		}
	}

	return  bRet;
}

//-----------------------------------------------------------------------------
// �I�u�W�F�N�g�̃��C�ƃ��b�V���ڐG����p�z��Ƃ̐ڐG����ƃX���X������������
//																		// -- 2024.9.10
//	�d�͂��l�����A�ڐG����ƈړ����s��
//  �܂��A����͂Q�x�s��
//  
//  const VECTOR3& positionOld     �I�u�W�F�N�g��1�O�̈ʒu
//  VECTOR3& position              �I�u�W�F�N�g�̌��݈ʒu(in/out)
//  float fRadius                  �I�u�W�F�N�g�̔��a�i�ȗ��l��0.2�j
//  
//  �߂�l�@bool bRet
//		�ڐG�����Ƃ��@		true
//		�ڐG���Ă��Ȃ��Ƃ�	false
//-----------------------------------------------------------------------------
bool CCollision::IsCollisionMove(const VECTOR3& positionOld, VECTOR3& position, float fRadius)
{
	VECTOR3 vHit, vNormal;
	return IsCollisionMove( positionOld, position, vHit, vNormal, fRadius);
}
//-----------------------------------------------------------------------------
// �I�u�W�F�N�g�̃��C�ƃ��b�V���ڐG����p�z��Ƃ̐ڐG����ƃX���X������������
//																		// -- 2024.9.10
//	�d�͂��l�����A�ڐG����ƈړ����s��
//  �܂��A����͂Q�x�s��
//  
//  const VECTOR3& positionOld     �I�u�W�F�N�g��1�O�̈ʒu
//  VECTOR3& position              �I�u�W�F�N�g�̌��݈ʒu(in/out)
//  VECTOR3 &vHit					�ڐG�_�̍��W�i�o�́j
//  VECTOR3 &vNormal				�ڐG�_�̖@���x�N�g���i�o�́j
//  float fRadius					�I�u�W�F�N�g�̔��a�i�ȗ��l��0.2�j
// 
//  �߂�l�@bool bRet
//		�ڐG�����Ƃ��@		true
//		�ڐG���Ă��Ȃ��Ƃ�	false
//-----------------------------------------------------------------------------
bool CCollision::IsCollisionMove(const VECTOR3& positionOld, VECTOR3& position, VECTOR3& vHit, VECTOR3& vNormal, float fRadius)
{

	bool bRet = false;
	int  nRet = 0;

	VECTOR3 vOld = positionOld;
	VECTOR3 vNow = position;

	// �ړ��}�b�v�̎��O�������s��
	if (m_bMoveFlag)  // �R���W�����}�b�v���ړ����Ă��邩�ǂ���
	{
		// �R���W�����}�b�v���ړ����Ă���Ƃ��́A�L�����N�^�[���t�Ɉړ����Ă���ƌ��Ȃ��Ĕ��������
		// ���̂��߂ɁA�L�����N�^�[�̈ʒu�}�g���b�N�X�Ƀ}�b�v�ړ��̋t�s����|���Ĕ�����s��
		vOld = XMVector3TransformCoord( vOld, m_mWorldInv);
		vNow = XMVector3TransformCoord( vNow, m_mWorldInv);
	}
	else {
		// �R���W�����}�b�v���ړ����Ă��Ȃ��Ƃ��́A�L�����N�^�[�̈ʒu�����̂܂܎g�p����
	}

	// �ڐG����ƃX���X��������������s��
	nRet = checkCollisionMove(vOld, vNow, vHit, vNormal, fRadius);    // -- 2022.11.14
	if ( nRet != 0)   // �ڐG���Ĉړ������Ƃ��́A2�x�ڂ̐ڐG����ƈړ����s��
	{
		checkCollisionMove(vOld, vNow, vHit, vNormal, fRadius);    // -- 2022.11.14
	}

	// �ړ��}�b�v�̎��㏈�����s��
	if (m_bMoveFlag)  // �R���W�����}�b�v���ړ����Ă��邩�ǂ���
	{
		// �R���W�����}�b�v���ړ����Ă���Ƃ��́A�}�b�v�ړ��̍s����|���Č��̒l�ɖ߂�
		position = XMVector3TransformCoord(vNow, m_mWorld);
	}
	else {
		// �R���W�����}�b�v���ړ����Ă��Ȃ��Ƃ��́A���̂܂܎g�p����
		position = vNow;
	}
	
	if (nRet != 0) bRet = true;

	return bRet;
}

//-----------------------------------------------------------------------------
// �I�u�W�F�N�g�̃��C�ƃ��b�V���ڐG����p�z��Ƃ̐ڐG����ƃX���X������������
//																		2024.9.10
//  
//	�d�͂��l�����A�ڐG����ƓK�؂Ȉʒu�ւ̈ړ����s��
//  
//  const VECTOR3& positionOld   �I�u�W�F�N�g��1�O�̈ʒu
//  VECTOR3& position            �I�u�W�F�N�g�̌��݈ʒu(in/out)
//  VECTOR3 &vHit				 �ڐG�_�̍��W�i�o�́j
//  VECTOR3 &vNormal			 �ڐG�_�̖@���x�N�g���i�o�́j
//  float fRadius                �I�u�W�F�N�g�̔��a
//  
//  �߂�l�@int nRet
//		�ڐG�����Ƃ��@		�P
//		�ڐG���Ă��Ȃ��Ƃ�	�O
//		�Ȃ��A�����ŋߐڂ̂Ƃ��@�Q
//-----------------------------------------------------------------------------
int CCollision::checkCollisionMove(const VECTOR3& positionOld, VECTOR3& position, VECTOR3& vHit, VECTOR3& vNormal, float fRadius)
{
	int      nRet = 0;
	int      n, i;
	int      x, y, z, m;
	int      nStartX, nEndX, nStartY, nEndY, nStartZ, nEndZ;
	ChkFace* p;

	VECTOR3  vNow, vOld;

	// �ʒu�𓾂�
	vOld = positionOld;
	vNow = position;

	VECTOR3  vVert[3], vFaceNorm, vInsPt, vMove;
	float    fNowDist, fOldDist, fLayDist;

	float    fLenMin = 9999999.0f;				// ��_�ƈړ��O�_�Ƃ̋����̍ŏ��l
	float    fNowDistSave;
	VECTOR3  vFaceNormSave;

	float    fNearMin = 9999999.0f;				// �H�����݋����ŏ��l
	float    fNowDistNearSave;
	VECTOR3  vFaceNormNearSave;

	// ���C�ƃ��b�V���Ƃ̏Փ˔�����s��
	for (n = 0; n<m_nNum; n++)
	{
		// ���������x�z��̌���
		for (i = 0; i < MESHCKTBL_FACE_MAX; i++)
		{
			if (m_ChkColMesh[n].ChkBlkArray[i].ppChkFace == nullptr) break;	// �z��Ƀf�[�^���Ȃ��Ƃ�

			// �z��̑ΏۂƂ���J�n�_�ƏI���_�̃u���b�N�ԍ������߂�
			getMeshLimit(n, i, vNow, vOld, fRadius, nStartX, nEndX, nStartY, nEndY, nStartZ, nEndZ);

			// �z��̌������J�n����
			for (y = nStartY; y <= nEndY; y++) {
				for (z = nStartZ; z <= nEndZ; z++) {
					for (x = nStartX; x <= nEndX; x++) {
						m = y * m_ChkColMesh[n].ChkBlkArray[i].dwNumX * m_ChkColMesh[n].ChkBlkArray[i].dwNumZ
							+ z * m_ChkColMesh[n].ChkBlkArray[i].dwNumX + x;

						p = m_ChkColMesh[n].ChkBlkArray[i].ppChkFace[m];

						while (p != nullptr)  // ���X�g�\���̍Ō�܂�
						{
							// �R�p�`�|���S���̒l�𓾂�
							vVert[0] = m_ColArray[n].pVert[p->pFace->dwIdx[0]];
							vVert[1] = m_ColArray[n].pVert[p->pFace->dwIdx[1]];
							vVert[2] = m_ColArray[n].pVert[p->pFace->dwIdx[2]];
							vFaceNorm = p->pFace->vNormal;

							// �����̂R�p�`�|���S���@�������̋��������߂�
							getDistNormal(vVert, vNow, vOld, vFaceNorm, fNowDist, fOldDist, fLayDist);

							// �R�p�`���ʂƈړ���_���ߐڂ��Ă��邩�̃`�F�b�N���s��
							if (checkNear(vVert, vNow, vFaceNorm, fNowDist, fRadius, vInsPt) == 1)
							{
								nRet = 2;   // �ߐڂ��Ă���

								if (fNearMin > fNowDist)		// ���H������ł��鋗����T��
								{
									fNearMin = fNowDist;
									fNowDistNearSave = fNowDist;
									vFaceNormNearSave = vFaceNorm;

									if (m_bMoveFlag)  // �R���W�����}�b�v���ړ����Ă��邩�ǂ���    // -- 2022.11.14
									{
										// �R���W�����}�b�v���ړ����Ă���Ƃ��́A���ۂ̕`��ʒu�ɖ߂��ďo�͂���
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
								// �R�p�`�|���S���ƒ����i���C�j�Ƃ̐ڐG������s��
								if (checkLay(vVert, vNow, vOld, vFaceNorm, fNowDist, fOldDist, fLayDist, vInsPt) == 1)
								{
									nRet = 1;   // �ڐG���Ă���

									float len = magnitude(vOld - vInsPt);
									if (fLenMin > len)		// ���߂���_��T��
									{
										fLenMin = len;
										fNowDistSave = fNowDist;
										vFaceNormSave = vFaceNorm;

										if (m_bMoveFlag)  // �R���W�����}�b�v���ړ����Ă��邩�ǂ���    // -- 2022.11.14
										{
											// �R���W�����}�b�v���ړ����Ă���Ƃ��́A���ۂ̕`��ʒu�ɖ߂��ďo�͂���
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
							p = p->pNext;     // ���X�g�\�������ǂ��Ď��̃|���S����T��
						}
					}
				}
			}
		}
	}

	// �ڐG�����̈ړ�����
	//  0:�ڐG�E�ߐڂ��Ă��Ȃ�  1:�ڐG���Ă���  2:�ߐڂ��Ă���
	// �i�ڐG�E�ߐڂ��Ă���Ƃ��́A���ʂ��fRadius�����O���Ɉړ�������j
	if (nRet != 0)
	{
		if (nRet == 1)
		{
			// �ڐG���Ă���
			vMove = vNow + vFaceNormSave * (-fNowDistSave + fRadius);
		}
		else {
			// �ߐڂ��Ă���
			vMove = vNow + vFaceNormNearSave * (-fNowDistNearSave + fRadius);
		}

		// �����̈ړ�����
		position = vMove;

		nRet = 1;   // �S�ĐڐG�Ƃ���
	}
	return  nRet;

}

//-----------------------------------------------------------------------------
// �I�u�W�F�N�g�̃��C�ƃ��b�V���ڐG����p�z��Ƃ̐ڐG����ƃX���X������������
//																		2024.9.10
//	���፷�Əd�͂��l�������A�ڐG����ƈړ����s��
//  �悸�@�ǂ̔���i�Q�x�s���j���s���A���ɇA��������A�Ō�ɇB��������s��
//  
//  const VECTOR3& positionOld   �I�u�W�F�N�g��1�O�̈ʒu
//  VECTOR3& position            �I�u�W�F�N�g�̌��݈ʒu(in/out)
//  float fRadius                �I�u�W�F�N�g�̔��a�i�ȗ��l��0.2�j
//  
//  �߂�l�@CollRet  RetFloor
//		�G���[		= clError
//		�ʏ���ړ�	= clMove
//		���n		= clLand
//		������		= clFall
//-----------------------------------------------------------------------------
CollRet CCollision::IsCollisionMoveGravity(const VECTOR3& positionOld, VECTOR3& position, float fRadius)
{
	VECTOR3 vHit, vNormal;
	return IsCollisionMoveGravity(positionOld, position, vHit, vNormal, fRadius);
}
//-----------------------------------------------------------------------------
// �I�u�W�F�N�g�̃��C�ƃ��b�V���ڐG����p�z��Ƃ̐ڐG����ƃX���X������������
//																		2024.9.10
//	���፷�Əd�͂��l�������A�ڐG����ƈړ����s��
//  �悸�@�ǂ̔���i�Q�x�s���j���s���A���ɇA��������A�Ō�ɇB��������s��
//  
//  const VECTOR3& positionOld     �I�u�W�F�N�g��1�O�̈ʒu
//  VECTOR3& position              �I�u�W�F�N�g�̌��݈ʒu(in/out)
//  VECTOR3 &vHit				   �ڐG�_�̍��W�i�o�́j
//  VECTOR3 &vNormal			   �ڐG�_�̖@���x�N�g���i�o�́j
//  float fRadius                  �I�u�W�F�N�g�̔��a�i�ȗ��l��0.2�j
//  
//  �߂�l�@CollRet  RetFloor
//		�G���[		= clError
//		�ʏ���ړ�	= clMove
//		���n		= clLand
//		������		= clFall
//-----------------------------------------------------------------------------
CollRet CCollision::IsCollisionMoveGravity(const VECTOR3& positionOld, VECTOR3& position, VECTOR3& vHit, VECTOR3& vNormal, float fRadius)
{
	CollRet  RetFloor = clError;
	int nRetWall = 0;

	VECTOR3 vOld = positionOld;
	VECTOR3 vNow = position;

	// �ړ��}�b�v�̎��O�������s��
	if (m_bMoveFlag)  // �R���W�����}�b�v���ړ����Ă��邩�ǂ���
	{
		// �R���W�����}�b�v���ړ����Ă���Ƃ��́A�L�����N�^�[���t�Ɉړ����Ă���ƌ��Ȃ��Ĕ��������
		// ���̂��߂ɁA�L�����N�^�[�̈ʒu�}�g���b�N�X�Ƀ}�b�v�ړ��̋t�s����|���Ĕ�����s��
		vOld = XMVector3TransformCoord(vOld, m_mWorldInv);
		vNow = XMVector3TransformCoord(vNow, m_mWorldInv);
	}
	else {
		// �R���W�����}�b�v���ړ����Ă��Ȃ��Ƃ��́A�L�����N�^�[�̈ʒu�����̂܂܎g�p����
	}

	// ��������ϐ��̏�����
	initHeightCheck();

	//	���b�V���̕ǂƂ̐ڐG����ƓK�؂Ȉʒu�ւ̈ړ�
	nRetWall = checkWallMove(vOld, vNow, vHit, vNormal, fRadius);    // -- 2022.11.14

	if (nRetWall != 0)   // �ڐG���Ĉړ������Ƃ��́A2�x�ڂ̐ڐG������s��
	{
		checkWallMove(vOld, vNow, vHit, vNormal, fRadius);    // -- 2022.11.14
	}
	
	// ���Ƃ̐ڐG����Ə㉺�ړ�
	// �Ȃ��AUNDERFOOTLIMIT�i0.05f)�́A������̍���
	checkHeight(vOld, vNow, UNDERFOOTLIMIT);	//	���݈ʒu���Ⴂ�ʂ̒��ň�ԍ����ʂ�T��
	RetFloor = checkFloorMove(vOld, vNow);	//	���𔻒肵���ɉ����Ĉړ���A���n��������B�߂�l clError:-1�G���[ clMove:�ʏ���ړ�1 clLand:���n2 clFall:������3
	
	// �ړ��}�b�v�̎��㏈�����s��
	if (m_bMoveFlag)  // �R���W�����}�b�v���ړ����Ă��邩�ǂ���
	{
		// �R���W�����}�b�v���ړ����Ă���Ƃ��́A�}�b�v�ړ��̍s����|���Č��̒l�ɖ߂�
		vNow = XMVector3TransformCoord(vNow, m_mWorld);
		if (nRetWall == 0 && (RetFloor == clMove || RetFloor == clLand))  // �ǂɐڐG���Ă��Ȃ��āA�ړ��}�b�v�ɒ��n���Ă���Ƃ�
		{
			// �ړ��}�b�v�̐����ړ��̏���
			// ���������Ɉړ����������ړ�������
			VECTOR3 vMM = GetPositionVector(m_mWorld) - GetPositionVector(m_mWorldOld);
			vNow.x += vMM.x;
			vNow.z += vMM.z;
		}
		position = vNow;
	}
	else {
		// �R���W�����}�b�v���ړ����Ă��Ȃ��Ƃ��́A���̂܂܎g�p����
		position = vNow;
	}

	return RetFloor;
}

//-----------------------------------------------------------------------------
// �I�u�W�F�N�g�̃��C�ƕǃ��b�V���Ƃ̐ڐG����ƃX���X������������
//																		2022.11.14
// �ǂƔ��肳�ꂽ�O�p�`�|���S���̂ݐڐG������s��
// �ڐG����ƓK�؂Ȉʒu�ւ̈ړ��i�w�y�����̂݁j���s��
//  
//  const VECTOR3& positionOld   �I�u�W�F�N�g��1�O�̈ʒu
//  VECTOR3& position            �I�u�W�F�N�g�̌��݈ʒu(in/out)
//  VECTOR3 &vHit				 �ڐG�_�̍��W�i�o�́j
//  VECTOR3 &vNormal			 �ڐG�_�̖@���x�N�g���i�o�́j
//  float fRadius                �I�u�W�F�N�g�̔��a
//  
//  �߂�l�@int nRet
//		�ڐG�����Ƃ��@		�P
//		�ڐG���Ă��Ȃ��Ƃ�	�O
//-----------------------------------------------------------------------------
int CCollision::checkWallMove(const VECTOR3& positionOld, VECTOR3& position, VECTOR3& vHit, VECTOR3& vNormal, float fRadius)
{
	int      nRet = 0;
	int      n, i;
	int      x, y, z, m;
	int      nStartX, nEndX, nStartY, nEndY, nStartZ, nEndZ;
	ChkFace* p;

	// �ʒu�𓾂�
	VECTOR3  vOld = positionOld;
	VECTOR3  vNow = position;

	VECTOR3  vVert[3], vFaceNorm, vInsPt, vMove;
	float    fNowDist, fOldDist, fLayDist;

	float    fLenMin = 9999999.0f;				// ��_�ƈړ��O�_�Ƃ̋����̍ŏ��l
	float    fNowDistSave;
	VECTOR3  vFaceNormSave;

	float    fNearMin = 9999999.0f;				// �H�����݋����ŏ��l
	float    fNowDistNearSave;
	VECTOR3  vFaceNormNearSave;

	// �W�����v�ŏ㏸�����ǂ������肷��B(���ݍ������㏸���Ă��邩)	// -- 2019.9.3
	// �i�덷���l������0.0001f�𒲐�����j
	bool bJumpUp = (vNow.y - 0.0001f > vOld.y);

	// ���C�ƃ��b�V���i�ǁj�Ƃ̏Փ˔�����s��
	for (n = 0; n<m_nNum; n++)
	{
		// ���������x�z��̌���
		for (i = 0; i < MESHCKTBL_FACE_MAX; i++)
		{
			if (m_ChkColMesh[n].ChkBlkArray[i].ppChkFace == nullptr) break;	// �z��Ƀf�[�^���Ȃ��Ƃ�

			// �z��̑ΏۂƂ���J�n�_�ƏI���_�̃u���b�N�ԍ������߂�
			getMeshLimit(n, i, vNow, vOld, fRadius, nStartX, nEndX, nStartY, nEndY, nStartZ, nEndZ);

			// �z��̌������J�n����
			for (y = nStartY; y <= nEndY; y++) {
				for (z = nStartZ; z <= nEndZ; z++) {
					for (x = nStartX; x <= nEndX; x++) {
						m = y * m_ChkColMesh[n].ChkBlkArray[i].dwNumX * m_ChkColMesh[n].ChkBlkArray[i].dwNumZ
							+ z * m_ChkColMesh[n].ChkBlkArray[i].dwNumX + x;

						p = m_ChkColMesh[n].ChkBlkArray[i].ppChkFace[m];

						while (p != nullptr)  // ���X�g�\���̍Ō�܂�
						{
							// �ǂ������𔻒肵�ǂ̏ꍇ�̂ݔ�����s��(���̎��͓ǂݔ�΂�)
							// �������A�W�����v�㏸���͑S�ĕǂƌ��Ȃ�  // -- 2019.9.3
							if (!bJumpUp && (p->pFace->vNormal.y > GROUND || p->pFace->vNormal.y < -GROUND))  // -- 2019.9.3
							{
								;
							}
							else {
								// �R�p�`�|���S���̒l�𓾂�
								vVert[0] = m_ColArray[n].pVert[p->pFace->dwIdx[0]];
								vVert[1] = m_ColArray[n].pVert[p->pFace->dwIdx[1]];
								vVert[2] = m_ColArray[n].pVert[p->pFace->dwIdx[2]];
								vFaceNorm = p->pFace->vNormal;

								// �����̂R�p�`�|���S���@�������̋��������߂�
								getDistNormal(vVert, vNow, vOld, vFaceNorm, fNowDist, fOldDist, fLayDist);

								// �R�p�`���ʂƈړ���_���ߐڂ��Ă��邩�̃`�F�b�N���s��
								if (checkNear(vVert, vNow, vFaceNorm, fNowDist, fRadius, vInsPt) == 1)
								{
									nRet = 2;   // �ߐڂ��Ă���

									if (fNearMin > fNowDist)		// ���H������ł��鋗����T��
									{
										fNearMin = fNowDist;
										fNowDistNearSave = fNowDist;
										vFaceNormNearSave = vFaceNorm;

										if (m_bMoveFlag)  // �R���W�����}�b�v���ړ����Ă��邩�ǂ���    // -- 2022.11.14
										{
											// �R���W�����}�b�v���ړ����Ă���Ƃ��́A���ۂ̕`��ʒu�ɖ߂��ďo�͂���
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
									// �R�p�`�|���S���ƒ����i���C�j�Ƃ̐ڐG������s��
									if (checkLay(vVert, vNow, vOld, vFaceNorm, fNowDist, fOldDist, fLayDist, vInsPt) == 1)
									{
										nRet = 1;   // �ڐG���Ă���

										float len = magnitude(vOld - vInsPt);
										if (fLenMin > len)		// ���߂���_��T��
										{
											fLenMin = len;
											fNowDistSave = fNowDist;
											vFaceNormSave = vFaceNorm;

											if (m_bMoveFlag)  // �R���W�����}�b�v���ړ����Ă��邩�ǂ���    // -- 2022.11.14
											{
												// �R���W�����}�b�v���ړ����Ă���Ƃ��́A���ۂ̕`��ʒu�ɖ߂��ďo�͂���
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
							p = p->pNext;   // ���X�g�\�������ǂ��Ď��̃|���S����T��
						}
					}
				}
			}
		}
	}

	// �ڐG�����̈ړ�����
	//  0:�ڐG�E�ߐڂ��Ă��Ȃ�  1:�ڐG���Ă���  2:�ߐڂ��Ă���
	// �i�ڐG�E�ߐڂ��Ă���Ƃ��́A���ʂ��fRadius�����O���Ɉړ�������j
	if (nRet != 0)
	{
		if (nRet == 1)
		{
			// �ڐG���Ă���
			vMove = vNow + vFaceNormSave * (-fNowDistSave + fRadius);
		}
		else {
			// �ߐڂ��Ă���
			vMove = vNow + vFaceNormNearSave * (-fNowDistNearSave + fRadius);
		}

		// �����̈ړ������B�w�y�����݈̂ړ����A�x�����ɂ͈ړ������Ȃ�
		vNow.x = vMove.x;
		vNow.z = vMove.z;
		position = vNow;

		nRet = 1;   // �S�ĐڐG�Ƃ���
	}

	return  nRet;

}


//----------------------------------------------------------------------------
//	��������ϐ��̏�����
//																		2019.8.6
//
//	�����E�߂�l�@�Ȃ�
//----------------------------------------------------------------------------
void CCollision::initHeightCheck()
{
	m_vVertexH[0] = VECTOR3(0.0f, 0.0f, 0.0f);
	m_vVertexH[2] = m_vVertexH[1] = m_vVertexH[0];
	m_pIndex = nullptr;
	m_vNormalH = VECTOR3(0.0f, 1.0f, 0.0f);
	m_fHeight = -100000000.0f;    // �Œ�l
}


//----------------------------------------------------------------------------
//	���݈ʒu���Ⴂ�ʂ̒��ň�ԍ����ʂ�T��
//																			2024.9.10
//
//	�����̊֐��̖ړI�́A�����������Ă���Ƃ��A�K�؂ȏ��ɒ��n�����邽�߂ɕK�v�ł���
//	�@���s��A�u��������ϐ��v�ɒl���ݒ肳���B
//
//  const VECTOR3& positionOld   �I�u�W�F�N�g��1�O�̈ʒu
//  VECTOR3& position            �I�u�W�F�N�g�̌��݈ʒu(in/out)
//	const float& fObjheight      �I�u�W�F�N�g�̒��S�̍���UNDERFOOTLIMIT
//
//	�߂�l bool bRet
//		true  = ��荂���ʂ𔭌�����
//		false = �������ĂȂ�
//----------------------------------------------------------------------------
bool CCollision::checkHeight(const VECTOR3& positionOld, VECTOR3& position, const float fObjheight)
{
	bool     bRet = false;
	int      n, i;
	int      x, y, z, m;
	int      nStartX, nEndX, nStartY, nEndY, nStartZ, nEndZ;
	ChkFace* p;
	const    float  fHeightRadius = 1.0f;  // �������莞�̃L�����N�^�̔��a���Œ�1.0�Ƃ���

	// ���[���h�}�g���b�N�X����ʒu�𓾂�
	VECTOR3  vOld = positionOld;
	VECTOR3  vNow = position;
	VECTOR3  vVert[3], vFaceNorm;

	struct ColFace*  pWIndex = nullptr;
	VECTOR3 vXP(vNow.x, 0, vNow.z);     // �ړ���_��Y�����x�N�g�����O�ɂ��ĕ��ʏ�̈ʒu�𓾂�
	FLOAT wkHeight = m_fHeight;
	FLOAT MaxY = max(vOld.y, vNow.y);
	FLOAT MinY = min(vOld.y, vNow.y);

	float fRadius = (fObjheight < fHeightRadius) ? fHeightRadius : fObjheight;  // �L�����N�^�̔��a���Œ�fHeightRadius�Ƃ���
	
	CAABB     NowAABB;							// �ړ���_vNow��AABB
	NowAABB.MakeAABB(vOld, vNow, fRadius);		// �ړ���_vNow��AABB���쐬����B�Œᕝ���a��fRadius�Ƃ���B

	// ���C�ƃ��b�V���i���j�Ƃ̏Փ˔�����s��
	for (n = 0; n<m_nNum; n++)
	{
		// ���������x�z��̌���
		for (i = 0; i < MESHCKTBL_FACE_MAX; i++)
		{
			if (m_ChkColMesh[n].ChkBlkArray[i].ppChkFace == nullptr) break;	// �z��Ƀf�[�^���Ȃ��Ƃ�

			// �z��̑ΏۂƂ���J�n�_�ƏI���_�̃u���b�N�ԍ������߂�
			getMeshLimit(n, i, vNow, vOld, fRadius, nStartX, nEndX, nStartY, nEndY, nStartZ, nEndZ);

			// �z��̌������J�n����
			for (y = nStartY; y <= nEndY; y++)
			{
				for (z = nStartZ; z <= nEndZ; z++)
				{
					for (x = nStartX; x <= nEndX; x++)
					{
						m = y * m_ChkColMesh[n].ChkBlkArray[i].dwNumX * m_ChkColMesh[n].ChkBlkArray[i].dwNumZ
							+ z * m_ChkColMesh[n].ChkBlkArray[i].dwNumX + x;

						p = m_ChkColMesh[n].ChkBlkArray[i].ppChkFace[m];

						while (p != nullptr)  // ���X�g�\���̍Ō�܂�
						{
							// �ǂ������𔻒肵�A���̏ꍇ�̂݃`�F�b�N����(�ǂ̎��͓ǂݔ�΂�)
							if (p->pFace->vNormal.y <= GROUND)
							{
								;
							}
							else {
								if (p->pFace->AABB.HitcheckXZ(NowAABB))  // �܂��AAABB�����ʏ�ŐڐG���Ă��邩�̃`�F�b�N������
								{
									// �R�p�`�|���S���̒l�𓾂�
									vVert[0] = m_ColArray[n].pVert[p->pFace->dwIdx[0]];
									vVert[1] = m_ColArray[n].pVert[p->pFace->dwIdx[1]];
									vVert[2] = m_ColArray[n].pVert[p->pFace->dwIdx[2]];
									vFaceNorm = p->pFace->vNormal;

									// Y�����x�N�g�����O�ɂ��ĕ��ʏ�̂R�p�`�̒��_�ʒu�𓾂�
									VECTOR3 vX0(vVert[0].x, 0, vVert[0].z);
									VECTOR3 vX1(vVert[1].x, 0, vVert[1].z);
									VECTOR3 vX2(vVert[2].x, 0, vVert[2].z);

									// �ړ���_�̕��ʏ�̈ʒu���O�p�`�|���S���̕��ʏ�̈ʒu�̒��ɂ��邩�ǂ����̔���
									//   ��_�����_�Ƃ����Ƃ��A�R�p�`�|���S���̊e���_�ƌ��_�Ƃ̊p�x�����v����֐�AddAngle�ł���
									//   ���̊֐��̖߂�l���R�U�O���i�Q�΁j�̎��́A�ڐG�_���R�̂R�c�x�N�g���̒��ɂ���
									//   �������A�덷���l�����Q�΂ł͂Ȃ�ADDANGLELIMIT(1.99f) * �΂Ŕ��f���Ă���B
									if (AddAngle(vX0 - vXP, vX1 - vXP, vX2 - vXP) >= ADDANGLELIMIT * XM_PI)
									{
										// �ړ���_���O�p�`�|���S���̒��ɂ���Ƃ�
										float FaceDist = dot(vFaceNorm, vVert[0]);	// ���_����O�p�`���ʂ܂ł̋���

										// �ړ���_���O�p�`�|���S���ƌ�������_�̍���
										float wh = -(vFaceNorm.x*vXP.x + vFaceNorm.z*vXP.z - FaceDist) / vFaceNorm.y;

										wh += fObjheight;	// �����ɃI�u�W�F�N�g�̏�����̍�������������

										if (wh < MaxY+UPHILLLIMIT && wh > wkHeight)	// ���݂̍������Ⴍ�čő�̍����ł��邩�H�]�T��UPHILLLIMIT�Ƃ�
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
							p = p->pNext;   // ���X�g�\�������ǂ��Ď��̃|���S����T��
						}
					}
				}
			}
		}
	}

	if (pWIndex != nullptr)
	{
		//	�����������݂̍������Ⴍ�čő卂���̖ʂ��L������
		m_pIndex = pWIndex;			// �ʃf�[�^�̃A�h���X
		m_vNormalH = m_pIndex->vNormal;
		//m_fHeight = (wkHeight == m_fHeight) ? m_fHeight : wkHeight;  // �ő卂�����L������
		m_fHeight = wkHeight;  // �ő卂�����L������  // -- 2019.9.3

		bRet = true;	// �ő卂������������
	}

	return  bRet;
}

//----------------------------------------------------------------------------
//	���𔻒肵���ɉ����Ĉړ���A���n��������
//																		2019.8.6
//�@�i���Ӂj���̊֐��́AHeightCheck���s��ɌĂяo�����ƁI�I
//				
//  const VECTOR3& positionOld   �I�u�W�F�N�g��1�O�̈ʒu
//  VECTOR3& position            �I�u�W�F�N�g�̌��݈ʒu(in/out)
//				
//  �߂�l�@CollRet  RetFloor
//		�G���[		= clError
//		�ʏ���ړ�	= clMove
//		���n		= clLand
//		������		= clFall
//----------------------------------------------------------------------------
CollRet CCollision::checkFloorMove(const VECTOR3& positionOld, VECTOR3& position)
{
	CollRet  Ret = clError;

	// �ʒu�𓾂�
	VECTOR3  vOld = positionOld;
	VECTOR3  vNow = position;

	// �W�����v�ŏ㏸�����ǂ������肷��B(���ݍ������㏸���Ă��邩)
	// �i�덷���l������0.0001f�𒲐�����j
	bool bJumpUp = (vNow.y - 0.0001f > vOld.y);	

	FLOAT MaxY = max(vOld.y, vNow.y);
	FLOAT MinY = min(vOld.y, vNow.y);

	//vOld = vNow;   // ?????

	if (m_pIndex && m_fHeight <= vNow.y + 0.000001f && m_fHeight >= vNow.y - 0.000001f) {		// �ʏ�𐅕��Ɉړ����̂Ƃ��i�덷���l����0.000001f�𒲐�����j// -- 2025.1.6
		Ret = clMove;
	}
	else if (!bJumpUp && m_pIndex && m_fHeight >= vNow.y) {
		// �W�����v�㏸���łȂ��āA�����ړ���̍�����荂���Ƃ��i���n�����Ƃ��j
		if (m_vNormalH.y > GROUND) {
			// ���ɒ��n����
			// �i�덷�͍l�����Ȃ��j
			vNow.y = m_fHeight;
			Ret = clLand;	 // ���n
		}
		else {
			// �ǂɒ��n����
			// �i�덷���l����0.000001f�𒲐�����j
			vNow.y = m_fHeight + 0.000001f;
			Ret = clFall;	// ������
		}
	}
	else {
		if (!bJumpUp && m_pIndex && m_vNormalH.y > GROUND && m_fHeight <= MaxY && m_fHeight >= MinY + LOWFLOORLIMIT)
		{
			// �W�����v�㏸���łȂ��āA�����ړ��O�ƈړ���̍����̊Ԃɂ���Ƃ��B�܂��́A��������LOWFLOORLIMIT(-0.2m)�ȓ��̎�
			vNow.y = m_fHeight;	// �ړ���̍��������̍����ɕ␳����
			Ret = clLand; 	 // ���n
			//MessageBox(0, _T("CheckFloorMove()   Ret=2 clLand") , nullptr, MB_OK);
		}
		else {
			// �W�����v�㏸���܂��́A�������̂Ƃ�
			Ret = clFall;	// ������
			//MessageBox(0, _T("CheckFloorMove()   Ret=3 clFall") , nullptr, MB_OK);
		}
	}

	// ���ɍ��킹���㉺�ړ��̏���
	if (Ret >= clMove)	 // �G���[�ł͂Ȃ��Ƃ�
	{
		position = vNow;
	}

	return Ret;
}


//-----------------------------------------------------------------------------
// �ړ��x�N�g���̂R�p�`�|���S���@�������̋��������߂�֐�
//																	2019.9.6
//  
//  const VECTOR3  vVec[3]		�O�p�`�x�N�g��
//  const VECTOR3& vNow			�����̏I�_
//  const VECTOR3& vOld			�����̎n�_
//  const VECTOR3& vFaceNorm	���ʂ̖@��
//  float& fNowDist				�ړ���_�ƎO�p�`���ʂƂ̋����i�o�́j
//  float& fOldDist				�ړ��O�_�ƎO�p�`���ʂƂ̋����i�o�́j
//  float& fLayDist				�ړ��x�N�g��Lay�̖@�������̋����i�o�́j
//  
//  �߂�l
//      �Ȃ�
//-----------------------------------------------------------------------------
void  CCollision::getDistNormal(const VECTOR3 vVec[], const VECTOR3& vNow, const VECTOR3& vOld, const VECTOR3& vFaceNorm, float& fNowDist, float& fOldDist, float& fLayDist)
{
	float   fFaceDist = dot(vFaceNorm, vVec[0]);		// ���_����O�p�`���ʂ܂ł̋���
	fNowDist = dot(vFaceNorm, vNow) - fFaceDist;		// �ړ���_�ƎO�p�`���ʂƂ̋����B���̎��͕\���A���̎��͗���
	fOldDist = dot(vFaceNorm, vOld) - fFaceDist;		// �ړ��O�_�ƎO�p�`���ʂƂ̋����B���̎��͕\���A���̎��͗���
	fLayDist = fOldDist - fNowDist;						// �ړ��x�N�g��Lay�̖@�������̋���
}

//-----------------------------------------------------------------------------
// ���C�ƂR�p�`�|���S���Ƃ̐ڐG����
//																	2019.8.6
//  
//  const VECTOR3  vVec[3]		�O�p�`�x�N�g��
//  const VECTOR3& vNow			�����̏I�_
//  const VECTOR3& vOld			�����̎n�_
//  const VECTOR3& vFaceNorm	���ʂ̖@��
//  const float& fNowDist		�ړ���_�ƎO�p�`���ʂƂ̋���
//  const float& fOldDist		�ړ��O�_�ƎO�p�`���ʂƂ̋���
//  const float& fLayDist		�ړ��x�N�g��Lay�̖@�������̋���
//  VECTOR3 &vHit				�ڐG�_�̍��W�i�o�́j
//  
//  �߂�l�@int
//		�ڐG�����Ƃ��@		�P
//		�ڐG���Ă��Ȃ��Ƃ�	�O
//-----------------------------------------------------------------------------
int  CCollision::checkLay(const VECTOR3 vVec[], const VECTOR3& vNow, const VECTOR3& vOld, const VECTOR3& vFaceNorm, const float& fNowDist, const float& fOldDist, const float& fLayDist, VECTOR3& vHit)
{
	int nRet = 0;

	if (fNowDist <= 0.0f && fOldDist >= 0.0f)	// �ړ��O�_���O�p�`���ʂ̕\�ŁA�ړ���_�����ʂ̗����̎��̂ݔ���
	{
		if (fLayDist != 0.0f)		// �ړ��x�N�g���̖@�������̋������O�̂Ƃ��́A�����ƎO�p�`���ʂ����s�Ȃ̂őΏۊO
		{
			// ���ʂƂ̌�_vInsPt�����߂�
			//   �@�@�ړ��x�N�g���̌�_����ړ���_�܂ł̋����̔䗦�����߂�@�@�@�@�@fNowDist / fLayDist
			//   �A�@��_����ړ���_�܂ł̈ړ��x�N�g�������߂�@�@�@�@�@�@�@�@�@�@�@(vOld - vNow) * fNowDist / fLayDist
			//   �B�@��L�A���ړ���_����������Ƃɂ���āA��_�̃x�N�g�������߂�@�@vNow - ( (vOld - vNow) * fNowDist / fLayDist)
			VECTOR3 vInsPt = vNow - ((vOld - vNow) * fNowDist / fLayDist);

			// �R�p�`�|���S���ƐڐG���Ă��邩�ǂ����̔���
			//   ��_�����_�Ƃ����Ƃ��A�R�p�`�|���S���̊e���_�ƌ��_�Ƃ̊p�x�����v����֐�AddAngle�ł���
			//   ���̊֐��̖߂�l���R�U�O���i�Q�΁j�̎��́A�ڐG�_���R�̂R�c�x�N�g���̒��ɂ���
			//   �������A�덷���l�����Q�΂ł͂Ȃ�ADDANGLELIMIT(1.99f) * �΂Ŕ��f���Ă���B
			if (AddAngle(vVec[0] - vInsPt, vVec[1] - vInsPt, vVec[2] - vInsPt) >= ADDANGLELIMIT * XM_PI)
			{
				nRet = 1;         // ��_���O�p�`�̓��ɂ���̂ŐڐG���Ă���
				vHit = vInsPt;    // ��_��ۑ�
			}
		}
	}

	return nRet;
}


//-----------------------------------------------------------------------------
// ���C�ƂR�p�`�|���S���Ƃ̋ߐڔ���
//																	2019.8.6
//  
//  const VECTOR3  vVec[3]		�O�p�`�x�N�g��
//  const VECTOR3& vNow			�����̏I�_
//  const VECTOR3& vFaceNorm	���ʂ̖@��
//  const float& fNowDist		�ړ���_�ƎO�p�`���ʂƂ̋���
//  float fRadius				�I�u�W�F�N�g�̔��a
//  VECTOR3 &vHit				�ڐG�_�̍��W�i�o�́j
//  
//  �߂�l�@int
//		�ڐG�����Ƃ��@		�P
//		�ڐG���Ă��Ȃ��Ƃ�	�O
//-----------------------------------------------------------------------------
int  CCollision::checkNear(const VECTOR3 vVec[], const VECTOR3& vNow, const VECTOR3& vFaceNorm, const float& fNowDist, float fRadius, VECTOR3& vHit)
{
	int nRet = 0;

	// �ړ���_�����ʂ���+-fRadius�ȓ��̂Ƃ��̂ݔ���
	// �Q��ڂ̋ߐڃ`�F�b�N���̌덷���l������0.00001f���߂Ĕ��肷��
	if (fNowDist > -fRadius + 0.00001f && fNowDist < fRadius - 0.00001f)
	{

		// ���ʂƂ̌�_vInsPt�����߂�
		//   �@�@�ړ���_���畽�ʂ܂ł̖@�������̃x�N�g�������߂�@�@�@�@�@�@�@vFaceNorm * -fNowDist
		//   �A�@��L�A���ړ���_�ɑ������Ƃɂ���āA��_�̃x�N�g�������߂�@�@vNow + vFaceNorm * -fNowDist
		VECTOR3 vInsPt = vNow + vFaceNorm * -fNowDist;

		// �R�p�`�|���S���ƐڐG���Ă��邩�ǂ����̔���
		//   ��_�����_�Ƃ����Ƃ��A�R�p�`�|���S���̊e���_�ƌ��_�Ƃ̊p�x�����v����֐�AddAngle�ł���
		//   ���̊֐��̖߂�l���R�U�O���i�Q�΁j�̎��́A�ڐG�_���R�̂R�c�x�N�g���̒��ɂ���
		//   �������A�덷���l�����Q�΂ł͂Ȃ�ADDANGLELIMIT(1.99f) * �΂Ŕ��f���Ă���B
		if (AddAngle(vVec[0] - vInsPt, vVec[1] - vInsPt, vVec[2] - vInsPt) >= ADDANGLELIMIT * XM_PI)
		{
			nRet = 1;         // ��_���O�p�`�̓��ɂ���̂ŐڐG���Ă���
			vHit = vInsPt;    // ��_��ۑ�
		}
	}

	return nRet;
}

//------------------------------------------------------------------------
//	���[���h�}�g���b�N�X������������
//																			2022.11.14
//  ����
//      const MATRIX4X4& mWorld
//
//	�߂�l 
//------------------------------------------------------------------------
void CCollision::InitWorldMatrix(const MATRIX4X4& mWorld)
{
	m_mWorldOld  = mWorld;
	m_mWorld     = mWorld;
	m_mWorldInv  = XMMatrixInverse(nullptr, m_mWorld);
	m_bMoveFlag  = true;
}

//------------------------------------------------------------------------
//	���[���h�}�g���b�N�X��ݒ肷��
//																			2019.8.6
//  ����
//      const MATRIX4X4& mWorld
//
//	�߂�l 
//------------------------------------------------------------------------
void CCollision::SetWorldMatrix(const MATRIX4X4& mWorld)
{
	m_mWorld = mWorld;
	m_mWorldInv = XMMatrixInverse(nullptr, m_mWorld);
	m_bMoveFlag = true;
}


//------------------------------------------------------------------------  // -- 2020.12.3
//	���������x�}�b�v����R���W�����}�b�v�S�̂��͂�AABB�����߂�
//		
//	�E���̏�����Collision.cpp�̒��ł͎g�p���Ȃ�	
//	�@�O���ŃR���W�����}�b�v�̑傫����m�肽���Ƃ��Ɏg�p����
//		
//  ����
//      VECTOR3& vMin  �o�E���f�B���O�{�b�N�X�̍ŏ��l(Out)
//      VECTOR3& vMax  �o�E���f�B���O�{�b�N�X�̍ő�l(Out)
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
// AABB�i�����s���E�{�b�N�X�FAxis - Aligned Bounding Box�j
//
//------------------------------------------------------------------------

//------------------------------------------------------------------------
//
// �O�p�|���S�����͂�AABB���쐬����
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
// v1�`v2�����̒����ŁA���ar�̕���AABB���쐬����
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
// �Q��AABB�̐ڐG������s��
//
// ����
//    AABB other
//
// �߂�l
//    true:�ڐG���Ă���    false:�ڐG���Ă��Ȃ� 
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
// �Q��AABB�̂w�y�����̂݁i�������ʁj�ڐG������s��
//
// ����
//    AABB other
//
// �߂�l
//    true:�ڐG���Ă���    false:�ڐG���Ă��Ȃ� 
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
// AABB��8���_�̈����̓Y���ʒu�̍��W�l���擾����
//
// ����
//    int nIdx   0�`7�̐����l
//
// �߂�l
//    VECTOR3  ���W�l
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
