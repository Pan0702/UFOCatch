//-----------------------------------------------------------------------------
//
//  Direct3D�𗘗p���邽�߂̃��C�u����                ver 3.5         2024.11.2
// 
//	�@�@Direct3D�̏�����	
//	�A�@�e�N�X�`���[�T���v���[�ƃu�����h�X�e�[�g�̍쐬
//	�B�@�C���[�W�t�@�C����ǂݍ���Ńe�N�X�`�����쐬����֐�
//	�C�@�R���p�C���ς݃V�F�[�_�[�̓ǂݍ��݂�����֐�
//	�D�@�����_�[�^�[�Q�b�g�̐ݒ������֐�
//	�E�@�R���p�C���ς݃V�F�[�_�[�̓ǂݍ��݂�����֐�  x64�Ή�
//
//																Direct3D.cpp
//
//-----------------------------------------------------------------------------

#include "Direct3D.h"
#include "../GameMain/Macro.h"

#pragma warning(disable : 6387)
//------------------------------------------------------------------------
//
//	Direct3D�̃R���X�g���N�^	
//
//  �����@�Ȃ�
//
//------------------------------------------------------------------------
CDirect3D::CDirect3D()
{
	ZeroMemory(this, sizeof(CDirect3D));
}
//------------------------------------------------------------------------
//
//	Direct3D�̃f�X�g���N�^	
//
//------------------------------------------------------------------------
CDirect3D::~CDirect3D()
{
	DestroyD3D();

}
//------------------------------------------------------------------------
//
//	Direct3D�̏�����	
//
//  HWND hWnd         �E�B���h�E�n���h��
//  DWORD dwWidth     �쐬�����ʂ̕�
//  DWORD dwHeight    �쐬�����ʂ̍���
//
//	�߂�l HRESULT
//		S_OK	= ����
//		E_FAIL	= �ُ�
//
//------------------------------------------------------------------------
HRESULT CDirect3D::InitD3D(HWND hWnd, DWORD dwWidth, DWORD dwHeight)
{
	//
	m_hWnd = hWnd;
	m_dwWindowWidth = dwWidth;
	m_dwWindowHeight = dwHeight;
	
	// DXGI�C���^�[�t�F�C�X					// -- 2024.11.2
	IDXGIFactory1* pGFactory = nullptr;
	if (FAILED(CreateDXGIFactory1(__uuidof(IDXGIFactory1), (VOID**)(&pGFactory))))
	{
		return FALSE;
	}
	// �O���t�B�b�N�J�[�h�̏��	 					// -- 2024.11.2
	m_pAdapter = nullptr;
	UINT iAdapter = 0;
	IDXGIAdapter* pAdapterTemp = nullptr;
	SIZE_T  vramsize = 0;

	while (pGFactory->EnumAdapters(iAdapter, &pAdapterTemp) != DXGI_ERROR_NOT_FOUND)
	{
		// �A�_�v�^�[���
		DXGI_ADAPTER_DESC descAdapter;
		pAdapterTemp->GetDesc(&descAdapter);
		if (m_pAdapter == nullptr)
		{
			m_pAdapter = pAdapterTemp;
			vramsize = descAdapter.DedicatedVideoMemory / 1024 / 1024;
		}
		else if (descAdapter.DedicatedVideoMemory / 1024 / 1024 > vramsize) {	   // VRAM�̑傫�����̂�I��
			SAFE_RELEASE(m_pAdapter);
			m_pAdapter = pAdapterTemp;
			vramsize = descAdapter.DedicatedVideoMemory / 1024 / 1024;
		}
		else {
			SAFE_RELEASE(pAdapterTemp);
		}
		++iAdapter;
	}
	SAFE_RELEASE(pGFactory);							// -- 2024.11.2

	// �f�o�C�X�ƃX���b�v�`�F�[���̍쐬
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferDesc.Width  = m_dwWindowWidth;
	sd.BufferDesc.Height = m_dwWindowHeight;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.SampleDesc.Count = SAMPLE_COUNT;
	sd.SampleDesc.Quality = SAMPLE_QUALITY;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = m_hWnd;
	sd.Windowed = true;
	//sd.Flags = DXGI_SWAP_CHAIN_FLAG_GDI_COMPATIBLE;  //  -- 2017.1.28  GetDC�΍� �G���[�ɂȂ�H

	D3D_FEATURE_LEVEL pFeatureLevels = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL* pFeatureLevel = nullptr;

	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		//nullptr,
		m_pAdapter,								// -- 2024.11.2
		//D3D_DRIVER_TYPE_HARDWARE, 
		D3D_DRIVER_TYPE_UNKNOWN,				// -- 2024.11.2
		nullptr,
		0, 
		&pFeatureLevels, 
		1, 
		D3D11_SDK_VERSION, 
		&sd, 
		&m_pSwapChain, 
		&m_pDevice,
		pFeatureLevel, 
		&m_pDeviceContext);

	if( hr == E_FAIL){
		MessageBox(0, _T("Direct3D.cpp D3D�f�o�C�X�ƃX���b�v�`�F�[���̍쐬�Ɏ��s���܂���"), nullptr, MB_OK);
		return E_FAIL;
	}

	//�e��e�N�X�`���[�ƁA����ɕt�т���e��r���[���쐬

	//�o�b�N�o�b�t�@�[�e�N�X�`���[���擾�i���ɂ���̂ō쐬�ł͂Ȃ��j
	ID3D11Texture2D *pBackBuffer_Tex;
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer_Tex);
	//���̃e�N�X�`���[�ɑ΂������_�[�^�[�Q�b�g�r���[(RTV)���쐬
	m_pDevice->CreateRenderTargetView(pBackBuffer_Tex, nullptr, &m_pBackBuffer_TexRTV);
	SAFE_RELEASE(pBackBuffer_Tex);

	//�f�v�X�X�e���V���r���[�p�̃e�N�X�`���[���쐬
	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = m_dwWindowWidth;
	descDepth.Height = m_dwWindowHeight;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = SAMPLE_COUNT;
	descDepth.SampleDesc.Quality = SAMPLE_QUALITY;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;

	m_pDevice->CreateTexture2D(&descDepth, nullptr, &m_pBackBuffer_DSTex);
	//���̃e�N�X�`���[�ɑ΂��f�v�X�X�e���V���r���[(DSV)���쐬
	m_pDevice->CreateDepthStencilView(m_pBackBuffer_DSTex, nullptr, &m_pBackBuffer_DSTexDSV);

	//�����_�[�^�[�Q�b�g�r���[�Ɛ[�x�X�e���V���r���[���p�C�v���C���Ƀo�C���h
	SetRenderTarget(nullptr, nullptr);    // �����_�[�^�[�Q�b�g�̐ݒ�   // -- 2019.4.19

	//�r���[�|�[�g�̐ݒ�
	D3D11_VIEWPORT vp;
	vp.Width = (float)m_dwWindowWidth;
	vp.Height = (float)m_dwWindowHeight;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_pDeviceContext->RSSetViewports(1, &vp);

	//���X�^���C�Y�ݒ�  (�����̐ݒ�ŕ\���ƃ��C���[�t���[���\�������߂�)
	D3D11_RASTERIZER_DESC rdc;
	ZeroMemory(&rdc, sizeof(rdc));

	//rdc.CullMode = D3D11_CULL_NONE;	// ��ɂ��ׂĂ̎O�p�`��`��
	rdc.CullMode = D3D11_CULL_BACK;		// �������̎O�p�`��`�悵�܂���

	rdc.FrontCounterClockwise = FALSE;		// �O�p�`�̒��_�������_�[�^�[�Q�b�g��ŉE���Ȃ�ΎO�p�`�͑O�����ƌ��Ȃ����  	 // -- 2024.3.23

	rdc.FillMode = D3D11_FILL_SOLID;
	m_pDevice->CreateRasterizerState(&rdc, &m_pRStateR);			 	 // -- 2024.3.23

	rdc.FillMode = D3D11_FILL_WIREFRAME;	// ���C���[�t���[���\��
	m_pDevice->CreateRasterizerState(&rdc, &m_pRStateRW);			 	 // -- 2024.3.23

	m_pDeviceContext->RSSetState(m_pRStateR);  // �E���͑O�����Ɛݒ肷��	 	 // -- 2024.3.23

	// �e�N�X�`���[�T���v���[���쐬����
	InitSampler();

	// �u�����h�X�e�[�g���쐬����
	InitBlendState();

	// COM �I�u�W�F�N�g(CLSID_WICImagingFactory)�̍쐬
	hr = CoCreateInstance(
			CLSID_WICImagingFactory,
			nullptr,
			CLSCTX_INPROC_SERVER,
			IID_PPV_ARGS(&m_pFactory)
	);
	if (hr == E_FAIL) {
		MessageBox(0, _T("Direct3D.cpp  COM �I�u�W�F�N�g(CLSID_WICImagingFactory)�̍쐬�Ɏ��s���܂���"), nullptr, MB_OK);
		return E_FAIL;
	}

	// DirectXMath���C�u�������g���邩�ǂ����`�F�b�N����
	if (XMVerifyCPUSupport() != true)
	{
		MessageBox(0, _T("Direct3D.cpp  DirectXMath���C�u�����̂r�r�d���g���܂���B�������I���������܂�"), nullptr, MB_OK);
		return E_FAIL;
	}

	return S_OK;

}

