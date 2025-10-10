//-----------------------------------------------------------------------------
// DInput関数 : DirectInputを簡単に利用するためのライブラリ(本体)
// 
//												       ver 3.3        2024.3.23
//
// Copyright (c) 2019 静岡産業技術専門学校 ゲームクリエイト科 All rights reserved.
//-----------------------------------------------------------------------------
#include <stdio.h>
#include "DInput.h"


// グローバル変数(コールバック関数を使用するため必要)
static CDirectInput*	g_pDI = nullptr;

//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
CDirectInput::CDirectInput(void)
{
	ZeroMemory(this, sizeof(CDirectInput));
	m_bInputActive = true;

	g_pDI = this;
}
//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
CDirectInput::~CDirectInput()
{
	EndDirectInput();
}
//-----------------------------------------------------------------------------
// DirectInputの開始
// 
// 引数		HINSTANCE	hInst	インスタンスハンドル
//			HWND		hWnd	ウィンドウハンドル
//			int			flag	初期化するディバイスの選択（INIT_KEYBOARD|INIT_MOUSE|INIT_JOYSTICK）
//			DWORD		dwWidth	ウィンドの幅
//			DWORD		dwHeightウィンドの高さ
// 
// 戻り値	true:成功　false:失敗
//-----------------------------------------------------------------------------
bool CDirectInput::StartDirectInput(HINSTANCE hInst, HWND hWnd, int flag, DWORD dwWidth, DWORD dwHeight)
{
	// DirectInput8オブジェクトの作成
	m_hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&m_pDI8, nullptr);
	if (DI_OK != m_hr) {
		MessageBox(nullptr, _T("DInput.cpp : StartDirectInput() : DirectInputを初期化することができません。"), nullptr, MB_OK);
		return false;
	}
	m_hWnd = hWnd;

	if (flag & INIT_KEYBOARD) InitKey(hWnd);
	if (flag & INIT_MOUSE)    InitMouse(hWnd);
	if (flag & INIT_JOYSTICK) InitJoy(hWnd);

	m_ViewWidth  = (float)dwWidth;
	m_ViewHeight = (float)dwHeight;

	return true;
}
//-----------------------------------------------------------------------------
// DirectInputの終了
// 
// 戻り値	true:成功　false:失敗
//-----------------------------------------------------------------------------
bool CDirectInput::EndDirectInput(void)
{
	// DirectInputDevice(keyboard)を解放
	if( m_pBufferKey ){
		delete []m_pBufferKey;	// バッファを解放
		m_pBufferKey = nullptr;
	}
	if (m_pKey) {
		m_pKey->Unacquire();
		m_pKey->Release();
		m_pKey = nullptr;
	}

	// DirectInputDevice(mouse)を解放
	if( m_pBufferMouse ){
		delete []m_pBufferMouse;	// バッファを解放
		m_pBufferMouse = nullptr;
	}
	if (m_pMouse) {
		m_pMouse->Unacquire();
		m_pMouse->Release();
		m_pMouse = nullptr;
	}

	int i, j;
	for (i=0; i<m_nJoySum; i++) {
		if( m_bJoyFF[i] ){
			// DirectInputDevice(ForceFeedback Joystick)を解放
			for (j=0; j<m_nJoyEFSum; j++) {
				if( m_pJoyEffect[i][j] ){
					m_pJoyEffect[i][j]->Unload();
					m_pJoyEffect[i][j]->Release();
					m_pJoyEffect[i][j] = nullptr;
				}
			}

		}

		// DirectInputDevice(joystick)を解放
		if( m_pBufferJoy[i] ){
			delete []m_pBufferJoy[i];	// バッファを解放
			m_pBufferJoy[i] = nullptr;
		}

		if (m_pJoy[i]) {
			m_pJoy[i]->Unacquire();
			m_pJoy[i]->Release();
			m_pJoy[i] = nullptr;
		}
	}

	// DirectInputを解放
	if (m_pDI8) {
		m_pDI8->Release();
		m_pDI8 = nullptr;
	}

	return true;
}
//-----------------------------------------------------------------------------
// 入力デバイスのアクセス権の制御
//-----------------------------------------------------------------------------
void CDirectInput::SetAcquire(void)
{
	int i;

	if (m_bInputActive) {
		if (m_pKey) m_pKey->Acquire();
		if (m_pMouse) m_pMouse->Acquire();
		for (i=0; i<m_nJoySum; i++) if (m_pJoy[i]) m_pJoy[i]->Acquire();
	} else {
		if (m_pKey) m_pKey->Unacquire();
		if (m_pMouse) m_pMouse->Unacquire();
		for (i=0; i<m_nJoySum; i++) if (m_pJoy[i]) m_pJoy[i]->Unacquire();
	}
}
//-----------------------------------------------------------------------------
// データ読み込み
// 
// 戻り値	true:成功　false:失敗
//-----------------------------------------------------------------------------
bool CDirectInput::GetInput(void)
{
	if (m_pKey) GetKey();
	if (m_pMouse) GetMouse();
	if (m_pJoy[0]) GetJoy();
	return true;
}
//*****************************************************************************
//*** Keyboard                                                              ***
//*****************************************************************************
//-----------------------------------------------------------------------------
// Keyboardデバイスの初期化
// 
// 引数		HWND hWnd	ウィンドウハンドル
// 
// 戻り値	true:成功　false:失敗
//-----------------------------------------------------------------------------
bool CDirectInput::InitKey(HWND hWnd)
{
	//-----------------------------------------------------------------------------
	// keyboardデバイスのインスタンス作成・初期化
	m_hr = m_pDI8->CreateDevice(GUID_SysKeyboard, &m_pKey, nullptr);
	if (DI_OK != m_hr) {
		MessageBox(nullptr, _T("DInput.cpp : StartDirectInput() : DirectInputDeviceEx(keyboard)を初期化することができません。"),nullptr,MB_OK);
		return false;
	}
	// DirectInputデバイスのデータ形式の設定
	m_pKey->SetDataFormat(&c_dfDIKeyboard);
	if (DI_OK != m_hr) {
		MessageBox(nullptr, _T("DInput.cpp : StartDirectInput() : DirectInputデバイスのデータ形式の設定に失敗しました。"),nullptr,MB_OK);
		return false;
	}
	// keyboardデバイスのインスタンスに対する協調レベルの確立
	m_pKey->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	if (DI_OK != m_hr) {
		MessageBox(nullptr, _T("DInput.cpp : StartDirectInput() : keyboardデバイスのインスタンスに対する協調レベルの確立に失敗しました。"),nullptr,MB_OK);
		return false;
	}
	return SetPropertyKey();
}
//-----------------------------------------------------------------------------
// バッファサイズの設定(keyboard)
// 
// 戻り値	true:成功　false:失敗
//-----------------------------------------------------------------------------
bool CDirectInput::SetPropertyKey(void)
{
	m_BufferRestKey = 0;
	m_pBufferPositionKey = nullptr;
	m_pBufferKey = new DIDEVICEOBJECTDATA[BufferSize];	// バッファを確保
	DIPROPDWORD dipdw;
	ZeroMemory(&dipdw, sizeof(dipdw));
	dipdw.diph.dwSize = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipdw.diph.dwObj = 0;
	dipdw.diph.dwHow = DIPH_DEVICE;
	dipdw.dwData = BufferSize;
	m_pKey->Unacquire();				// 一旦アクセス権を解放する
	m_hr = m_pKey->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);
	m_pKey->Acquire();					// アクセス権を得る
	return !FAILED(m_hr);
}
//-----------------------------------------------------------------------------
// キーボードの状態を得る
// 
// 戻り値	true:成功　false:失敗
//-----------------------------------------------------------------------------
bool CDirectInput::GetKey(void)
{
	if (!m_pKey) return false;
	if (m_pKey->GetDeviceState(sizeof(m_diKeyState), m_diKeyState) != DI_OK) {
		m_pKey->Acquire();
		if (FAILED(m_pKey->GetDeviceState(sizeof(m_diKeyState), m_diKeyState))) {
			//MessageBox(nullptr,_T("DInput.cpp : GetKey() : GetDeviceStateが異常終了しました。") ,nullptr,MB_OK );
			return false;
		}
	}

	m_BufferRestKey = BufferSize;
	if (FAILED(m_pKey->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), m_pBufferKey, &m_BufferRestKey, 0))) {
		// 失敗したらもう1度だけ試みる
		m_BufferRestKey = BufferSize;
		m_pKey->Acquire();
		if (FAILED(m_pKey->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), m_pBufferKey, &m_BufferRestKey, 0))) {
			//MessageBox(nullptr,_T("DInput.cpp : GetKey() : GetDeviceDataが異常終了しました。"), nullptr, MB_OK);
			return false;
		}
	}
	m_BufferRestBackupKey = m_BufferRestKey;	// 読み込んだバッファ数を退避

	return true;
}
//-----------------------------------------------------------------------------
// キー押下チェック(keyboard)
// 
// 引数		const int&		kmode	チェックするモード
//			const DWORD&	kcode	チェックするキーコード　
// 
// 戻り値	true:チェックＯＫ　false:チェックＮＧ
//-----------------------------------------------------------------------------
bool CDirectInput::CheckKey(const int& kmode, const DWORD& kcode)
{
	if (!m_bInputActive || !m_pKey) return false;
	switch (kmode) {
	case KD_DAT :						// 直接データ(現在のキー押下状態)
		return m_diKeyState[kcode] & 0x80;
	case KD_TRG :						// キーを押した瞬間
	case KD_UTRG :						// キーを離した瞬間
		m_BufferRestKey = m_BufferRestBackupKey;	// 読み込んだバッファ数の復元
		m_pBufferPositionKey = m_pBufferKey;
		while (m_BufferRestKey > 0) {
			m_BufferRestKey--;
			m_didodKey = m_pBufferPositionKey;
			m_pBufferPositionKey++;
			if (m_didodKey->dwOfs == kcode) {	//キーの種類
				if (kmode == KD_TRG) {
					if (m_didodKey->dwData) return true;
				} else {
					if (!(m_didodKey->dwData)) return true;
				}
			}
		}
		return false;
	default :
		MessageBox(nullptr, _T("DInput.cpp : CheckKey() : 指定したキーモードが間違っています。"), nullptr, MB_OK);
		return false;
	}
}
//*****************************************************************************
//*** Mouse                                                                 ***
//*****************************************************************************
//-----------------------------------------------------------------------------
// Mouseデバイスの初期化
// 
// 引数		HWND hWnd	ウィンドウハンドル
// 
// 戻り値	true:成功　false:失敗
//-----------------------------------------------------------------------------
bool CDirectInput::InitMouse(HWND hWnd)
{
	//-----------------------------------------------------------------------------
	// DirectInputDevice初期化(mouse)
	m_hr = m_pDI8->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr);
	if (DI_OK != m_hr) {
		MessageBox(nullptr, _T("DInput.cpp : StartDirectInput() : DirectInputDeviceEx(mouse)を初期化することができません。"),nullptr,MB_OK);
		return false;
	}
	if (m_pMouse->SetDataFormat(&c_dfDIMouse) != DI_OK) {
		MessageBox(nullptr, _T("DInput.cpp : StartDirectInput() : SetDataFormatメソッド(mouse)が異常終了しました。"), nullptr, MB_OK);
		return false;
	}
	if (m_pMouse->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE) != DI_OK) {
		MessageBox(nullptr, _T("DInput.cpp : StartDirectInput() : SetCooperativeLevelメソッド(mouse)が異常終了しました。"), nullptr, MB_OK);
		return false;
	}

	return SetPropertyMouse();
}
//-----------------------------------------------------------------------------
// バッファサイズの設定(mouse)
// 
// 戻り値	true:成功　false:失敗
//-----------------------------------------------------------------------------
bool CDirectInput::SetPropertyMouse(void)
{
	m_BufferRestMouse = 0;
	m_pBufferPositionMouse = nullptr;
	m_pBufferMouse = new DIDEVICEOBJECTDATA[BufferSize];	// バッファを確保
	DIPROPDWORD dipdw;
	ZeroMemory(&dipdw, sizeof(dipdw));
	dipdw.diph.dwSize = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipdw.diph.dwObj = 0;
	dipdw.diph.dwHow = DIPH_DEVICE;
	dipdw.dwData = BufferSize;
	m_pMouse->Unacquire();				// 一旦アクセス権を解放する
	m_hr = m_pMouse->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);
	m_pMouse->Acquire();					// アクセス権を得る
	return !FAILED(m_hr);
}
//-----------------------------------------------------------------------------
// マウスの状態を得る
// 
// 戻り値	true:成功　false:失敗
//-----------------------------------------------------------------------------
bool CDirectInput::GetMouse(void)
{
	if (!m_pMouse) return false;
	if (m_pMouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_dims) != DI_OK) {
		m_pMouse->Acquire();
		if (FAILED(m_pMouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_dims))) {
			//MessageBox(nullptr,_T("DInput.cpp : GetMouse() : GetDeviceStateが異常終了しました。"), nullptr, MB_OK);
			return false;
		}
	}

	m_BufferRestMouse = BufferSize;
	if (FAILED(m_pMouse->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), m_pBufferMouse, &m_BufferRestMouse, 0))) {
		// 失敗したらもう1度だけ試みる
		m_BufferRestMouse = BufferSize;
		m_pMouse->Acquire();
		if (FAILED(m_pMouse->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), m_pBufferMouse, &m_BufferRestMouse, 0))) {
			//MessageBox(nullptr,_T("DInput.cpp : GetMouse() : GetDeviceDataが異常終了しました。"), nullptr, MB_OK);
			return false;
		}
	}
	m_BufferRestBackupMouse = m_BufferRestMouse;	// 読み込んだバッファ数を退避

	return true;
}
//-----------------------------------------------------------------------------
// キー押下チェック(mouse)
// 
// 引数		const int&		kmode	チェックするモード
//			const DWORD&	kcode	チェックするキーコード　
// 
// 戻り値	true:チェックＯＫ　false:チェックＮＧ
//-----------------------------------------------------------------------------
bool CDirectInput::CheckMouse(const int& kmode, const DWORD& kcode)
{
	if (!m_bInputActive || !m_pMouse) return false;

	// 移動方向の取得のとき
	if (kcode == DIM_LEFT || kcode == DIM_RIGHT || kcode == DIM_UP || kcode == DIM_DOWN) {

		if (m_dims.lX > 0 && kcode == DIM_RIGHT) {
			return true;
		}
		else if (m_dims.lX < 0 && kcode == DIM_LEFT) {
			return true;
		}
		else if (m_dims.lY < 0 && kcode == DIM_UP) {
			return true;
		}
		else if (m_dims.lY > 0 && kcode == DIM_DOWN) {
			return true;
		}
		return false;

	}else{ // 各キー状態の取得のとき

		switch (kmode) {
			case KD_DAT:						// 直接データ(現在のキー押下状態)
				return m_dims.rgbButtons[kcode] & 0x80;
			case KD_TRG:						// キーを押した瞬間
			case KD_UTRG:						// キーを離した瞬間
				m_BufferRestMouse = m_BufferRestBackupMouse;	// 読み込んだバッファ数の復元
				m_pBufferPositionMouse = m_pBufferMouse;
				while (m_BufferRestMouse > 0) {
					m_BufferRestMouse--;
					m_didodMouse = m_pBufferPositionMouse;
					m_pBufferPositionMouse++;
					if (m_didodMouse->dwOfs == DIMOFS_BUTTON(kcode)) {	//キーの種類
						if (kmode == KD_TRG) {
							if (m_didodMouse->dwData) return true;
						}
						else {
							if (!(m_didodMouse->dwData)) return true;
						}
					}
				}
				return false;
			default:
				MessageBox(nullptr, _T("DInput.cpp : CheckMouse() : 指定したキーモードが間違っています。"), nullptr, MB_OK);
				return false;
		}
	}

}
//-----------------------------------------------------------------------------
// マウスステートを得る(mouse)
//
// 戻り値	DIMOUSESTATE構造体の値。マウスデバイスの状態を表す。
//-----------------------------------------------------------------------------
DIMOUSESTATE CDirectInput::GetMouseState( void )
{
	return  m_dims;
}

