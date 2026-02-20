//=============================================================================
//		�R�c�Q�[���v���O����			               ver 3.2        2023.1.31
//
//		�E�B���h�E�Y�̎又��
//																Main.h
//=============================================================================

#pragma once

//�w�b�_�[�t�@�C���̃C���N���[�h
#include <atomic>
#include <stdio.h>
#include <thread>
#include <windows.h>

//#include "GameMain.h"

 // #define WINDOW_WIDTH   1920 //横幅
 // #define WINDOW_HEIGHT  1080 //高さ

#define WINDOW_WIDTH   1366   
#define WINDOW_HEIGHT  768    

#define APP_NAME _T("UFOAbducition")

class CGameMain;

//  ---------------------------------------------------------
//  Windows���C���N���X
//  ---------------------------------------------------------
class CMain
{
public:
	// �����o�ϐ�
	CGameMain*	   m_pGMain;		  // �Q�[�����C���N���X
	HINSTANCE      m_hInstance;
	HWND           m_hWnd;
	bool           m_bLoopFlag;       // ���[�v�t���O�@true:���[�v�@false:���[�v�I��
	double         m_MainLoopTime;    // ���[�v�X�s�[�h�@1/60Sec

public:
	// ���\�b�h
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
	// 追加：ゲームループ用スレッド
	std::thread        m_GameThread;
	std::atomic<bool>  m_RunGame{false};

	void GameThreadFunc(); // スレッド本体
};
// �֐��v���g�^�C�v�̐錾
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// imGUI2�̃v���g�^�C�v                                                       // -- 2020.8.7
LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

