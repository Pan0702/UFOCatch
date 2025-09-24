//=============================================================================
//		３Ｄゲームプログラム			               ver 3.2        2023.1.31
//
//		ウィンドウズの主処理
//																Main.h
//=============================================================================

#pragma once

//ヘッダーファイルのインクルード
#include <stdio.h>
#include <windows.h>

#include "resource.h"
//#include "GameMain.h"

//定数定義
//#define WINDOW_WIDTH   1920 //ウィンドウ幅
//#define WINDOW_HEIGHT  1080 //ウィンドウ高さ

#define WINDOW_WIDTH   1366   //ウィンドウ幅
#define WINDOW_HEIGHT  768    //ウィンドウ高さ

#define APP_NAME _T("Project")

class CGameMain;

//  ---------------------------------------------------------
//  Windowsメインクラス
//  ---------------------------------------------------------
class CMain
{
public:
	// メンバ変数
	CGameMain*	   m_pGMain;		  // ゲームメインクラス
	HINSTANCE      m_hInstance;
	HWND           m_hWnd;
	bool           m_bLoopFlag;       // ループフラグ　true:ループ　false:ループ終了
	double         m_MainLoopTime;    // ループスピード　1/60Sec

public:
	// メソッド
	HRESULT InitWindow(HINSTANCE,INT,INT,INT,INT,LPCTSTR);
	LRESULT MsgProc(HWND,UINT,WPARAM,LPARAM);
	HRESULT Init();
	void    MessageLoop();
	bool    MessageProcess(MSG* msg);
	void    Loop();
	void    DispFps();
	void    Quit();
	CMain();
	~CMain();
};
// 関数プロトタイプの宣言
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// imGUI2のプロトタイプ                                                       // -- 2020.8.7
LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