//-----------------------------------------------------------------------------
// マウス座標を得る(mouse)
//
// この関数はDirectInputではなくWindowsの関数を使用している
//
// 戻り値	POINT構造体の値。マウスのＸＹ座標を表す。
//-----------------------------------------------------------------------------
POINT CDirectInput::GetMousePos(void)
{
	POINT pt;
	RECT  rc;
	float scaleX, scaleY;
	GetCursorPos(&pt); //マウスの現在のスクリーン座標を取得する
	ScreenToClient(m_hWnd, &pt); // クライアント座標に変換する

	// 画面の拡縮に対応してマウス座標を計算する
	GetClientRect(m_hWnd, &rc); // クライアント領域のサイズ
	scaleX = m_ViewWidth  / (rc.right - rc.left);	// 幅　比率
	scaleY = m_ViewHeight / (rc.bottom - rc.top);	// 高さ比率
	
	pt.x = (long)(pt.x * scaleX);
	pt.y = (long)(pt.y * scaleY);

	return pt;
}
//-----------------------------------------------------------------------------
// マウスカーソルの表示・非表示を切り替える
//
// この関数はDirectInputではなくWindowsの関数を使用している
//
// 引数  bool bFlag  true:カーソル表示  FLASE:カーソル非表示
//
// 戻り値	なし
//-----------------------------------------------------------------------------
void CDirectInput::ShowMouseCursor(bool bFlag)
{
	ShowCursor(bFlag);
}

