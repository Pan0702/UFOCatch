//------------------------------------------------------------------------
//
//   �f�B�X�v���C�X�����g�}�b�s���O�𗘗p���郉�C�u����
//                                                ver 3.3
//
//      (�g�ɉ����A�n�ⓙ�̏����ɑΉ�����)
//
//																 2024.3.23
//------------------------------------------------------------------------

#include "Displace.h"
#include "../03_GameMain/GameMain.h"

//------------------------------------------------------------------------
//
//	�g�̃R���X�g���N�^	
//
//  �����@
//
//------------------------------------------------------------------------
CWave::CWave() : CWave(GameDevice()->m_pShader)				 // -- 2024.3.23
{
}
CWave::CWave(const int& iWidth, const int& iHeight, const TCHAR* FName, const TCHAR* TexName, const TCHAR* SpecName)
					: CWave(GameDevice()->m_pShader, iWidth, iHeight, FName, TexName, SpecName)
{
}
// -----------------------------------------------------------------------
CWave::CWave(CShader* pShader)
{
	ZeroMemory(this, sizeof(CWave));
	m_pD3D = pShader->m_pD3D;
	m_pShader = pShader;

	Init();  // �������֐�
}
//------------------------------------------------------------------------
//
//	�g�̃R���X�g���N�^	
//
//  CShader* pShader
//  const int iWidth      �g�̑傫���@��
//  const int iHeight     �g�̑傫���@���s��
//  const TCHAR* FName     �f�B�X�v���C�X�����g�}�b�s���O�p�̃m�[�}���e�N�X�`��
//
//------------------------------------------------------------------------
CWave::CWave(CShader* pShader, const int& iWidth, const int& iHeight, const TCHAR* FName, const TCHAR* TexName, const TCHAR* SpecName)
{
	ZeroMemory(this, sizeof(CWave));
	m_pD3D = pShader->m_pD3D;
	m_pShader = pShader;

	Init();  // �������֐�
	SetSrc(iWidth, iHeight, FName, TexName, SpecName);  //	�g�̕`��p�ɕ��ʃ|���S���𐶐�����
}
//------------------------------------------------------------------------
//
//	�g�̃f�X�g���N�^	
//
//------------------------------------------------------------------------
CWave::~CWave()
{
	DeleteAll();
}
//------------------------------------------------------------------------
//	�������֐�
//																		2017.2.18
//
//  �e��ݒ�l�̏�����
//
//  �����@�@�Ȃ�
//
//  �߂�l�@�@�@�Ȃ�
//------------------------------------------------------------------------
void CWave::Init()
{
	m_vWaveMove    = VECTOR4(0.0f, 0.0f, 0.0f, 0.0f);
	m_vWaveSpeed   = VECTOR2(0.0004f, 0.0002f);		      // �g�̈ړ����x
	m_fWaveHeight  = 0.35f;							      // �g�̍���
	m_vDiffuse     = VECTOR4(0.0f, 0.35f, 0.65f, 0.8f);   // �f�B�t���[�Y�J���[
	m_vSpecular    = VECTOR4(0.8f, 0.8f, 0.8f, 1.0f);     // �X�y�L�����[
	m_fMinDistance = 1.0;    // �ŏ�����(�ő啪�����ɒB���鋗��)
	m_fMaxDistance = 30.0;   // �ő勗��(���̋������番�����n�܂�)
	m_iMaxDevide   = 128;    // �P�̃|���S���̍ő啪����

}

//------------------------------------------------------------------------
//	����E�폜�֐�
//																		2017.2.18
//
//  �e�탊�\�[�X�̉���ƍ폜
//
//  �����@�@�Ȃ�
//
//  �߂�l�@�@�@�Ȃ�
//------------------------------------------------------------------------
void CWave::DeleteAll()
{
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pIndexBuffer);
	SAFE_DELETE_ARRAY(m_pIndexBuffer);
	SAFE_RELEASE(m_pTexture);
	SAFE_RELEASE(m_pNormalTexture);
	SAFE_RELEASE(m_pSpecularTexture);         // -- 2021.2.4
}

