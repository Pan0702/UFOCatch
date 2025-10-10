//=============================================================================
//		���b�V���̓ǂݍ��݂ƕ`��̃v���O����
//�@                                                  ver 4.0        2024.10.7
//
//		���b�V������
//
//      (���b�V���R���g���[���N���X�Ńe�N�X�`���̑����Ǘ����s��)
//
//																	FbxMesh.cpp
//=============================================================================
#include "FbxMesh.h"
#include "../GameMain/GameMain.h"
#include "Animator.h"

//------------------------------------------------------------------------
//
//	CFbxMesh  �R���X�g���N�^	
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

	m_fHeightMax = 0.01f;                // �f�B�X�v���[�X�����g�}�b�s���O�̍���     // -- 2020.1.24
	m_iMaxDevide = 1;                   // �f�B�X�v���[�X�����g�}�b�s���O�̕�����   // -- 2020.1.24
	m_vDiffuse = VECTOR4(1, 1, 1, 1);    // �f�B�t���[�Y�F                           // -- 2020.1.24
}
CFbxMesh::CFbxMesh(CFbxMeshCtrl* pFbxMeshCtrl, const TCHAR* FName)
{
	ZeroMemory(this, sizeof(this));
	m_pD3D = pFbxMeshCtrl->m_pD3D;
	m_pShader = pFbxMeshCtrl->m_pShader;
	m_pFbxMeshCtrl = pFbxMeshCtrl;                    // -- 2021.2.4
	Load(FName);

	m_fHeightMax = 0.01f;                // �f�B�X�v���[�X�����g�}�b�s���O�̍���     // -- 2020.1.24
	m_iMaxDevide = 1;                   // �f�B�X�v���[�X�����g�}�b�s���O�̕�����   // -- 2020.1.24
	m_vDiffuse = VECTOR4(1, 1, 1, 1);  // �f�B�t���[�Y�F                           // -- 2020.1.24
}
//------------------------------------------------------------------------
//
//	CFbxMesh  �f�X�g���N�^	
//
//------------------------------------------------------------------------
CFbxMesh::~CFbxMesh()
{
	DestroyD3D();
}
//------------------------------------------------------------------------
//
//	���b�V���̏I������
//
// �����@�Ȃ�
//
//	�߂�l �Ȃ�
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
//	���b�V��(�o�C�i��)�t�@�C���̓ǂݍ��ݏ���
//
// ����
//  const TCHAR*         FName          Fbx�t�@�C����
//
//	�߂�l bool
//         true:����   false:�ُ�
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

	StaticVertex*     staticvertices = nullptr;  // �X�^�e�B�b�N���b�V�����_�f�[�^
	SkinVertex*       skinvertices = nullptr;    // �X�L�����b�V�����_�f�[�^

	// ���b�V���t�@�C��������p�X���𓾂�PathName�Ɋi�[���Ă���
	// �i��ŁA�e�N�X�`���t�@�C�����̃p�X�Ƃ��Ďg�����߁j
	_tsplitpath_s(FName, PathName, sizeof(PathName) / sizeof(TCHAR), PathName2, sizeof(PathName2) / sizeof(TCHAR),
		NULL, NULL, NULL, NULL);
	_tcscat_s(PathName, PathName2);

	// �t�@�C���̃I�[�v������
	hFile = CreateFile(FName, GENERIC_READ,
		FILE_SHARE_READ, nullptr, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, nullptr);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		// �ő�l�A�ŏ��l�̏����ݒ�                      // -- 2021.2.4
		m_vMax = VECTOR3(-999999, -999999, -999999);
		m_vMin = VECTOR3(999999, 999999, 999999);

		dwBufSize = GetFileSize(hFile, nullptr);     // ���̓t�@�C���̃T�C�Y�𓾂�B
		pBuf = new BYTE[dwBufSize];				  // ���̓o�b�t�@���m�ۂ��� 

		ReadFile(hFile, pBuf, dwBufSize, &dwLen, nullptr);		// �t�@�C�����o�b�t�@�ɓǂݍ���

		p = pBuf;	// �o�b�t�@�̐擪�|�C���^���Z�b�g

		// -----------------------------------------------------------------
		// �t�@�C���w�b�_(WCHAR��8����)�̃`�F�b�N
		WCHAR Head[8];
		memcpy_s(Head, sizeof(Head), p, sizeof(Head));
		if (Head[0] == L'M' && Head[1] == L'E' && Head[2] == L'S' && Head[3] == L'H')
		{
			;
		}
		else {
			MessageBox(nullptr, FName, _T("������ ���b�V���t�@�C��('MESH')�ł͂���܂��� ������"), MB_OK);
			return false;
		}
		// �o�[�W�����̃`�F�b�N�͍s��Ȃ�	 		// -- 2024.3.23
		if (Head[4] < L'2')
		{
			;
		}

		// ���b�V���^�C�v�̃`�F�b�N
		if (Head[6] == L'1')
		{
			// �X�^�e�B�b�N���b�V��
			m_nMeshType = 1;
		}
		else if (Head[6] == L'2')
		{
			// �X�L�����b�V��
			m_nMeshType = 2;
		}
		else {
			m_nMeshType = 0;
			MessageBox(nullptr, FName, _T("������ ���b�V���^�C�v���قȂ�܂��i1,2�ȊO�s�j ������"), MB_OK);
			return false;
		}
		p += sizeof(Head);

		// -----------------------------------------------------------------------------------
		// �e�N�X�`���[�Ɋւ��鏈��
		memcpy_s(&m_dwTextureNum, sizeof(DWORD), p, sizeof(int));         // �e�N�X�`���[���𓾂�

		m_pMaterialDiffuseArray = new VECTOR4[m_dwTextureNum];            // �}�e���A���f�B�t���[�Y�J���[�z��     // -- 2020.12.15
		m_pMaterialSpecularArray = new VECTOR4[m_dwTextureNum];           // �}�e���A���X�y�L�����|�J���[�z��     // -- 2020.12.15
		m_pTextureArray = new ID3D11ShaderResourceView*[m_dwTextureNum];  // �e�N�X�`���[�z��𐶐�����
		m_pTextureNormalArray = new ID3D11ShaderResourceView*[m_dwTextureNum];  // �e�N�X�`���[Normal�z��𐶐�����
		m_pTextureHeightArray = new ID3D11ShaderResourceView*[m_dwTextureNum];  // �e�N�X�`���[Height�z��𐶐�����   // -- 2020.1.15
		m_pTextureSpecularArray = new ID3D11ShaderResourceView*[m_dwTextureNum];  // �e�N�X�`���[Specular�z��𐶐�����
		p += sizeof(int);

		for (ti = 0; ti < m_dwTextureNum; ti++)   // �e�N�X�`���[�������J��Ԃ�
		{
			WCHAR WName[128];
			TCHAR TTName[256] = { 0x00 };   // �}���`�o�C�g�̂Ƃ��́A�ϊ���ɕ�����Ō��\0���쐬����Ȃ��̂ł܂���摤���[���N�����[���Ă���;

			memcpy_s(WName, sizeof(WName), p, sizeof(WCHAR) * 128);	// �e�N�X�`���[��(WCHAR�^)�𓾂�

			if (WName[0] == _T('\0'))     // -- 2020.12.15
			{
				// �}�e���A���J���[���g�p����Ƃ�
#if _UNICODE
				SetMaterial(ti, WName);   // -- 2020.12.15
#else
				WideCharToMultiByte(CP_ACP, 0, WName, -1, TTName, sizeof(TTName), nullptr, nullptr);
				SetMaterial(ti, TTName); // �}���`�o�C�g�ɑΉ����܂���
#endif
			}
			else {
				// �e�N�X�`�����g�p����Ƃ�

#if _UNICODE
				// unicode�̏ꍇ�AWName(WCHAR)�����̂܂�TTName�ɃR�s�[
				_tcscpy_s(TTName, WName);
#else
				// �}���`�o�C�g�����w��̏ꍇ�AWName(WCHAR)���}���`�o�C�g�ɕϊ����ăR�s�[
				// Unicode �����R�[�h(WName)��������Ŏw�肵�������R�[�h�ɕϊ�����( CP_ACP �͓��{��Windows�ł̓V�t�gJIS�R�[�h )
				WideCharToMultiByte(CP_ACP, 0, WName, -1, TTName, sizeof(TTName), nullptr, nullptr);
#endif
				_tcscpy_s(FullName, 256, PathName);
				_tcscat_s(FullName, TTName);		// �p�X���̌��Ƀe�N�X�`���[����A������
				SetTexture(ti, FullName);			// �e�N�X�`���[���e�N�X�`���[�z��ɃZ�b�g����
			}

			p += sizeof(WCHAR) * 128;
		}

		// -------------------------------------------------------------------------------------
		// ���b�V���Ɋւ��鏈��
		// ���b�V���J�E���g�f�[�^
		memcpy_s(&m_dwMeshNum, sizeof(DWORD), p, sizeof(int));       // ���b�V�����𓾂�
		m_pMeshArray = new CFbxMeshArray[m_dwMeshNum];               // ���b�V���z��𐶐�����
		p += sizeof(int);
		for (mi = 0; mi < m_dwMeshNum; mi++)   // ���b�V���������J��Ԃ�
		{
			// ���b�V�����𓾂�
			WCHAR TMeshName[128];
			memcpy_s(TMeshName, sizeof(TMeshName), p, sizeof(WCHAR) * 128);	// ���b�V����(WCHAR�^)�𓾂�
																			//  ���݁A���b�V�����͎g�p���Ă��Ȃ�
			p += sizeof(WCHAR) * 128;

			// ���b�V���^�C�v���Ƃ̏���
			if (m_nMeshType == 1)     // �X�^�e�B�b�N���b�V��
			{
				// ���_���f�[�^
				memcpy_s(&m_pMeshArray[mi].m_dwVerticesNum, sizeof(DWORD), p, sizeof(int));       // ���_���𓾂�
				staticvertices = new StaticVertex[m_pMeshArray[mi].m_dwVerticesNum];  // ���_�z��𐶐�����
				m_pMeshArray[mi].m_vStaticVerticesNormal = new StaticVertexNormal[m_pMeshArray[mi].m_dwVerticesNum];  // ���_�z��o���v�}�b�v�p�𐶐�����
				p += sizeof(int);

				// ���_�f�[�^
				memcpy_s(staticvertices, sizeof(StaticVertex)*m_pMeshArray[mi].m_dwVerticesNum, p, sizeof(StaticVertex)*m_pMeshArray[mi].m_dwVerticesNum);       // ���_�z��ɒ��_�𓾂�
				p += sizeof(StaticVertex)*m_pMeshArray[mi].m_dwVerticesNum;

				// ���_�̍ő�l�ƍŏ��l�����߂�                             // -- 2021.2.4
				for (xi = 0; xi < m_pMeshArray[mi].m_dwVerticesNum; xi++)
				{
					if (m_vMax.x < (staticvertices + xi)->Pos.x) m_vMax.x = (staticvertices + xi)->Pos.x;
					if (m_vMax.y < (staticvertices + xi)->Pos.y) m_vMax.y = (staticvertices + xi)->Pos.y;
					if (m_vMax.z < (staticvertices + xi)->Pos.z) m_vMax.z = (staticvertices + xi)->Pos.z;
					if (m_vMin.x > (staticvertices + xi)->Pos.x) m_vMin.x = (staticvertices + xi)->Pos.x;
					if (m_vMin.y > (staticvertices + xi)->Pos.y) m_vMin.y = (staticvertices + xi)->Pos.y;
					if (m_vMin.z > (staticvertices + xi)->Pos.z) m_vMin.z = (staticvertices + xi)->Pos.z;
				}

				// �C���f�b�N�X���f�[�^
				memcpy_s(&m_pMeshArray[mi].m_dwIndicesNum, sizeof(DWORD), p, sizeof(int));       // �C���f�b�N�X���𓾂�
				m_pMeshArray[mi].m_nIndices = new DWORD[m_pMeshArray[mi].m_dwIndicesNum];      // �C���f�b�N�X�z��𐶐�����
				p += sizeof(int);

				memcpy_s(m_pMeshArray[mi].m_nIndices, sizeof(DWORD)*m_pMeshArray[mi].m_dwIndicesNum, p, sizeof(DWORD)*m_pMeshArray[mi].m_dwIndicesNum);  // �C���f�b�N�X�z��ɃC���f�b�N�X�𓾂�
				if (Head[4] < L'2') ConvIndicesData(mi);	// ���o�[�W�����̂Ƃ��͉E���\�ɕϊ�����   // -- 2024.3.23
				p += sizeof(DWORD)*m_pMeshArray[mi].m_dwIndicesNum;

				// ���_�̃��C�A�E�g��ϊ����A�ڐ��Ə]�@�����v�Z
				ChangeStaticVertexLayout(staticvertices, m_pMeshArray[mi].m_nIndices, m_pMeshArray[mi].m_dwIndicesNum, m_pMeshArray[mi].m_vStaticVerticesNormal);

				// �o�[�e�b�N�X�o�b�t�@�ƃC���f�b�N�X�o�b�t�@���Z�b�g
				SetStaticVIBuffer(mi, m_pMeshArray[mi].m_vStaticVerticesNormal, m_pMeshArray[mi].m_nIndices);

				// ��̃��b�V���̃e�N�X�`���[���i���o�[�W�����ł͕K���P�̂͂��j
				memcpy_s(&tnum, sizeof(DWORD), p, sizeof(int));       // �e�N�X�`���[��
				p += sizeof(int);
				// �e�N�X�`���[�ԍ��̃e�N�X�`����ݒ肷��
				for (int i = 0; i < tnum; i++)
				{
					memcpy_s(&n, sizeof(DWORD), p, sizeof(int));       // �e�N�X�`���[�ԍ��𓾂�
					m_pMeshArray[mi].m_pTexture = m_pTextureArray[n];   // �e�N�X�`���[�z��̃A�h���X���Z�b�g
					m_pMeshArray[mi].m_pTextureNormal = m_pTextureNormalArray[n];   // �e�N�X�`���[Normal�z��̃A�h���X���Z�b�g
					m_pMeshArray[mi].m_pTextureHeight = m_pTextureHeightArray[n];   // �e�N�X�`���[Height�z��̃A�h���X���Z�b�g
					m_pMeshArray[mi].m_pTextureSpecular = m_pTextureSpecularArray[n];   // �e�N�X�`���[Specular�z��̃A�h���X���Z�b�g
					m_pMeshArray[mi].m_pMaterialDiffuse = m_pMaterialDiffuseArray[n];   // �}�e���A���E�f�B�t���[�Y�F   // -- 2020.12.15
					m_pMeshArray[mi].m_pMaterialSpecular = m_pMaterialSpecularArray[n]; // �}�e���A���E�X�y�L�����[�F   // -- 2020.12.15
					p += sizeof(int);
				}

				// ���b�V���̒��S�_�̍��W�����߂�                                      // -- 2018.8.1
				m_pMeshArray[mi].m_vCenterPos = GetStaticCenterPos(staticvertices, m_pMeshArray[mi].m_dwVerticesNum);

				// �s�v�ɂȂ����ꎞ�@���_�z����폜����
				SAFE_DELETE_ARRAY(staticvertices);

			}
			else if (m_nMeshType == 2)    // �X�L�����b�V��
			{

				// ���_���f�[�^
				memcpy_s(&m_pMeshArray[mi].m_dwVerticesNum, sizeof(DWORD), p, sizeof(int));       // ���_���𓾂�
				skinvertices = new SkinVertex[m_pMeshArray[mi].m_dwVerticesNum];  // ���_�z��𐶐�����
				m_pMeshArray[mi].m_vSkinVerticesNormal = new SkinVertexNormal[m_pMeshArray[mi].m_dwVerticesNum];  // ���_�z��o���v�}�b�v�p�𐶐�����
				p += sizeof(int);

				// ���_�f�[�^
				memcpy_s(skinvertices, sizeof(SkinVertex)*m_pMeshArray[mi].m_dwVerticesNum, p, sizeof(SkinVertex)*m_pMeshArray[mi].m_dwVerticesNum);       // ���_�z��ɒ��_�𓾂�
				p += sizeof(SkinVertex)*m_pMeshArray[mi].m_dwVerticesNum;

				// ���_�̍ő�l�ƍŏ��l�����߂�                             // -- 2021.2.4
				for (xi = 0; xi < m_pMeshArray[mi].m_dwVerticesNum; xi++)
				{
					if (m_vMax.x < (skinvertices + xi)->Pos.x) m_vMax.x = (skinvertices + xi)->Pos.x;
					if (m_vMax.y < (skinvertices + xi)->Pos.y) m_vMax.y = (skinvertices + xi)->Pos.y;
					if (m_vMax.z < (skinvertices + xi)->Pos.z) m_vMax.z = (skinvertices + xi)->Pos.z;
					if (m_vMin.x > (skinvertices + xi)->Pos.x) m_vMin.x = (skinvertices + xi)->Pos.x;
					if (m_vMin.y > (skinvertices + xi)->Pos.y) m_vMin.y = (skinvertices + xi)->Pos.y;
					if (m_vMin.z > (skinvertices + xi)->Pos.z) m_vMin.z = (skinvertices + xi)->Pos.z;
				}

				// �C���f�b�N�X���f�[�^
				memcpy_s(&m_pMeshArray[mi].m_dwIndicesNum, sizeof(DWORD), p, sizeof(int));       // �C���f�b�N�X���𓾂�
				m_pMeshArray[mi].m_nIndices = new DWORD[m_pMeshArray[mi].m_dwIndicesNum];      // �C���f�b�N�X�z��𐶐�����
				p += sizeof(int);

				memcpy_s(m_pMeshArray[mi].m_nIndices, sizeof(DWORD)*m_pMeshArray[mi].m_dwIndicesNum, p, sizeof(DWORD)*m_pMeshArray[mi].m_dwIndicesNum);  // �C���f�b�N�X�z��ɃC���f�b�N�X�𓾂�
				if (Head[4] < L'2') ConvIndicesData(mi);	// ���o�[�W�����̂Ƃ��͉E���\�ɕϊ�����   // -- 2024.3.23
				p += sizeof(DWORD)*m_pMeshArray[mi].m_dwIndicesNum;

				// ���_�̃��C�A�E�g��ϊ����A�ڐ��Ə]�@�����v�Z
				ChangeSkinVertexLayout(skinvertices, m_pMeshArray[mi].m_nIndices, m_pMeshArray[mi].m_dwIndicesNum, m_pMeshArray[mi].m_vSkinVerticesNormal);

				// �o�[�e�b�N�X�o�b�t�@�ƃC���f�b�N�X�o�b�t�@���Z�b�g
				SetSkinVIBuffer(mi, m_pMeshArray[mi].m_vSkinVerticesNormal, m_pMeshArray[mi].m_nIndices);

				// ��̃��b�V���̃e�N�X�`���[���i���o�[�W�����ł͕K���P�̂͂��j
				memcpy_s(&tnum, sizeof(DWORD), p, sizeof(int));       // �e�N�X�`���[��
				p += sizeof(int);
				// �e�N�X�`���[�ԍ��̃e�N�X�`����ݒ肷��
				for (int i = 0; i < tnum; i++)
				{
					memcpy_s(&n, sizeof(DWORD), p, sizeof(int));       // �e�N�X�`���[�ԍ��𓾂�
					m_pMeshArray[mi].m_pTexture = m_pTextureArray[n];   // �e�N�X�`���[�z��̃A�h���X���Z�b�g
					m_pMeshArray[mi].m_pTextureNormal = m_pTextureNormalArray[n];   // �m�[�}���e�N�X�`���[�z��̃A�h���X���Z�b�g
					m_pMeshArray[mi].m_pTextureHeight = m_pTextureHeightArray[n];   // �n�C�g�e�N�X�`���[�z��̃A�h���X���Z�b�g
					m_pMeshArray[mi].m_pTextureSpecular = m_pTextureSpecularArray[n];   // Specular�e�N�X�`���[�z��̃A�h���X���Z�b�g
					m_pMeshArray[mi].m_pMaterialDiffuse = m_pMaterialDiffuseArray[n];    // �}�e���A���E�f�B�t���[�Y�F  // -- 2020.12.15
					m_pMeshArray[mi].m_pMaterialSpecular = m_pMaterialSpecularArray[n];  // �}�e���A���E�X�y�L�����[�F  // -- 2020.12.15
					p += sizeof(int);
				}

				// ���b�V���̒��S�_�̍��W�����߂�                                      // -- 2018.8.1
				m_pMeshArray[mi].m_vCenterPos = GetSkinCenterPos(skinvertices, m_pMeshArray[mi].m_dwVerticesNum);

				// �s�v�ɂȂ����ꎞ�@���_�z��A�C���f�b�N�X�z����폜����
				SAFE_DELETE_ARRAY(skinvertices);

			}
		}

		// �����_�����O���̓Y���z��̗̈���m��          // -- 2018.8.1
		m_dwRenderIdxArray = new DWORD[m_dwMeshNum];
		// �����_�����O�������߂邽�߂̋����z��̗̈���m��  // -- 2018.8.1
		m_fRenderDistanceArray = new float[m_dwMeshNum];

		// ----------------------------------------------------------------------------------------------
		CloseHandle(hFile);
		SAFE_DELETE_ARRAY(pBuf);  // ���̓o�b�t�@���폜����

		m_dwLoadTime = timeGetTime() - startTime;
		timeEndPeriod(1);

		return true;

	}
	else {
		MessageBox(nullptr, FName, _T("������ ���b�V���t�@�C��(.mesh)������܂��� ������"), MB_OK);
	}

	return false;
}