//*****************************************************************************
//*** Joystickの処理                                                        ***
//*****************************************************************************
//-----------------------------------------------------------------------------
// 列挙コールバック関数 (Joystick)
//-----------------------------------------------------------------------------
BOOL CALLBACK EnumJoysticksCallback(LPCDIDEVICEINSTANCE pInst, LPVOID lpvContext)
{
	return g_pDI->EnumJoysticksCb( pInst, lpvContext );
}

//-----------------------------------------------------------------------------
// 列挙コールバック (Joystick)対応メソッド
//-----------------------------------------------------------------------------
BOOL CDirectInput::EnumJoysticksCb(LPCDIDEVICEINSTANCE pInst, LPVOID lpvContext)
{
	LPDIRECTINPUTDEVICE8 pDevice = nullptr;
	DIDEVCAPS            diDevCaps = { 0 };

	m_hr = m_pDI8->CreateDevice(pInst->guidInstance, &pDevice, nullptr);
	if (DI_OK != m_hr) return DIENUM_CONTINUE;	// 次のデバイスを列挙

	diDevCaps.dwSize = sizeof( DIDEVCAPS );
	m_hr = pDevice->GetCapabilities( &diDevCaps );
	if( FAILED(m_hr) ){
		pDevice->Release();
		pDevice = nullptr;
		return DIENUM_CONTINUE;	// 次のデバイスを列挙
	}

	m_pJoy[m_nJoySum] = pDevice;

	//if( diDevCaps.dwFlags == DIDC_FORCEFEEDBACK ){
	if( pInst->guidFFDriver != GUID_NULL ){
		m_bJoyFF[m_nJoySum] = true;		// フォースフィードバックジョイススティック
		m_nJoyFFNum++;
	}else{
		m_bJoyFF[m_nJoySum] = false;	// 通常のジョイスティック
	}

	if (++m_nJoySum >= JOYSTICK_COUNT) return DIENUM_STOP;
	return DIENUM_CONTINUE;
}