//------------------------------------------------------------------------  // -- 2021.2.4
//	�g�̕`��p�ɕ��ʃ|���S���𐶐�����
//
//
//	���Ɖ��s�����w�肵�āA�����̎l�p�|���S���𐶐�����
//	�m�[�}���e�N�X�`����ǂݍ���Őݒ肷��
//
//	const int&   iWidth  :�S�̂̕�
//  const int&   iHeight :�S�̂̉��s��
//  const TCHAR* FName    �f�B�X�v���C�X�����g�}�b�s���O�p�̃m�[�}���e�N�X�`��(�n�C�g�}�b�v�Ƃ��Ďg�p)
//  const TCHAR* TexName  �f�B�t���[�Y�e�N�X�`����(�ȗ���)
//  const TCHAR* SpecName �X�y�L�����e�N�X�`����(�ȗ���)
//
//  �߂�l�@�@�@����:S_OK   �ُ�:E_FAILE
//------------------------------------------------------------------------
HRESULT CWave::SetSrc(const int& iWidth, const int& iHeight, const TCHAR* FName, const TCHAR* TexName, const TCHAR* SpecName)
{
	// ���ʂ̎l�p�`���f��

	DeleteAll();  // �e�탊�\�[�X�̍폜�B�����A�Q�x�ڈȍ~�ɌĂяo���ꂽ�Ƃ��̑΍�B

/*
	// �P���|���S���̃o�[�e�b�N�X�f�[�^����i�O�p�|���S��2�j�̂ݍ쐬����Ƃ�

	float RSize = 2.0f;    // �e�N�X�`���̃T�C�Y

	//�o�[�e�b�N�X�f�[�^�쐬
	DISPLACE_VERTEX pVertices[] =
	{
		{ VECTOR3(-iWidth/2, 0, -iHeight/2), VECTOR2(           0, iHeight/RSize), VECTOR3(0, 1, 0) },    //���_0  ����
		{ VECTOR3(-iWidth/2, 0,  iHeight/2), VECTOR2(           0,             0), VECTOR3(0, 1, 0) },    //���_1  ����
		{ VECTOR3( iWidth/2, 0,  iHeight/2), VECTOR2(iWidth/RSize,             0), VECTOR3(0, 1, 0) },    //���_2  �E��
		{ VECTOR3( iWidth/2, 0, -iHeight/2), VECTOR2(iWidth/RSize, iHeight/RSize), VECTOR3(0, 1, 0) },    //���_3  �E��
	};
	// �C���f�b�N�X�f�[�^�쐬�@3�p�`���X�g�~�Q
	DWORD pIndex[] = { 
		0, 1, 2, 0, 2, 3         // -- 2024.3.23
		//0, 2, 1, 0, 3, 2
	};

	m_dwNumIndex = 2 * 3;
*/

	// �P��RSize�̎l�p�|���S���i�O�p�|���S��2�j�̃o�[�e�b�N�X�f�[�^�𕡐��쐬����Ƃ�
	// �w��T�C�Y��RSize�̐����{�łȂ������Ƃ��A�[���͐؂�̂Ă����

	float RSize = 4.0f;    // �|���S���S�p�`�@�P�ӂ̒����B�e�N�X�`���̃T�C�Y�ł�����B

	// ��̎l�p�|���S���̊�{�f�[�^
	DISPLACE_VERTEX VertexConst[] =
	{
		{ VECTOR3(    0, 0,     0), VECTOR2(0, 1), VECTOR3(0, 1, 0) },    //���_0  ����
		{ VECTOR3(    0, 0, RSize), VECTOR2(0, 0), VECTOR3(0, 1, 0) },    //���_1  ����
		{ VECTOR3(RSize, 0, RSize), VECTOR2(1, 0), VECTOR3(0, 1, 0) },    //���_2  �E��
		{ VECTOR3(RSize, 0,     0), VECTOR2(1, 1), VECTOR3(0, 1, 0) },    //���_3  �E��
	};
	// �C���f�b�N�X�f�[�^�쐬�@3�p�`���X�g�~�Q�̊�{�f�[�^
	DWORD IndexConst[] = {
		0, 1, 2, 0, 2, 3      // -- 2024.3.23
		//0, 2, 1, 0, 3, 2
	};

	// �������Əc�����ɉ��̃|���S�����K�v��
	DWORD numX = (DWORD)(iWidth / RSize);
	DWORD numZ = (DWORD)(iHeight / RSize);

	// �K�v���̒��_�f�[�^�ƃC���f�b�N�X�f�[�^���i�[����z����m�ۂ���
	DISPLACE_VERTEX* pVertices = new DISPLACE_VERTEX[4 * numX * numZ];
	DWORD*  pIndex = new DWORD[2 * 3 * numX * numZ];
	// �C���f�b�N�X����ۑ�����
	m_dwNumIndex = 2 * 3 * numX * numZ;

	// ���_�f�[�^���쐬����
	for (DWORD z=0; z < numZ; z++)
	{
		for (DWORD x = 0; x < numX; x++)
		{
			pVertices[(z*numX + x) * 4 + 0] = VertexConst[0];
			pVertices[(z*numX + x) * 4 + 1] = VertexConst[1];
			pVertices[(z*numX + x) * 4 + 2] = VertexConst[2];
			pVertices[(z*numX + x) * 4 + 3] = VertexConst[3];

			pVertices[(z*numX + x) * 4 + 0].vPos.x = VertexConst[0].vPos.x + x*RSize;
			pVertices[(z*numX + x) * 4 + 0].vPos.z = VertexConst[0].vPos.z + z*RSize;
			pVertices[(z*numX + x) * 4 + 1].vPos.x = VertexConst[1].vPos.x + x*RSize;
			pVertices[(z*numX + x) * 4 + 1].vPos.z = VertexConst[1].vPos.z + z*RSize;
			pVertices[(z*numX + x) * 4 + 2].vPos.x = VertexConst[2].vPos.x + x*RSize;
			pVertices[(z*numX + x) * 4 + 2].vPos.z = VertexConst[2].vPos.z + z*RSize;
			pVertices[(z*numX + x) * 4 + 3].vPos.x = VertexConst[3].vPos.x + x*RSize;
			pVertices[(z*numX + x) * 4 + 3].vPos.z = VertexConst[3].vPos.z + z*RSize;
		}
	}

	// �C���f�b�N�X�f�[�^���쐬����
	for (DWORD z = 0; z < numZ; z++)
	{
		for (DWORD x = 0; x < numX; x++)
		{
			pIndex[(z*numX + x) * 6 + 0] = IndexConst[0] + (z*numX + x) * 4;
			pIndex[(z*numX + x) * 6 + 1] = IndexConst[1] + (z*numX + x) * 4;
			pIndex[(z*numX + x) * 6 + 2] = IndexConst[2] + (z*numX + x) * 4;
			pIndex[(z*numX + x) * 6 + 3] = IndexConst[3] + (z*numX + x) * 4;
			pIndex[(z*numX + x) * 6 + 4] = IndexConst[4] + (z*numX + x) * 4;
			pIndex[(z*numX + x) * 6 + 5] = IndexConst[5] + (z*numX + x) * 4;
		}
	}

	//�o�[�e�b�N�X�o�b�t�@�[���쐬
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(DISPLACE_VERTEX) * 4 * numX * numZ;
	//bd.ByteWidth = sizeof(DISPLACE_VERTEX) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = pVertices;
	if (FAILED(m_pD3D->m_pDevice->CreateBuffer(&bd, &InitData, &m_pVertexBuffer)))
	{
		MessageBox(0, _T("�f�B�X�v���C�X�����g�}�b�s���O�@�o�[�e�b�N�X�o�b�t�@ �쐬���s"), nullptr, MB_OK);
		return false;
	}

	//�C���f�b�N�X�o�b�t�@�[���쐬
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(DWORD) * 2 * 3 * numX * numZ;
	//bd.ByteWidth = sizeof(DWORD) * 2 * 3;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	InitData.pSysMem = pIndex;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;
	if (FAILED(m_pD3D->m_pDevice->CreateBuffer(&bd, &InitData, &m_pIndexBuffer))) 
	{
		MessageBox(0,_T("�f�B�X�v���C�X�����g�}�b�s���O�@�C���f�b�N�X�o�b�t�@ �쐬���s"), nullptr, MB_OK);
		return false;
	}

	SAFE_DELETE_ARRAY(pVertices); 
	SAFE_DELETE_ARRAY(pIndex);

	// �f�B�t���[�Y�e�N�X�`���[���쐬                                                // -- 2021.2.4
	if (TexName)
	{
		if (FAILED(m_pD3D->CreateShaderResourceViewFromFile(TexName, &m_pTexture)))
		{
			MessageBox(0, _T("�f�B�X�v���C�X�����g�}�b�s���O�@�e�N�X�`�� �ǂݍ��ݎ��s"), nullptr, MB_OK);
			return E_FAIL;
		}
	}

	//�m�[�}���e�N�X�`���[���쐬(�n�C�g�}�b�v�Ƃ��Ďg�p)
	if (FName)
	{
		if (FAILED(m_pD3D->CreateShaderResourceViewFromFile(FName, &m_pNormalTexture)))
		{
			MessageBox(0, _T("�f�B�X�v���C�X�����g�}�b�s���O�@�m�[�}���e�N�X�`�� �ǂݍ��ݎ��s"), nullptr, MB_OK);
			return E_FAIL;
		}
	}

	//�X�y�L�����e�N�X�`���[���쐬                                                // -- 2021.2.4
	if (SpecName)
	{
		if (FAILED(m_pD3D->CreateShaderResourceViewFromFile(SpecName, &m_pSpecularTexture)))
		{
			MessageBox(0, _T("�f�B�X�v���C�X�����g�}�b�s���O�@�X�y�L�����e�N�X�`�� �ǂݍ��ݎ��s"), nullptr, MB_OK);
			return E_FAIL;
		}
	}

	return S_OK;
}
//------------------------------------------------------------------------
//  �V�[������ʂɃ����_�����O
//																		2024.3.23
//
//	�ʒu���w�肵�āA�g�������_�����O����
//
//	�����@�Ȃ�
//
//  �߂�l�@�@�@�Ȃ�
//------------------------------------------------------------------------
void CWave::Render(const MATRIX4X4& mWorld)
{
	Render(mWorld, GameDevice()->m_mView, GameDevice()->m_mProj, GameDevice()->m_vLightDir, GameDevice()->m_vEyePt);
}
//------------------------------------------------------------------------
//  �V�[������ʂɃ����_�����O
//																		2017.2.18
//
//	�ʒu�Ɗ������w�肵�āA�g�������_�����O����
//
//	const MATRIX4X4& mWorld	:���[���h�}�g���b�N�X 
//�@const MATRIX4X4& mView	:�r���[�}�g���b�N�X 
//�@const MATRIX4X4& mProj		:�v���W�F�N�V�����}�g���b�N�X 
//�@const VECTOR3& vLight	:���C�g�̍��W 
//�@const VECTOR3& vEye	:���_���W
//
//  �߂�l�@�@�@�Ȃ�
//------------------------------------------------------------------------
void CWave::Render(const MATRIX4X4& mWorld, const MATRIX4X4& mView, const MATRIX4X4& mProj, const VECTOR3& vLight, const VECTOR3& vEye)
{

	//m_pD3D->m_pDeviceContext->RSSetState(m_pD3D->m_pRStateLW);  // �����A���C���[�t���[���\��    // -- 2021.1.11

	// �g�p����V�F�[�_�[�̓o�^
	m_pD3D->m_pDeviceContext->VSSetShader(m_pShader->m_pDisplaceWave_VS, nullptr, 0);
	m_pD3D->m_pDeviceContext->HSSetShader(m_pShader->m_pDisplaceWave_HS, nullptr, 0);
	m_pD3D->m_pDeviceContext->DSSetShader(m_pShader->m_pDisplaceWave_DS, nullptr, 0);
	m_pD3D->m_pDeviceContext->PSSetShader(m_pShader->m_pDisplaceWave_PS, nullptr, 0);


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
		cb.vDiffuse  = m_vDiffuse;

		// �e�����n���B(�g���Ă��Ȃ�)
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

		// �ŏ������A�ő勗��
		sg.fMinDistance = m_fMinDistance;
		sg.fMaxDistance = m_fMaxDistance;
		// �ő啪����
		sg.iMaxDevide = m_iMaxDevide;

		// �g�@�֘A
		m_vWaveMove.x += m_vWaveSpeed.x;
		m_vWaveMove.y += m_vWaveSpeed.y;

		// �g�̈ʒu�ω��ʂ�n��
		sg.vWaveMove  = m_vWaveMove;

		// �g�̍�����n��
		sg.vHeight  = VECTOR2(0.0f, m_fWaveHeight);

		// �X�y�L������n��
		sg.vSpecular = m_vSpecular;

		memcpy_s(pData.pData, pData.RowPitch, (void*)&sg, sizeof(CONSTANT_BUFFER_DISPLACE));
		m_pD3D->m_pDeviceContext->Unmap(m_pShader->m_pConstantBufferDisplace, 0);
	}
	// ���̃R���X�^���g�o�b�t�@�[���g���V�F�[�_�[�̓o�^
	m_pD3D->m_pDeviceContext->VSSetConstantBuffers(1, 1, &m_pShader->m_pConstantBufferDisplace); // �o�[�e�b�N�X�V�F�[�_�[�Ŏg��
	m_pD3D->m_pDeviceContext->DSSetConstantBuffers(1, 1, &m_pShader->m_pConstantBufferDisplace); // �h���C���V�F�[�_�[�Ŏg��
	m_pD3D->m_pDeviceContext->HSSetConstantBuffers(1, 1, &m_pShader->m_pConstantBufferDisplace); // �n���V�F�[�_�[�Ŏg��
	m_pD3D->m_pDeviceContext->PSSetConstantBuffers(1, 1, &m_pShader->m_pConstantBufferDisplace); // �s�N�Z���V�F�[�_�[�Ŏg��


	// �e�N�X�`���[�T���v���[���h���C���V�F�[�_�[�ƃs�N�Z���V�F�[�_�[�ɓn��
	m_pD3D->m_pDeviceContext->DSSetSamplers(0, 1, &m_pD3D->m_pSampleLinear);
	m_pD3D->m_pDeviceContext->PSSetSamplers(0, 1, &m_pD3D->m_pSampleLinear);

	// �f�B�t���[�Y�e�N�X�`���[���h���C���V�F�[�_�[�ƃs�N�Z���V�F�[�_�[�ɓn��    // -- 2021.2.4
	if (m_pTexture)
	{
		m_pD3D->m_pDeviceContext->DSSetShaderResources(0, 1, &m_pTexture);
		m_pD3D->m_pDeviceContext->PSSetShaderResources(0, 1, &m_pTexture);
	}
	else {
		ID3D11ShaderResourceView* Nothing[1] = { 0 };
		m_pD3D->m_pDeviceContext->DSSetShaderResources(0, 1, Nothing);
		m_pD3D->m_pDeviceContext->PSSetShaderResources(0, 1, Nothing);
	}
	// �m�[�}���e�N�X�`���[���h���C���V�F�[�_�[�ƃs�N�Z���V�F�[�_�[�ɓn��(�n�C�g�}�b�v�Ƃ��Ďg�p)
	if (m_pNormalTexture)
	{
		m_pD3D->m_pDeviceContext->DSSetShaderResources(1, 1, &m_pNormalTexture);
		m_pD3D->m_pDeviceContext->PSSetShaderResources(1, 1, &m_pNormalTexture);
	}
	else {
		ID3D11ShaderResourceView* Nothing[1] = { 0 };
		m_pD3D->m_pDeviceContext->DSSetShaderResources(1, 1, Nothing);
		m_pD3D->m_pDeviceContext->PSSetShaderResources(1, 1, Nothing);
	}
	// �X�y�L�����e�N�X�`���[���h���C���V�F�[�_�[�ƃs�N�Z���V�F�[�_�[�ɓn��    // -- 2021.2.4
	if (m_pSpecularTexture)
	{
		m_pD3D->m_pDeviceContext->DSSetShaderResources(3, 1, &m_pSpecularTexture);
		m_pD3D->m_pDeviceContext->PSSetShaderResources(3, 1, &m_pSpecularTexture);
	}
	else {
		ID3D11ShaderResourceView* Nothing[1] = { 0 };
		m_pD3D->m_pDeviceContext->DSSetShaderResources(3, 1, Nothing);
		m_pD3D->m_pDeviceContext->PSSetShaderResources(3, 1, Nothing);
	}


	// ���_�C���v�b�g���C�A�E�g���Z�b�g
	m_pD3D->m_pDeviceContext->IASetInputLayout(m_pShader->m_pDisplaceWave_VertexLayout);
	// �v���~�e�B�u�E�g�|���W�[���Z�b�g
	m_pD3D->m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);

	// �o�[�e�b�N�X�o�b�t�@�[���Z�b�g
	UINT stride = sizeof(DISPLACE_VERTEX);
	UINT offset = 0;
	m_pD3D->m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	// �C���f�b�N�X�o�b�t�@�[���Z�b�g
	stride = sizeof(DWORD);
	offset = 0;
	m_pD3D->m_pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// �v���~�e�B�u�������_�����O
	m_pD3D->m_pDeviceContext->DrawIndexed(m_dwNumIndex, 0, 0);

	// �n���V�F�[�_�[�ƃh���C���V�F�[�_�[�����Z�b�g����
	m_pD3D->m_pDeviceContext->HSSetShader(nullptr, nullptr, 0);
	m_pD3D->m_pDeviceContext->DSSetShader(nullptr, nullptr, 0);


}
