//-----------------------------------------------------------------------------
// DInput.h : DirectInputを簡単に利用するためのライブラリ(ヘッダ)
//
//  								                    ver 3.3        2024.3.23
//
// Copyright (c) 2019 静岡産業技術専門学校 ゲームクリエイト科 All rights reserved.
//-----------------------------------------------------------------------------

#ifndef _DINPUT_H_
#define _DINPUT_H_

#define DIRECTINPUT_VERSION 0x0800

#include <dinput.h>
#include <tchar.h>

//必要なライブラリファイルのロード
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dinput8.lib")

//-----------------------------------------------------------------------------
// key mode (ALL)
//-----------------------------------------------------------------------------
#define KD_DAT	0						// 直接データ(現在のキー押下状態)
#define KD_TRG	1						// キーを押した瞬間
#define KD_UTRG	2						// キーを離した瞬間

#define BufferSize	20
//-----------------------------------------------------------------------------
// mouse
//-----------------------------------------------------------------------------
#define DIM_LBUTTON	0
#define DIM_RBUTTON	1
#define DIM_MBUTTON	2

#define DIM_LEFT	10
#define DIM_RIGHT	11
#define DIM_UP		12
#define DIM_DOWN	13

//-----------------------------------------------------------------------------
// Joystick (最大同時接続数)
//-----------------------------------------------------------------------------
#define JOYSTICK_COUNT	(4)
#define JOY_PLAYER1		(0)
#define JOY_PLAYER2		(1)
#define JOY_PLAYER3		(2)
#define JOY_PLAYER4		(3)

//-----------------------------------------------------------------------------
// ForceFeedback Joystick 効果　３２種類まで
//-----------------------------------------------------------------------------
#define JOY_EF_COUNT    32	

#define DIJ_VOLUME	850
//-----------------------------------------------------------------------------
// DirectInput scan code (Microsoft SideWinder game pad)
//-----------------------------------------------------------------------------
#define DIJ_A		0
#define DIJ_B		1
#define DIJ_C		2
#define DIJ_X		3
#define DIJ_Y		4
#define DIJ_Z		5
#define DIJ_L		6
#define DIJ_R		7
#define DIJ_START	8
#define DIJ_M		9
//-----------------------------------------------------------------------------
// 左右上下 (for Microsoft SideWinder game pad)
//-----------------------------------------------------------------------------
#define DIJ_LEFT	10
#define DIJ_RIGHT	11
#define DIJ_UP		12
#define DIJ_DOWN	13
//-----------------------------------------------------------------------------
// DirectInput scan code (Microsoft ForceFeedback Joystick)
//-----------------------------------------------------------------------------
#define DIF_FIRE		0
#define DIF_SHIELD		1
#define DIF_STOP		2
#define DIF_STOPD		3				// STOPボタンの下のボタン
#define DIF_A			4
#define DIF_B			5
#define DIF_C			6
#define DIF_D			7
#define DIF_RESERVED	8
#define DIF_ARROW		9				// 矢印ボタン
#define DIF_THROW		-1				// 不明

#define DIF_LEFT		10
#define DIF_RIGHT		11
#define DIF_UP			12
#define DIF_DOWN		13
//-----------------------------------------------------------------------------
// 初期化デバイス
//-----------------------------------------------------------------------------
#define INIT_KEYBOARD	1
#define INIT_MOUSE		2
#define INIT_JOYSTICK	4

#define DIMOFS_BUTTON(n) (FIELD_OFFSET(DIMOUSESTATE, rgbButtons) + (n))

#define DEADZONE		2500			// 25% of the axis range
#define RANGE_MAX		1000			// maximum positive axis value
#define RANGE_MIN		-1000			// minimum negative axis value
#define FF_CHILD		5000			// "Child" gain == 50%
#define FF_ADULT		7500			// "Adult" gain == 75%
#define FF_BODYBUILDER	10000			// "Bodybuilder" gain == 100%
//-----------------------------------------------------------------------------
// "effect flags" - these are internal identifiers and in no way map to DirectInput
//-----------------------------------------------------------------------------
#define EF_BOUNCE   0
#define EF_EXPLODE  1
#define EF_FIRE     2