//------------------------------------------------------------------------
//
//	�e�N�X�`���[�T���v���[���쐬����
//
//  �����@�Ȃ�
//
//	�߂�l HRESULT
//		S_OK	= ����
//		E_FAIL	= �ُ�
//
//------------------------------------------------------------------------
HRESULT CDirect3D::InitSampler()
{
	// ----------------------------------------------------------------------
	//�e�N�X�`���[�p�T���v���[�쐬
	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));

	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	//SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	//SamDesc.Filter = D3D11_FILTER_ANISOTROPIC;

	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	SamDesc.MipLODBias = 0;                            // �v�Z���ꂽ�~�b�v�}�b�v ���x������̃o�C�A�X
	//SamDesc.MaxAnisotropy = 16;                        // �T���v�����O�Ɉٕ�����Ԃ��g�p���Ă���ꍇ�̌��E�l�B�L���Ȓl�� 1 �` 16 �B
	SamDesc.MaxAnisotropy = 1;                         // �T���v�����O�Ɉٕ�����Ԃ��g�p���Ă���ꍇ�̌��E�l�B�L���Ȓl�� 1 �` 16 �B
	SamDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;  // ��r�I�v�V�����B
	SamDesc.MinLOD = 0;                                // �A�N�Z�X�\�ȃ~�b�v�}�b�v�̉����l
	SamDesc.MaxLOD = D3D11_FLOAT32_MAX;                // �A�N�Z�X�\�ȃ~�b�v�}�b�v�̏���l

	m_pDevice->CreateSamplerState(&SamDesc, &m_pSampleLinear);


	// ----------------------------------------------------------------------
	//�e�N�X�`���[�p�T���v���[�쐬 
	// (Border�@���b�v�������E�F�œh��Ԃ�)
	ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));

	//SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	//SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	SamDesc.Filter = D3D11_FILTER_ANISOTROPIC;

	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;

	static const VECTOR4 One = VECTOR4(1, 1, 1, 0);
	memcpy_s(SamDesc.BorderColor, sizeof(SamDesc.BorderColor), &One , sizeof(VECTOR4)); // Address�ɂ�_BORDER���w�肳��Ă���ꍇ�Ɏg�p���鋫�E�F (�͈�: 0.0~1.0)

	SamDesc.MipLODBias = 0;                            // �v�Z���ꂽ�~�b�v�}�b�v ���x������̃o�C�A�X
	SamDesc.MaxAnisotropy = 16;                        // �T���v�����O�Ɉٕ�����Ԃ��g�p���Ă���ꍇ�̌��E�l�B�L���Ȓl�� 1 �` 16 �B
	//SamDesc.MaxAnisotropy = 1;                         // �T���v�����O�Ɉٕ�����Ԃ��g�p���Ă���ꍇ�̌��E�l�B�L���Ȓl�� 1 �` 16 �B
	SamDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;  // ��r�I�v�V�����B
	SamDesc.MinLOD = 0;                                // �A�N�Z�X�\�ȃ~�b�v�}�b�v�̉����l
	SamDesc.MaxLOD = D3D11_FLOAT32_MAX;                // �A�N�Z�X�\�ȃ~�b�v�}�b�v�̏���l

	m_pDevice->CreateSamplerState(&SamDesc, &m_pSampleBorder);


	return S_OK;
}


