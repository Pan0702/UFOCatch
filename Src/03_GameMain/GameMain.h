//=============================================================================
//		�R�c�Q�[���v���O�����@�@�@�@�@                 ver 3.2        2023.1.31
//
//		�Q�[���̃��C������
//																GameMain.h
//=============================================================================

#pragma once

//�w�b�_�[�t�@�C���̃C���N���[�h
#include <stdio.h>
#include <windows.h>
#include <vector>

#include "../01_WinMain/Main.h"
#include "../02_DirectX/Direct3D.h"
#include "../04_FrameWork/Shader.h"
#include "../06_GameLib/FbxMesh.h""
#include "../02_DirectX/XAudio.h"
#include "../02_DirectX/DInput.h"
#include "../06_GameLib/Sprite3D.h"
#include "../06_GameLib/MyImGui.h""
#include "Macro.h"

#include "../01_WinMain/MainControl.h"

// --------------------------------------------
class CMain;
class CDirect3D;
class CXAudio;
class CDirectInput;
class CFontTexture;
class CShader;
class CFbxMeshCtrl;

// --------------------------------------------------------------------------------------------------
//  �Q�[�����C���N���X
// --------------------------------------------------------------------------------------------------
class CGameMain
{
public:
	// �����o�ϐ�
	CMain*			m_pMain;		// ���C���N���X
	CDirect3D*		m_pD3D;			//	Direct3D�I�u�W�F�N�g
	CXAudio*		m_pXAudio;		//	XAudio�I�u�W�F�N�g
	CDirectInput*	m_pDI;			//	DirectInputD�I�u�W�F�N�g
	CFontTexture*	m_pFont;		//	�t�H���g�e�N�X�`���[
	CShader*		m_pShader;		//	�V�F�[�_�[
	CFbxMeshCtrl*	m_pFbxMeshCtrl;	// ���b�V���R���g���[���N���X    // -- 2021.2.4
	
	// �J�����E���C�g�E�r���[
	VECTOR3         m_vEyePt;		//�J�����i���_�j�ʒu
	VECTOR3         m_vLookatPt;	//�����ʒu
	MATRIX4X4       m_mView;
	MATRIX4X4       m_mProj;
	VECTOR3         m_vLightDir;	//�f�B���N�V���i�����C�g�̕���
	VECTOR4         m_vLightIntensity;	//���C�g�̋��x(RGB) + Alpha
public:
	// ���\�b�h
	CGameMain(CMain* pMain);
	~CGameMain();

	HRESULT Init();
	void    Update();
	void	Draw();
	void    Quit();

	void    GameMain();
	HRESULT ChangeScreenMode(int nMode=-1);    // -- 2020.1.15

	void SetWindowName(const char* name);

	// ���C�g�̋��x��ݒ�/�擾
	void    SetLightIntensity(float r, float g, float b, float a = 1.0f);
	void    SetLightIntensity(VECTOR4 intensity);
	VECTOR4 GetLightIntensity() const { return m_vLightIntensity; }
};

CGameMain* GameDevice();