//------------------------------------------------------------------------  // -- 2024.3.23
//	 �o�[�W������2.0.0�ȑO�̂Ƃ��̓|���S���̍���肪�\�ɂȂ��Ă���̂�
//   �E���\�ɕύX����
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
// �X�^�e�B�b�N���b�V���̒��S�_�̍��W�����߂�֐�
// 
// ����
//   const StaticVertex*  vertex   ���_�z��;
//   const DWORD&         Num      ���_��;
// 
// �߂�l
//   VECTOR3  ���b�V���̒��S�_�̍��W
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
// �X�^�e�B�b�N���b�V���@���_�̃��C�A�E�g���m�[�}���}�b�v�p�ɕϊ����A�ڐ��Ə]�@�����v�Z���ǉ�����
// 
// ����
//   const StaticVertex*       vertices             ���_�z��
//   const DWORD*              indices              �C���f�b�N�X�z��
//   const DWORD&              IndicesNum           �C���f�b�N�X��
//   StaticVertexNormal*       verticesNormal(OUT)  �m�[�}���}�b�v�p���_�z��i�o�́j
// 
// �߂�l
//   bool  true
// 
//------------------------------------------------------------------------
bool CFbxMesh::ChangeStaticVertexLayout(const StaticVertex* vertices, const DWORD* indices, const DWORD& IndicesNum, StaticVertexNormal* verticesNormal)
{

	// �R�p�`�|���S�����ɁA�S�Ẵ|���S���̏������s��
	for (int i = 0; i < IndicesNum / 3; i++) {
		// ���_�A�@���A�e�N�X�`�����W���m�[�}���}�b�v�p�̒��_���C�A�E�g�ɃR�s�[����
		verticesNormal[indices[i * 3]].Pos = vertices[indices[i * 3]].Pos;
		verticesNormal[indices[i * 3 + 1]].Pos = vertices[indices[i * 3 + 1]].Pos;
		verticesNormal[indices[i * 3 + 2]].Pos = vertices[indices[i * 3 + 2]].Pos;
		verticesNormal[indices[i * 3]].Normal = vertices[indices[i * 3]].Normal;
		verticesNormal[indices[i * 3 + 1]].Normal = vertices[indices[i * 3 + 1]].Normal;
		verticesNormal[indices[i * 3 + 2]].Normal = vertices[indices[i * 3 + 2]].Normal;
		verticesNormal[indices[i * 3]].vTex = vertices[indices[i * 3]].vTex;
		verticesNormal[indices[i * 3 + 1]].vTex = vertices[indices[i * 3 + 1]].vTex;
		verticesNormal[indices[i * 3 + 2]].vTex = vertices[indices[i * 3 + 2]].vTex;

		//	���_3��UV���W�R�����Ƃɐڐ��Ə]�@�����v�Z����
		CalcTangentSub(verticesNormal[indices[i * 3]].Pos, verticesNormal[indices[i * 3 + 1]].Pos, verticesNormal[indices[i * 3 + 2]].Pos,
			verticesNormal[indices[i * 3]].vTex, verticesNormal[indices[i * 3 + 1]].vTex, verticesNormal[indices[i * 3 + 2]].vTex,
			verticesNormal[indices[i * 3]].Tangent, verticesNormal[indices[i * 3]].Binormal);

		//	����ꂽ�ڐ��Ə]�@�����A�c��̂Q�̒��_�ɂ��R�s�[����
		verticesNormal[indices[i * 3 + 1]].Tangent = verticesNormal[indices[i * 3]].Tangent;
		verticesNormal[indices[i * 3 + 2]].Tangent = verticesNormal[indices[i * 3]].Tangent;
		verticesNormal[indices[i * 3 + 1]].Binormal = verticesNormal[indices[i * 3]].Binormal;
		verticesNormal[indices[i * 3 + 2]].Binormal = verticesNormal[indices[i * 3]].Binormal;
	}

	return true;
}

