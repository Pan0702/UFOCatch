//=============================================================================
//		３Ｄゲームプログラム　　　　　                 ver 3.2        2023.1.31
//
//		ゲームのメイン処理
//																GameMain.h
//=============================================================================

#pragma once

//ヘッダーファイルのインクルード
#include <stdio.h>
#include <windows.h>
#include <vector>

#include "Main.h"
#include "Direct3D.h"
#include "Shader.h"
#include "FbxMesh.h"
#include "XAudio.h"
#include "Dinput.h"
#include "Sprite3D.h"
#include "MyImgui.h"
#include "Macro.h"

#include "MainControl.h"

// --------------------------------------------
class CMain;
class CDirect3D;
class CXAudio;
class CDirectInput;
class CFontTexture;
class CShader;
class CFbxMeshCtrl;

// --------------------------------------------------------------------------------------------------
//  ゲームメインクラス
// --------------------------------------------------------------------------------------------------
class CGameMain
{
public:
	// メンバ変数
	CMain*			m_pMain;		// メインクラス
	CDirect3D*		m_pD3D;			//	Direct3Dオブジェクト
	CXAudio*		m_pXAudio;		//	XAudioオブジェクト
	CDirectInput*	m_pDI;			//	DirectInputDオブジェクト
	CFontTexture*	m_pFont;		//	フォントテクスチャー
	CShader*		m_pShader;		//	シェーダー
	CFbxMeshCtrl*	m_pFbxMeshCtrl;	// メッシュコントロールクラス    // -- 2021.2.4
	
	// カメラ・ライト・ビュー
	VECTOR3         m_vEyePt;		//カメラ（視点）位置
	VECTOR3         m_vLookatPt;	//注視位置
	MATRIX4X4       m_mView;
	MATRIX4X4       m_mProj;
	VECTOR3         m_vLightDir;	//ディレクショナルライトの方向
public:
	// メソッド
	CGameMain(CMain* pMain);
	~CGameMain();

	HRESULT Init();
	void    Update();
	void	Draw();
	void    Quit();

	void    GameMain();
	HRESULT ChangeScreenMode(int nMode=-1);    // -- 2020.1.15

	void SetWindowName(const char* name);
};

CGameMain* GameDevice();