// ＤｉｒｅｃｔＩｎｐｕｔクラス
class CDirectInput
{
private:
	//-----------------------------------------------------------------------------
	// common
	//-----------------------------------------------------------------------------
	HWND           m_hWnd;
	HRESULT        m_hr;
	float          m_ViewWidth;				// 画面サイズ　幅
	float          m_ViewHeight;			// 画面サイズ　高さ
	LPDIRECTINPUT8 m_pDI8;					// DirectInput オブジェクト
	BYTE           m_diKeyState[256];		// DirectInput keyboard state
	DIMOUSESTATE   m_dims;				    // DirectInput mouse state
	DIJOYSTATE2    m_js[JOYSTICK_COUNT];	// DirectInput joystick state  2005.11.30
	bool           m_bInputActive;
	//-----------------------------------------------------------------------------
	// Keyboard
	//-----------------------------------------------------------------------------
	LPDIRECTINPUTDEVICE8 m_pKey;
	DWORD m_BufferRestKey;
	DWORD m_BufferRestBackupKey;
	DIDEVICEOBJECTDATA* m_pBufferKey;
	DIDEVICEOBJECTDATA* m_pBufferPositionKey;
	LPDIDEVICEOBJECTDATA m_didodKey;
	//-----------------------------------------------------------------------------
	// Mouse
	//-----------------------------------------------------------------------------
	LPDIRECTINPUTDEVICE8 m_pMouse;
	DWORD m_BufferRestMouse;
	DWORD m_BufferRestBackupMouse;
	DIDEVICEOBJECTDATA* m_pBufferMouse;
	DIDEVICEOBJECTDATA* m_pBufferPositionMouse;
	LPDIDEVICEOBJECTDATA m_didodMouse;
	//-----------------------------------------------------------------------------
	// Joystick
	//-----------------------------------------------------------------------------
	LPDIRECTINPUTDEVICE8 m_pJoy[JOYSTICK_COUNT];
	bool m_bJoyFF[JOYSTICK_COUNT];
	DWORD m_BufferRestJoy;
	DWORD m_BufferRestBackupJoy[JOYSTICK_COUNT];
	DIDEVICEOBJECTDATA* m_pBufferJoy[JOYSTICK_COUNT];
	DIDEVICEOBJECTDATA* m_pBufferPositionJoy;
	LPDIDEVICEOBJECTDATA m_didodJoy[JOYSTICK_COUNT];
	LPDIRECTINPUTEFFECT m_pJoyEffect[JOYSTICK_COUNT][JOY_EF_COUNT];
	int m_nJoySum;
	int m_nJoyFFNum;
	int m_nJoyEFSum;
	int m_nJoyEFI;

public:
	//-----------------------------------------------------------------------------
	// 共通
	//-----------------------------------------------------------------------------
	CDirectInput(void);
	~CDirectInput();

	bool StartDirectInput(HINSTANCE, HWND, int, DWORD, DWORD);
	bool EndDirectInput(void);
	void SetAcquire(void);
	bool GetInput(void);
	//-----------------------------------------------------------------------------
	// Keyboard
	//-----------------------------------------------------------------------------
	bool GetKey(void);
	bool CheckKey(const int&, const DWORD&);
	bool InitKey(HWND);
	bool SetPropertyKey(void);
	//-----------------------------------------------------------------------------
	// Mouse
	//-----------------------------------------------------------------------------
	bool GetMouse(void);
	bool CheckMouse(const int&, const DWORD&);
	DIMOUSESTATE GetMouseState( void );
	POINT GetMousePos(); //マウスの現在の座標を取得する
	void ShowMouseCursor(bool bFlag);
	bool InitMouse(HWND);
	bool SetPropertyMouse(void);
	//-----------------------------------------------------------------------------
	// Joystick
	//-----------------------------------------------------------------------------
	bool GetJoy(void);
	bool IfJoyFF(int nSum=JOY_PLAYER1);
	DIJOYSTATE2 GetJoyState(int nSum=JOY_PLAYER1);
	int  GetJoyNum(void);                                // -- 2018.8.27
	bool CheckJoy(const int&, const DWORD&, int nSum=JOY_PLAYER1);
	bool CheckUpDownLeftRight(int, DWORD, int nSum=JOY_PLAYER1);
	bool InitJoy(HWND);
	bool InitJoyNormal(HWND);
	bool InitJoyFF(HWND);
	bool SetPropertyJoy(void);
	bool CheckJoyImm(DWORD, int nSum=JOY_PLAYER1);
	bool PlayJoyEffect(int, int nNum=1, int nSum=JOY_PLAYER1);
	bool CreateJoyEffectStandard(void);
	bool ReadJoyEffect(const TCHAR* szFName, int &nEffectNo, int &nNum);

	//-----------------------------------------------------------------------------
	// CALLBACK
	//-----------------------------------------------------------------------------
	BOOL EnumJoysticksCb(LPCDIDEVICEINSTANCE pInst, LPVOID lpvContext);
	BOOL EnumEffectsInFileCb(LPCDIFILEEFFECT lpdife, LPVOID pvRef );

};

#endif
