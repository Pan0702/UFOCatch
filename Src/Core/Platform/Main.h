//=============================================================================
//		・ｽR・ｽc・ｽQ・ｽ[・ｽ・ｽ・ｽv・ｽ・ｽ・ｽO・ｽ・ｽ・ｽ・ｽ			               ver 3.2        2023.1.31
//
//		・ｽE・ｽB・ｽ・ｽ・ｽh・ｽE・ｽY・ｽﾌ主処・ｽ・ｽ
//																Main.h
//=============================================================================

#pragma once

//・ｽw・ｽb・ｽ_・ｽ[・ｽt・ｽ@・ｽC・ｽ・ｽ・ｽﾌイ・ｽ・ｽ・ｽN・ｽ・ｽ・ｽ[・ｽh
#include <atomic>
#include <stdio.h>
#include <thread>
#include <windows.h>

//#include "GameMain.h"

 // #define WINDOW_WIDTH   1920 //讓ｪ蟷・
 // #define WINDOW_HEIGHT  1080 //鬮倥＆

#define WINDOW_WIDTH   1366   
#define WINDOW_HEIGHT  768    

#define APP_NAME _T("UFOAbducition")

class CGameMain;

//  ---------------------------------------------------------
//  Windows・ｽ・ｽ・ｽC・ｽ・ｽ・ｽN・ｽ・ｽ・ｽX
//  ---------------------------------------------------------
class CMain
{
public:
	// ・ｽ・ｽ・ｽ・ｽ・ｽo・ｽﾏ撰ｿｽ
	CGameMain*	   m_pGMain;		  // ・ｽQ・ｽ[・ｽ・ｽ・ｽ・ｽ・ｽC・ｽ・ｽ・ｽN・ｽ・ｽ・ｽX
	HINSTANCE      m_hInstance;
	HWND           m_hWnd;
	bool           m_bLoopFlag;       // ・ｽ・ｽ・ｽ[・ｽv・ｽt・ｽ・ｽ・ｽO・ｽ@true:・ｽ・ｽ・ｽ[・ｽv・ｽ@false:・ｽ・ｽ・ｽ[・ｽv・ｽI・ｽ・ｽ
	double         m_MainLoopTime;    // ・ｽ・ｽ・ｽ[・ｽv・ｽX・ｽs・ｽ[・ｽh・ｽ@1/60Sec

public:
	// ・ｽ・ｽ・ｽ\・ｽb・ｽh
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

private:
	// 霑ｽ蜉・壹ご繝ｼ繝繝ｫ繝ｼ繝礼畑繧ｹ繝ｬ繝・ラ
	std::thread        m_GameThread;
	std::atomic<bool>  m_RunGame{false};

	void GameThreadFunc(); // 繧ｹ繝ｬ繝・ラ譛ｬ菴・
};
// ・ｽﾖ撰ｿｽ・ｽv・ｽ・ｽ・ｽg・ｽ^・ｽC・ｽv・ｽﾌ宣言
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// imGUI2・ｽﾌプ・ｽ・ｽ・ｽg・ｽ^・ｽC・ｽv                                                       // -- 2020.8.7
LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