//------------------------------------------------------------------------
//
//	�u�����h�X�e�[�g���쐬����
//
//  �����@�Ȃ�
//
//	�߂�l HRESULT
//		S_OK	= ����
//		E_FAIL	= �ُ�
//
//------------------------------------------------------------------------
HRESULT CDirect3D::InitBlendState()
{

	// -------------------------------------------------------------------------------
	// ���݂̃u�����h�X�e�[�g���擾����
	UINT mask = 0xffffffff;
	m_pDeviceContext->OMGetBlendState(&m_pBlendStateNormal, nullptr, &mask);


	//�A���t�@�u�����h�p�u�����h�X�e�[�g�쐬
	//png�t�@�C�����ɃA���t�@��񂪂���B�A���t�@�ɂ�蓧�߂���悤�w�肵�Ă���
	D3D11_BLEND_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BLEND_DESC));
	bd.IndependentBlendEnable = false;
	bd.AlphaToCoverageEnable = false;

	bd.RenderTarget[0].BlendEnable = true;
	bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	//bd.RenderTarget[0].DestBlend = D3D11_BLEND_DEST_COLOR;  // ������?
	bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	if (FAILED(m_pDevice->CreateBlendState(&bd, &m_pBlendStateTrapen)))
	{
		MessageBox(0, _T("Direct3D.cpp �A���t�@�u�����h�p�u�����h�X�e�[�g�쐬�ł��܂���"), _T(""), MB_OK);
		return E_FAIL;
	}

	// ���Z�����p�u�����h�X�e�[�g�쐬
	ZeroMemory(&bd, sizeof(D3D11_BLEND_DESC));
	bd.IndependentBlendEnable = false;
	bd.AlphaToCoverageEnable = false;

	bd.RenderTarget[0].BlendEnable = true;
	bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	bd.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	//bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	if (FAILED(m_pDevice->CreateBlendState(&bd, &m_pBlendStateAdd)))
	{
		MessageBox(0, _T("Direct3D.cpp ���Z�����p�u�����h�X�e�[�g�쐬�ł��܂���"), _T(""), MB_OK);
		return E_FAIL;
	}

	// �f�t�H���g�̃u�����h�X�e�[�g���A���t�@�u�����h�p�u�����h�X�e�[�g�ɂ���            // -- 2020.1.15
	// (�����F�̃u�����f�B���O��ݒ�)
	m_pDeviceContext->OMSetBlendState(m_pBlendStateTrapen, nullptr, mask);

	return S_OK;
}