//------------------------------------------------------------------------
//
//	�X�^�e�B�b�N���b�V���@�o�[�e�b�N�X�o�b�t�@�ƃC���f�b�N�X�o�b�t�@���Z�b�g���鏈��
//
// ����
//   const DWORD&        mi        �Z�b�g���郁�b�V���z��̓Y��
//   const SimpleVertex* vertices  ���_�z��
//   const DWORD*        indices   �C���f�b�N�X�z��
//
//	�߂�l 
//
//------------------------------------------------------------------------
HRESULT  CFbxMesh::SetStaticVIBuffer(const DWORD& mi, const StaticVertexNormal* vertices, const DWORD* indices)
{
	D3D11_BUFFER_DESC bd;
	D3D11_SUBRESOURCE_DATA InitData;

	//�o�[�e�b�N�X�o�b�t�@�[�쐬
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

	//�C���f�b�N�X�o�b�t�@�[�쐬
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
// �X�L�����b�V���̒��S�_�̍��W�����߂�֐�
// 
// ����
//   const SkinVertex*    vertex   ���_�z��;
//   const DWORD&          Num      ���_��;
// 
// �߂�l
//   VECTOR3  ���b�V���̒��S�_�̍��W
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
// �X�L�����b�V���@���_�̃��C�A�E�g���m�[�}���}�b�v�p�ɕϊ����A�ڐ��Ə]�@�����v�Z���ǉ�����
// 
// ����
//   const SkinVertex*    vertices             ���_�z��
//   const DWORD*         indices              �C���f�b�N�X�z��
//   const DWORD&         IndicesNum           �C���f�b�N�X��
//   SkinVertexNormal*    verticesNormal(OUT)  �m�[�}���}�b�v�p���_�z��i�o�́j
// 
// �߂�l
//   bool  true
// 
//------------------------------------------------------------------------
bool CFbxMesh::ChangeSkinVertexLayout(const SkinVertex* vertices, const DWORD* indices, const DWORD& IndicesNum, SkinVertexNormal* verticesNormal)
{

	// �R�p�`�|���S�����ɁA�S�Ẵ|���S���̏������s��
	for (int i = 0; i < IndicesNum / 3; i++) {
		// ���_�A�@���A�e�N�X�`�����W�A�N���X�^�[(�{�[��)�C���f�b�N�X�A�E�F�C�g���m�[�}���}�b�v�p�̒��_���C�A�E�g�ɃR�s�[����
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

		//	���_3��UV���W�R�����Ƃɐڐ��Ə]�@�����v�Z����
		CalcTangentSub(verticesNormal[indices[i * 3]].Pos, verticesNormal[indices[i * 3 + 1]].Pos, verticesNormal[indices[i * 3 + 2]].Pos,
			verticesNormal[indices[i * 3]].vTex, verticesNormal[indices[i * 3 + 1]].vTex, verticesNormal[indices[i * 3 + 2]].vTex,
			verticesNormal[indices[i * 3]].Tangent, verticesNormal[indices[i * 3]].Binormal);

		//	����ꂽ�ڐ��Ə]�@�����A�c��̂Q�̒��_�ɂ��R�s�[����
		verticesNormal[indices[i * 3 + 1]].Tangent = verticesNormal[indices[i * 3]].Tangent;
		verticesNormal[indices[i * 3 + 2]].Tangent = verticesNormal[indices[i * 3]].Tangent;
		verticesNormal[indices[i * 3 + 1]].Binormal = verticesNormal[indices[i * 3]].Binormal;
		verticesNormal[indices[i * 3 + 2]].Binormal = verticesNormal[indices[i * 3]].Binormal;
	}

	return true;
}

//------------------------------------------------------------------------
//
//	�X�L�����b�V���@�o�[�e�b�N�X�o�b�t�@�ƃC���f�b�N�X�o�b�t�@���Z�b�g���鏈��
//
// ����
//   const DWORD&            mi        �Z�b�g���郁�b�V���z��̓Y��
//   const SkinVertexNormal* vertices  ���_�z��
//   const DWORD*            indices   �C���f�b�N�X�z��
//
//	�߂�l 
//
//------------------------------------------------------------------------
HRESULT CFbxMesh::SetSkinVIBuffer(const DWORD& mi, const SkinVertexNormal* vertices, const DWORD* indices)
{
	D3D11_BUFFER_DESC bd;
	D3D11_SUBRESOURCE_DATA InitData;

	//�o�[�e�b�N�X�o�b�t�@�[�쐬
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

	//�C���f�b�N�X�o�b�t�@�[�쐬
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
// 	���b�V���̕`�揇�����肵�Am_dwRenderIdxArray�ɐݒ肷��֐�
// 
//	�E���_���烁�b�V���̒��S�_�܂ł̋��������ɕ��ёւ��A
//    �`�揇��m_dwRenderIdxArray�ɐݒ肷��
// 
// ����
//   const MATRIX4X4& mWorld   ���b�V���̃��[���h�}�g���b�N�X
//   const VECTOR3& vEye    ���_���W
// 
// �߂�l
//   �Ȃ�
// 
//------------------------------------------------------------------------
void CFbxMesh::SetRenderIdxArray(const MATRIX4X4& mWorld, const VECTOR3& vEye)
{
	DWORD widx;
	float wdis;
	MATRIX4X4 mPos;

	// ���_���烁�b�V���̒��S�_�܂ł̋�����ݒ肷��
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

	// �����̍~���ɕ��בւ���i�����@�j
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
//	���_3��UV���W�R�����Ƃɐڐ��Ə]�@�����v�Z����B
//
//  const VECTOR3& v1          ���_�P
//  const VECTOR3& v2          ���_�Q
//  const VECTOR3& v3          ���_�R
//  const VECTOR2& uv1         UV���W�P
//  const VECTOR2& uv2         UV���W�Q
//  const VECTOR2& uv3         UV���W�R
//	VECTOR3 &Tangent           �ڐ�(OUT)
//	VECTOR3 &Binormal          �]�@��(OUT)
//
//	�߂�l bool
//
//------------------------------------------------------------------------
bool CFbxMesh::CalcTangentSub(const VECTOR3& v1, const VECTOR3& v2, const VECTOR3& v3,
	const VECTOR2& uv1, const VECTOR2& uv2, const VECTOR2& uv3, VECTOR3 &Tangent, VECTOR3 &Binormal)
{
	//
	// 1���_5����(x,y,z,u,v)��1���_3����(x,u,v)�ɕϊ�
	// 3���_���畽�ʂ����
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

	// ���ʃp�����[�^����UV�����W�Z�o
	float U[3], V[3];
	for (int i = 0; i < 3; ++i) {
		VECTOR3 V1 = CP1[i] - CP0[i];
		VECTOR3 V2 = CP2[i] - CP1[i];
		VECTOR3 abc;
		abc = cross(V1, V2);

		if (abc.x == 0.0f) {
			// �|���S����UV��̃|���S�����k�ނ��Ă��āA�Z�o�s��
			//MessageBox(0, _T("CalcTangentSub() : ���W�AUV������̏d�����_�����o���܂���"), _T("�G���["), MB_OK);
			Tangent = VECTOR3(1, 0, 0);   // �G���[�̂��߂O�ɂ���
			Binormal = VECTOR3(0, 1, 0);
			return false;
		}
		U[i] = -abc.y / abc.x;
		V[i] = -abc.z / abc.x;
	}

	// �ڃx�N�g�����R�s�[�A���K��
	Tangent = VECTOR3(U[0], U[1], U[2]);
	Tangent = normalize(Tangent);

	// �]�@���x�N�g�����R�s�[�A���K��
	Binormal = VECTOR3(V[0], V[1], V[2]);
	Binormal = normalize(Binormal);

	return true;
}


//------------------------------------------------------------------------
//
//	�e�N�X�`�����Z�b�g���鏈��
//
// ����
//  const DWORD& ti        �Z�b�g����e�N�X�`���[�z��̓Y��
//  const TCHAR* TexName   �e�N�X�`���[��
//  
//	�߂�l 
//
//------------------------------------------------------------------------
HRESULT CFbxMesh::SetTexture(const DWORD& ti, const TCHAR* TexName)
{

	// �}�e���A���J���[�̃N�����[
	m_pMaterialDiffuseArray[ti] = VECTOR4(0, 0, 0, 0);     // -- 2020.12.15
	m_pMaterialSpecularArray[ti] = VECTOR4(0, 0, 0, 0);     // -- 2020.12.15

	//�e�N�X�`���[�쐬
	m_pTextureArray[ti] = nullptr;
	m_pTextureArray[ti] = m_pFbxMeshCtrl->SetTextureList(TexName);                     // -- 2021.2.4
	if (m_pTextureArray[ti] == nullptr)
	{
		MessageBox(nullptr, TexName, _T("������ �e�N�X�`���t�@�C��������܂��� ������"), MB_OK);
		return E_FAIL;
	}

	// �m�[�}���e�N�X�`���쐬
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

	// �n�C�g�e�N�X�`���쐬                                                       // -- 2020.1.15
	_tcscpy_s(fullname, drv);
	_tcscat_s(fullname, path);
	_tcscat_s(fullname, fname);
	_tcscat_s(fullname, _T("_HM"));
	_tcscat_s(fullname, ext);

	m_pTextureHeightArray[ti] = nullptr;
	m_pTextureHeightArray[ti] = m_pFbxMeshCtrl->SetTextureList(fullname);                     // -- 2021.2.4

	// �X�y�L�����[�e�N�X�`���쐬
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
//	�}�e���A���J���[���Z�b�g���鏈��
//
// ����
//  const DWORD& ti        �Z�b�g����e�N�X�`���[�z��̓Y��
//  const TCHAR* Color     �J���[
//  
//	�߂�l 
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
//	�A�j���[�V�����t�@�C���̓ǂݍ��ݏ���
//
// ����
//  const TCHAR*         FName          �A�j���t�@�C����
//  const ROOTANIMTYPE&  RAType         ���[�g�{�[���A�j���^�C�v�i�ȗ��l:eRootAnimNone�j
//
//	�߂�l bool
//         true:����   false:�ُ�
//
//------------------------------------------------------------------------
bool CFbxMesh::LoadAnimation(int id, const TCHAR* FName, bool loopFlag, const ROOTANIMTYPE& RAType)
{
	// ���b�V���t�@�C�����ǂݍ��܂�Ă��Ȃ��ƃG���[
	if (m_pMeshArray == nullptr)
	{
		MessageBox(nullptr, FName, _T("������ �Ή�����X�L�����b�V���t�@�C��(.mesh)���ǂݍ��܂�Ă��܂��� ������"), MB_OK);
		return false;
	}
	// �X�^�e�B�b�N���b�V���t�@�C���̂Ƃ��̓G���[
	if (m_nMeshType != 2 )
	{
		MessageBox(nullptr, FName, _T("������ �X�^�e�B�b�N���b�V���ɂ̓A�j���[�V�����͐ݒ�ł��܂��� ������"), MB_OK);
		return false;
	}

	timeBeginPeriod(1);
	DWORD startTime = timeGetTime();

	HANDLE	hFile;
	DWORD	dwLen, dwBufSize;
	BYTE  *pBuf = nullptr;
	BYTE  *p;
	int   mi, bi, n;

	// �t�@�C���̃I�[�v������
	hFile = CreateFile(FName, GENERIC_READ,
		FILE_SHARE_READ, nullptr, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, nullptr);
	if (hFile != INVALID_HANDLE_VALUE)
	{

		dwBufSize = GetFileSize(hFile, nullptr);     // ���̓t�@�C���̃T�C�Y�𓾂�B
		pBuf = new BYTE[dwBufSize];				  // ���̓o�b�t�@���m�ۂ��� 

		ReadFile(hFile, pBuf, dwBufSize, &dwLen, nullptr);		// �t�@�C�����o�b�t�@�ɓǂݍ���

		p = pBuf;	// �o�b�t�@�̐擪�|�C���^���Z�b�g

		// -----------------------------------------------------------------    // -- 2020.12.15 -- 3
		// �t�@�C���w�b�_(WCHAR��8����)�̃`�F�b�N
		WCHAR Head[8];
		memcpy_s(Head, sizeof(Head), p, sizeof(Head));
		if (Head[0] == L'A' && Head[1] == L'N' && Head[2] == L'M' && Head[3] == L'X')
		{
			;
		}
		else {
			MessageBox(nullptr, FName, _T("������ �A�j���[�V�����t�@�C��('ANMX')�ł͂���܂��� ������"), MB_OK);
			return false;
		}
		// �o�[�W�����̃`�F�b�N�͍s��Ȃ�
		if (Head[4] == L'1')
		{
			;
		}
		p += sizeof(Head);

		// ------------------------------------------------------------------------------------------------
		// �A�j���[�V�����Ɋւ���f�[�^
		ANIMATION* pAnim = &m_Animation[id];
		pAnim->used = true;
		pAnim->loop = loopFlag;

		memcpy_s(&pAnim->startFrame, sizeof(int), p, sizeof(int));       // �J�n�t���[���𓾂�
		p += sizeof(int);
		memcpy_s(&pAnim->endFrame, sizeof(int), p, sizeof(int));         // �I���t���[���𓾂�
		p += sizeof(int);

		// ���[�g�{�[���Ɋւ���f�[�^
		memcpy_s(&pAnim->RootBoneMesh, sizeof(int), p, sizeof(int));       // ���[�g�{�[�����b�V���ԍ��𓾂�    // -- 2020.12.15 -- 3
		p += sizeof(int);
		memcpy_s(&pAnim->RootBone, sizeof(int), p, sizeof(int));           // ���[�g�{�[���ԍ��𓾂�            // -- 2020.12.15 -- 3
		p += sizeof(int);

		// ------------------------------------------------------------------------------------------------
		// ���b�V�����̏���
		for (mi = 0; mi < m_dwMeshNum; mi++)   // ���b�V���������J��Ԃ�
		{
			// �{�[�����f�[�^
			memcpy_s(&m_pMeshArray[mi].m_NumBones, sizeof(DWORD), p, sizeof(int));       // �{�[�����𓾂�
			p += sizeof(int);

			// �e�{�[�����̏���
			for (bi = 0; bi < m_pMeshArray[mi].m_NumBones; bi++)
			{
				BONE* pBone = &m_pMeshArray[mi].m_BoneArray[id][bi];
				// �o�C���h�|�[�Y�̎��o��
				// �i�o�C���h�|�[�Y�́A�t�s��ƂȂ��Ă���j
				memcpy_s(&pBone->bindPose, sizeof(MATRIX4X4), p, sizeof(MATRIX4X4));  // �o�C���h�|�[�Y�s��̓]��
				p += sizeof(MATRIX4X4);

				// �t���[���|�[�Y���̎��o��
				memcpy_s(&pBone->framePoseNum, sizeof(int), p, sizeof(int));
				p += sizeof(int);
				SAFE_DELETE_ARRAY(pBone->framePose);
				pBone->framePose = new MATRIX4X4[pBone->framePoseNum];  // �t���[���|�[�Y���̃{�[���z��𐶐�����

				// �t���[���|�[�Y�̎��o��
				for (n = 0; n < pBone->framePoseNum; n++)
				{
					memcpy_s(&pBone->framePose[n], sizeof(MATRIX4X4), p, sizeof(MATRIX4X4));  // �t���[���|�[�Y�s��̓]��
					p += sizeof(MATRIX4X4);
				}
			}
		}

		// -------------------------------------------------------------------------------
		CloseHandle(hFile);
		SAFE_DELETE_ARRAY(pBuf);  // ���̓o�b�t�@���폜����

		// -------------------------------------------------------------------------------
		// ���[�g�{�[���A�j���[�V�����ƃV�F�[�_�[�}�g���b�N�X�̍쐬                // -- 2020.12.15 -- 3
		MakeRootAnimAndShaderMatrix(id, RAType);

		// -------------------------------------------------------------------------------
		m_dwLoadTime = timeGetTime() - startTime;
		timeEndPeriod(1);

		return true;

	}
	else {
		MessageBox(nullptr, FName, _T("������ �A�j���[�V�����t�@�C��(.anmx)������܂��� ������"), MB_OK);
	}

	return false;
}

//==========================================================================================================================================================
//
// ���[�g�{�[���A�j���[�V�����ƃV�F�[�_�[�}�g���b�N�X�̍쐬                                                                           // -- 2020.12.15 -- 3
// 
// const int& animNum  : �Z�b�g����A�j���[�V�����ԍ�
// ROOTANIMTYPE RAType : ���[�g�{�[���A�j���^�C�v ( eRootAnimNone:���[�g�A�j���Ȃ�  eRootAnimXZ:���[�g�A�j��XZ  eRootAnim:���[�g�A�j��)
//
//==========================================================================================================================================================
void CFbxMesh::MakeRootAnimAndShaderMatrix(const int& animNum, ROOTANIMTYPE RAType)
{
	int RootMeshNo;
	int RootBoneNo;

	if (m_nMeshType != 2)
	{
		MessageBox(nullptr, _T("MakeRootAnimAndShaderMatrix()"), _T("������ �X�^�e�B�b�N���b�V���Ƀ��[�g�A�j���[�V�����̐ݒ�͂ł��܂��� ������"), MB_OK);
		return;
	}
	if (!m_Animation[animNum].used)
	{
		MessageBox(nullptr, _T("������ MakeRootAnimAndShaderMatrix() ������"), _T("������ �X�L�����b�V���ɁA�w�肳�ꂽ�A�j���[�V�������ǂݍ��܂�Ă��܂��� ������"), MB_OK);
		return;
	}

	if (m_Animation[animNum].RootBoneMesh == -1)  // ���[�g�{�[�����Ȃ��Ƃ�(�ʏ�͂��蓾�Ȃ�)
	{
		if (RAType == eRootAnimNone)  // ���[�g�A�j���Ȃ��̂Ƃ�
		{
			RootMeshNo = 0;
			RootBoneNo = 0;
		}
		else {
			// ���[�g�A�j���w�肪�ł��Ȃ��̂ŃG���[
			MessageBox(nullptr, _T("MakeRootAnimAndShaderMatrix()"), _T("������ ���[�g�{�[�����������߁A���[�g�A�j���[�V�����͂ł��܂��� ������"), MB_OK);
			return;
		}
	}
	else {
		// ���[�g�{�[���̃��b�V���ԍ��ƃ{�[���ԍ���ݒ�
		RootMeshNo = m_Animation[animNum].RootBoneMesh;
		RootBoneNo = m_Animation[animNum].RootBone;
	}

	// ���[�g�{�[���֌W�̕ϐ���ݒ�
	m_RootBoneArray[animNum].bindPose = m_pMeshArray[RootMeshNo].m_BoneArray[animNum][RootBoneNo].bindPose;            // ���[�g�{�[���̃o�C���h�|�[�Y
	m_RootBoneArray[animNum].framePoseNum = m_pMeshArray[RootMeshNo].m_BoneArray[animNum][RootBoneNo].framePoseNum;    // ���[�g�{�[���̃t���[����
	SAFE_DELETE_ARRAY(m_RootBoneArray[animNum].framePose);                                              // �t���[���s��������N�����[����
	m_RootBoneArray[animNum].framePose = new MATRIX4X4[m_RootBoneArray[animNum].framePoseNum];          // �t���[���s��𐶐�����
	m_RootAnimType[animNum] = RAType;																	// ���[�g�A�j���^�C�v��ۑ�����

	// ���[�g�{�[���z��̍쐬
	for (int n = 0; n < m_RootBoneArray[animNum].framePoseNum; n++)
	{
		// ���[�g�{�[���A�j���^�C�v���Ƃ̏���
		switch (RAType)
		{
		case eRootAnimNone:
			// ���[�g�{�[���A�j�����s��Ȃ��̂ŏ����l��ݒ肷��
			m_RootBoneArray[animNum].framePose[n] = XMMatrixIdentity();
			break;

		case eRootAnimXZ:
			// ���[�g�{�[���̃o�C���h�|�[�Y����̍����i�w�y�ړ��̂݁j�̍s����쐬����
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
			// ���[�g�{�[���̃o�C���h�|�[�Y����̍����̍s����쐬����
			m_RootBoneArray[animNum].framePose[n] = m_RootBoneArray[animNum].bindPose *
				m_pMeshArray[RootMeshNo].m_BoneArray[animNum][RootBoneNo].framePose[n];
		}
		break;
		}
	}

	// �V�F�[�_�[�֓n���}�g���b�N�X�̍쐬
	for (int mi = 0; mi < m_dwMeshNum; mi++)   // ���b�V���������J��Ԃ�
	{
		// �V�F�[�_�[�֓n���`���̍s����쐬
		// �i�V�F�[�_�[�֓n���Ƃ��́A�@�e�t���[�����̇A�{�[�����ɕ��ׂĂ����K�v������j
		// �i�t���[���|�[�Y���́A��̃��b�V���̃{�[���ł͑S�ē����Ȃ̂ŁAm_BoneArray[animNum][0].framePoseNum���g�p����j
		m_pMeshArray[mi].m_pBoneShader[animNum] = new BONESHADER[m_pMeshArray[mi].m_BoneArray[animNum][0].framePoseNum];  // �V�F�[�_�[�{�[���z��𐶐�����

		for (int i = 0; i < m_pMeshArray[mi].m_BoneArray[animNum][0].framePoseNum; i++)  // �s��̏�����
		{
			for (int j = 0; j < MAX_BONES; j++)
			{
				m_pMeshArray[mi].m_pBoneShader[animNum][i].shaderFramePose[j] = XMMatrixIdentity();
				m_pMeshArray[mi].m_pBoneShader[animNum][i].shaderFramePose[j] =
					XMMatrixTranspose(m_pMeshArray[mi].m_pBoneShader[animNum][i].shaderFramePose[j]);  // �]�u�s�񏉊��l
			}
		}

		for (int n = 0; n < m_pMeshArray[mi].m_BoneArray[animNum][0].framePoseNum; n++)  // �e�t���[���|�[�Y���̏���
		{
			for (int bi = 0; bi < m_pMeshArray[mi].m_NumBones; bi++)  // �e�{�[�����̏���
			{
				MATRIX4X4 mFramePose;

				if (RAType == eRootAnimNone)
				{
					// ���[�g�{�[���A�j�������Ȃ��Ƃ��́A�o�C���h�{�[�Y�̋t�s��ƃt���[���|�[�Y���|�����킹�A�o�C���h�{�[�Y����̍����̍s����쐬����
					mFramePose = m_pMeshArray[mi].m_BoneArray[animNum][bi].bindPose * m_pMeshArray[mi].m_BoneArray[animNum][bi].framePose[n];
				}
				else {
					// ���[�g�{�[���A�j��������Ƃ�
					mFramePose = m_pMeshArray[mi].m_BoneArray[animNum][bi].framePose[n] *
									XMMatrixInverse(nullptr, m_RootBoneArray[animNum].framePose[n]);      // �e�t���[���|�[�Y�s������[�g�{�[���s��Ƃ̍����s��ɂ���            // -- 2020.12.15 -- 3
					mFramePose = m_pMeshArray[mi].m_BoneArray[animNum][bi].bindPose * mFramePose;      // �o�C���h�{�[�Y�̋t�s��ƃt���[���|�[�Y���|�����킹�A�o�C���h�{�[�Y����̍����̍s����쐬����            // -- 2020.12.15 -- 3
				}
				// �t���[���|�[�Y��]�u�s��ɂ��Ă��̂܂܃V�F�[�_�[�ɓn����悤�ɂ���
				m_pMeshArray[mi].m_pBoneShader[animNum][n].shaderFramePose[bi] = XMMatrixTranspose(mFramePose);  // �]�u�s��
			}
		}
	}
}

//==========================================================================================================================================================
//
// �A�j���[�V�����̍ŏI�t���[���ԍ����擾 
// (endFrame-startFrame��Ԃ�) 
//
//==========================================================================================================================================================
int CFbxMesh::GetEndFrame(const int& animID)
{
	if (m_nMeshType != 2)
	{
		MessageBox(nullptr, _T("������ GetEndFrame() ������"), _T("������ �X�L�����b�V���ł͂���܂��� ������"), MB_OK);
		return 0;
	}
	if (!m_Animation[animID].used)				  // -- 2024.9.5
	{
		MessageBox(nullptr, _T("������ GetEndFrame() ������"), _T("������ �X�L�����b�V���ɁA�w�肳�ꂽ�A�j���[�V�������ǂݍ��܂�Ă��܂��� ������"), MB_OK);
		return 0;
	}
	return m_Animation[animID].endFrame - m_Animation[animID].startFrame;			 // -- 2024.9.5
}

//==========================================================================================================================================================
// �w��{�[���̃}�g���b�N�X���擾																						    // -- 2024.9.5
// �i���̂܂܂̑��΃}�g���b�N�X���擾�j
// 
// �����F
// 	  Animator* animStatus          �A�j���[�^�[
//    const DWORD& nBone            �{�[���ԍ�
//    const DWORD& nMesh            ���b�V���ԍ�(�ȗ��l�O)
// �߂�l�F
//    MATRIX4X4                     �w��{�[���̃}�g���b�N�X�i���̂܂܂̑��΃}�g���b�N�X�j
//==========================================================================================================================================================
MATRIX4X4 CFbxMesh::GetFrameMatrices(Animator* animStatus, const DWORD& nBone, const DWORD& nMesh)
{
	MATRIX4X4 mBoneWorld;

	if (m_nMeshType == 1)
	{
		MessageBox(nullptr, _T("������ GetFrameMatrices() ������"), _T("������ �X�^�e�B�b�N���b�V���ɂ͎g�p�ł��܂��� ������"), MB_OK);
		return XMMatrixIdentity();
	}
	if (nMesh >= m_dwMeshNum || nBone >= m_pMeshArray[nMesh].m_NumBones)
	{
		MessageBox(nullptr, _T("������ GetFrameMatrices() ������"), _T("������ ���b�V���ԍ��s���@���́@�{�[���ԍ��s���B ������"), MB_OK);
		OutputDebugString(_T("������ GetFrameMatrices() ���b�V���ԍ��s���@���́@�{�[���ԍ��s�� ������\n"));
		return XMMatrixIdentity();
	}
	if (animStatus == nullptr || !m_Animation[animStatus->PlayingID()].used)		   // -- 2024.9.5
	{
		MessageBox(nullptr, _T("������ GetFrameMatrices() ������"), _T("������ �X�L�����b�V���ɁA�w�肳�ꂽ�A�j���[�V�������ǂݍ��܂�Ă��܂��� ������"), MB_OK);
		return XMMatrixIdentity();
	}

	//  �A�j���[�V�������ς���Ă����ꍇ�A��Ɨ̈��Ńt���[�����ŏ��ɖ߂� --------------------------------- // -- 2018.8.28
	// �Ȃ��A�ŏ��Ƃ�animFrame���O�t���[���ɂ��邱�ƁBstartFrame�ł͂Ȃ��B���R��animFrame�͓Y���ԍ�������ł���B
	int  animFrameW;
	animFrameW = animStatus->CurrentFrame();		// -- 2024.9.5
	//if (animStatus.animNum != animStatus.animNumOld) animFrameW = 0;

	// �w�胁�b�V���̎w��A�j���[�V�����̎w��{�[���̎w��t���[���̃}�g���b�N�X���擾
	//mBoneWorld = m_pMeshArray[nMesh].m_BoneArray[animStatus->PlayingID()][nBone].framePose[animFrameW]; // -- 2018.8.28
	mBoneWorld = GetMixBoneMatrix(animStatus, nBone, nMesh);											  // -- 2024.10.7

	// ���[�g�{�[���A�j��������Ƃ��̏���                                   // -- 2020.12.15
	if( m_RootAnimType[animStatus->PlayingID()] != eRootAnimNone)	  	// -- 2024.9.5
	{
		// ���[�g�{�[���A�j���s��̋t�s����|�����킹�ă}�g���b�N�X���쐬����
		mBoneWorld =
			mBoneWorld * XMMatrixInverse(nullptr, m_RootBoneArray[animStatus->PlayingID()].framePose[animFrameW]);
	}

	// �}�g���b�N�X���E����W�n���獶����W�n�ɕϊ�(Z���𔽓])
	mBoneWorld = XMMatrixScaling(1, 1, -1) * mBoneWorld;    // -- 2020.7.15

	return mBoneWorld;
}

//==========================================================================================================================================================
// �w��{�[���̃}�g���b�N�X���擾																					     // -- 2024.9.5
// �i���[���h�}�g���b�N�X�Ɗ|�����킹�����ʂ��擾�j
// 
// �����F
// 	  Animator* animStatus          �A�j���[�^�[
//    const MATRIX4X4& mWorld       ���[���h�}�g���b�N�X
//    const DWORD& nBone            �{�[���ԍ�
//    const DWORD& nMesh            ���b�V���ԍ�(�ȗ��l�O)
// �߂�l�F
//    MATRIX4X4                    �w��{�[���̃}�g���b�N�X�i���[���h�}�g���b�N�X�Ɗ|�����킹���}�g���b�N�X�j
//==========================================================================================================================================================
MATRIX4X4 CFbxMesh::GetFrameMatrices(Animator* animStatus, const MATRIX4X4& mWorld, const DWORD& nBone, const DWORD& nMesh)
{
	MATRIX4X4 mBoneWorld;

	if (m_nMeshType == 1)
	{
		MessageBox(nullptr, _T("������ GetFrameMatrices() ������"), _T("������ �X�^�e�B�b�N���b�V���ɂ͎g�p�ł��܂��� ������"), MB_OK);
		return mWorld;
	}
	if (nMesh >= m_dwMeshNum || nBone >= m_pMeshArray[nMesh].m_NumBones)
	{
		MessageBox(nullptr, _T("������ GetFrameMatrices() ������"), _T("������ ���b�V���ԍ��s���@���́@�{�[���ԍ��s���B ������"), MB_OK);
		OutputDebugString(_T("������ GetFrameMatrices() ���b�V���ԍ��s���@���́@�{�[���ԍ��s�� ������\n"));
		return mWorld;
	}
	if (animStatus == nullptr ||  !m_Animation[animStatus->PlayingID()].used)						  	// -- 2024.9.5
	{
		MessageBox(nullptr, _T("������ GetFrameMatrices() ������"), _T("������ �X�L�����b�V���ɁA�w�肳�ꂽ�A�j���[�V�������ǂݍ��܂�Ă��܂��� ������"), MB_OK);
		return mWorld;
	}

	//  �A�j���[�V�������ς���Ă����ꍇ�A��Ɨ̈��Ńt���[�����ŏ��ɖ߂� --------------------------------- // -- 2018.8.28
	// �Ȃ��A�ŏ��Ƃ�animFrame���O�t���[���ɂ��邱�ƁBstartFrame�ł͂Ȃ��B���R��animFrame�͓Y���ԍ�������ł���B
	int  animFrameW;
	animFrameW = animStatus->CurrentFrame();
	//if (animStatus.animNum != animStatus.animNumOld) animFrameW = 0;

	// �w�胁�b�V���̎w��A�j���[�V�����̎w��{�[���̎w��t���[���̃}�g���b�N�X���擾
	//mBoneWorld = m_pMeshArray[nMesh].m_BoneArray[animStatus->PlayingID()][nBone].framePose[animFrameW];      // -- 2024.9.5
	mBoneWorld = GetMixBoneMatrix(animStatus, nBone, nMesh);											       // -- 2024.10.7

	// ���[�g�{�[���A�j��������Ƃ��̏���                                   // -- 2020.12.15
	if (m_RootAnimType[animStatus->PlayingID()] != eRootAnimNone)						   // -- 2024.9.5
	{
		// ���[�g�{�[���A�j���s��̋t�s����|�����킹�ă}�g���b�N�X���쐬����
		mBoneWorld =
			mBoneWorld * XMMatrixInverse(nullptr, m_RootBoneArray[animStatus->PlayingID()].framePose[animFrameW]);
	}

	// �}�g���b�N�X���E����W�n���獶����W�n�ɕϊ�(Z���𔽓])
	mBoneWorld = XMMatrixScaling(1, 1, -1) * mBoneWorld;    // -- 2020.7.15

	return mBoneWorld * mWorld;  // ���[���h�}�g���b�N�X�Ɗ|�����킹��
}

// �{�[���z�񂩂�~�b�N�X�����{�[���}�g���b�N�X�𓾂�
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
	if (animStatus->Rate() >= 1.0f) { // �u�����h����K�v�Ȃ�
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
// ���[�g�{�[���A�j���[�V�����̃}�g���b�N�X���擾                                                                                    // -- 2024.9.5
// 
// �����F
//    const int& UpFrame            �A�j���[�V�����t���[���̌��ݒl����̑����i�ȗ��l�F�O�j
//
// �߂�l�F
//    MATRIX4X4                    ���[�g�{�[���̃}�g���b�N�X
//==========================================================================================================================================================
MATRIX4X4 CFbxMesh::GetRootAnimMatrices(Animator* animStatus, const int& UpFrame)
{
	MATRIX4X4 mBoneWorld;

	if (m_nMeshType != 2)
	{
		MessageBox(nullptr, _T("������ GetRootAnimMatrices() ������"), _T("������ �X�^�e�B�b�N���b�V���ɂ͎g�p�ł��܂��� ������"), MB_OK);
		return XMMatrixIdentity();
	}
	if (animStatus == nullptr || !m_Animation[animStatus->PlayingID()].used)	   // -- 2024.9.5
	{
		MessageBox(nullptr, _T("������ GetRootAnimMatrices() ������"), _T("������ �X�L�����b�V���ɁA�w�肳�ꂽ�A�j���[�V�������ǂݍ��܂�Ă��܂��� ������"), MB_OK);
		return XMMatrixIdentity();
	}

	// �A�j���[�V�����t���[���ɑ����l���������Č��ݒl�Ƃ���B
	// ����E�����l�𒴂��Ă����ꍇ�͓K�؂Ȉʒu�ɒ�������B
	// �Ȃ��A�ŏ��Ƃ�animFrame���O�t���[���ɂ��邱�ƁBstartFrame�ł͂Ȃ��B���R��animFrame�͓Y���ԍ�������ł���B
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
	//  �A�j���[�V�������ς���Ă����ꍇ�A��Ɨ̈��Ńt���[�����ŏ��ɖ߂�
	// �Ȃ��A�ŏ��Ƃ�animFrame���O�t���[���ɂ��邱�ƁBstartFrame�ł͂Ȃ��B���R��animFrame�͓Y���ԍ�������ł���B
	//if (animStatus.animNum != animStatus.animNumOld) animFrameW = 0;

	// ���[�g�{�[���A�j���̌��݃t���[���̃}�g���b�N�X���擾
	mBoneWorld = m_RootBoneArray[animStatus->PlayingID()].framePose[animFrameW];

	return mBoneWorld;
}

//==========================================================================================================================================================
// ���[�g�{�[���A�j���[�V�����̌��ݒl�t���[���̈�O�̃t���[���ʒu����̑����}�g���b�N�X���擾����                                      // -- 2024.9.5
// 
// �����F
//    const int& UpFrame            �A�j���[�V�����t���[���̌��ݒl����̑����i�ȗ��l�F�O�j
//    const int& StartFrameUp       �A�j���t���[�����J�n�t���[���i�t���[���O�j�̂Ƃ��̑����l�̏������@�i�ȗ��l�F�P�j
//                                    0:XMMatrixIdentity()�ɂ���   1:�O�`�P�̑����l���g�p����
//
// �߂�l�F
//    MATRIX4X4                     ���[�g�{�[���̈�O�̈ʒu����̑����}�g���b�N�X
//==========================================================================================================================================================
MATRIX4X4 CFbxMesh::GetRootAnimUpMatrices(Animator* animStatus, const int& UpFrame, const int& StartFrameUp)
{
	MATRIX4X4 mBoneWorld, mBoneWorldOld;

	if (m_nMeshType != 2)
	{
		MessageBox(nullptr, _T("������ GetRootAnimUpMatrices() ������"), _T("������ �X�^�e�B�b�N���b�V���ɂ͎g�p�ł��܂��� ������"), MB_OK);
		return XMMatrixIdentity();
	}
	if (animStatus == nullptr || !m_Animation[animStatus->PlayingID()].used)		   // -- 2024.9.5
	{
		MessageBox(nullptr, _T("������ GetRootAnimUpMatrices() ������"), _T("������ �X�L�����b�V���ɁA�w�肳�ꂽ�A�j���[�V�������ǂݍ��܂�Ă��܂��� ������"), MB_OK);
		return XMMatrixIdentity();
	}

	// �A�j���[�V�����t���[���ɑ����l���������Č��ݒl�Ƃ���B
	// ����E�����l�𒴂��Ă����ꍇ�͓K�؂Ȉʒu�ɒ�������B
	// �Ȃ��A�ŏ��Ƃ�animFrame���O�t���[���ɂ��邱�ƁBstartFrame�ł͂Ȃ��B���R��animFrame�͓Y���ԍ�������ł���B
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
	//  �A�j���[�V�������ς���Ă����ꍇ�A��Ɨ̈��Ńt���[�����ŏ��ɖ߂�
	// �Ȃ��A�ŏ��Ƃ�animFrame���O�t���[���ɂ��邱�ƁBstartFrame�ł͂Ȃ��B���R��animFrame�͓Y���ԍ�������ł���B
	//if (animStatus.animNum != animStatus.animNumOld) animFrameW = 0;

	// �A�j���[�V�����t���[���̌��ݒl���O�̏ꍇ
	if (animFrameW <= 0)
	{
		if (StartFrameUp == 0) return XMMatrixIdentity();  // �����l�͂O�ƂȂ�
		animFrameW = 1;                                    // �O�`�P�̑����l���g�p����
	}

	// ���[�g�{�[���A�j���̌��݃t���[���̃}�g���b�N�X���擾
	mBoneWorld = m_RootBoneArray[animStatus->PlayingID()].framePose[animFrameW];

	// ���[�g�{�[���A�j���̌��ݒl����O�̃t���[���̃}�g���b�N�X���擾
	mBoneWorldOld = m_RootBoneArray[animStatus->PlayingID()].framePose[animFrameW-1];

	// ���ݒl�t���[���}�g���b�N�X�̈�O�̃t���[���}�g���b�N�X����̑����}�g���b�N�X�𓾂�
	mBoneWorld = XMMatrixInverse(nullptr, mBoneWorldOld) * mBoneWorld;

	return mBoneWorld;
}

//------------------------------------------------------------------------
//																			 // -- 2024.9.5
//	���b�V������ʂɃ����_�����O���鏈��
//
//	(�S�Ẵ����_�����O�̌Ăт������s������)
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
//	���b�V������ʂɃ����_�����O���鏈��
//
// ����
//      const MATRIX4X4& mWorld              ���[���h�}�g���b�N�X
//      const MATRIX4X4& mView               �r���[�}�g���b�N�X
//      const MATRIX4X4& mProj               �v���W�F�N�V�����}�g���b�N�X
//      const VECTOR3&   vLight              �����x�N�g��
//      const VECTOR3&   vEye                ���_�x�N�g��
//
//	�߂�l HRESULT
//		S_OK	= ����
//		E_FAIL	= �ُ�
//
//------------------------------------------------------------------------
void CFbxMesh::Render(const MATRIX4X4& mWorld, const MATRIX4X4& mView, const MATRIX4X4& mProj, const VECTOR3& vLight, const VECTOR3& vEye)
{
	if (m_nMeshType == 1)
	{
		RenderStatic(mWorld, mView, mProj, vLight, vEye);
	}
	else  if (m_nMeshType == 2) {
		MessageBox(nullptr, _T("������ Render() ������"), _T("������ �X�L�����b�V���Ȃ̂ɃA�j���[�V����Animator�̎w�肪����܂��� ������"), MB_OK);
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
		MessageBox(nullptr, _T("������ RenderDisplace() ������"), _T("������ �X�L�����b�V���Ȃ̂ɃA�j���[�V����Animator�̎w�肪����܂��� ������"), MB_OK);
		return;
	}
}

//------------------------------------------------------------------------
//
//	�X�^�e�B�b�N���b�V������ʂɃ����_�����O���鏈��
//
// ����
//      const MATRIX4X4& mWorld              ���[���h�}�g���b�N�X
//      const MATRIX4X4& mView               �r���[�}�g���b�N�X
//      const MATRIX4X4& mProj               �v���W�F�N�V�����}�g���b�N�X
//      const VECTOR3&   vLight              �����x�N�g��
//      const VECTOR3&   vEye                ���_�x�N�g��
//
//	�߂�l HRESULT
//		S_OK	= ����
//		E_FAIL	= �ُ�
//
//------------------------------------------------------------------------
void CFbxMesh::RenderStatic(const MATRIX4X4& mWorld, const MATRIX4X4& mView, const MATRIX4X4& mProj, const VECTOR3& vLight, const VECTOR3& vEye)
{

	//�g�p����V�F�[�_�[�̓o�^	
	m_pD3D->m_pDeviceContext->VSSetShader(m_pShader->m_pFbxStaticMesh_VS, nullptr, 0);
	m_pD3D->m_pDeviceContext->PSSetShader(m_pShader->m_pFbxStaticMesh_PS, nullptr, 0);

	//�V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��	
	D3D11_MAPPED_SUBRESOURCE pData;
	CONSTANT_BUFFER_WVLED cb;
	if (SUCCEEDED(m_pD3D->m_pDeviceContext->Map(m_pShader->m_pConstantBufferWVLED, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		//���[���h�s���n��
		cb.mW = XMMatrixTranspose(mWorld);

		//���[���h�A�J�����A�ˉe�s���n��
		cb.mWVP = XMMatrixTranspose(mWorld * mView * mProj);

		//���C�g������n��
		cb.vLightDir = VECTOR4(vLight.x, vLight.y, vLight.z, 0);

		// ���_��n��
		cb.vEyePos = VECTOR4(vEye.x, vEye.y, vEye.z, 1);

		//�J���[��n��
		cb.vDiffuse = m_vDiffuse;

		// �e�����n���B(�g���Ă��Ȃ�)
		cb.vDrawInfo = VECTOR4(0, 0, 0, 0);

		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		m_pD3D->m_pDeviceContext->Unmap(m_pShader->m_pConstantBufferWVLED, 0);
	}

	//���̃R���X�^���g�o�b�t�@�[���g���V�F�[�_�[�̓o�^
	m_pD3D->m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pShader->m_pConstantBufferWVLED);
	m_pD3D->m_pDeviceContext->PSSetConstantBuffers(0, 1, &m_pShader->m_pConstantBufferWVLED);


	//���_�C���v�b�g���C�A�E�g���Z�b�g
	m_pD3D->m_pDeviceContext->IASetInputLayout(m_pShader->m_pFbxStaticMesh_VertexLayout);

	//�v���~�e�B�u�E�g�|���W�[���Z�b�g
	m_pD3D->m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//�e�N�X�`���[�T���v���[���V�F�[�_�[�ɓn��
	m_pD3D->m_pDeviceContext->PSSetSamplers(0, 1, &m_pD3D->m_pSampleLinear);

	// ���b�V���̕`�揇�����肷��m_dwRenderIdxArray�̐ݒ�   // -- 2018.8.1
	SetRenderIdxArray(mWorld, vEye);

	// ���b�V���̐������e�N�X�`���[�A�o�[�e�b�N�X�o�b�t�@�A�C���f�b�N�X�o�b�t�@���Z�b�g���āA�����_�����O����
	for (DWORD mi = 0; mi < m_dwMeshNum; mi++)   // -- 2018.8.1
	{
		DWORD i = m_dwRenderIdxArray[mi];   // -- 2018.8.1

		// �e�N�X�`���i�f�B�t���[�Y�A�m�[�}���A�X�y�L�����j���s�N�Z���V�F�[�_�[�ɓn���B�X���b�g�ԍ���0,1,3
		SetShaderTexture(_T("PS"), 0, &m_pMeshArray[i].m_pTexture);             // �f�B�t���[�Y�e�N�X�`��
		SetShaderTexture(_T("PS"), 1, &m_pMeshArray[i].m_pTextureNormal);       // �m�[�}���e�N�X�`��
		SetShaderTexture(_T("PS"), 3, &m_pMeshArray[i].m_pTextureSpecular);     // �X�y�L�����e�N�X�`��

		// �f�B�t���[�Y�e�N�X�`�����Ȃ��Ƃ��̂݃V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ƀ}�e���A���J���[��n��        // -- 2020.12.15
		if (m_pMeshArray[i].m_pTexture == nullptr)
		{
			SetShaderMatColor(_T("PS"), 3, i);   // �}�e���A���J���[��PS�ɓn��
		}

		// �o�[�e�b�N�X�o�b�t�@�[���Z�b�g
		UINT stride = sizeof(StaticVertexNormal);
		UINT offset = 0;
		m_pD3D->m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pMeshArray[i].m_pVertexBuffer, &stride, &offset);

		//�C���f�b�N�X�o�b�t�@�[���Z�b�g
		m_pD3D->m_pDeviceContext->IASetIndexBuffer(m_pMeshArray[i].m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		//�v���~�e�B�u�������_�����O
		// �i�C���f�b�N�X�̐����w�肵�ă����_�����O�j
		m_pD3D->m_pDeviceContext->DrawIndexed(m_pMeshArray[i].m_dwIndicesNum, 0, 0);
	}

}

//------------------------------------------------------------------------
//
//	�X�^�e�B�b�N���b�V������ʂɃ����_�����O���鏈��(�f�B�X�v���[�X�����g�}�b�s���O)
//
// ����
//      const MATRIX4X4&  mWorld              ���[���h�}�g���b�N�X
//      const MATRIX4X4&4 mView               �r���[�}�g���b�N�X
//      const MATRIX4X4&  mProj               �v���W�F�N�V�����}�g���b�N�X
//      const VECTOR3&    vLight              �����x�N�g��
//      const VECTOR3&    vEye                ���_�x�N�g��
//
//	�߂�l HRESULT
//		S_OK	= ����
//		E_FAIL	= �ُ�
//
//------------------------------------------------------------------------
void CFbxMesh::RenderDisplaceStatic(const MATRIX4X4& mWorld, const MATRIX4X4& mView, const MATRIX4X4& mProj, const VECTOR3& vLight, const VECTOR3& vEye)
{

	//m_pD3D->m_pDeviceContext->RSSetState(m_pD3D->m_pRStateLW);  // �����A���C���[�t���[���\��

	//�g�p����V�F�[�_�[�̓o�^	
	m_pD3D->m_pDeviceContext->VSSetShader(m_pShader->m_pDisplaceStaticMesh_VS, nullptr, 0);
	m_pD3D->m_pDeviceContext->HSSetShader(m_pShader->m_pDisplaceStaticMesh_HS, nullptr, 0);
	m_pD3D->m_pDeviceContext->DSSetShader(m_pShader->m_pDisplaceStaticMesh_DS, nullptr, 0);
	m_pD3D->m_pDeviceContext->PSSetShader(m_pShader->m_pDisplaceStaticMesh_PS, nullptr, 0);


	// �V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��	
	D3D11_MAPPED_SUBRESOURCE pData;
	CONSTANT_BUFFER_WVLED cb;
	if (SUCCEEDED(m_pD3D->m_pDeviceContext->Map(m_pShader->m_pConstantBufferWVLED, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		// ���[���h�s���n��
		cb.mW = XMMatrixTranspose(mWorld);

		// ���[���h�A�J�����A�ˉe�s���n��
		cb.mWVP = XMMatrixTranspose(mWorld * mView * mProj);

		// ���_�ʒu��n��
		cb.vEyePos = VECTOR4(vEye.x, vEye.y, vEye.z, 1);

		// ���C�g�̕�����n��
		cb.vLightDir.x = vLight.x;
		cb.vLightDir.y = vLight.y;
		cb.vLightDir.z = vLight.z;
		cb.vLightDir.w = 0;

		// �J���[���V�F�[�_�[�ɓn��
		cb.vDiffuse = m_vDiffuse;

		// �e�����n���B(�g���Ă��Ȃ�)       // -- 2020.12.15
		cb.vDrawInfo = VECTOR4(0, 0, 0, 0);

		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		m_pD3D->m_pDeviceContext->Unmap(m_pShader->m_pConstantBufferWVLED, 0);
	}
	// ���̃R���X�^���g�o�b�t�@�[���A�ǂ̃V�F�[�_�[�Ŏg�������w��
	m_pD3D->m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pShader->m_pConstantBufferWVLED); // �o�[�e�b�N�X�V�F�[�_�[�Ŏg��
	m_pD3D->m_pDeviceContext->DSSetConstantBuffers(0, 1, &m_pShader->m_pConstantBufferWVLED); // �h���C���V�F�[�_�[�Ŏg��
	m_pD3D->m_pDeviceContext->HSSetConstantBuffers(0, 1, &m_pShader->m_pConstantBufferWVLED); // �n���V�F�[�_�[�Ŏg��
	m_pD3D->m_pDeviceContext->PSSetConstantBuffers(0, 1, &m_pShader->m_pConstantBufferWVLED); // �s�N�Z���V�F�[�_�[�Ŏg��


	// �f�B�X�v���[�X�����g�}�b�s���O�̊e�v�f���V�F�[�_�[�ɓn��
	CONSTANT_BUFFER_DISPLACE sg;
	if (SUCCEEDED(m_pD3D->m_pDeviceContext->Map(m_pShader->m_pConstantBufferDisplace, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{

		// ���f�����猩�����_�ʒu�i�܂�A���f���̋t���[���h�����������_�j��n��
		sg.vEyePosInv = XMVector3TransformCoord(vEye, XMMatrixInverse(nullptr, mWorld));

		// �ŏ�����(1.0�ȏ�)�A�ő勗��
		sg.fMinDistance = 5.0f;
		sg.fMaxDistance = 40.0f;
		// �ő啪����
		sg.iMaxDevide = m_iMaxDevide;

		// ������n��
		sg.vHeight = VECTOR2(0.0f, m_fHeightMax);

		// �_�~�[
		sg.vWaveMove = VECTOR4(0.0f, 0.0f, 0.0f, 0.0f);
		sg.vSpecular = VECTOR4(0.0f, 0.0f, 0.0f, 0.0f);

		memcpy_s(pData.pData, pData.RowPitch, (void*)&sg, sizeof(CONSTANT_BUFFER_DISPLACE));
		m_pD3D->m_pDeviceContext->Unmap(m_pShader->m_pConstantBufferDisplace, 0);
	}
	// ���̃R���X�^���g�o�b�t�@�[���g���V�F�[�_�[�̓o�^
	m_pD3D->m_pDeviceContext->VSSetConstantBuffers(1, 1, &m_pShader->m_pConstantBufferDisplace); // �o�[�e�b�N�X�V�F�[�_�[�Ŏg��
	m_pD3D->m_pDeviceContext->DSSetConstantBuffers(1, 1, &m_pShader->m_pConstantBufferDisplace); // �h���C���V�F�[�_�[�Ŏg��
	m_pD3D->m_pDeviceContext->HSSetConstantBuffers(1, 1, &m_pShader->m_pConstantBufferDisplace); // �n���V�F�[�_�[�Ŏg��
	m_pD3D->m_pDeviceContext->PSSetConstantBuffers(1, 1, &m_pShader->m_pConstantBufferDisplace); // �s�N�Z���V�F�[�_�[�Ŏg��


	// ���_�C���v�b�g���C�A�E�g���Z�b�g
	//  (�f�B�X�v���C�X�����g�}�b�s���O���A���_���C�A�E�g�́A�X�^�e�B�b�N���b�V���Ƌ��p)
	m_pD3D->m_pDeviceContext->IASetInputLayout(m_pShader->m_pFbxStaticMesh_VertexLayout);

	// �v���~�e�B�u�E�g�|���W�[���Z�b�g(�f�B�X�v���C�X�����g�}�b�s���O�p)
	m_pD3D->m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);


	// �e�N�X�`���[�T���v���[���V�F�[�_�[�ɓn��
	// m_pD3D->m_pDeviceContext->DSSetSamplers(0, 1, &m_pD3D->m_pSampleLinear);
	m_pD3D->m_pDeviceContext->PSSetSamplers(0, 1, &m_pD3D->m_pSampleLinear);

	// ���b�V���̕`�揇�����肷��m_dwRenderIdxArray�̐ݒ�   // -- 2018.8.1
	SetRenderIdxArray(mWorld, vEye);

	// ���b�V���̐������e�N�X�`���[�A�o�[�e�b�N�X�o�b�t�@�A�C���f�b�N�X�o�b�t�@���Z�b�g���āA�����_�����O����
	for (DWORD mi = 0; mi < m_dwMeshNum; mi++)   // -- 2018.8.1
	{
		DWORD i = m_dwRenderIdxArray[mi];   // -- 2018.8.1

		// �e�N�X�`���i�f�B�t���[�Y�A�m�[�}���A�n�C�g�A�X�y�L�����j���h���C���V�F�[�_�[�ƃs�N�Z���V�F�[�_�[�ɓn���B�X���b�g�ԍ���0�`3
		SetShaderTexture(_T("DSPS"), 0, &m_pMeshArray[i].m_pTexture);             // �f�B�t���[�Y�e�N�X�`��
		SetShaderTexture(_T("DSPS"), 1, &m_pMeshArray[i].m_pTextureNormal);       // �m�[�}���e�N�X�`��
		SetShaderTexture(_T("DSPS"), 2, &m_pMeshArray[i].m_pTextureHeight);       // �n�C�g�e�N�X�`��
		SetShaderTexture(_T("DSPS"), 3, &m_pMeshArray[i].m_pTextureSpecular);     // �X�y�L�����e�N�X�`��

		// �f�B�t���[�Y�e�N�X�`�����Ȃ��Ƃ��̂݃V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ƀ}�e���A���J���[��n��        // -- 2020.12.15
		if (m_pMeshArray[i].m_pTexture == nullptr)
		{
			SetShaderMatColor(_T("DSPS"), 3, i);   // �}�e���A���J���[��DSPS�ɓn��
		}

		// �o�[�e�b�N�X�o�b�t�@�[���Z�b�g
		UINT stride = sizeof(StaticVertexNormal);
		UINT offset = 0;
		m_pD3D->m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pMeshArray[i].m_pVertexBuffer, &stride, &offset);

		//�C���f�b�N�X�o�b�t�@�[���Z�b�g
		m_pD3D->m_pDeviceContext->IASetIndexBuffer(m_pMeshArray[i].m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		//�v���~�e�B�u�������_�����O
		// �i�C���f�b�N�X�̐����w�肵�ă����_�����O�j
		m_pD3D->m_pDeviceContext->DrawIndexed(m_pMeshArray[i].m_dwIndicesNum, 0, 0);

	}

	// �n���V�F�[�_�[�ƃh���C���V�F�[�_�[�����Z�b�g����
	m_pD3D->m_pDeviceContext->HSSetShader(nullptr, nullptr, 0);
	m_pD3D->m_pDeviceContext->DSSetShader(nullptr, nullptr, 0);

	//m_pD3D->m_pDeviceContext->RSSetState(m_pD3D->m_pRStateL);  // �����ʏ�\��

}

//------------------------------------------------------------------------
//																		  // -- 2024.9.5
//	���b�V������ʂɃ����_�����O���鏈��
//
// ����
//      Animator* animStatus    �A�j���[�V�����X�e�[�^�X
//      const MATRIX4X4& mWorld         ���[���h�}�g���b�N�X
//      const MATRIX4X4& mView          �r���[�}�g���b�N�X
//      const MATRIX4X4& mProj          �v���W�F�N�V�����}�g���b�N�X
//      const VECTOR3&   vLight         �����x�N�g��
//      const VECTOR3&   vEye           ���_�x�N�g��
//
//	�߂�l 
//		�Ȃ�
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
// �V�F�[�_�[�{�[���z��̃{�[���}�g���b�N�X���~�b�N�X����
// �Ȃ��A�V�F�[�_�[�{�[���}�g���b�N�X�͓]�u�s��ɂȂ��Ă���
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
	if (animStatus->Rate() >= 1.0f) { // �u�����h����K�v�Ȃ�
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
//	�X�L�����b�V������ʂɃ����_�����O���鏈��
//
// ����
//      ANIMATION_STATUS& animStatus    �A�j���[�V�����X�e�[�^�X
//      const MATRIX4X4& mWorld         ���[���h�}�g���b�N�X
//      const MATRIX4X4& mView          �r���[�}�g���b�N�X
//      const MATRIX4X4& mProj          �v���W�F�N�V�����}�g���b�N�X
//      const VECTOR3&   vLight         �����x�N�g��
//      const VECTOR3&   vEye           ���_�x�N�g��
//
//	�߂�l 
//		�Ȃ�
//
//------------------------------------------------------------------------
void CFbxMesh::RenderSkin(Animator* animStatus, const MATRIX4X4& mWorld, const MATRIX4X4& mView, const MATRIX4X4& mProj, const VECTOR3& vLight, const VECTOR3& vEye)
{
	// ���b�V���̕`��
	Draw(animStatus, mWorld, mView, mProj, vLight, vEye);				   // -- 2024.9.5
}

//------------------------------------------------------------------------
//																		   // -- 2024.9.5
//	�X�L�����b�V������ʂɕ`�悷�鏈��
//
// ����
//      const int&       animNum   �A�j���[�V�����ԍ� 
//      const int&       frame     �A�j���[�V�����t���[��
//      const MATRIX4X4& mWorld    ���[���h�}�g���b�N�X
//      const MATRIX4X4& mView     �r���[�}�g���b�N�X
//      const MATRIX4X4& mProj     �v���W�F�N�V�����}�g���b�N�X
//      const VECTOR3&   vLight    �����x�N�g��
//      const VECTOR3&   vEye      ���_�x�N�g��
//
//	�߂�l 
//		�Ȃ�
//
//------------------------------------------------------------------------
void CFbxMesh::Draw(Animator* animStatus, const MATRIX4X4& mWorld, const MATRIX4X4& mView, const MATRIX4X4& mProj, const VECTOR3& vLight, const VECTOR3& vEye)
{
	if (animStatus == nullptr) return;    // -- 2024.9.5

	//�g�p����V�F�[�_�[�̓o�^	
	m_pD3D->m_pDeviceContext->VSSetShader(m_pShader->m_pFbxSkinMesh_VS, nullptr, 0);
	m_pD3D->m_pDeviceContext->PSSetShader(m_pShader->m_pFbxSkinMesh_PS, nullptr, 0);

	//�V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��	
	D3D11_MAPPED_SUBRESOURCE pData;
	CONSTANT_BUFFER_WVLED cb;
	if (SUCCEEDED(m_pD3D->m_pDeviceContext->Map(m_pShader->m_pConstantBufferWVLED, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		//���[���h�s���n��
		cb.mW = XMMatrixTranspose(mWorld);

		//���[���h�A�J�����A�ˉe�s���n��
		cb.mWVP = XMMatrixTranspose(mWorld * mView * mProj);

		//���C�g������n��
		cb.vLightDir = VECTOR4(vLight.x, vLight.y, vLight.z, 0);

		// ���_��n��
		cb.vEyePos = VECTOR4(vEye.x, vEye.y, vEye.z, 1);

		//�J���[��n��
		cb.vDiffuse = m_vDiffuse;

		// �e�����n���B(�g���Ă��Ȃ�)       // -- 2020.12.15
		cb.vDrawInfo = VECTOR4(0, 0, 0, 0);

		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		m_pD3D->m_pDeviceContext->Unmap(m_pShader->m_pConstantBufferWVLED, 0);
	}

	//���̃R���X�^���g�o�b�t�@�[���g���V�F�[�_�[�̓o�^
	m_pD3D->m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pShader->m_pConstantBufferWVLED);
	m_pD3D->m_pDeviceContext->PSSetConstantBuffers(0, 1, &m_pShader->m_pConstantBufferWVLED);


	//���̃R���X�^���g�o�b�t�@�[���g���V�F�[�_�[�̓o�^
	m_pD3D->m_pDeviceContext->VSSetConstantBuffers(1, 1, &m_pShader->m_pConstantBufferBone2);
	m_pD3D->m_pDeviceContext->PSSetConstantBuffers(1, 1, &m_pShader->m_pConstantBufferBone2);

	//���_�C���v�b�g���C�A�E�g���Z�b�g
	m_pD3D->m_pDeviceContext->IASetInputLayout(m_pShader->m_pFbxSkinMesh_VertexLayout);

	//�v���~�e�B�u�E�g�|���W�[���Z�b�g
	m_pD3D->m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//�e�N�X�`���[�T���v���[���V�F�[�_�[�ɓn��
	m_pD3D->m_pDeviceContext->PSSetSamplers(0, 1, &m_pD3D->m_pSampleLinear);

	// ���b�V���̕`�揇�����肷��m_dwRenderIdxArray�̐ݒ�   // -- 2018.8.1
	SetRenderIdxArray(mWorld, vEye);

	// ���b�V���̐������e�N�X�`���[�A�o�[�e�b�N�X�o�b�t�@�A�C���f�b�N�X�o�b�t�@�A�{�[���s����Z�b�g���āA�����_�����O����
	for (DWORD mi = 0; mi < m_dwMeshNum; mi++)   // -- 2018.8.1
	{
		DWORD i = m_dwRenderIdxArray[mi];   // -- 2018.8.1

		// �e�N�X�`���i�f�B�t���[�Y�A�m�[�}���A�X�y�L�����j���s�N�Z���V�F�[�_�[�ɓn���B�X���b�g�ԍ���0,1,3
		SetShaderTexture(_T("PS"), 0, &m_pMeshArray[i].m_pTexture);             // �f�B�t���[�Y�e�N�X�`��
		SetShaderTexture(_T("PS"), 1, &m_pMeshArray[i].m_pTextureNormal);       // �m�[�}���e�N�X�`��
		SetShaderTexture(_T("PS"), 3, &m_pMeshArray[i].m_pTextureSpecular);     // �X�y�L�����e�N�X�`��

		// �f�B�t���[�Y�e�N�X�`�����Ȃ��Ƃ��̂݃V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ƀ}�e���A���J���[��n��        // -- 2020.12.15
		if (m_pMeshArray[i].m_pTexture == nullptr)
		{
			SetShaderMatColor(_T("PS"), 3, i);   // �}�e���A���J���[��PS�ɓn��
		}

		// �o�[�e�b�N�X�o�b�t�@�[���Z�b�g
		UINT stride = sizeof(SkinVertexNormal);
		UINT offset = 0;
		m_pD3D->m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pMeshArray[i].m_pVertexBuffer, &stride, &offset);

		//�C���f�b�N�X�o�b�t�@�[���Z�b�g
		m_pD3D->m_pDeviceContext->IASetIndexBuffer(m_pMeshArray[i].m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		// �{�[���s����{�[���̃R���X�^���g�o�b�t�@�ɃZ�b�g
		if (SUCCEEDED(m_pD3D->m_pDeviceContext->Map(
			m_pShader->m_pConstantBufferBone2,   // �}�b�v���郊�\�[�X�E�{�[���s��p�R���X�^���g�o�b�t�@
			0,                               // �T�u���\�[�X�̃C���f�b�N�X�ԍ�
			D3D11_MAP_WRITE_DISCARD,         // �������݃A�N�Z�X
			0,                               //
			&pData)))                        // �f�[�^�̏������ݐ�|�C���^
		{
			// ���̃��b�V���̃V�F�[�_�[�{�[���z��̂����A�w�肳�ꂽ�A�j���[�V�����ԍ��ianimNum�j�A�t���[���ԍ�(frame)�̃{�[���s��z���擪����{�[���������]������
			MakeBoneMatrix(animStatus, (MATRIX4X4*)pData.pData, m_pMeshArray[i]);
//			memcpy_s(pData.pData, sizeof(MATRIX4X4)*m_pMeshArray[i].m_NumBones, &m_pMeshArray[i].m_pBoneShader[animNum][frame].shaderFramePose, sizeof(MATRIX4X4)*m_pMeshArray[i].m_NumBones);		// cbBones�̏������� �S�{�[���̃t���[���|�[�Y�s���n��
			m_pD3D->m_pDeviceContext->Unmap(m_pShader->m_pConstantBufferBone2, 0);
		}

		//�v���~�e�B�u�������_�����O
		// �i�C���f�b�N�X�̐����w�肵�ă����_�����O�j
		m_pD3D->m_pDeviceContext->DrawIndexed(m_pMeshArray[i].m_dwIndicesNum, 0, 0);
	}

}

//------------------------------------------------------------------------    // -- 2024.9.5
//
//	�X�L�����b�V������ʂɃ����_�����O���鏈��(�f�B�X�v���[�X�����g�}�b�s���O)
//
// ����
//      ANIMATION_STATUS& animStatus    �A�j���[�V�����X�e�[�^�X
//      const MATRIX4X4& mWorld         ���[���h�}�g���b�N�X
//      const MATRIX4X4& mView          �r���[�}�g���b�N�X
//      const MATRIX4X4& mProj          �v���W�F�N�V�����}�g���b�N�X
//      const VECTOR3&   vLight         �����x�N�g��
//      const VECTOR3&   vEye           ���_�x�N�g��
//
//	�߂�l 
//		�Ȃ�
//
//------------------------------------------------------------------------
void CFbxMesh::RenderDisplaceSkin(Animator* animStatus, const MATRIX4X4& mWorld, const MATRIX4X4& mView, const MATRIX4X4& mProj, const VECTOR3& vLight, const VECTOR3& vEye)
{
	// ���b�V���̕`��
	DrawDisplace(animStatus, mWorld, mView, mProj, vLight, vEye);		  // -- 2024.9.5
}

//------------------------------------------------------------------------  // -- 2020.1.15
//
//	�X�L�����b�V������ʂɕ`�悷�鏈��(�f�B�X�v���[�X�����g�}�b�s���O)
//
// ����
//      const int&       animNum   �A�j���[�V�����ԍ� 
//      const int&       frame     �A�j���[�V�����t���[��
//      const MATRIX4X4& mWorld    ���[���h�}�g���b�N�X
//      const MATRIX4X4& mView     �r���[�}�g���b�N�X
//      const MATRIX4X4& mProj     �v���W�F�N�V�����}�g���b�N�X
//      const VECTOR3&   vLight    �����x�N�g��
//      const VECTOR3&   vEye      ���_�x�N�g��
//
//	�߂�l 
//		�Ȃ�
//
//------------------------------------------------------------------------
void CFbxMesh::DrawDisplace(Animator* animStatus, const MATRIX4X4& mWorld, const MATRIX4X4& mView, const MATRIX4X4& mProj, const VECTOR3& vLight, const VECTOR3& vEye)
{
	if (animStatus == nullptr) return;    // -- 2024.9.5

	//�g�p����V�F�[�_�[�̓o�^	
	m_pD3D->m_pDeviceContext->VSSetShader(m_pShader->m_pDisplaceSkinMesh_VS, nullptr, 0);
	m_pD3D->m_pDeviceContext->HSSetShader(m_pShader->m_pDisplaceSkinMesh_HS, nullptr, 0);
	m_pD3D->m_pDeviceContext->DSSetShader(m_pShader->m_pDisplaceSkinMesh_DS, nullptr, 0);
	m_pD3D->m_pDeviceContext->PSSetShader(m_pShader->m_pDisplaceSkinMesh_PS, nullptr, 0);


	// �V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��	
	D3D11_MAPPED_SUBRESOURCE pData;
	CONSTANT_BUFFER_WVLED cb;
	if (SUCCEEDED(m_pD3D->m_pDeviceContext->Map(m_pShader->m_pConstantBufferWVLED, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		// ���[���h�s���n��
		cb.mW = XMMatrixTranspose(mWorld);

		// ���[���h�A�J�����A�ˉe�s���n��
		cb.mWVP = XMMatrixTranspose(mWorld * mView * mProj);

		// ���_�ʒu��n��
		cb.vEyePos = VECTOR4(vEye.x, vEye.y, vEye.z, 1);

		// ���C�g�̕�����n��
		cb.vLightDir.x = vLight.x;
		cb.vLightDir.y = vLight.y;
		cb.vLightDir.z = vLight.z;
		cb.vLightDir.w = 0;

		// �J���[���V�F�[�_�[�ɓn��
		cb.vDiffuse = m_vDiffuse;

		// �e�����n���B(�g���Ă��Ȃ�)       // -- 2020.12.15
		cb.vDrawInfo = VECTOR4(0, 0, 0, 0);

		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		m_pD3D->m_pDeviceContext->Unmap(m_pShader->m_pConstantBufferWVLED, 0);
	}
	// ���̃R���X�^���g�o�b�t�@�[���A�ǂ̃V�F�[�_�[�Ŏg�������w��
	m_pD3D->m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pShader->m_pConstantBufferWVLED); // �o�[�e�b�N�X�V�F�[�_�[�Ŏg��
	m_pD3D->m_pDeviceContext->DSSetConstantBuffers(0, 1, &m_pShader->m_pConstantBufferWVLED); // �h���C���V�F�[�_�[�Ŏg��
	m_pD3D->m_pDeviceContext->HSSetConstantBuffers(0, 1, &m_pShader->m_pConstantBufferWVLED); // �n���V�F�[�_�[�Ŏg��
	m_pD3D->m_pDeviceContext->PSSetConstantBuffers(0, 1, &m_pShader->m_pConstantBufferWVLED); // �s�N�Z���V�F�[�_�[�Ŏg��


	// �f�B�X�v���[�X�����g�}�b�s���O�̊e�v�f���V�F�[�_�[�ɓn��
	CONSTANT_BUFFER_DISPLACE sg;
	if (SUCCEEDED(m_pD3D->m_pDeviceContext->Map(m_pShader->m_pConstantBufferDisplace, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{

		// ���f�����猩�����_�ʒu�i�܂�A���f���̋t���[���h�����������_�j��n��
		sg.vEyePosInv = XMVector3TransformCoord(vEye, XMMatrixInverse(nullptr, mWorld));

		// �ŏ�����(1.0�ȏ�)�A�ő勗��
		sg.fMinDistance = 3.0f;
		sg.fMaxDistance = 30.0f;
		// �ő啪����
		sg.iMaxDevide = m_iMaxDevide;

		// ������n��
		sg.vHeight = VECTOR2(0.0f, m_fHeightMax);

		// �_�~�[
		sg.vWaveMove = VECTOR4(0.0f, 0.0f, 0.0f, 0.0f);
		sg.vSpecular = VECTOR4(0.0f, 0.0f, 0.0f, 0.0f);

		memcpy_s(pData.pData, pData.RowPitch, (void*)&sg, sizeof(CONSTANT_BUFFER_DISPLACE));
		m_pD3D->m_pDeviceContext->Unmap(m_pShader->m_pConstantBufferDisplace, 0);
	}
	// ���̃R���X�^���g�o�b�t�@�[���g���V�F�[�_�[�̓o�^
	m_pD3D->m_pDeviceContext->VSSetConstantBuffers(1, 1, &m_pShader->m_pConstantBufferDisplace);
	m_pD3D->m_pDeviceContext->DSSetConstantBuffers(1, 1, &m_pShader->m_pConstantBufferDisplace);
	m_pD3D->m_pDeviceContext->HSSetConstantBuffers(1, 1, &m_pShader->m_pConstantBufferDisplace);
	m_pD3D->m_pDeviceContext->PSSetConstantBuffers(1, 1, &m_pShader->m_pConstantBufferDisplace);


	//���̃R���X�^���g�o�b�t�@�[���g���V�F�[�_�[�̓o�^
	m_pD3D->m_pDeviceContext->VSSetConstantBuffers(2, 1, &m_pShader->m_pConstantBufferBone2);
	m_pD3D->m_pDeviceContext->PSSetConstantBuffers(2, 1, &m_pShader->m_pConstantBufferBone2);


	// ���_�C���v�b�g���C�A�E�g���Z�b�g
	//  (�f�B�X�v���C�X�����g�}�b�s���O���A���_���C�A�E�g�́A�X�L�����b�V���Ƌ��p)
	m_pD3D->m_pDeviceContext->IASetInputLayout(m_pShader->m_pFbxSkinMesh_VertexLayout);

	// �v���~�e�B�u�E�g�|���W�[���Z�b�g(�f�B�X�v���C�X�����g�}�b�s���O�p)
	m_pD3D->m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);


	// �e�N�X�`���[�T���v���[���V�F�[�_�[�ɓn��
	// m_pD3D->m_pDeviceContext->DSSetSamplers(0, 1, &m_pD3D->m_pSampleLinear);
	m_pD3D->m_pDeviceContext->PSSetSamplers(0, 1, &m_pD3D->m_pSampleLinear);

	// ���b�V���̕`�揇�����肷��m_dwRenderIdxArray�̐ݒ� 
	SetRenderIdxArray(mWorld, vEye);

	// ���b�V���̐������e�N�X�`���[�A�o�[�e�b�N�X�o�b�t�@�A�C���f�b�N�X�o�b�t�@���Z�b�g���āA�����_�����O����
	for (DWORD mi = 0; mi < m_dwMeshNum; mi++)
	{
		DWORD i = m_dwRenderIdxArray[mi];

		// �e�N�X�`���i�f�B�t���[�Y�A�m�[�}���A�n�C�g�A�X�y�L�����j���h���C���V�F�[�_�[�ƃs�N�Z���V�F�[�_�[�ɓn���B�X���b�g�ԍ���0�`3
		SetShaderTexture(_T("DSPS"), 0, &m_pMeshArray[i].m_pTexture);             // �f�B�t���[�Y�e�N�X�`��
		SetShaderTexture(_T("DSPS"), 1, &m_pMeshArray[i].m_pTextureNormal);       // �m�[�}���e�N�X�`��
		SetShaderTexture(_T("DSPS"), 2, &m_pMeshArray[i].m_pTextureHeight);       // �n�C�g�e�N�X�`��
		SetShaderTexture(_T("DSPS"), 3, &m_pMeshArray[i].m_pTextureSpecular);     // �X�y�L�����e�N�X�`��

		// �f�B�t���[�Y�e�N�X�`�����Ȃ��Ƃ��̂݃V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ƀ}�e���A���J���[��n��        // -- 2020.12.15
		if (m_pMeshArray[i].m_pTexture == nullptr)
		{
			SetShaderMatColor(_T("DSPS"), 3, i);   // �}�e���A���J���[��DSPS�ɓn��
		}

		// �o�[�e�b�N�X�o�b�t�@�[���Z�b�g
		UINT stride = sizeof(SkinVertexNormal);
		UINT offset = 0;
		m_pD3D->m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pMeshArray[i].m_pVertexBuffer, &stride, &offset);

		//�C���f�b�N�X�o�b�t�@�[���Z�b�g
		m_pD3D->m_pDeviceContext->IASetIndexBuffer(m_pMeshArray[i].m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		// �{�[���s����{�[���̃R���X�^���g�o�b�t�@�ɃZ�b�g
		if (SUCCEEDED(m_pD3D->m_pDeviceContext->Map(
			m_pShader->m_pConstantBufferBone2,   // �}�b�v���郊�\�[�X�E�{�[���s��p�R���X�^���g�o�b�t�@
			0,                               // �T�u���\�[�X�̃C���f�b�N�X�ԍ�
			D3D11_MAP_WRITE_DISCARD,         // �������݃A�N�Z�X
			0,                               //
			&pData)))                        // �f�[�^�̏������ݐ�|�C���^
		{
			// ���̃��b�V���̃V�F�[�_�[�{�[���z��̂����A�w�肳�ꂽ�A�j���[�V�����ԍ��ianimNum�j�A�t���[���ԍ�(frame)�̃{�[���s��z���擪����{�[���������]������
//			memcpy_s(pData.pData, sizeof(MATRIX4X4)*m_pMeshArray[i].m_NumBones, &m_pMeshArray[i].m_pBoneShader[animNum][frame].shaderFramePose, sizeof(MATRIX4X4)*m_pMeshArray[i].m_NumBones);		// cbBones�̏������� �S�{�[���̃t���[���|�[�Y�s���n��
			MakeBoneMatrix(animStatus, (MATRIX4X4*)pData.pData, m_pMeshArray[i]);
			m_pD3D->m_pDeviceContext->Unmap(m_pShader->m_pConstantBufferBone2, 0);
		}

		//�v���~�e�B�u�������_�����O
		// �i�C���f�b�N�X�̐����w�肵�ă����_�����O�j
		m_pD3D->m_pDeviceContext->DrawIndexed(m_pMeshArray[i].m_dwIndicesNum, 0, 0);

	}

	// �n���V�F�[�_�[�ƃh���C���V�F�[�_�[�����Z�b�g����
	m_pD3D->m_pDeviceContext->HSSetShader(nullptr, nullptr, 0);
	m_pD3D->m_pDeviceContext->DSSetShader(nullptr, nullptr, 0);

}

//------------------------------------------------------------------------
//
//	�e�N�X�`�����V�F�[�_�[�ɓn������
//
// ����
//  TCHAR ProfileName[]        : �ݒ肷��V�F�[�_�[�̎�ށi"PS" or "DSPS"�j
//  int   SlotNo               : �ݒ肷��X���b�g�ԍ�
//  ID3D11ShaderResourceView** : �n���e�N�X�`��
//  
//  �߂�l 
//
//------------------------------------------------------------------------
void  CFbxMesh::SetShaderTexture(const TCHAR ProfileName[], int SlotNo, ID3D11ShaderResourceView** ppTexture)
{
	int prof = _tcscmp(_T("PS"), ProfileName);  // "PS"�݂̂̂Ƃ���0, "DSPS"�̂Ƃ���0�ȊO

	if (ppTexture && *ppTexture)
	{
		// �e�N�X�`��������Ƃ�
		if (prof != 0) m_pD3D->m_pDeviceContext->DSSetShaderResources(SlotNo, 1, ppTexture);  // �e�N�X�`���[���h���C���V�F�[�_�[�ɓn��
		m_pD3D->m_pDeviceContext->PSSetShaderResources(SlotNo, 1, ppTexture);  // �e�N�X�`���[���s�N�Z���V�F�[�_�[�ɓn��
	}
	else {
		// �e�N�X�`���������Ƃ�
		ID3D11ShaderResourceView* Nothing[1] = { 0 };
		if (prof != 0) m_pD3D->m_pDeviceContext->DSSetShaderResources(SlotNo, 1, Nothing);
		m_pD3D->m_pDeviceContext->PSSetShaderResources(SlotNo, 1, Nothing);
	}
}

//------------------------------------------------------------------------
//
//	�}�e���A���J���[���V�F�[�_�[�ɓn������
//
// ����
//  TCHAR ProfileName[]        : �ݒ肷��V�F�[�_�[�̎�ށi"PS" or "DSPS"�j
//  int   SlotNo               : �ݒ肷��X���b�g�ԍ�
//  int   i                    : ���b�V���ԍ�
//  
//  �߂�l 
//
//------------------------------------------------------------------------
void  CFbxMesh::SetShaderMatColor(const TCHAR ProfileName[], int SlotNo, int i )
{
	int prof = _tcscmp(_T("PS"), ProfileName);  // "PS"�݂̂̂Ƃ���0, "DSPS"�̂Ƃ���0�ȊO

	//�V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��
	D3D11_MAPPED_SUBRESOURCE pData;
	CONSTANT_BUFFER_MATERIAL cb = {};
	if (SUCCEEDED(m_pD3D->m_pDeviceContext->Map(m_pShader->m_pConstantBufferMaterial, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		// �}�e���A���J���[��n��
		cb.vMatDuffuse = m_pMeshArray[i].m_pMaterialDiffuse;
		cb.vMatSpecular = m_pMeshArray[i].m_pMaterialSpecular;
		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		m_pD3D->m_pDeviceContext->Unmap(m_pShader->m_pConstantBufferMaterial, 0);
	}

	//���̃R���X�^���g�o�b�t�@�[���g���V�F�[�_�[�̓o�^
	if (prof != 0) m_pD3D->m_pDeviceContext->DSSetConstantBuffers(SlotNo, 1, &m_pShader->m_pConstantBufferMaterial);
	m_pD3D->m_pDeviceContext->PSSetConstantBuffers(SlotNo, 1, &m_pShader->m_pConstantBufferMaterial);
}

// ----------------------------------------------------------------------------------------------------------------
// 
// FbxMeshCtrl  ���b�V���R���g���[���N���X                                              // -- 2021.2.4
// 
// ���b�V���̑����I�ȊǗ����s���N���X
// ��Ƀe�N�X�`���̊Ǘ����s���B
// �����̃��b�V���Ԃœ���̃e�N�X�`�����g���Ă���ꍇ�A�d������ǂݍ��݂�r������
// 
// -----------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------
//
//	CFbxMeshCtrl  �R���X�g���N�^	
//
//------------------------------------------------------------------------
CFbxMeshCtrl::CFbxMeshCtrl(CShader* pShader)
{
	m_pD3D = pShader->m_pD3D;
	m_pShader = pShader;
}
//------------------------------------------------------------------------
//
//	CFbxMeshCtrl  �f�X�g���N�^	
//
//------------------------------------------------------------------------
CFbxMeshCtrl::~CFbxMeshCtrl()
{
	std::list<TextureList>::iterator itr;    // ���X�g�̃C�e���[�^
	for (itr = m_TexList.begin(); itr != m_TexList.end(); itr++)
	{
		// ���ۂ͎��O�Ƀ����[�X����Ă����ɂ͗��Ȃ��͂������A�O�̂��߃����[�X����
		SAFE_RELEASE(itr->m_pTexture);   // �e�N�X�`���������[�X
	}
	m_TexList.clear();		// ���X�g�폜
}

//------------------------------------------------------------------------
//
//	�e�N�X�`����ǂݍ��ޏ���	
//
//	�����e�N�X�`�����d�����ēǂݍ��܂Ȃ��悤�Ƀ��X�g�ŊǗ�����	
//	���ꖼ�̂̃e�N�X�`���́A�d�����ēǂݍ��܂��A���X�g��̃e�N�X�`���A�h���X��Ԃ�
//	���X�g��ɂȂ��e�N�X�`���i�V�K�̃e�N�X�`���j�́A�ǂݍ���Ń��X�g�̖����ɓo�^����B���̃A�h���X��Ԃ��B
//	���X�g�̃J�E���^(m_nCnt)�ŁA����e�N�X�`�����g�p���Ă��郁�b�V���̐��𐔂��Ă����B
//
//	����
//	  const TCHAR FName[] : �e�N�X�`����
//
//	�߂�l
//	  ID3D11ShaderResourceView*	�e�N�X�`��
//------------------------------------------------------------------------
ID3D11ShaderResourceView* CFbxMeshCtrl::SetTextureList(const TCHAR FName[])
{
	TextureList tl;
	std::list<TextureList>::iterator itr;    // ���X�g�̃C�e���[�^

	for (itr = m_TexList.begin(); itr != m_TexList.end(); itr++)
	{
		if (_tcscmp(itr->m_FName, FName) == 0)  // ���X�g�̒��Ɉ�v����e�N�X�`�����������Ƃ�
		{
			itr->m_nCnt++;            // �J�E���g�ɂP�������i����͎g�p���Ă���ӏ��̃J�E���g�ɂȂ�j
			return itr->m_pTexture;   // ���X�g�̒��̃e�N�X�`���A�h���X��Ԃ�
		}
	}

	// ���X�g�̒��Ɉ�v����e�N�X�`�����Ȃ������Ƃ��̓e�N�X�`����ǂݍ���Ń��X�g�ɒǉ�����
	if (FAILED(m_pD3D->CreateShaderResourceViewFromFile(FName, &tl.m_pTexture, 3)))
	{
		// �e�N�X�`�����Ȃ������Ƃ�
		//MessageBox(nullptr, FName, _T("������ SetTextureList �e�N�X�`���t�@�C��������܂��� ������"), MB_OK);
		return nullptr;
	}
	// �e�N�X�`�����������Ƃ�
	_tcscpy_s(tl.m_FName, FName);
	tl.m_nCnt = 1;				// �J�E���g���P�ɂ���
	m_TexList.push_back(tl);  // ���X�g�̖����ɒǉ�����

	return tl.m_pTexture;	// �ǂݍ��񂾃e�N�X�`���A�h���X��Ԃ�
}

//------------------------------------------------------------------------
//
//	�w��̃e�N�X�`�������X�g����폜����	
//
//	���X�g�����ǂ�w��̃e�N�X�`���i�A�h���X�j����������
//	����e�N�X�`��������������A�J�E���^(m_nCnt)����P������
//	�J�E���^���O�ɂȂ�����A�������̃e�N�X�`�����g�p���Ă��郁�b�V����
//	����Ȃ��Ȃ����ƌ������ƂȂ̂ŁA�e�N�X�`������������̃��X�g���폜����B
//
//	����
//	  ID3D11ShaderResourceView* pTexture :	�e�N�X�`���A�h���X
//
//------------------------------------------------------------------------
void CFbxMeshCtrl::DeleteTextureList(ID3D11ShaderResourceView* pTexture)
{
	std::list<TextureList>::iterator itr;    // ���X�g�̃C�e���[�^

	for (itr = m_TexList.begin(); itr != m_TexList.end(); itr++)
	{
		if (itr->m_pTexture == pTexture)  // �w��̃e�N�X�`�����������Ƃ�
		{
			itr->m_nCnt--;              // �J�E���^����P������
			if (itr->m_nCnt <= 0)
			{
				// �J�E���^���O�ɂȂ����̂ŁA�ǂ��ł��g���Ă��Ȃ��e�N�X�`���̂��ߍ폜����
				SAFE_RELEASE(itr->m_pTexture);
				m_TexList.erase(itr);
			}
			break;
		}
	}
}