//-----------------------------------------------------------------------------
// Joystickデバイスの初期化
// 
// 引数		HWND hWnd	ウィンドウハンドル
// 
// 戻り値	true:成功　false:失敗
//-----------------------------------------------------------------------------
bool CDirectInput::InitJoy(HWND hWnd)
{
	//-----------------------------------------------------------------------------
	// アタッチ可能なデバイスを列挙する(joystick)
	m_pDI8->EnumDevices( DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback, nullptr, DIEDFL_ATTACHEDONLY);
	if(!m_pJoy[0]) {
		//MessageBox(nullptr,_T("DInput.cpp : StartDirectInput() : joystickが接続されていません。"), nullptr, MB_OK);
		return false;
	}
	for (int i=0; i<m_nJoySum; i++) {
		if (!m_pJoy[i]) continue;

		m_hr = m_pJoy[i]->SetDataFormat(&c_dfDIJoystick2);
		if (DI_OK != m_hr) {
			MessageBox(nullptr, _T("DInput.cpp : StartDirectInput() : SetDataFormatメソッド(joystick)が異常終了しました。"),nullptr,MB_OK);
			return false;
		}

		if( m_nJoyFFNum > 0 ){	// ＦＦジョイスティックが１つでもあるときは、全部を排他モードにする
			m_hr = m_pJoy[i]->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
		}else{
			m_hr = m_pJoy[i]->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
		}
		if (DI_OK != m_hr) {
			MessageBox(nullptr, _T("DInput.cpp : StartDirectInput() : SetCooperativeLevelメソッド(joystick)が異常終了しました。"),nullptr,MB_OK);
			return false;
		}
	}

	InitJoyNormal(hWnd);	// 通常ジョイスティックの初期化
	InitJoyFF(hWnd);		// ＦＦジョイスティックの初期化

	return true;

}
//-----------------------------------------------------------------------------
// 通常のJoystickデバイスの初期化
// 
// 引数		HWND hWnd	ウィンドウハンドル
// 
// 戻り値	true:成功　false:失敗
//-----------------------------------------------------------------------------
bool CDirectInput::InitJoyNormal(HWND hWnd)
{
	int  i;

	DIPROPRANGE diprg;
	diprg.diph.dwSize = sizeof(diprg);
	diprg.diph.dwHeaderSize = sizeof(diprg.diph);
	diprg.diph.dwHow = DIPH_BYOFFSET;
	diprg.lMin = RANGE_MIN;
	diprg.lMax = RANGE_MAX;

	for (i=0; i<m_nJoySum; i++) {
		if (!m_pJoy[i]) continue;
		if( m_bJoyFF[i] ) continue;	// ＦＦジョイスティックを除く 

		diprg.diph.dwObj = DIJOFS_X;
		m_hr = m_pJoy[i]->SetProperty(DIPROP_RANGE, &diprg.diph);
		if (DI_OK != m_hr) {
			MessageBox(nullptr, _T("DInput.cpp : StartDirectInput() : SetPropertyメソッド(joystick:DIJOFS_X)が異常終了しました。"),nullptr,MB_OK);
			return false;
		}

		diprg.diph.dwObj = DIJOFS_Y;
		m_hr = m_pJoy[i]->SetProperty(DIPROP_RANGE, &diprg.diph);
		if (DI_OK != m_hr) {
			MessageBox(nullptr, _T("DInput.cpp : StartDirectInput() : SetPropertyメソッド(joystick:DIJOFS_Y)が異常終了しました。"),nullptr,MB_OK);
			return false;
		}
	}

	return SetPropertyJoy();
}
//-----------------------------------------------------------------------------
// 通常のjoystickのバッファサイズの設定(joystick)
// 
// 戻り値	true:成功　false:失敗
//-----------------------------------------------------------------------------
bool CDirectInput::SetPropertyJoy(void)
{
	m_BufferRestJoy = 0;
	m_pBufferPositionJoy = nullptr;
	for (int i=0; i<m_nJoySum; i++) {
		if (!m_pJoy[i]) continue;
		if( m_bJoyFF[i] ) continue;	// ＦＦジョイスティックを除く

		m_pBufferJoy[i] = new DIDEVICEOBJECTDATA[BufferSize];	// バッファを確保
		DIPROPDWORD dipdw;
		ZeroMemory(&dipdw, sizeof(dipdw));
		dipdw.diph.dwSize = sizeof(DIPROPDWORD);
		dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		dipdw.diph.dwObj = 0;
		dipdw.diph.dwHow = DIPH_DEVICE;
		dipdw.dwData = BufferSize;
		m_pJoy[i]->Unacquire();				// 一旦アクセス権を解放する
		m_hr = m_pJoy[i]->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);
		m_pJoy[i]->Acquire();					// アクセス権を得る
		if (FAILED(m_hr)) return !FAILED(m_hr);
	}
	return true;
}
//-----------------------------------------------------------------------------
// ForceFeedback Joystickデバイスの初期化
// 
// 引数		HWND hWnd	ウィンドウハンドル
// 
// 戻り値	true:成功　false:失敗
//-----------------------------------------------------------------------------
bool CDirectInput::InitJoyFF(HWND hWnd)
{
	int  i;

	DIPROPRANGE dipr;
	dipr.diph.dwSize = sizeof(DIPROPRANGE);
	dipr.diph.dwHeaderSize = sizeof(dipr.diph);
	dipr.diph.dwHow = DIPH_BYOFFSET;
	dipr.lMin = RANGE_MIN;				// negative to the left/top
	dipr.lMax = RANGE_MAX;				// positive to the right/bottom

	for (i=0; i<m_nJoySum; i++) {
		if (!m_pJoy[i]) continue;
		if( !m_bJoyFF[i] ) continue;	// 通常ジョイスティックを除く

		m_pJoy[i]->Unacquire();

		// X軸範囲の設定
		dipr.diph.dwObj = DIJOFS_X;
		if (m_pJoy[i]->SetProperty(DIPROP_RANGE, &dipr.diph) != DI_OK) {
			MessageBox(nullptr, _T("DInput.cpp : StartDirectInput() : SetProperty(X軸範囲(RANGE):FFJoystick)が異常終了しました。"), nullptr, MB_OK);
			return false;
		}

		// Y軸範囲の設定
		dipr.diph.dwObj = DIJOFS_Y;
		if (m_pJoy[i]->SetProperty(DIPROP_RANGE, &dipr.diph) != DI_OK) {
			MessageBox(nullptr, _T("DInput.cpp : StartDirectInput() : SetProperty(Y軸範囲(RANGE):FFJoystick)が異常終了しました。"), nullptr, MB_OK);
			return false;
		}
	}

	// deadzoneの設定
	DIPROPDWORD dipdw;
	dipdw.diph.dwSize = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(dipdw.diph);
	dipdw.diph.dwHow = DIPH_BYOFFSET;
	dipdw.dwData = DEADZONE;

	for (i=0; i<m_nJoySum; i++) {
		if (!m_pJoy[i]) continue;
		if( !m_bJoyFF[i] ) continue;	// 通常ジョイスティックを除く

		// X軸範囲の設定
		dipdw.diph.dwObj = DIJOFS_X;
		if (m_pJoy[i]->SetProperty(DIPROP_DEADZONE, &dipdw.diph) != DI_OK) {
			MessageBox(nullptr, _T("DInput.cpp : StartDirectInput() : SetProperty(X軸範囲(DEADZONE):FFJoystick)が異常終了しました。"), nullptr, MB_OK);
			return false;
		}

		// Y軸範囲の設定
		dipdw.diph.dwObj = DIJOFS_Y;
		if (m_pJoy[i]->SetProperty(DIPROP_DEADZONE, &dipdw.diph) != DI_OK) {
			MessageBox(nullptr, _T("DInput.cpp : StartDirectInput() : SetProperty(Y軸範囲(RANGE):FFJoystick)が異常終了しました。"), nullptr, MB_OK);
			return false;
		}
	}

	// 自動センタリング
	for (i=0; i<m_nJoySum; i++) {
		if (!m_pJoy[i]) continue;
		if( !m_bJoyFF[i] ) continue;	// 通常ジョイスティックを除く

		dipdw.diph.dwSize = sizeof(DIPROPDWORD);
		dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		dipdw.diph.dwObj = 0;
		dipdw.diph.dwHow = DIPH_DEVICE;
		//dipdw.dwData = DIPROPAUTOCENTER_ON;
		dipdw.dwData = DIPROPAUTOCENTER_OFF;	// 自動センタリング無効
		m_hr = m_pJoy[i]->SetProperty(DIPROP_AUTOCENTER, &dipdw.diph);
		if (FAILED(m_hr)) {
			MessageBox(nullptr, _T("DInput.cpp : StartDirectInput() : SetProperty(自動センタリング):FFJoystick)が異常終了しました。"),nullptr,MB_OK);
			return false;
		}
	}

	m_BufferRestJoy = 0;
	m_pBufferPositionJoy = nullptr;

	for (i=0; i<m_nJoySum; i++) {
		if (!m_pJoy[i]) continue;
		if( !m_bJoyFF[i] ) continue;	// 通常ジョイスティックを除く

		m_pBufferJoy[i] = new DIDEVICEOBJECTDATA[BufferSize];	// バッファを確保
		ZeroMemory(&dipdw, sizeof(dipdw));
		dipdw.diph.dwSize = sizeof(DIPROPDWORD);
		dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		dipdw.diph.dwObj = 0;
		dipdw.diph.dwHow = DIPH_DEVICE;
		dipdw.dwData = BufferSize;
		// 復帰値はチェックしない(DI_POLLEDDEVICEが返る)
		m_pJoy[i]->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);

		if (m_pJoy[i]->Acquire() != DI_OK) {
			//CreateJoyEffectStandard(); // なぜか下と二重になっていたので削除した
			;
		}

		// get the device capabilities
		DIDEVCAPS didc;
		didc.dwSize = sizeof(DIDEVCAPS);
		if (m_pJoy[i]->GetCapabilities(&didc) != DI_OK) {
			MessageBox(nullptr, _T("DInput.cpp : StartDirectInput() : GetCapabilities(ForceFeedback Joystick)が異常終了しました。"), nullptr, MB_OK);
			return false;
		}

		// 標準のＦＦ効果を設定する
		if (didc.dwFlags & DIDC_FORCEFEEDBACK) {
			//MessageBox(nullptr,_T("DInput.cpp : StartDirectInput() : ForceFeedback device found.\n"), nullptr, MB_OK); // -- 2018.8.27
			if (!CreateJoyEffectStandard()) {
				MessageBox(nullptr,_T("DInput.cpp : StartDirectInput() : CreateEffect(ForceFeedback Joystick)が異常終了しました。"), nullptr, MB_OK);
				return false;
			}
		}
	}

	return true;
}

