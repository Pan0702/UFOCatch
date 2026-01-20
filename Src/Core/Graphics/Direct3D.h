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
//																Direct3D.h
//
//-----------------------------------------------------------------------------

#pragma once

//�w�b�_�[�t�@�C���̃C���N���[�h
#include <stdio.h>
#include <windows.h>
#include <d3d11.h>
#include <d3dCompiler.h>
#include <d2d1.h>
#include <wincodec.h>

#include "../Game/Macro.h"
#include "../../Utils/MyMath.h"

//�K�v�ȃ��C�u�����t�@�C���̃��[�h
#pragma comment(lib,"winmm.lib")
//#pragma comment(lib, "d2d1.lib")
//#pragma comment(lib,"windowscodecs.lib")
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dCompiler.lib")
#pragma comment(lib,"dxgi.lib")				   		// -- 2024.11.2

//�}�N��
#define ALIGN16 _declspec(align(16))


// �A���`�G�C���A�X ------------ // -- 2018.8.3 ---------------------------
// �A���`�G�C���A�X�������Ȃ��Ƃ�
#define SAMPLE_COUNT    1
#define SAMPLE_QUALITY  0

// �A���`�G�C���A�X��������Ƃ�
//#define SAMPLE_COUNT    4
//#define SAMPLE_QUALITY  0.5


// Direct3D�N���X
class CDirect3D
{
public:
	// �����o�ϐ�
	HWND  m_hWnd;
	DWORD m_dwWindowWidth;
	DWORD m_dwWindowHeight;

	// Dx11
	ID3D11Device*           m_pDevice;
	ID3D11DeviceContext*    m_pDeviceContext;
	IDXGISwapChain*         m_pSwapChain;
	ID3D11RenderTargetView* m_pBackBuffer_TexRTV;
	ID3D11DepthStencilView* m_pBackBuffer_DSTexDSV;
	ID3D11Texture2D*        m_pBackBuffer_DSTex;
	IDXGIAdapter*           m_pAdapter;		   // -- 2024.11.2

	// �u�����h�X�e�[�g
	ID3D11BlendState*         m_pBlendStateNormal;
	ID3D11BlendState*         m_pBlendStateTrapen;
	ID3D11BlendState*         m_pBlendStateAdd;

	//�e�N�X�`���[�̃T���v���[
	ID3D11SamplerState*       m_pSampleLinear;
	ID3D11SamplerState*       m_pSampleBorder;

	// COM �I�u�W�F�N�g(CLSID_WICImagingFactory)
	IWICImagingFactory*       m_pFactory;    // -- 2018.12.26

	// ���X�^���C�U�X�e�[�g
	ID3D11RasterizerState* m_pRStateR;  // �E����\�ʂƂ���							 // -- 2024.3.23
	ID3D11RasterizerState* m_pRStateRW;  // �E����\�ʂƂ���B���C���[�t���[���\��		 // -- 2024.3.23

	ID3D11RenderTargetView*   m_pTarget_TexRTV;   // ���݂̃����_�[�^�[�Q�b�g   // -- 2019.4.19
	ID3D11DepthStencilView*   m_pTarget_DSTexDSV; // ���݂̃����_�[�^�[�Q�b�g   // -- 2019.4.19

	// ---------------------------------------------------------------
	// ���\�b�h
	CDirect3D();
	~CDirect3D();
	HRESULT InitD3D(HWND, DWORD, DWORD );
	HRESULT InitBlendState();
	HRESULT InitSampler();
	HRESULT ReadCso(const TCHAR* csoName, BYTE** ppByteArray, DWORD* pCsoSize);
	HRESULT CreateTextureFromFile(const TCHAR* TName, ID3D11Texture2D** ppTexture2D, DWORD& dwImageWidth, DWORD& dwImageHeight, UINT MipLevels = 1);
	HRESULT MakeNoMipmapTexture(const DWORD& dwImageWidth, const DWORD& dwImageHeight, const D3D11_SUBRESOURCE_DATA& initData, ID3D11Texture2D**	ppTexture2D);
	HRESULT MakeMipmapTexture(const DWORD& dwImageWidth, const DWORD& dwImageHeight, const D3D11_SUBRESOURCE_DATA& initData, const UINT& MipLevels, ID3D11Texture2D**	ppTexture2D);
	HRESULT CreateShaderResourceViewFromFile(const TCHAR* TName, ID3D11ShaderResourceView** ppTexture, DWORD& dwImageWidth, DWORD& dwImageHeight, UINT MipLevels = 1);
	HRESULT CreateShaderResourceViewFromFile(const TCHAR* TName, ID3D11ShaderResourceView** ppTexture, UINT MipLevels = 1);

	void SetRenderTarget(ID3D11RenderTargetView* pTexRTV, ID3D11DepthStencilView* pDSTexDSV);  // -- 2019.4.19
	void ClearRenderTarget(float ClearColor[]);                                                // -- 2019.4.19
	void SetZBuffer(bool bZBuf);                                                               // -- 2019.4.19

	// --------------------------------------------------
	void DestroyD3D();

};
