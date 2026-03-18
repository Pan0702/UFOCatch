//=============================================================================
//		・ｽR・ｽc・ｽQ・ｽ[・ｽ・ｽ・ｽv・ｽ・ｽ・ｽO・ｽ・ｽ・ｽ・ｽ・ｽ@・ｽ@・ｽ@・ｽ@・ｽ@                 ver 3.2        2023.1.31
//
//		・ｽQ・ｽ[・ｽ・ｽ・ｽﾌ・ｿｽ・ｽC・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ
//																GameMain.h
//=============================================================================

#pragma once

//・ｽw・ｽb・ｽ_・ｽ[・ｽt・ｽ@・ｽC・ｽ・ｽ・ｽﾌイ・ｽ・ｽ・ｽN・ｽ・ｽ・ｽ[・ｽh
#include <stdio.h>
#include <windows.h>
#include <vector>

#include "../Platform/Main.h"
#include "../Graphics/Direct3D.h"
#include "../../Framework/Shader.h"
#include "../../Utils/FbxMesh.h"
#include "../Graphics/XAudio.h"
#include "../Graphics/DInput.h"
#include "../../Utils/Sprite3D.h"
#include "../../Utils/MyImGui.h"
#include "Macro.h"

#include "../Platform/MainControl.h"

// --------------------------------------------
class CMain;
class CDirect3D;
class CXAudio;
class CDirectInput;
class CFontTexture;
class CShader;
class CFbxMeshCtrl;

// --------------------------------------------------------------------------------------------------
//  ・ｽQ・ｽ[・ｽ・ｽ・ｽ・ｽ・ｽC・ｽ・ｽ・ｽN・ｽ・ｽ・ｽX
// --------------------------------------------------------------------------------------------------
class CGameMain
{
public:
	// ・ｽ・ｽ・ｽ・ｽ・ｽo・ｽﾏ撰ｿｽ
	CMain*			m_pMain;		// ・ｽ・ｽ・ｽC・ｽ・ｽ・ｽN・ｽ・ｽ・ｽX
	CDirect3D*		m_pD3D;			//	Direct3D・ｽI・ｽu・ｽW・ｽF・ｽN・ｽg
	CXAudio*		m_pXAudio;		//	XAudio・ｽI・ｽu・ｽW・ｽF・ｽN・ｽg
	CDirectInput*	m_pDI;			//	DirectInputD・ｽI・ｽu・ｽW・ｽF・ｽN・ｽg
	CFontTexture*	m_pFont;		//	・ｽt・ｽH・ｽ・ｽ・ｽg・ｽe・ｽN・ｽX・ｽ`・ｽ・ｽ・ｽ[
	CShader*		m_pShader;		//	・ｽV・ｽF・ｽ[・ｽ_・ｽ[
	CFbxMeshCtrl*	m_pFbxMeshCtrl;	// ・ｽ・ｽ・ｽb・ｽV・ｽ・ｽ・ｽR・ｽ・ｽ・ｽg・ｽ・ｽ・ｽ[・ｽ・ｽ・ｽN・ｽ・ｽ・ｽX    // -- 2021.2.4
	
	// ・ｽJ・ｽ・ｽ・ｽ・ｽ・ｽE・ｽ・ｽ・ｽC・ｽg・ｽE・ｽr・ｽ・ｽ・ｽ[
	VECTOR3         m_vEyePt;		//・ｽJ・ｽ・ｽ・ｽ・ｽ・ｽi・ｽ・ｽ・ｽ_・ｽj・ｽﾊ置
	VECTOR3         m_vLookatPt;	//・ｽ・ｽ・ｽ・ｽ・ｽﾊ置
	MATRIX4X4       m_mView;
	MATRIX4X4       m_mProj;
	VECTOR3         m_vLightDir;	//・ｽf・ｽB・ｽ・ｽ・ｽN・ｽV・ｽ・ｽ・ｽi・ｽ・ｽ・ｽ・ｽ・ｽC・ｽg・ｽﾌ包ｿｽ・ｽ・ｽ
	VECTOR4         m_vLightIntensity;	//・ｽ・ｽ・ｽC・ｽg・ｽﾌ具ｿｽ・ｽx(RGB) + Alpha
public:
	// ・ｽ・ｽ・ｽ\・ｽb・ｽh
	CGameMain(CMain* pMain);
	~CGameMain();

	HRESULT Init();
	void    Update();
	void	Draw();
	void    Quit();

	void    GameMain();
	HRESULT ChangeScreenMode(int nMode=-1);    // -- 2020.1.15

	void SetWindowName(const char* name);

	// ・ｽ・ｽ・ｽC・ｽg・ｽﾌ具ｿｽ・ｽx・ｽ・ｽﾝ抵ｿｽ/・ｽ謫ｾ
	void    SetLightIntensity(float r, float g, float b, float a = 1.0f);
	void    SetLightIntensity(VECTOR4 intensity);
	VECTOR4 GetLightIntensity() const { return m_vLightIntensity; }
};

CGameMain* GameDevice();