//-----------------------------------------------------------------------------
// ジョイスティックの現在のキー押下チェック(直接データ)
// 
// 引数		DWORD	kcode	チェックするキーコード
//			int		nSum	ジョイスティック番号。省略値はJOY_PLAYER1
// 
// 戻り値	true:チェックＯＫ　false:チェックＮＧ
//-----------------------------------------------------------------------------
bool CDirectInput::CheckJoyImm(DWORD kcode, int nSum)
{
	if (DIJ_LEFT > kcode) {
		return m_js[nSum].rgbButtons[kcode] & 0x80;
	} else {
		switch (kcode) {	// 上下左右は、デジタル（DIJ_VOLUME限界）で判断
		case DIJ_LEFT :		// DIF_LEFTも同一コード
			return m_js[nSum].lX < (-1 * DIJ_VOLUME);
		case DIJ_RIGHT :	// DIF_RIGHTも同一コード
			return m_js[nSum].lX > DIJ_VOLUME;
		case DIJ_UP :		// DIF_UPも同一コード
			return m_js[nSum].lY < (-1 * DIJ_VOLUME);
		case DIJ_DOWN :		// DIF_DOWNも同一コード
			return m_js[nSum].lY > DIJ_VOLUME;
		default :
			MessageBox(nullptr, _T("DInput.cpp : CheckJoystickImm() : kcodeの指定が間違っています。"), nullptr, MB_OK);
			return false;
		}
	}
}
//-----------------------------------------------------------------------------
// ジョイスティックの状態を得る
// 
// 戻り値	true:成功　false:失敗
//-----------------------------------------------------------------------------
bool CDirectInput::GetJoy(void)
{
	for (int i=0; i<m_nJoySum; i++) {
		if (!m_pJoy[i]) return false;
		m_pJoy[i]->Poll();
		m_pJoy[i]->GetDeviceState(sizeof(DIJOYSTATE2), &m_js[i]);

		m_BufferRestJoy = BufferSize;
		m_hr = m_pJoy[i]->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), m_pBufferJoy[i], &m_BufferRestJoy, 0);
		if (FAILED(m_hr)) {				// 失敗したら1度だけリトライ
			m_BufferRestJoy = BufferSize;
			m_pJoy[i]->Acquire();
			m_hr = m_pJoy[i]->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), m_pBufferJoy[i], &m_BufferRestJoy, 0);
			if (FAILED(m_hr)) {
				//MessageBox(nullptr,_T("DInput.cpp : GetJoy() : GetDeviceDataが異常終了しました。"),nullptr,MB_OK);
				return false;
			}
		}
		m_BufferRestBackupJoy[i] = m_BufferRestJoy;	// 読み込んだバッファ数を退避
	}
	return true;
}
//-----------------------------------------------------------------------------
// Joystick 上下左右の制御チェック(joystick)
// 
// 引数		int		kmode	チェックするモード
//			DWORD	kcode	チェックするキーコード　
//			int		nSum	ジョイスティック番号。省略値はJOY_PLAYER1
// 
// 戻り値	true:チェックＯＫ　false:チェックＮＧ
//-----------------------------------------------------------------------------
bool CDirectInput::CheckUpDownLeftRight(int kmode, DWORD kcode, int nSum)
{
	if (m_didodJoy[nSum]->dwOfs == 4) {	// Y軸(上下)?
		switch (kcode) {
		case DIJ_UP :
			if (kmode == KD_TRG) {
				if ((long)m_didodJoy[nSum]->dwData < (-1 * DIJ_VOLUME)) return true;
			} else {
				if (!((long)m_didodJoy[nSum]->dwData < (-1 * DIJ_VOLUME))) return true;
			}
			break;
		case DIJ_DOWN :
			if (kmode == KD_TRG) {
				if ((long)m_didodJoy[nSum]->dwData > DIJ_VOLUME) return true;
			} else {
				if (!((long)m_didodJoy[nSum]->dwData > DIJ_VOLUME)) return true;
			}
		}
	} else {
		if (m_didodJoy[nSum]->dwOfs == 0) {	// X軸(左右)?
			switch (kcode) {
			case DIJ_LEFT :
				if (kmode == KD_TRG) {
					if ((long)m_didodJoy[nSum]->dwData < (-1 * DIJ_VOLUME)) return true;
				} else {
					if (!((long)m_didodJoy[nSum]->dwData < (-1 * DIJ_VOLUME))) return true;
				}
				break;
			case DIJ_RIGHT :
				if (kmode == KD_TRG) {
					if ((long)m_didodJoy[nSum]->dwData > DIJ_VOLUME) return true;
				} else {
					if (!((long)m_didodJoy[nSum]->dwData > DIJ_VOLUME)) return true;
				}
			}
		}
	}
	return false;
}