//------------------------------------------------------------------------ // -- 2019.4.19
//
//	�����_�[�^�[�Q�b�g��ݒ肷��	
//	(�ݒ肵�������_�[�^�[�Q�b�g�e�N�X�`�����́A���ɐݒ肷��܂ŋL�^����Ă���)
//
//	����
//		ID3D11RenderTargetView* m_pTarget_TexRTV;
//		ID3D11DepthStencilView* m_pTarget_DSTexDSV;
//
//	�߂�l
//		�Ȃ�
//------------------------------------------------------------------------
void   CDirect3D::SetRenderTarget(ID3D11RenderTargetView* pTexRTV, ID3D11DepthStencilView* pDSTexDSV)
{
	if (pTexRTV == nullptr)
	{
		// nullptr�̎��́A�o�b�N�o�b�t�@
		m_pTarget_TexRTV = m_pBackBuffer_TexRTV;
		m_pTarget_DSTexDSV = m_pBackBuffer_DSTexDSV;
	}
	else {
		m_pTarget_TexRTV = pTexRTV;
		m_pTarget_DSTexDSV = pDSTexDSV;
	}
	m_pDeviceContext->OMSetRenderTargets(1, &m_pTarget_TexRTV, m_pTarget_DSTexDSV);
}

//------------------------------------------------------------------------ // -- 2019.4.19
//
//	�����_�[�^�[�Q�b�g���N�����[�F�ŃN�����[����	
//	(�ݒ肵�������_�[�^�[�Q�b�g�e�N�X�`�����ŃN�����[)
//	(�y�o�b�t�@������Ƃ��͂y�o�b�t�@���N�����[)
//
//	����
//		float ClearColor[4]:�N�����[�F
//
//	�߂�l
//		�Ȃ�
//------------------------------------------------------------------------
void   CDirect3D::ClearRenderTarget(float ClearColor[])
{
	m_pDeviceContext->ClearRenderTargetView(m_pTarget_TexRTV, ClearColor); // ��ʃN���A
	if (m_pTarget_DSTexDSV)
	{
		m_pDeviceContext->ClearDepthStencilView(m_pTarget_DSTexDSV, D3D11_CLEAR_DEPTH, 1.0f, 0); // �[�x�o�b�t�@�N���A
	}
}
//------------------------------------------------------------------------ // -- 2019.4.19
//
//	�����_�[�^�[�Q�b�g�̂y�o�b�t�@��ݒ肷��	
//	(�ݒ肵�������_�[�^�[�Q�b�g�e�N�X�`�����ły�o�b�t�@���n�m�^�n�e�e����)
//
//	����
//		bool bZbuf;    true:�y�o�b�t�@�n�m  false:�y�o�b�t�@�n�e�e
//
//	�߂�l
//		�Ȃ�
//------------------------------------------------------------------------
void   CDirect3D::SetZBuffer(bool bZBuf)
{
	// �����_�[�^�[�Q�b�g�̂y�o�b�t�@�@�n�m�^�n�e�e
	if (bZBuf)
	{
		m_pDeviceContext->OMSetRenderTargets(1, &m_pTarget_TexRTV, m_pTarget_DSTexDSV);
	}
	else {
		m_pDeviceContext->OMSetRenderTargets(1, &m_pTarget_TexRTV, nullptr);
	}
}