//-----------------------------------------------------------------------------
// キー押下チェック(joystick) 
// 
// 引数		const int&		kmode	チェックするモード
//			const DWORD&	kcode	チェックするキーコード　
//			int		nSum	ジョイスティック番号。省略値はJOY_PLAYER1
// 
// 戻り値	true:チェックＯＫ　false:チェックＮＧ
//-----------------------------------------------------------------------------
bool CDirectInput::CheckJoy(const int& kmode, const DWORD& kcode, int nSum)
{
	if (!m_bInputActive || !m_pJoy[nSum]) return false;


	switch (kmode) {
	case KD_DAT :						// 直接データ(現在のキー押下状態)
		return CheckJoyImm(kcode, nSum);
	case KD_TRG :						// キーを押した瞬間
	case KD_UTRG :						// キーを離した瞬間
		m_BufferRestJoy = m_BufferRestBackupJoy[nSum];	// 読み込んだバッファ数の復元
		m_pBufferPositionJoy = m_pBufferJoy[nSum];
		while (m_BufferRestJoy > 0) {
			m_BufferRestJoy--;
			m_didodJoy[nSum] = m_pBufferPositionJoy;
			m_pBufferPositionJoy++;
#if 0
				TCHAR chBuffer[128];
				_stprintf(chBuffer, _T("DInput.cpp : CheckNJoy() : cdwOfs=%ld, dwData=%ld\n"), m_didodJoy[nSum]->dwOfs, m_didodJoy[nSum]->dwData);
				ErrorMessage(chBuffer);
#endif
			if (kcode >= DIJ_LEFT && kcode <= DIJ_DOWN) {
				return CheckUpDownLeftRight(kmode, kcode, nSum);
			} else {
				if (m_didodJoy[nSum]->dwOfs == (DIJOFS_BUTTON(kcode))) {	//キーの種類
					if (kmode == KD_TRG) {
						if (m_didodJoy[nSum]->dwData) return true;
					} else {
						if (!(m_didodJoy[nSum]->dwData)) return true;
					}
				}
			}
		}
		return false;
	default :
		MessageBox(nullptr, _T("DInput.cpp : CheckJoy() : 指定したキーモードが間違っています。"), nullptr, MB_OK);
		return false;
	}
}
//-----------------------------------------------------------------------------
// フォースフィードバックディバイスかどうかをチェックする(joystick) 
//
// 引数		int nSum		ジョイスティック番号。省略値はJOY_PLAYER1
//
// 戻り値	true:フォースフィードバックディバイス　false:一般ディバイス
//-----------------------------------------------------------------------------
bool CDirectInput::IfJoyFF(int nSum)
{
	if(!m_pJoy[nSum]) return false;
	if( m_bJoyFF[nSum] ){	// ForceFeedback Joystickかどうか
		return true;
	}else{
		return false;
	}
}
//-----------------------------------------------------------------------------
// ジョイスティックのステートを得る
//
// 引数		int nSum		ジョイスティック番号。省略値はJOY_PLAYER1
//
// 戻り値	DIJOYSTATE2構造体の値。拡張機能を持つジョイスティック デバイスの状態を表す。
//			特に、LONG lX;　LONG lY;　は方向キーのアナログ値を表す。
//-----------------------------------------------------------------------------
DIJOYSTATE2 CDirectInput::GetJoyState(int nSum)
{
	return m_js[nSum];
}
//-----------------------------------------------------------------------------
// 接続されているジョイスティックの数を得る              // -- 2018.8.27
//
// 引数		なし
//
// 戻り値	int 接続されているジョイスティックの数
//-----------------------------------------------------------------------------
int CDirectInput::GetJoyNum(void)
{
	return m_nJoySum;
}

/*
//-----------------------------------------------------------------------------
// 標準効果の生成 (for ForceFeedback Joystick)
//                                                             !! OLD  !!
// （EF_BOUNCE､EF_FIRE､EF_EXPLODEの３つの効果を生成する）
//-----------------------------------------------------------------------------
bool CDirectInput::CreateJoyEffectStandard(void)
{
	DIEFFECT diEffect;
	DIENVELOPE diEnvelope;
	DWORD rgdwAxes[2];
	LONG rglDirections[2];
	DICONSTANTFORCE dicf;
	DIPERIODIC dipf;
	int i;

	ZeroMemory(&diEffect, sizeof(DIEFFECT));
	ZeroMemory(&diEnvelope, sizeof(DIENVELOPE));

	// these fields are the same for all effects we will be creating
	diEffect.dwSize = sizeof(DIEFFECT);
	diEffect.dwSamplePeriod = 0;		// use default sample period
	diEffect.dwTriggerButton = DIEB_NOTRIGGER;
	diEffect.dwTriggerRepeatInterval = 0;
	diEffect.rgdwAxes = rgdwAxes;
	diEffect.rglDirection = rglDirections;
	diEffect.dwGain = FF_BODYBUILDER;

	for (i = 0; i<m_nJoySum; i++) {
		if (!m_pJoy[i]) continue;
		if (!m_bJoyFF[i]) continue;	// 通常ジョイスティックを除く


		// BOUNCEエフェクトの生成
		if (m_pJoyEffect[i][EF_BOUNCE]) {
			m_pJoyEffect[i][EF_BOUNCE]->Release();
			m_pJoyEffect[i][EF_BOUNCE] = nullptr;
		}

		dicf.lMagnitude = 10000;

		rgdwAxes[0] = DIJOFS_X;
		rgdwAxes[1] = DIJOFS_Y;
		rglDirections[0] = 0;
		rglDirections[1] = 0;

		diEffect.dwFlags = DIEFF_OBJECTOFFSETS | DIEFF_POLAR;
		diEffect.dwDuration = 200000;
		diEffect.cAxes = 2;
		diEffect.lpEnvelope = nullptr;
		diEffect.cbTypeSpecificParams = sizeof(DICONSTANTFORCE);
		diEffect.lpvTypeSpecificParams = &dicf;

		if (m_pJoy[i]->CreateEffect(GUID_ConstantForce, &diEffect, &m_pJoyEffect[i][EF_BOUNCE], nullptr) != DI_OK) {
			MessageBox(nullptr, _T("DInput.cpp : CreateEffect() : CreateEffect(Bounce)が異常終了しました。"), nullptr, MB_OK);
			return false;
		}

		// FIREエフェクトの生成
		if (m_pJoyEffect[i][EF_FIRE]) {
			m_pJoyEffect[i][EF_FIRE]->Release();
			m_pJoyEffect[i][EF_FIRE] = nullptr;
		}

		dicf.lMagnitude = 10000;

		rgdwAxes[0] = DIJOFS_Y;
		rglDirections[0] = 1;

		diEffect.dwFlags = DIEFF_OBJECTOFFSETS | DIEFF_CARTESIAN;
		diEffect.dwDuration = 20000;
		diEffect.cAxes = 1;
		diEffect.lpEnvelope = nullptr;
		diEffect.cbTypeSpecificParams = sizeof(DICONSTANTFORCE);
		diEffect.lpvTypeSpecificParams = &dicf;

		if (m_pJoy[i]->CreateEffect(GUID_ConstantForce, &diEffect, &m_pJoyEffect[i][EF_FIRE], nullptr) != DI_OK) {
			MessageBox(nullptr, _T("DInput.cpp : CreateEffect() : CreateEffect(Fire)が異常終了しました。"), nullptr, MB_OK);
			return false;
		}

		// EXPLODEエフェクトの生成
		if (m_pJoyEffect[i][EF_EXPLODE]) {
			m_pJoyEffect[i][EF_EXPLODE]->Release();
			m_pJoyEffect[i][EF_EXPLODE] = nullptr;
		}

		diEnvelope.dwSize = sizeof(DIENVELOPE);
		diEnvelope.dwAttackLevel = 0;
		diEnvelope.dwAttackTime = 0;
		diEnvelope.dwFadeLevel = 0;
		diEnvelope.dwFadeTime = 1000000;

		dipf.dwMagnitude = 10000;
		dipf.lOffset = 0;
		dipf.dwPhase = 0;
		dipf.dwPeriod = 100000;

		rgdwAxes[0] = DIJOFS_X;
		rglDirections[0] = 0;

		diEffect.dwFlags = DIEFF_OBJECTOFFSETS | DIEFF_CARTESIAN;
		diEffect.dwDuration = 1000000;
		diEffect.cAxes = 1;
		diEffect.lpEnvelope = &diEnvelope;
		diEffect.cbTypeSpecificParams = sizeof(DIPERIODIC);
		diEffect.lpvTypeSpecificParams = &dipf;

		if (m_pJoy[i]->CreateEffect(GUID_Square, &diEffect, &m_pJoyEffect[i][EF_EXPLODE], nullptr) != DI_OK) {
			MessageBox(nullptr, _T("DInput.cpp : CreateEffect() : CreateEffect(Explode)が異常終了しました。"), nullptr, MB_OK);
			return false;
		}

		m_nJoyEFSum = 3;	// 標準効果が３つ登録された

	}

	return true;
}
*/