//------------------------------------------------------------------------
//
//	�C���[�W�t�@�C����ǂݍ���Ńe�N�X�`�����쐬����֐�
//
//	�w�肵���t�@�C�����̃C���[�W��ǂݍ���
//	�e�N�X�`�����쐬����
//
//  const TCHAR*		TName				�C���[�W�t�@�C����
//  ID3D11Texture2D**	ppTexture2D(OUT)	�쐬���ꂽ�e�N�X�`��
//  DWORD&				dwImageWidth(OUT)	�C���[�W�̕�(OUT)
//  DWORD&				dwImageHeight(OUT)	�C���[�W�̍���(OUT)
//  UINT				MipLevels			�~�b�v�}�b�v���x��(�ȗ��l�͂P)
//
//	�߂�l HRESULT
//		S_OK	= ����
//		E_FAIL	= �t�@�C������������Ȃ�
//------------------------------------------------------------------------
HRESULT CDirect3D::CreateTextureFromFile(const TCHAR* TName, ID3D11Texture2D** ppTexture2D, DWORD& dwImageWidth, DWORD& dwImageHeight, UINT MipLevels)
{
	// ---------------------------------------------------------------------------------------
	//
	// Windows Imaging Component ���g�����摜�t�@�C���̓ǂݏ���
	// 
	//     Windows Imaging Component �� Windows SDK �Ɋ܂܂�Ă���A
	//     �Î~��⓮���ǂݏ������邽�߂� API ���񋟂���Ă��܂��B
	//     ��Ǝ菇�͉��L�̒ʂ�ł��B
	//       1.�摜�t�@�C�������f�C���[�W���擾����B
	//       2.�摜�t�H�[�}�b�g�̃R���o�[�g
	//       3.�摜�f�[�^����e�N�X�`���𐶐�����
	//
	// ---------------------------------------------------------------------------------------

	// ---------------------------------------------------------------------------
	//
	//     1.�摜�t�@�C�������f�C���[�W���擾����B
	//     		�@�@IWICImaginFactory �I�u�W�F�N�g����̃f�R�[�_�̐����i�摜�t�@�C���̓ǂݍ��݁j
	//     		�A�@�f�R�[�_����̃t���[���̐���
	//     		�B�@�t���[������̉摜�T�C�Y�̎擾
	//     		�C�@�t���[������摜�t�H�[�}�b�g�̎擾
	//
	// ---------------------------------------------------------------------------

	IWICBitmapDecoder* decoder = nullptr;

	// �摜�t�@�C����ǂݍ���Ńf�R�[�_���쐬����

#if _UNICODE
	// unicode�̏ꍇ
	if (FAILED(m_pFactory->CreateDecoderFromFilename(TName, 0,
				GENERIC_READ, WICDecodeMetadataCacheOnDemand, &decoder)))
	{
		//MessageBox(0, _T("Direct3D.cpp �X�v���C�g�@�e�N�X�`���[��ǂݍ��߂܂���"), TName, MB_OK);
		return E_FAIL;
	}
#else
	// �}���`�o�C�g�����̏ꍇ�A
	// �������CHAR�^��WCHAR�^�ɕϊ�
	WCHAR WName[512] = { 0x00 };   // �ϊ���ɕ�����Ō��\0���쐬����Ȃ��̂ł܂���摤���[���N�����[���Ă���
	MultiByteToWideChar(CP_OEMCP, MB_PRECOMPOSED, TName, (int)strlen(TName), WName, (sizeof(WName)) / 2);
	if (FAILED(m_pFactory->CreateDecoderFromFilename(WName, 0,
				GENERIC_READ, WICDecodeMetadataCacheOnDemand, &decoder)))
	{
		//MessageBox(0, _T("Direct3D.cpp �X�v���C�g�@�e�N�X�`���[��ǂݍ��߂܂���"), TName, MB_OK);
		return E_FAIL;
	}
#endif

	// �f�R�[�_����t���[�����쐬����
	IWICBitmapFrameDecode* frame = nullptr;
	if (FAILED(decoder->GetFrame(0, &frame)))
	{
		MessageBox(0, _T("Direct3D.cpp decoder����frame���擾�ł��܂���"), TName, MB_OK);
		return E_FAIL;
	}

	// �t���[������摜�T�C�Y�̎擾
	UINT width, height;
	if (FAILED(frame->GetSize(&width, &height)))
	{
		MessageBox(0, _T("Direct3D.cpp �t���[������T�C�Y���擾�ł��܂���"), TName, MB_OK);
		return E_FAIL;

	}

	dwImageWidth = width;
	dwImageHeight = height;

	// �t���[������摜�t�H�[�}�b�g�̎擾
	WICPixelFormatGUID pixelFormat;
	if (FAILED(frame->GetPixelFormat(&pixelFormat)))
	{
		MessageBox(0, _T("Direct3D.cpp �t���[������s�N�Z���t�H�[�}�b�g���擾�ł��܂���"), TName, MB_OK);
		return E_FAIL;

	}

	// ---------------------------------------------------------------------------
	//
	//     2.�摜�t�H�[�}�b�g�̃R���o�[�g
	//     		�@�@�摜�t�H�[�}�b�g��RGBA�i32�r�b�g�j�̌`���ł��邩�ǂ������ׂ�
	//     		�A�@RGBA�i32�r�b�g�j�`���łȂ��ꍇ�̓t�H�[�}�b�g��RGBA�`���ɃR���o�[�g����
	//     		�B�@�f�R�[�_�i�R���o�[�g��j����摜�f�[�^���R�s�[����
	//
	// ---------------------------------------------------------------------------

	// �쐬����C���[�W�f�[�^�iRGBA�i32�r�b�g�j�`���j
	UINT  ImageStride = dwImageWidth * 4;					// �C���[�W�̕�(�o�C�g��)�@(��)1�s�N�Z����32�r�b�g(4�o�C�g)
	UINT  ImageSize = dwImageWidth * dwImageHeight * 4;		// �C���[�W�̑傫��(�o�C�g��)
	BYTE* pImageData = new BYTE[ImageSize];					// �C���[�W���i�[����o�b�t�@

	// RGBA�i32�r�b�g�j�̌`���łȂ�������ARGBA�`���Ƀt�H�[�}�b�g�R���o�[�g����
	if (pixelFormat != GUID_WICPixelFormat32bppRGBA)  // �摜�t�H�[�}�b�g��RGBA�i32�r�b�g�j��
	{
		// RGBA�i32�r�b�g�j�̌`���łȂ��̂ŁARGBA�`���Ƀt�H�[�}�b�g�R���o�[�g����
		IWICFormatConverter* FC = nullptr;

		if (FAILED(m_pFactory->CreateFormatConverter(&FC)))   // �t�H�[�}�b�g�R���o�[�^�̐���
		{
			MessageBox(0, _T("Direct3D.cpp �t�H�[�}�b�g�R���o�[�^�쐬�ł��܂���"), TName, MB_OK);
			return E_FAIL;
		}

		if (FAILED(FC->Initialize(frame, GUID_WICPixelFormat32bppRGBA      // �t�H�[�}�b�g�R���o�[�g����
			, WICBitmapDitherTypeErrorDiffusion
			, 0, 0, WICBitmapPaletteTypeCustom)))
		{
			MessageBox(0, _T("Direct3D.cpp �t�H�[�}�b�g�R���o�[�gInitialize�ł��܂���"), TName, MB_OK);
			return E_FAIL;
		}
		FC->CopyPixels(0, ImageStride, ImageSize, pImageData);   // �R���o�[�g��̉摜�f�[�^��pImageData�ɃR�s�[����

		SAFE_RELEASE(FC);

	}
	else {
		frame->CopyPixels(0, ImageStride, ImageSize, pImageData); // frame�̉摜�f�[�^��pImageData�ɃR�s�[����
	}

	// ---------------------------------------------------------------------------------------
	//
	//     3.�摜�f�[�^����e�N�X�`���𐶐�����
	//
	// ---------------------------------------------------------------------------------------

	// �T�u���\�[�X�f�[�^�[���쐬����
	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = pImageData;
	initData.SysMemPitch = ImageStride;
	initData.SysMemSlicePitch = ImageSize;

	// �~�b�v�}�b�v�e�N�X�`�����쐬���邩
	if (MipLevels == 1)
	{
		// �~�b�v�}�b�v�e�N�X�`�����쐬���Ȃ�
		if (FAILED(MakeNoMipmapTexture(dwImageWidth, dwImageHeight, initData, ppTexture2D)))
		{
			MessageBox(0, _T("Direct3D.cpp �~�b�v�}�b�v�����@�e�N�X�`���𐶐��ł��܂���"), TName, MB_OK);
			return E_FAIL;
		}
	}
	else {
		// �~�b�v�}�b�v�e�N�X�`�����쐬����
		if (FAILED(MakeMipmapTexture(dwImageWidth, dwImageHeight, initData, MipLevels, ppTexture2D)))
		{
			MessageBox(0, _T("Direct3D.cpp �~�b�v�}�b�v�@�e�N�X�`���𐶐��ł��܂���"), TName, MB_OK);
			return E_FAIL;
		}
	}


	//
	// �ꎞ�t�@�C���̉��
	//
	SAFE_RELEASE(frame);
	SAFE_RELEASE(decoder);
	SAFE_DELETE_ARRAY(pImageData);

	return S_OK;

}


//------------------------------------------------------------------------
//
//	�T�u���\�[�X�f�[�^�[����e�N�X�`�����쐬����֐�
//	�i�~�b�v�}�b�v�����j
//
//  const DWORD&				dwImageWidth	�C���[�W�̕�
//  const DWORD&				dwImageHeight	�C���[�W�̍���
//  D3D11_SUBRESOURCE_DATA		initData		�T�u���\�[�X�f�[�^�[
//  ID3D11Texture2D**			ppTexture2D(OUT)	�쐬���ꂽ�e�N�X�`��
//
//	�߂�l HRESULT
//		S_OK	= ����
//		E_FAIL	= �t�@�C������������Ȃ�
//------------------------------------------------------------------------
HRESULT CDirect3D::MakeNoMipmapTexture( const DWORD& dwImageWidth, const DWORD& dwImageHeight, const D3D11_SUBRESOURCE_DATA& initData, ID3D11Texture2D**	ppTexture2D)
{

	// �e�N�X�`���̍쐬
	D3D11_TEXTURE2D_DESC desc;
	desc.Width = dwImageWidth;
	desc.Height = dwImageHeight;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	if (FAILED(m_pDevice->CreateTexture2D(&desc, &initData, ppTexture2D)))
	{
		//MessageBox(0, _T("Direct3D.cpp �~�b�v�}�b�v�����@�e�N�X�`���𐶐��ł��܂���"), nullptr, MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

//------------------------------------------------------------------------
//
//	�T�u���\�[�X�f�[�^�[����e�N�X�`�����쐬����֐�
//	�i�~�b�v�}�b�v�L��j
//
//  const DWORD&				dwImageWidth	�C���[�W�̕�
//  const DWORD&				dwImageHeight	�C���[�W�̍���
//  const D3D11_SUBRESOURCE_DATA& initData		�T�u���\�[�X�f�[�^�[
//  const UINT&					MipLevels		�~�b�v�}�b�v���x��
//  ID3D11Texture2D**			ppTexture2D(OUT)	�쐬���ꂽ�e�N�X�`��
//
//	�߂�l HRESULT
//		S_OK	= ����
//		E_FAIL	= �t�@�C������������Ȃ�
//------------------------------------------------------------------------
HRESULT CDirect3D::MakeMipmapTexture(const DWORD& dwImageWidth, const DWORD& dwImageHeight, const D3D11_SUBRESOURCE_DATA& initData, const UINT& MipLevels, ID3D11Texture2D**	ppTexture2D)
{

	// �摜�f�[�^����ꎞ�e�N�X�`���̍쐬
	ID3D11Texture2D* pWTexture2D;

	D3D11_TEXTURE2D_DESC desc;
	desc.Width = dwImageWidth;
	desc.Height = dwImageHeight;
	desc.MipLevels = 1;           // �����ł̓~�b�v�}�b�v�͂P�ɂ���
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_STAGING;             // GPU ���� CPU �ւ̃f�[�^�]�� (�R�s�[) ���T�|�[�g
	desc.BindFlags = 0;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;  // ���\�[�X���}�b�v�\�ɂ��ACPU �����̃��\�[�X�̓��e��ǂݎ���悤�ɂ���
	desc.MiscFlags = 0;

	if (FAILED(m_pDevice->CreateTexture2D(&desc, &initData, &pWTexture2D)))
	{
		//MessageBox(0, _T("Direct3D.cpp �~�b�v�}�b�v�쐬�p�ꎞ�e�N�X�`���𐶐��ł��܂���"), nullptr, MB_OK);
		return E_FAIL;
	}

	// ��e�N�X�`���[�쐬
	desc.Width = dwImageWidth;
	desc.Height = dwImageHeight;
	desc.MipLevels = MipLevels;            // �~�b�v�}�b�v���x���̎w��
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_RENDER_TARGET |           // �`��^�[�Q�b�g�i�~�b�v�}�b�v�̐���������Ƃ��͕K�{�j
						D3D11_BIND_SHADER_RESOURCE;       // �V�F�[�_�Ŏg��
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;   // ��Ń~�b�v�}�b�v�̐������ł���悤�ɂ���

	if (FAILED(m_pDevice->CreateTexture2D(&desc, nullptr, ppTexture2D)))
	{
		//MessageBox(0, _T("Direct3D.cpp ��e�N�X�`���𐶐��ł��܂���"), nullptr, MB_OK);
		return E_FAIL;
	}

	D3D11_MAPPED_SUBRESOURCE mappedResource;

	// �摜�f�[�^����쐬�����e�N�X�`���[���A�ǂݍ��݃��[�h�Ń}�b�v����
	if (FAILED(m_pDeviceContext->Map(pWTexture2D, NULL, D3D11_MAP_READ, 0, &mappedResource)))
	{
		//MessageBox(0, _T("Direct3D.cpp Map�����ł��܂���"), nullptr, MB_OK);
		return E_FAIL;
	}

	// ��e�N�X�`���[�̃T�u���\�[�X���摜�f�[�^����쐬�����e�N�X�`���[�̓��e�ōX�V����
	// �{���́@for (UINT iMip = 0; iMip < desc.MipLevels; iMip++)�@�܂ōX�V���ׂ�����
	// ���f�[�^�Ƀ~�b�v�}�b�v���Ȃ����߁A�P���x���̂ݍX�V����
	for (UINT iMip = 0; iMip < 1; iMip++)
	{
		m_pDeviceContext->UpdateSubresource(*ppTexture2D,
										D3D11CalcSubresource(iMip, 0, desc.MipLevels),
										nullptr,
										mappedResource.pData,
										mappedResource.RowPitch,
										0
		);
	}

	m_pDeviceContext->Unmap(pWTexture2D, 0);

	SAFE_RELEASE(pWTexture2D);

	return S_OK;

}
//------------------------------------------------------------------------
//
//	�C���[�W�t�@�C����ǂݍ���ŃV�F�[�_�[���\�[�X�r���[���쐬����֐�
//
//	�w�肵���t�@�C�����̃C���[�W��ǂݍ��݃e�N�X�`�����쐬����
//	(CreateTextureFromFile���\�b�h���g�p����)
//	�e�N�X�`������V�F�[�_�[���\�[�X�r���[���쐬����
//
//  const TCHAR*		TName					�C���[�W�t�@�C����
//  ID3D11ShaderResourceView** ppTexture(OUT)	�쐬���ꂽ�V�F�[�_�[���\�[�X�r���[
//  DWORD&				dwImageWidth(OUT)		�C���[�W�̕�(OUT)
//  DWORD&				dwImageHeight(OUT)		�C���[�W�̍���(OUT)
//  UINT				MipLevels				�~�b�v�}�b�v���x��(�ȗ��l�͂P)
//
//	�߂�l HRESULT
//		S_OK	= ����
//		E_FAIL	= �t�@�C������������Ȃ�
//------------------------------------------------------------------------
HRESULT CDirect3D::CreateShaderResourceViewFromFile(const TCHAR* TName, ID3D11ShaderResourceView** ppTexture, DWORD& dwImageWidth, DWORD& dwImageHeight, UINT MipLevels)
{
	// ---------------------------------------------------------------------------------------
	//
	//     �摜�f�[�^����e�N�X�`���𐶐�����
	//     		�@�@�摜�f�[�^����e�N�X�`���𐶐�
	//     		�A�@�e�N�X�`������V�F�[�_���\�[�X�r���[�𐶐�
	//
	// ---------------------------------------------------------------------------------------

	ID3D11Texture2D*	pTexture2D = nullptr;

	// �~�b�v�}�b�v���x���𒲐�����
	UINT  MaxMipLevels = 8;
	if (MipLevels < 1 || MipLevels > MaxMipLevels)
	{
		MipLevels = MaxMipLevels;
	}

	//  �摜�f�[�^����e�N�X�`���𐶐�����
	if (FAILED(CreateTextureFromFile(TName, &pTexture2D, dwImageWidth, dwImageHeight, MipLevels)))
	{
		return E_FAIL;
	}

	// �e�N�X�`������V�F�[�_�[���\�[�X�r���[m_pTexture�̍쐬
	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
	SRVDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D.MipLevels = MipLevels;

	if (FAILED(m_pDevice->CreateShaderResourceView(pTexture2D, &SRVDesc, ppTexture)))
	{
		MessageBox(0, _T("Direct3D.cpp �e�N�X�`��ShaderResourceView���쐬�ł��܂���"), TName, MB_OK);
		return E_FAIL;
	}

	// �~�b�v�}�b�v�𐶐�(�~�b�v�}�b�v���x�����Q�ȏ�̏ꍇ�̂ݕK�v)
	// �~�b�v�}�b�v���Q�ȏ�̏ꍇ�ɁA�e�N�X�`����ɉ摜�f�[�^���P���x����
	// �����Ȃ����߁A�����őS�Ẵ~�b�v�}�b�v�𐶐�����
	if (MipLevels > 1) m_pDeviceContext->GenerateMips(*ppTexture);

	//
	// �ꎞ�t�@�C���̉��
	//
	SAFE_RELEASE(pTexture2D);

	return S_OK;
}

//------------------------------------------------------------------------
//
//	�C���[�W�t�@�C����ǂݍ���ŃV�F�[�_�[���\�[�X�r���[���쐬����֐�
//
//	�w�肵���t�@�C�����̃C���[�W��ǂݍ��݃e�N�X�`�����쐬����
//	(CreateTextureFromFile���\�b�h���g�p����)
//	�e�N�X�`������V�F�[�_�[���\�[�X�r���[���쐬����
//
//  const TCHAR*		TName					�C���[�W�t�@�C����
//  ID3D11ShaderResourceView** ppTexture(OUT)	�쐬���ꂽ�V�F�[�_�[���\�[�X�r���[
//  UINT				MipLevels				�~�b�v�}�b�v���x��(�ȗ��l�͂P)
//
//	�߂�l HRESULT
//		S_OK	= ����
//		E_FAIL	= �t�@�C������������Ȃ�
//------------------------------------------------------------------------
HRESULT CDirect3D::CreateShaderResourceViewFromFile(const TCHAR* TName, ID3D11ShaderResourceView** ppTexture, UINT MipLevels)
{
	DWORD dwImageWidth;
	DWORD dwImageHeight;

	return CreateShaderResourceViewFromFile(TName, ppTexture, dwImageWidth, dwImageHeight, MipLevels);
}

//------------------------------------------------------------------------ // -- 2022.9.14
//
//	�R���p�C���ς݃V�F�[�_�[�̓ǂݍ��݂�����֐�
//
//  x64�ɑΉ����Ă���
//
// ����
//      const TCHAR* csoName       �b�r�n�t�@�C����
//      BYTE** ppByteArray         �b�r�n���󂯎��BYTE�z��̐擪�A�h���X(OUT)
//      DWORD* pCsoSize            �b�r�n�̃T�C�Y�A�h���X(OUT)
//
//	�߂�l HRESULT
//		S_OK	= ����
//		E_FAIL	= �ُ�
//
//------------------------------------------------------------------------
HRESULT CDirect3D::ReadCso(const TCHAR* csoName, BYTE** ppByteArray, DWORD* pCsoSize)
{
	FILE* fp;

#ifdef _M_AMD64
	// x64�̏ꍇ
#ifdef _DEBUG
	TCHAR FName[256] = _T("x64/Debug/");
#else
	TCHAR FName[256] = _T("x64/Release/");
#endif
#else
	// x86�̏ꍇ
#ifdef _DEBUG
	TCHAR FName[256] = _T("Debug/");
#else
	TCHAR FName[256] = _T("Release/");
#endif
#endif

#if _UNICODE
	// unicode�̏ꍇ
	lstrcat(FName, csoName);
	int ret = _wfopen_s(&fp, FName, L"rb");
#else
	// �}���`�o�C�g�����̏ꍇ�A
	strcat_s(FName, csoName);
	int ret = fopen_s(&fp, FName, "rb");
#endif

	if (ret != 0) {
		MessageBox(0, FName, _T("Direct3D.cpp CSO�t�@�C�� �ǂݍ��ݎ��s"), MB_OK);
		return E_FAIL;
	}
	fseek(fp, 0, SEEK_END);
	*pCsoSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	*ppByteArray = new BYTE[*pCsoSize];
	fread(*ppByteArray, *pCsoSize, 1, fp);

	fclose(fp);

	return S_OK;

}

//------------------------------------------------------------------------
//
//	�e��I�u�W�F�N�g�������[�X	
//
//  �����@�Ȃ�
//
//	�߂�l �Ȃ�
//
//------------------------------------------------------------------------
void CDirect3D::DestroyD3D()
{
	SAFE_RELEASE(m_pSwapChain);
	SAFE_RELEASE(m_pBackBuffer_TexRTV);
	SAFE_RELEASE(m_pBackBuffer_DSTexDSV);
	SAFE_RELEASE(m_pBackBuffer_DSTex);

	SAFE_RELEASE(m_pSampleLinear);
	SAFE_RELEASE(m_pSampleBorder);

	SAFE_RELEASE(m_pBlendStateNormal);
	SAFE_RELEASE(m_pBlendStateTrapen);
	SAFE_RELEASE(m_pBlendStateAdd);

	SAFE_RELEASE(m_pRStateR);	   	 // -- 2024.3.23
	SAFE_RELEASE(m_pRStateRW);	   	 // -- 2024.3.23

	SAFE_RELEASE(m_pFactory);      // -- 2018.12.26

	SAFE_RELEASE(m_pDevice);

	SAFE_RELEASE(m_pAdapter);		// -- 2024.11.2
}