//-----------------------------------------------------------------------------
// 標準効果の生成 (for ForceFeedback Joystick)                  // -- 2018.8.27
//
// （EF_BOUNCE､EF_FIRE､EF_EXPLODEの３つの効果を生成する）
//-----------------------------------------------------------------------------
bool CDirectInput::CreateJoyEffectStandard(void)
{
	DIEFFECT diEffect;
	DIENVELOPE diEnvelope;
	DWORD rgdwAxes[2] = { 0,0 };
	LONG rglDirections[2] = { 0,0 };
	DICONSTANTFORCE dicf = { 0 };
	DIPERIODIC dipf = { 0 };
	int i;


	for ( i=0; i<m_nJoySum; i++) {
		if (!m_pJoy[i]) continue;
		if( !m_bJoyFF[i] ) continue;	// 通常ジョイスティックを除く

		// エフェクトの初期化
		ZeroMemory(&diEffect, sizeof(DIEFFECT));
		ZeroMemory(&diEnvelope, sizeof(DIENVELOPE));

		// these fields are the same for all effects we will be creating
		diEffect.dwSize = sizeof(DIEFFECT);
		diEffect.dwSamplePeriod = 0;		// use default sample period
		diEffect.dwTriggerButton = DIEB_NOTRIGGER;
		diEffect.dwTriggerRepeatInterval = 0;
		diEffect.rgdwAxes = rgdwAxes;           // rgdwAxes配列のアドレス設定
		diEffect.rglDirection = rglDirections;  // rglDirections配列のアドレス設定
		//diEffect.dwGain = FF_BODYBUILDER;    // 強さ　100%
		diEffect.dwGain = FF_ADULT;    // 強さ　75%                    // -- 2023.1.3

		// BOUNCEエフェクトの生成
		if (m_pJoyEffect[i][EF_BOUNCE]) {
			m_pJoyEffect[i][EF_BOUNCE]->Release();
			m_pJoyEffect[i][EF_BOUNCE] = nullptr;
		}

		dicf.lMagnitude = 10000;

		rgdwAxes[0] = DIJOFS_X;
		rgdwAxes[1] = DIJOFS_Y;
		rglDirections[0] = 0;
		rglDirections[1] = 0;

		diEffect.dwFlags = DIEFF_OBJECTOFFSETS | DIEFF_POLAR;
		diEffect.dwDuration = 200000;
		diEffect.cAxes = 2;
		diEffect.lpEnvelope = nullptr;
		diEffect.cbTypeSpecificParams = sizeof(DICONSTANTFORCE);
		diEffect.lpvTypeSpecificParams = &dicf;

		if (m_pJoy[i]->CreateEffect(GUID_ConstantForce, &diEffect, &m_pJoyEffect[i][EF_BOUNCE], nullptr) != DI_OK) {
			MessageBox(nullptr, _T("DInput.cpp : CreateEffect() : CreateEffect(Bounce)が異常終了しました。"), nullptr, MB_OK);
			return false;
		}

		// FIREエフェクトの生成
		if (m_pJoyEffect[i][EF_FIRE]) {
			m_pJoyEffect[i][EF_FIRE]->Release();
			m_pJoyEffect[i][EF_FIRE] = nullptr;
		}

		dicf.lMagnitude = 3000;   // -- 2018.8.27

		rgdwAxes[0] = DIJOFS_Y;
		rgdwAxes[1] = 0;          // -- 2018.8.27
		rglDirections[0] = 1;
		rglDirections[1] = 0;     // -- 2018.8.27

		diEffect.dwFlags = DIEFF_OBJECTOFFSETS | DIEFF_CARTESIAN;
		//diEffect.dwDuration = 20000;
		diEffect.dwDuration = 200000;           // -- 2023.1.3
		diEffect.cAxes = 2;       // -- 2018.8.27
		diEffect.lpEnvelope = nullptr;
		diEffect.cbTypeSpecificParams = sizeof(DICONSTANTFORCE);
		diEffect.lpvTypeSpecificParams = &dicf;

		if (m_pJoy[i]->CreateEffect(GUID_ConstantForce, &diEffect, &m_pJoyEffect[i][EF_FIRE], nullptr) != DI_OK) {
			MessageBox(nullptr, _T("DInput.cpp : CreateEffect() : CreateEffect(Fire)が異常終了しました。"), nullptr, MB_OK);
			return false;
		}
		
		// EXPLODEエフェクトの生成
		if(m_pJoyEffect[i][EF_EXPLODE]) {
			m_pJoyEffect[i][EF_EXPLODE]->Release();
			m_pJoyEffect[i][EF_EXPLODE] = nullptr;
		}

		diEnvelope.dwSize = sizeof(DIENVELOPE);
		diEnvelope.dwAttackLevel = 0;
		diEnvelope.dwAttackTime = 0;
		diEnvelope.dwFadeLevel = 0;
		diEnvelope.dwFadeTime = 1000000;

		dipf.dwMagnitude = 10000;
		dipf.lOffset = 0;
		dipf.dwPhase = 0;
		dipf.dwPeriod = 100000;

		rgdwAxes[0] = DIJOFS_X;
		rgdwAxes[1] = 0;        // -- 2018.8.27
		rglDirections[0] = 0;
		rglDirections[1] = 0;   // -- 2018.8.27

		diEffect.dwFlags = DIEFF_OBJECTOFFSETS | DIEFF_CARTESIAN;
		//diEffect.dwDuration = 1000000;
		diEffect.dwDuration = 2000000;     // -- 2023.1.3
		diEffect.cAxes = 1;
		diEffect.lpEnvelope = &diEnvelope;
		diEffect.cbTypeSpecificParams = sizeof(DIPERIODIC);
		diEffect.lpvTypeSpecificParams = &dipf;

		if (m_pJoy[i]->CreateEffect(GUID_Square, &diEffect, &m_pJoyEffect[i][EF_EXPLODE], nullptr) != DI_OK) {
			MessageBox(nullptr, _T("DInput.cpp : CreateEffect() : CreateEffect(Explode)が異常終了しました。"), nullptr, MB_OK);
			return false;
		}

		m_nJoyEFSum = 3;	// 標準効果が３つ登録された

	}

	return true;
}
//-----------------------------------------------------------------------------
// ForceFeedback効果列挙のコールバック関数  (for ForceFeedback Joystick)
//-----------------------------------------------------------------------------
BOOL CALLBACK EnumEffectsInFileProc(LPCDIFILEEFFECT lpdife, LPVOID pvRef )
{
	
	return g_pDI->EnumEffectsInFileCb( lpdife,  pvRef );

}
//-----------------------------------------------------------------------------
// ForceFeedback効果列挙のコールバック(for ForceFeedback Joystic)対応のメソッド  
//-----------------------------------------------------------------------------
BOOL CDirectInput::EnumEffectsInFileCb(LPCDIFILEEFFECT lpdife, LPVOID pvRef )
{
	HRESULT hr;

	hr = m_pJoy[m_nJoyEFI]->CreateEffect( lpdife->GuidEffect, lpdife->lpDiEffect,
												&m_pJoyEffect[m_nJoyEFI][m_nJoyEFSum], nullptr );
	if( DI_OK != hr){
		MessageBox(nullptr, _T("DInput.cpp : EnumEffectsInFileProc() : CreateEffect()が異常終了しました。"), nullptr, MB_OK);
		return DIENUM_CONTINUE;	// 次のデバイスを列挙
	}

	if( ++m_nJoyEFSum >=  JOY_EF_COUNT) return DIENUM_STOP;
	return DIENUM_CONTINUE;

}

//-----------------------------------------------------------------------------
// ForceFeedbackの効果の読み込み (for ForceFeedback Joy)
// 
// 引数		TCHAR* szFName	効果ファイル名
//			int nEffectNo	効果番号（戻り値）　
//			int nNum		一つの効果の個数（戻り値）
// 
// 戻り値	true:成功　false:失敗
//-----------------------------------------------------------------------------
bool CDirectInput::ReadJoyEffect(const TCHAR* szFName, int &nEffectNo, int &nNum)
{
	int  i, nWSum, nMaxSum;

	nMaxSum = nWSum = m_nJoyEFSum;
	nEffectNo = 0;
	nNum   = 0;

	for ( i=0; i<m_nJoySum; i++) {
		if( !m_pJoy[i] ) continue;
		if( !m_bJoyFF[i] )    continue;		// 通常ジョイスティックを除く

		m_nJoyEFSum = nWSum;
		m_nJoyEFI   = i;

		if( m_pJoy[i]->EnumEffectsInFile( szFName, EnumEffectsInFileProc, nullptr, DIFEF_MODIFYIFNEEDED ) != DI_OK ){
			MessageBox(nullptr, _T("DInput.cpp : ReadJoyEffect() : EnumEffectsInFile()が異常終了しました。"), nullptr, MB_OK);
			return false;
		}
		if( m_nJoyEFSum > nMaxSum )  nMaxSum = m_nJoyEFSum;
	}
	m_nJoyEFSum = nMaxSum;
	nEffectNo   = nWSum;
	nNum        = m_nJoyEFSum - nEffectNo;

	return true;
}

//-----------------------------------------------------------------------------
// ForceFeedbackの効果演奏
// 
// 引数		int nEffectNo	効果番号(ReadJoyEffect関数の戻り値を使用する)　
//			int nNum		一つの効果の個数(ReadJoyEffect関数の戻り値を使用する)。省略値は１
//			int nSum		ジョイスティック番号。省略値はJOY_PLAYER1
// 
// 戻り値	true:成功　false:失敗
//-----------------------------------------------------------------------------
bool CDirectInput::PlayJoyEffect(int nEffectNo, int nNum, int nSum)
{
	int  i;

	if( !m_bJoyFF[nSum] ) return false;	// 通常ジョイスティックを除く

	for( i = 0; i < nNum; i++ ){
		if (m_pJoyEffect[nSum][nEffectNo+i]) {
			if (m_pJoyEffect[nSum][nEffectNo+i]->Start(1, 0) != DI_OK) {
				MessageBox(nullptr, _T("DInput.cpp : PlayJoyEffect() : Start(Joy effect)が異常終了しました。"), nullptr, MB_OK);
				return false;
			}
		}
	}
	return true;
}
