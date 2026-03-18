//-----------------------------------------------------------------------------
// DInput・ｽﾖ撰ｿｽ : DirectInput・ｽ・ｽ・ｽﾈ単・ｽﾉ暦ｿｽ・ｽp・ｽ・ｽ・ｽ驍ｽ・ｽﾟの・ｿｽ・ｽC・ｽu・ｽ・ｽ・ｽ・ｽ(・ｽ{・ｽ・ｽ)
// 
//                                          ver 3.3        2024.3.23
//
// Copyright (c) 2019 ・ｽﾃ会ｿｽ・ｽY・ｽﾆ技・ｽp・ｽ・ｽ・ｽw・ｽZ ・ｽQ・ｽ[・ｽ・ｽ・ｽN・ｽ・ｽ・ｽG・ｽC・ｽg・ｽ・ｽ All rights reserved.
//-----------------------------------------------------------------------------
#include <stdio.h>
#include "DInput.h"


// ・ｽO・ｽ・ｽ・ｽ[・ｽo・ｽ・ｽ・ｽﾏ撰ｿｽ(・ｽR・ｽ[・ｽ・ｽ・ｽo・ｽb・ｽN・ｽﾖ撰ｿｽ・ｽ・ｽ・ｽg・ｽp・ｽ・ｽ・ｽ驍ｽ・ｽﾟ必・ｽv)
static CDirectInput*    g_pDI = nullptr;

//-----------------------------------------------------------------------------
// ・ｽR・ｽ・ｽ・ｽX・ｽg・ｽ・ｽ・ｽN・ｽ^
//-----------------------------------------------------------------------------
CDirectInput::CDirectInput(void)
{
    ZeroMemory(this, sizeof(CDirectInput));
    m_bInputActive = true;

    g_pDI = this;
}
//-----------------------------------------------------------------------------
// ・ｽf・ｽX・ｽg・ｽ・ｽ・ｽN・ｽ^
//-----------------------------------------------------------------------------
CDirectInput::~CDirectInput()
{
    EndDirectInput();
}
//-----------------------------------------------------------------------------
// DirectInput・ｽﾌ開・ｽn
// 
// ・ｽ・ｽ・ｽ・ｽ    HINSTANCE  hInst  ・ｽC・ｽ・ｽ・ｽX・ｽ^・ｽ・ｽ・ｽX・ｽn・ｽ・ｽ・ｽh・ｽ・ｽ
//        HWND      hWnd   ・ｽE・ｽB・ｽ・ｽ・ｽh・ｽE・ｽn・ｽ・ｽ・ｽh・ｽ・ｽ
//        int          flag   ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽf・ｽB・ｽo・ｽC・ｽX・ｽﾌ選・ｽ・ｽ・ｽiINIT_KEYBOARD|INIT_MOUSE|INIT_JOYSTICK・ｽj
//        DWORD     dwWidth    ・ｽE・ｽB・ｽ・ｽ・ｽh・ｽﾌ包ｿｽ
//        DWORD     dwHeight・ｽE・ｽB・ｽ・ｽ・ｽh・ｽﾌ搾ｿｽ・ｽ・ｽ
// 
// ・ｽﾟゑｿｽl true:・ｽ・ｽ・ｽ・ｽ・ｽ@false:・ｽ・ｽ・ｽs
//-----------------------------------------------------------------------------
bool CDirectInput::StartDirectInput(HINSTANCE hInst, HWND hWnd, int flag, DWORD dwWidth, DWORD dwHeight)
{
    // DirectInput8・ｽI・ｽu・ｽW・ｽF・ｽN・ｽg・ｽﾌ作成
    m_hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&m_pDI8, nullptr);
    if (DI_OK != m_hr) {
       MessageBox(nullptr, _T("DInput.cpp : StartDirectInput() : DirectInput・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ驍ｱ・ｽﾆゑｿｽ・ｽﾅゑｿｽ・ｽﾜゑｿｽ・ｽ・ｽB"), nullptr, MB_OK);
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
// DirectInput・ｽﾌ終・ｽ・ｽ
// 
// ・ｽﾟゑｿｽl true:・ｽ・ｽ・ｽ・ｽ・ｽ@false:・ｽ・ｽ・ｽs
//-----------------------------------------------------------------------------
bool CDirectInput::EndDirectInput(void)
{
    // DirectInputDevice(keyboard)・ｽ・ｽ・ｽ・ｽ・ｽ
    if( m_pBufferKey ){
       delete []m_pBufferKey; // ・ｽo・ｽb・ｽt・ｽ@・ｽ・ｽ・ｽ・ｽ・ｽ
       m_pBufferKey = nullptr;
    }
    if (m_pKey) {
       m_pKey->Unacquire();
       m_pKey->Release();
       m_pKey = nullptr;
    }

    // DirectInputDevice(mouse)・ｽ・ｽ・ｽ・ｽ・ｽ
    if( m_pBufferMouse ){
       delete []m_pBufferMouse;   // ・ｽo・ｽb・ｽt・ｽ@・ｽ・ｽ・ｽ・ｽ・ｽ
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
          // DirectInputDevice(ForceFeedback Joystick)・ｽ・ｽ・ｽ・ｽ・ｽ
          for (j=0; j<m_nJoyEFSum; j++) {
             if( m_pJoyEffect[i][j] ){
                m_pJoyEffect[i][j]->Unload();
                m_pJoyEffect[i][j]->Release();
                m_pJoyEffect[i][j] = nullptr;
             }
          }

       }

       // DirectInputDevice(joystick)・ｽ・ｽ・ｽ・ｽ・ｽ
       if( m_pBufferJoy[i] ){
          delete []m_pBufferJoy[i];  // ・ｽo・ｽb・ｽt・ｽ@・ｽ・ｽ・ｽ・ｽ・ｽ
          m_pBufferJoy[i] = nullptr;
       }

       if (m_pJoy[i]) {
          m_pJoy[i]->Unacquire();
          m_pJoy[i]->Release();
          m_pJoy[i] = nullptr;
       }
    }

    // DirectInput・ｽ・ｽ・ｽ・ｽ・ｽ
    if (m_pDI8) {
       m_pDI8->Release();
       m_pDI8 = nullptr;
    }

    return true;
}
//-----------------------------------------------------------------------------
// ・ｽ・ｽ・ｽﾌ吐・ｽo・ｽC・ｽX・ｽﾌア・ｽN・ｽZ・ｽX・ｽ・ｽ・ｽﾌ撰ｿｽ・ｽ・ｽ
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
// ・ｽf・ｽ[・ｽ^・ｽﾇみ搾ｿｽ・ｽ・ｽ
// 
// ・ｽﾟゑｿｽl true:・ｽ・ｽ・ｽ・ｽ・ｽ@false:・ｽ・ｽ・ｽs
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
// Keyboard・ｽf・ｽo・ｽC・ｽX・ｽﾌ擾ｿｽ・ｽ・ｽ・ｽ・ｽ
// 
// ・ｽ・ｽ・ｽ・ｽ    HWND hWnd  ・ｽE・ｽB・ｽ・ｽ・ｽh・ｽE・ｽn・ｽ・ｽ・ｽh・ｽ・ｽ
// 
// ・ｽﾟゑｿｽl true:・ｽ・ｽ・ｽ・ｽ・ｽ@false:・ｽ・ｽ・ｽs
//-----------------------------------------------------------------------------
bool CDirectInput::InitKey(HWND hWnd)
{
    //-----------------------------------------------------------------------------
    // keyboard・ｽf・ｽo・ｽC・ｽX・ｽﾌイ・ｽ・ｽ・ｽX・ｽ^・ｽ・ｽ・ｽX・ｽ・ｬ・ｽE・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ
    m_hr = m_pDI8->CreateDevice(GUID_SysKeyboard, &m_pKey, nullptr);
    if (DI_OK != m_hr) {
       MessageBox(nullptr, _T("DInput.cpp : StartDirectInput() : DirectInputDeviceEx(keyboard)・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ驍ｱ・ｽﾆゑｿｽ・ｽﾅゑｿｽ・ｽﾜゑｿｽ・ｽ・ｽB"),nullptr,MB_OK);
       return false;
    }
    // DirectInput・ｽf・ｽo・ｽC・ｽX・ｽﾌデ・ｽ[・ｽ^・ｽ`・ｽ・ｽ・ｽﾌ設抵ｿｽ
    m_pKey->SetDataFormat(&c_dfDIKeyboard);
    if (DI_OK != m_hr) {
       MessageBox(nullptr, _T("DInput.cpp : StartDirectInput() : DirectInput・ｽf・ｽo・ｽC・ｽX・ｽﾌデ・ｽ[・ｽ^・ｽ`・ｽ・ｽ・ｽﾌ設抵ｿｽﾉ趣ｿｽ・ｽs・ｽ・ｽ・ｽﾜゑｿｽ・ｽ・ｽ・ｽB"),nullptr,MB_OK);
       return false;
    }
    // keyboard・ｽf・ｽo・ｽC・ｽX・ｽﾌイ・ｽ・ｽ・ｽX・ｽ^・ｽ・ｽ・ｽX・ｽﾉ対ゑｿｽ・ｽ驪ｦ・ｽ・ｽ・ｽ・ｽ・ｽx・ｽ・ｽ・ｽﾌ確・ｽ・ｽ
    m_pKey->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
    if (DI_OK != m_hr) {
       MessageBox(nullptr, _T("DInput.cpp : StartDirectInput() : keyboard・ｽf・ｽo・ｽC・ｽX・ｽﾌイ・ｽ・ｽ・ｽX・ｽ^・ｽ・ｽ・ｽX・ｽﾉ対ゑｿｽ・ｽ驪ｦ・ｽ・ｽ・ｽ・ｽ・ｽx・ｽ・ｽ・ｽﾌ確・ｽ・ｽ・ｽﾉ趣ｿｽ・ｽs・ｽ・ｽ・ｽﾜゑｿｽ・ｽ・ｽ・ｽB"),nullptr,MB_OK);
       return false;
    }
    return SetPropertyKey();
}
//-----------------------------------------------------------------------------
// ・ｽo・ｽb・ｽt・ｽ@・ｽT・ｽC・ｽY・ｽﾌ設抵ｿｽ(keyboard)
// 
// ・ｽﾟゑｿｽl true:・ｽ・ｽ・ｽ・ｽ・ｽ@false:・ｽ・ｽ・ｽs
//-----------------------------------------------------------------------------
bool CDirectInput::SetPropertyKey(void)
{
    m_BufferRestKey = 0;
    m_pBufferPositionKey = nullptr;
    m_pBufferKey = new DIDEVICEOBJECTDATA[BufferSize]; // ・ｽo・ｽb・ｽt・ｽ@・ｽ・ｽ・ｽm・ｽ・ｽ
    DIPROPDWORD dipdw;
    ZeroMemory(&dipdw, sizeof(dipdw));
    dipdw.diph.dwSize = sizeof(DIPROPDWORD);
    dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    dipdw.diph.dwObj = 0;
    dipdw.diph.dwHow = DIPH_DEVICE;
    dipdw.dwData = BufferSize;
    m_pKey->Unacquire();            // ・ｽ・ｽU・ｽA・ｽN・ｽZ・ｽX・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ
    m_hr = m_pKey->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);
    m_pKey->Acquire();             // ・ｽA・ｽN・ｽZ・ｽX・ｽ・ｽ・ｽｾゑｿｽ
    return !FAILED(m_hr);
}
//-----------------------------------------------------------------------------
// ・ｽL・ｽ[・ｽ{・ｽ[・ｽh・ｽﾌ擾ｿｽﾔを得ゑｿｽ
// 
// ・ｽﾟゑｿｽl true:・ｽ・ｽ・ｽ・ｽ・ｽ@false:・ｽ・ｽ・ｽs
//-----------------------------------------------------------------------------
bool CDirectInput::GetKey(void)
{
    if (!m_pKey) return false;
    if (m_pKey->GetDeviceState(sizeof(m_diKeyState), m_diKeyState) != DI_OK) {
       m_pKey->Acquire();
       if (FAILED(m_pKey->GetDeviceState(sizeof(m_diKeyState), m_diKeyState))) {
          //MessageBox(nullptr,_T("DInput.cpp : GetKey() : GetDeviceState・ｽ・ｽ・ｽﾙ擾ｿｽI・ｽ・ｽ・ｽ・ｽ・ｽﾜゑｿｽ・ｽ・ｽ・ｽB") ,nullptr,MB_OK );
          return false;
       }
    }

    m_BufferRestKey = BufferSize;
    if (FAILED(m_pKey->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), m_pBufferKey, &m_BufferRestKey, 0))) {
       // ・ｽ・ｽ・ｽs・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ1・ｽx・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽﾝゑｿｽ
       m_BufferRestKey = BufferSize;
       m_pKey->Acquire();
       if (FAILED(m_pKey->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), m_pBufferKey, &m_BufferRestKey, 0))) {
          //MessageBox(nullptr,_T("DInput.cpp : GetKey() : GetDeviceData・ｽ・ｽ・ｽﾙ擾ｿｽI・ｽ・ｽ・ｽ・ｽ・ｽﾜゑｿｽ・ｽ・ｽ・ｽB"), nullptr, MB_OK);
          return false;
       }
    }
    m_BufferRestBackupKey = m_BufferRestKey;   // ・ｽﾇみ搾ｿｽ・ｽｾバ・ｽb・ｽt・ｽ@・ｽ・ｽ・ｽ・ｽﾞ費ｿｽ

    return true;
}
//-----------------------------------------------------------------------------
// ・ｽL・ｽ[・ｽ・ｽ・ｽ・ｽ・ｽ`・ｽF・ｽb・ｽN(keyboard)
// 
// ・ｽ・ｽ・ｽ・ｽ    const int&    kmode  ・ｽ`・ｽF・ｽb・ｽN・ｽ・ｽ・ｽ驛ゑｿｽ[・ｽh
//        const DWORD&   kcode  ・ｽ`・ｽF・ｽb・ｽN・ｽ・ｽ・ｽ・ｽL・ｽ[・ｽR・ｽ[・ｽh・ｽ@
// 
// ・ｽﾟゑｿｽl true:・ｽ`・ｽF・ｽb・ｽN・ｽn・ｽj・ｽ@false:・ｽ`・ｽF・ｽb・ｽN・ｽm・ｽf
//-----------------------------------------------------------------------------
bool CDirectInput::CheckKey(const int& kmode, const DWORD& kcode)
{
    if (!m_bInputActive || !m_pKey) return false;
    switch (kmode) {
    case KD_DAT :                 // ・ｽ・ｽ・ｽﾚデ・ｽ[・ｽ^(・ｽ・ｽ・ｽﾝのキ・ｽ[・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ)
       return m_diKeyState[kcode] & 0x80;
    case KD_TRG :                 // ・ｽL・ｽ[・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽu・ｽ・ｽ
    case KD_UTRG :                // ・ｽL・ｽ[・ｽ｣ゑｿｽ・ｽ・ｽ・ｽu・ｽ・ｽ
       m_BufferRestKey = m_BufferRestBackupKey;   // ・ｽﾇみ搾ｿｽ・ｽｾバ・ｽb・ｽt・ｽ@・ｽ・ｽ・ｽﾌ包ｿｽ・ｽ・ｽ
       m_pBufferPositionKey = m_pBufferKey;
       while (m_BufferRestKey > 0) {
          m_BufferRestKey--;
          m_didodKey = m_pBufferPositionKey;
          m_pBufferPositionKey++;
          if (m_didodKey->dwOfs == kcode) {  //・ｽL・ｽ[・ｽﾌ趣ｿｽ・ｽ
             if (kmode == KD_TRG) {
                if (m_didodKey->dwData) return true;
             } else {
                if (!(m_didodKey->dwData)) return true;
             }
          }
       }
       return false;
    default :
       MessageBox(nullptr, _T("DInput.cpp : CheckKey() : ・ｽw・ｽ閧ｵ・ｽ・ｽ・ｽL・ｽ[・ｽ・ｽ・ｽ[・ｽh・ｽ・ｽ・ｽﾔ茨ｿｽ・ｽ・ｽﾄゑｿｽ・ｽﾜゑｿｽ・ｽB"), nullptr, MB_OK);
       return false;
    }
}
//*****************************************************************************
//*** Mouse                                                                 ***
//*****************************************************************************
//-----------------------------------------------------------------------------
// Mouse・ｽf・ｽo・ｽC・ｽX・ｽﾌ擾ｿｽ・ｽ・ｽ・ｽ・ｽ
// 
// ・ｽ・ｽ・ｽ・ｽ    HWND hWnd  ・ｽE・ｽB・ｽ・ｽ・ｽh・ｽE・ｽn・ｽ・ｽ・ｽh・ｽ・ｽ
// 
// ・ｽﾟゑｿｽl true:・ｽ・ｽ・ｽ・ｽ・ｽ@false:・ｽ・ｽ・ｽs
//-----------------------------------------------------------------------------
bool CDirectInput::InitMouse(HWND hWnd)
{
    //-----------------------------------------------------------------------------
    // DirectInputDevice・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ(mouse)
    m_hr = m_pDI8->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr);
    if (DI_OK != m_hr) {
       MessageBox(nullptr, _T("DInput.cpp : StartDirectInput() : DirectInputDeviceEx(mouse)"),nullptr,MB_OK);
       return false;
    }
    if (m_pMouse->SetDataFormat(&c_dfDIMouse) != DI_OK) {
       MessageBox(nullptr, _T("DInput.cpp : StartDirectInput() : SetDataFormat"), nullptr, MB_OK);
       return false;
    }
    if (m_pMouse->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE) != DI_OK) {
       MessageBox(nullptr, _T("DInput.cpp : StartDirectInput() : SetCooperativeLevel"), nullptr, MB_OK);
       return false;
    }

    return SetPropertyMouse();
}
//-----------------------------------------------------------------------------
// ・ｽo・ｽb・ｽt・ｽ@・ｽT・ｽC・ｽY・ｽﾌ設抵ｿｽ(mouse)
// 
// ・ｽﾟゑｿｽl true:・ｽ・ｽ・ｽ・ｽ・ｽ@false:・ｽ・ｽ・ｽs
//-----------------------------------------------------------------------------
bool CDirectInput::SetPropertyMouse(void)
{
    m_BufferRestMouse = 0;
    m_pBufferPositionMouse = nullptr;
    m_pBufferMouse = new DIDEVICEOBJECTDATA[BufferSize];   // ・ｽo・ｽb・ｽt・ｽ@・ｽ・ｽ・ｽm・ｽ・ｽ
    DIPROPDWORD dipdw;
    ZeroMemory(&dipdw, sizeof(dipdw));
    dipdw.diph.dwSize = sizeof(DIPROPDWORD);
    dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    dipdw.diph.dwObj = 0;
    dipdw.diph.dwHow = DIPH_DEVICE;
    dipdw.dwData = BufferSize;
    m_pMouse->Unacquire();          // ・ｽ・ｽU・ｽA・ｽN・ｽZ・ｽX・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ
    m_hr = m_pMouse->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);
    m_pMouse->Acquire();               // ・ｽA・ｽN・ｽZ・ｽX・ｽ・ｽ・ｽｾゑｿｽ
    return !FAILED(m_hr);
}
//-----------------------------------------------------------------------------
// ・ｽ}・ｽE・ｽX・ｽﾌ擾ｿｽﾔを得ゑｿｽ
// 
// ・ｽﾟゑｿｽl true:・ｽ・ｽ・ｽ・ｽ・ｽ@false:・ｽ・ｽ・ｽs
//-----------------------------------------------------------------------------
bool CDirectInput::GetMouse(void)
{
    if (!m_pMouse) return false;
    if (m_pMouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_dims) != DI_OK) {
       m_pMouse->Acquire();
       if (FAILED(m_pMouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_dims))) {
          //MessageBox(nullptr,_T("DInput.cpp : GetMouse() : GetDeviceState・ｽ・ｽ・ｽﾙ擾ｿｽI・ｽ・ｽ・ｽ・ｽ・ｽﾜゑｿｽ・ｽ・ｽ・ｽB"), nullptr, MB_OK);
          return false;
       }
    }

    m_BufferRestMouse = BufferSize;
    if (FAILED(m_pMouse->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), m_pBufferMouse, &m_BufferRestMouse, 0))) {
       // ・ｽ・ｽ・ｽs・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ1・ｽx・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽﾝゑｿｽ
       m_BufferRestMouse = BufferSize;
       m_pMouse->Acquire();
       if (FAILED(m_pMouse->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), m_pBufferMouse, &m_BufferRestMouse, 0))) {
          //MessageBox(nullptr,_T("DInput.cpp : GetMouse() : GetDeviceData・ｽ・ｽ・ｽﾙ擾ｿｽI・ｽ・ｽ・ｽ・ｽ・ｽﾜゑｿｽ・ｽ・ｽ・ｽB"), nullptr, MB_OK);
          return false;
       }
    }
    m_BufferRestBackupMouse = m_BufferRestMouse;   // ・ｽﾇみ搾ｿｽ・ｽｾバ・ｽb・ｽt・ｽ@・ｽ・ｽ・ｽ・ｽﾞ費ｿｽ

    return true;
}
//-----------------------------------------------------------------------------
// ・ｽL・ｽ[・ｽ・ｽ・ｽ・ｽ・ｽ`・ｽF・ｽb・ｽN(mouse)
// 
// ・ｽ・ｽ・ｽ・ｽ    const int&    kmode  ・ｽ`・ｽF・ｽb・ｽN・ｽ・ｽ・ｽ驛ゑｿｽ[・ｽh
//        const DWORD&   kcode  ・ｽ`・ｽF・ｽb・ｽN・ｽ・ｽ・ｽ・ｽL・ｽ[・ｽR・ｽ[・ｽh・ｽ@
// 
// ・ｽﾟゑｿｽl true:・ｽ`・ｽF・ｽb・ｽN・ｽn・ｽj・ｽ@false:・ｽ`・ｽF・ｽb・ｽN・ｽm・ｽf
//-----------------------------------------------------------------------------
bool CDirectInput::CheckMouse(const int& kmode, const DWORD& kcode)
{
    if (!m_bInputActive || !m_pMouse) return false;

    // ・ｽﾚ難ｿｽ・ｽ・ｽ・ｽ・ｽ・ｽﾌ取得・ｽﾌとゑｿｽ
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

    }else{ // ・ｽe・ｽL・ｽ[・ｽ・ｽﾔの取得・ｽﾌとゑｿｽ

       switch (kmode) {
          case KD_DAT:                  // ・ｽ・ｽ・ｽﾚデ・ｽ[・ｽ^(・ｽ・ｽ・ｽﾝのキ・ｽ[・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ)
             return m_dims.rgbButtons[kcode] & 0x80;
          case KD_TRG:                  // ・ｽL・ｽ[・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽu・ｽ・ｽ
          case KD_UTRG:                 // ・ｽL・ｽ[・ｽ｣ゑｿｽ・ｽ・ｽ・ｽu・ｽ・ｽ
             m_BufferRestMouse = m_BufferRestBackupMouse;   // ・ｽﾇみ搾ｿｽ・ｽｾバ・ｽb・ｽt・ｽ@・ｽ・ｽ・ｽﾌ包ｿｽ・ｽ・ｽ
             m_pBufferPositionMouse = m_pBufferMouse;
             while (m_BufferRestMouse > 0) {
                m_BufferRestMouse--;
                m_didodMouse = m_pBufferPositionMouse;
                m_pBufferPositionMouse++;
                if (m_didodMouse->dwOfs == DIMOFS_BUTTON(kcode)) { //・ｽL・ｽ[・ｽﾌ趣ｿｽ・ｽ
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
             MessageBox(nullptr, _T("DInput.cpp : CheckMouse() : ・ｽw・ｽ閧ｵ・ｽ・ｽ・ｽL・ｽ[・ｽ・ｽ・ｽ[・ｽh・ｽ・ｽ・ｽﾔ茨ｿｽ・ｽ・ｽﾄゑｿｽ・ｽﾜゑｿｽ・ｽB"), nullptr, MB_OK);
             return false;
       }
    }

}
//-----------------------------------------------------------------------------
// ・ｽ}・ｽE・ｽX・ｽX・ｽe・ｽ[・ｽg・ｽｾゑｿｽ(mouse)
//
// ・ｽﾟゑｿｽl DIMOUSESTATE・ｽ\・ｽ・ｽ・ｽﾌの値・ｽB・ｽ}・ｽE・ｽX・ｽf・ｽo・ｽC・ｽX・ｽﾌ擾ｿｽﾔゑｿｽ\・ｽ・ｽ・ｽB
//-----------------------------------------------------------------------------
DIMOUSESTATE CDirectInput::GetMouseState( void )
{
    return  m_dims;
}

float CDirectInput::GetMouseWheel() const
{
   return static_cast<float>(m_dims.lZ);
}

bool CDirectInput::IsMouseMove() const
{
   return (m_dims.lX != 0 || m_dims.lY != 0);
}

bool CDirectInput::IsMoveInput()
{
   if (CheckKey(KD_DAT, DIK_W)) return true;
   if (CheckKey(KD_DAT, DIK_A)) return true;
   if (CheckKey(KD_DAT, DIK_S)) return true;
   if (CheckKey(KD_DAT, DIK_D)) return true;
   return false;
}

//-----------------------------------------------------------------------------
// ・ｽ}・ｽE・ｽX・ｽ・ｽ・ｽW・ｽｾゑｿｽ(mouse)
//
// ・ｽ・ｽ・ｽﾌ関撰ｿｽ・ｽ・ｽDirectInput・ｽﾅはなゑｿｽWindows・ｽﾌ関撰ｿｽ・ｽ・ｽ・ｽg・ｽp・ｽ・ｽ・ｽﾄゑｿｽ・ｽ・ｽ
//
// ・ｽﾟゑｿｽl POINT・ｽ\・ｽ・ｽ・ｽﾌの値・ｽB・ｽ}・ｽE・ｽX・ｽﾌＸ・ｽx・ｽ・ｽ・ｽW・ｽ・ｽ\・ｽ・ｽ・ｽB
//-----------------------------------------------------------------------------
POINT CDirectInput::GetMousePos(void)
{
    POINT pt;
    RECT  rc;
    float scaleX, scaleY;
    GetCursorPos(&pt); //・ｽ}・ｽE・ｽX・ｽﾌ鯉ｿｽ・ｽﾝのス・ｽN・ｽ・ｽ・ｽ[・ｽ・ｽ・ｽ・ｽ・ｽW・ｽ・ｽ・ｽ謫ｾ・ｽ・ｽ・ｽ・ｽ
    ScreenToClient(m_hWnd, &pt); // ・ｽN・ｽ・ｽ・ｽC・ｽA・ｽ・ｽ・ｽg・ｽ・ｽ・ｽW・ｽﾉ変奇ｿｽ・ｽ・ｽ・ｽ・ｽ

    // ・ｽ・ｽﾊの拡・ｽk・ｽﾉ対会ｿｽ・ｽ・ｽ・ｽﾄマ・ｽE・ｽX・ｽ・ｽ・ｽW・ｽ・ｽ・ｽv・ｽZ・ｽ・ｽ・ｽ・ｽ
    GetClientRect(m_hWnd, &rc); // ・ｽN・ｽ・ｽ・ｽC・ｽA・ｽ・ｽ・ｽg・ｽﾌ茨ｿｽﾌサ・ｽC・ｽY
    scaleX = m_ViewWidth  / (rc.right - rc.left);  // ・ｽ・ｽ・ｽ@・ｽ范ｦ
    scaleY = m_ViewHeight / (rc.bottom - rc.top);  // ・ｽ・ｽ・ｽ・ｽ・ｽ范ｦ
    
    pt.x = (long)(pt.x * scaleX);
    pt.y = (long)(pt.y * scaleY);

    return pt;
}
//-----------------------------------------------------------------------------
// ・ｽ}・ｽE・ｽX・ｽJ・ｽ[・ｽ\・ｽ・ｽ・ｽﾌ表・ｽ・ｽ・ｽE・ｽ・ｽ\・ｽ・ｽ・ｽ・ｽﾘゑｿｽﾖゑｿｽ・ｽ・ｽ
//
// ・ｽ・ｽ・ｽﾌ関撰ｿｽ・ｽ・ｽDirectInput・ｽﾅはなゑｿｽWindows・ｽﾌ関撰ｿｽ・ｽ・ｽ・ｽg・ｽp・ｽ・ｽ・ｽﾄゑｿｽ・ｽ・ｽ
//
// ・ｽ・ｽ・ｽ・ｽ  bool bFlag  true:・ｽJ・ｽ[・ｽ\・ｽ・ｽ・ｽ\・ｽ・ｽ  FLASE:・ｽJ・ｽ[・ｽ\・ｽ・ｽ・ｽ・ｽ\・ｽ・ｽ
//
// ・ｽﾟゑｿｽl ・ｽﾈゑｿｽ
//-----------------------------------------------------------------------------
void CDirectInput::ShowMouseCursor(bool bFlag)
{
    ShowCursor(bFlag);
}

//*****************************************************************************
//*** Joystick・ｽﾌ擾ｿｽ・ｽ・ｽ                                                        ***
//*****************************************************************************
//-----------------------------------------------------------------------------
// ・ｽ塔R・ｽ[・ｽ・ｽ・ｽo・ｽb・ｽN・ｽﾖ撰ｿｽ (Joystick)
//-----------------------------------------------------------------------------
BOOL CALLBACK EnumJoysticksCallback(LPCDIDEVICEINSTANCE pInst, LPVOID lpvContext)
{
    return g_pDI->EnumJoysticksCb( pInst, lpvContext );
}

//-----------------------------------------------------------------------------
// ・ｽ塔R・ｽ[・ｽ・ｽ・ｽo・ｽb・ｽN (Joystick)・ｽﾎ会ｿｽ・ｽ・ｽ・ｽ\・ｽb・ｽh
//-----------------------------------------------------------------------------
BOOL CDirectInput::EnumJoysticksCb(LPCDIDEVICEINSTANCE pInst, LPVOID lpvContext)
{
    LPDIRECTINPUTDEVICE8 pDevice = nullptr;
    DIDEVCAPS            diDevCaps = { 0 };

    m_hr = m_pDI8->CreateDevice(pInst->guidInstance, &pDevice, nullptr);
    if (DI_OK != m_hr) return DIENUM_CONTINUE; // ・ｽ・ｽ・ｽﾌデ・ｽo・ｽC・ｽX・ｽ・ｽ・ｽ

    diDevCaps.dwSize = sizeof( DIDEVCAPS );
    m_hr = pDevice->GetCapabilities( &diDevCaps );
    if( FAILED(m_hr) ){
       pDevice->Release();
       pDevice = nullptr;
       return DIENUM_CONTINUE;    // ・ｽ・ｽ・ｽﾌデ・ｽo・ｽC・ｽX・ｽ・ｽ・ｽ
    }

    m_pJoy[m_nJoySum] = pDevice;

    //if( diDevCaps.dwFlags == DIDC_FORCEFEEDBACK ){
    if( pInst->guidFFDriver != GUID_NULL ){
       m_bJoyFF[m_nJoySum] = true;       // ・ｽt・ｽH・ｽ[・ｽX・ｽt・ｽB・ｽ[・ｽh・ｽo・ｽb・ｽN・ｽW・ｽ・ｽ・ｽC・ｽX・ｽX・ｽe・ｽB・ｽb・ｽN
       m_nJoyFFNum++;
    }else{
       m_bJoyFF[m_nJoySum] = false;   // ・ｽﾊ擾ｿｽﾌジ・ｽ・ｽ・ｽC・ｽX・ｽe・ｽB・ｽb・ｽN
    }

    if (++m_nJoySum >= JOYSTICK_COUNT) return DIENUM_STOP;
    return DIENUM_CONTINUE;
}


//-----------------------------------------------------------------------------
// Joystick・ｽf・ｽo・ｽC・ｽX・ｽﾌ擾ｿｽ・ｽ・ｽ・ｽ・ｽ
// 
// ・ｽ・ｽ・ｽ・ｽ    HWND hWnd  ・ｽE・ｽB・ｽ・ｽ・ｽh・ｽE・ｽn・ｽ・ｽ・ｽh・ｽ・ｽ
// 
// ・ｽﾟゑｿｽl true:・ｽ・ｽ・ｽ・ｽ・ｽ@false:・ｽ・ｽ・ｽs
//-----------------------------------------------------------------------------
bool CDirectInput::InitJoy(HWND hWnd)
{
    //-----------------------------------------------------------------------------
    // ・ｽA・ｽ^・ｽb・ｽ`・ｽﾂ能・ｽﾈデ・ｽo・ｽC・ｽX・ｽ・ｽ唐・ｽ・ｽ・ｽ(joystick)
    m_pDI8->EnumDevices( DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback, nullptr, DIEDFL_ATTACHEDONLY);
    if(!m_pJoy[0]) {
       //MessageBox(nullptr,_T("DInput.cpp : StartDirectInput() : joystick・ｽ・ｽ・ｽﾚ托ｿｽ・ｽ・ｽ・ｽ・ｽﾄゑｿｽ・ｽﾜゑｿｽ・ｽ・ｽB"), nullptr, MB_OK);
       return false;
    }
    for (int i=0; i<m_nJoySum; i++) {
       if (!m_pJoy[i]) continue;

       m_hr = m_pJoy[i]->SetDataFormat(&c_dfDIJoystick2);
       if (DI_OK != m_hr) {
          MessageBox(nullptr, _T("DInput.cpp : StartDirectInput()"),nullptr,MB_OK);
          return false;
       }

       if( m_nJoyFFNum > 0 ){ // ・ｽe・ｽe・ｽW・ｽ・ｽ・ｽC・ｽX・ｽe・ｽB・ｽb・ｽN・ｽ・ｽ・ｽP・ｽﾂでゑｿｽ・ｽ・ｽ・ｽ・ｽﾆゑｿｽ・ｽﾌ、・ｽS・ｽ・ｽ・ｽ・ｽr・ｽ・ｽ・ｽ・ｽ・ｽ[・ｽh・ｽﾉゑｿｽ・ｽ・ｽ
          m_hr = m_pJoy[i]->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
       }else{
          m_hr = m_pJoy[i]->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
       }
       if (DI_OK != m_hr) {
          MessageBox(nullptr, _T("DInput.cpp : StartDirectInput()"),nullptr,MB_OK);
          return false;
       }
    }

    InitJoyNormal(hWnd);   // ・ｽﾊ擾ｿｽW・ｽ・ｽ・ｽC・ｽX・ｽe・ｽB・ｽb・ｽN・ｽﾌ擾ｿｽ・ｽ・ｽ・ｽ・ｽ
    InitJoyFF(hWnd);      // ・ｽe・ｽe・ｽW・ｽ・ｽ・ｽC・ｽX・ｽe・ｽB・ｽb・ｽN・ｽﾌ擾ｿｽ・ｽ・ｽ・ｽ・ｽ

    return true;

}
//-----------------------------------------------------------------------------
// ・ｽﾊ擾ｿｽ・ｽJoystick・ｽf・ｽo・ｽC・ｽX・ｽﾌ擾ｿｽ・ｽ・ｽ・ｽ・ｽ
// 
// ・ｽ・ｽ・ｽ・ｽ    HWND hWnd  ・ｽE・ｽB・ｽ・ｽ・ｽh・ｽE・ｽn・ｽ・ｽ・ｽh・ｽ・ｽ
// 
// ・ｽﾟゑｿｽl true:・ｽ・ｽ・ｽ・ｽ・ｽ@false:・ｽ・ｽ・ｽs
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
       if( m_bJoyFF[i] ) continue;    // ・ｽe・ｽe・ｽW・ｽ・ｽ・ｽC・ｽX・ｽe・ｽB・ｽb・ｽN・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ 

       diprg.diph.dwObj = DIJOFS_X;
       m_hr = m_pJoy[i]->SetProperty(DIPROP_RANGE, &diprg.diph);
       if (DI_OK != m_hr) {
          MessageBox(nullptr, _T("DInput.cpp : StartDirectInput() "),nullptr,MB_OK);
          return false;
       }

       diprg.diph.dwObj = DIJOFS_Y;
       m_hr = m_pJoy[i]->SetProperty(DIPROP_RANGE, &diprg.diph);
       if (DI_OK != m_hr) {
          MessageBox(nullptr, _T("DInput.cpp : StartDirectInput()"),nullptr,MB_OK);
          return false;
       }
    }

    return SetPropertyJoy();
}
//-----------------------------------------------------------------------------
// ・ｽﾊ擾ｿｽ・ｽjoystick・ｽﾌバ・ｽb・ｽt・ｽ@・ｽT・ｽC・ｽY・ｽﾌ設抵ｿｽ(joystick)
// 
// ・ｽﾟゑｿｽl true:・ｽ・ｽ・ｽ・ｽ・ｽ@false:・ｽ・ｽ・ｽs
//-----------------------------------------------------------------------------
bool CDirectInput::SetPropertyJoy(void)
{
    m_BufferRestJoy = 0;
    m_pBufferPositionJoy = nullptr;
    for (int i=0; i<m_nJoySum; i++) {
       if (!m_pJoy[i]) continue;
       if( m_bJoyFF[i] ) continue;    // ・ｽe・ｽe・ｽW・ｽ・ｽ・ｽC・ｽX・ｽe・ｽB・ｽb・ｽN・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ

       m_pBufferJoy[i] = new DIDEVICEOBJECTDATA[BufferSize];  // ・ｽo・ｽb・ｽt・ｽ@・ｽ・ｽ・ｽm・ｽ・ｽ
       DIPROPDWORD dipdw;
       ZeroMemory(&dipdw, sizeof(dipdw));
       dipdw.diph.dwSize = sizeof(DIPROPDWORD);
       dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
       dipdw.diph.dwObj = 0;
       dipdw.diph.dwHow = DIPH_DEVICE;
       dipdw.dwData = BufferSize;
       m_pJoy[i]->Unacquire();             // ・ｽ・ｽU・ｽA・ｽN・ｽZ・ｽX・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ
       m_hr = m_pJoy[i]->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);
       m_pJoy[i]->Acquire();              // ・ｽA・ｽN・ｽZ・ｽX・ｽ・ｽ・ｽｾゑｿｽ
       if (FAILED(m_hr)) return !FAILED(m_hr);
    }
    return true;
}
//-----------------------------------------------------------------------------
// ForceFeedback Joystick・ｽf・ｽo・ｽC・ｽX・ｽﾌ擾ｿｽ・ｽ・ｽ・ｽ・ｽ
// 
// ・ｽ・ｽ・ｽ・ｽ    HWND hWnd  ・ｽE・ｽB・ｽ・ｽ・ｽh・ｽE・ｽn・ｽ・ｽ・ｽh・ｽ・ｽ
// 
// ・ｽﾟゑｿｽl true:・ｽ・ｽ・ｽ・ｽ・ｽ@false:・ｽ・ｽ・ｽs
//-----------------------------------------------------------------------------
bool CDirectInput::InitJoyFF(HWND hWnd)
{
    int  i;

    DIPROPRANGE dipr;
    dipr.diph.dwSize = sizeof(DIPROPRANGE);
    dipr.diph.dwHeaderSize = sizeof(dipr.diph);
    dipr.diph.dwHow = DIPH_BYOFFSET;
    dipr.lMin = RANGE_MIN;          // negative to the left/top
    dipr.lMax = RANGE_MAX;          // positive to the right/bottom

    for (i=0; i<m_nJoySum; i++) {
       if (!m_pJoy[i]) continue;
       if( !m_bJoyFF[i] ) continue;   // ・ｽﾊ擾ｿｽW・ｽ・ｽ・ｽC・ｽX・ｽe・ｽB・ｽb・ｽN・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ

       m_pJoy[i]->Unacquire();

       // X・ｽ・ｽ・ｽﾍ囲の設抵ｿｽ
       dipr.diph.dwObj = DIJOFS_X;
       if (m_pJoy[i]->SetProperty(DIPROP_RANGE, &dipr.diph) != DI_OK) {
          MessageBox(nullptr, _T("DInput.cpp : StartDirectInput() : SetProperty(X・ｽ・ｽ・ｽﾍ茨ｿｽ(RANGE):FFJoystick)・ｽ・ｽ・ｽﾙ擾ｿｽI・ｽ・ｽ・ｽ・ｽ・ｽﾜゑｿｽ・ｽ・ｽ・ｽB"), nullptr, MB_OK);
          return false;
       }

       // Y・ｽ・ｽ・ｽﾍ囲の設抵ｿｽ
       dipr.diph.dwObj = DIJOFS_Y;
       if (m_pJoy[i]->SetProperty(DIPROP_RANGE, &dipr.diph) != DI_OK) {
          MessageBox(nullptr, _T("DInput.cpp : StartDirectInput() : SetProperty(Y・ｽ・ｽ・ｽﾍ茨ｿｽ(RANGE):FFJoystick)・ｽ・ｽ・ｽﾙ擾ｿｽI・ｽ・ｽ・ｽ・ｽ・ｽﾜゑｿｽ・ｽ・ｽ・ｽB"), nullptr, MB_OK);
          return false;
       }
    }

    // deadzone・ｽﾌ設抵ｿｽ
    DIPROPDWORD dipdw;
    dipdw.diph.dwSize = sizeof(DIPROPDWORD);
    dipdw.diph.dwHeaderSize = sizeof(dipdw.diph);
    dipdw.diph.dwHow = DIPH_BYOFFSET;
    dipdw.dwData = DEADZONE;

    for (i=0; i<m_nJoySum; i++) {
       if (!m_pJoy[i]) continue;
       if( !m_bJoyFF[i] ) continue;   // ・ｽﾊ擾ｿｽW・ｽ・ｽ・ｽC・ｽX・ｽe・ｽB・ｽb・ｽN・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ

       // X・ｽ・ｽ・ｽﾍ囲の設抵ｿｽ
       dipdw.diph.dwObj = DIJOFS_X;
       if (m_pJoy[i]->SetProperty(DIPROP_DEADZONE, &dipdw.diph) != DI_OK) {
          MessageBox(nullptr, _T("DInput.cpp : StartDirectInput() : SetProperty(X・ｽ・ｽ・ｽﾍ茨ｿｽ(DEADZONE):FFJoystick)・ｽ・ｽ・ｽﾙ擾ｿｽI・ｽ・ｽ・ｽ・ｽ・ｽﾜゑｿｽ・ｽ・ｽ・ｽB"), nullptr, MB_OK);
          return false;
       }

       // Y・ｽ・ｽ・ｽﾍ囲の設抵ｿｽ
       dipdw.diph.dwObj = DIJOFS_Y;
       if (m_pJoy[i]->SetProperty(DIPROP_DEADZONE, &dipdw.diph) != DI_OK) {
          MessageBox(nullptr, _T("DInput.cpp : StartDirectInput() : SetProperty(Y・ｽ・ｽ・ｽﾍ茨ｿｽ(RANGE):FFJoystick)・ｽ・ｽ・ｽﾙ擾ｿｽI・ｽ・ｽ・ｽ・ｽ・ｽﾜゑｿｽ・ｽ・ｽ・ｽB"), nullptr, MB_OK);
          return false;
       }
    }

    // ・ｽ・ｽ・ｽ・ｽ・ｽZ・ｽ・ｽ・ｽ^・ｽ・ｽ・ｽ・ｽ・ｽO
    for (i=0; i<m_nJoySum; i++) {
       if (!m_pJoy[i]) continue;
       if( !m_bJoyFF[i] ) continue;   // ・ｽﾊ擾ｿｽW・ｽ・ｽ・ｽC・ｽX・ｽe・ｽB・ｽb・ｽN・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ

       dipdw.diph.dwSize = sizeof(DIPROPDWORD);
       dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
       dipdw.diph.dwObj = 0;
       dipdw.diph.dwHow = DIPH_DEVICE;
       //dipdw.dwData = DIPROPAUTOCENTER_ON;
       dipdw.dwData = DIPROPAUTOCENTER_OFF;   // ・ｽ・ｽ・ｽ・ｽ・ｽZ・ｽ・ｽ・ｽ^・ｽ・ｽ・ｽ・ｽ・ｽO・ｽ・ｽ・ｽ・ｽ
       m_hr = m_pJoy[i]->SetProperty(DIPROP_AUTOCENTER, &dipdw.diph);
       if (FAILED(m_hr)) {
          MessageBox(nullptr, _T("DInput.cpp : StartDirectInput() : SetProperty(・ｽ・ｽ・ｽ・ｽ・ｽZ・ｽ・ｽ・ｽ^・ｽ・ｽ・ｽ・ｽ・ｽO):FFJoystick)・ｽ・ｽ・ｽﾙ擾ｿｽI・ｽ・ｽ・ｽ・ｽ・ｽﾜゑｿｽ・ｽ・ｽ・ｽB"),nullptr,MB_OK);
          return false;
       }
    }

    m_BufferRestJoy = 0;
    m_pBufferPositionJoy = nullptr;

    for (i=0; i<m_nJoySum; i++) {
       if (!m_pJoy[i]) continue;
       if( !m_bJoyFF[i] ) continue;   // ・ｽﾊ擾ｿｽW・ｽ・ｽ・ｽC・ｽX・ｽe・ｽB・ｽb・ｽN・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ

       m_pBufferJoy[i] = new DIDEVICEOBJECTDATA[BufferSize];  // ・ｽo・ｽb・ｽt・ｽ@・ｽ・ｽ・ｽm・ｽ・ｽ
       ZeroMemory(&dipdw, sizeof(dipdw));
       dipdw.diph.dwSize = sizeof(DIPROPDWORD);
       dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
       dipdw.diph.dwObj = 0;
       dipdw.diph.dwHow = DIPH_DEVICE;
       dipdw.dwData = BufferSize;
       // ・ｽ・ｽ・ｽA・ｽl・ｽﾌ伝・ｽF・ｽb・ｽN・ｽ・ｽ・ｽﾈゑｿｽ(DI_POLLEDDEVICE・ｽ・ｽ・ｽﾔゑｿｽ)
       m_pJoy[i]->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);

       if (m_pJoy[i]->Acquire() != DI_OK) {
          //CreateJoyEffectStandard(); // ・ｽﾈゑｿｽ・ｽ・ｽ・ｽ・ｽ・ｽﾆ難ｿｽd・ｽﾉなゑｿｽ・ｽﾄゑｿｽ・ｽ・ｽ・ｽﾌで削除・ｽ・ｽ・ｽ・ｽ
          ;
       }

       // get the device capabilities
       DIDEVCAPS didc;
       didc.dwSize = sizeof(DIDEVCAPS);
       if (m_pJoy[i]->GetCapabilities(&didc) != DI_OK) {
          MessageBox(nullptr, _T("DInput.cpp : StartDirectInput() : GetCapabilities(ForceFeedback Joystick)・ｽ・ｽ・ｽﾙ擾ｿｽI・ｽ・ｽ・ｽ・ｽ・ｽﾜゑｿｽ・ｽ・ｽ・ｽB"), nullptr, MB_OK);
          return false;
       }

       // ・ｽW・ｽ・ｽ・ｽﾌＦ・ｽe・ｽ・ｽ・ｽﾊゑｿｽﾝ定す・ｽ・ｽ
       if (didc.dwFlags & DIDC_FORCEFEEDBACK) {
          //MessageBox(nullptr,_T("DInput.cpp : StartDirectInput() : ForceFeedback device found.\n"), nullptr, MB_OK); // -- 2018.8.27
          if (!CreateJoyEffectStandard()) {
             MessageBox(nullptr,_T("DInput.cpp : StartDirectInput() : CreateEffect(ForceFeedback Joystick)・ｽ・ｽ・ｽﾙ擾ｿｽI・ｽ・ｽ・ｽ・ｽ・ｽﾜゑｿｽ・ｽ・ｽ・ｽB"), nullptr, MB_OK);
             return false;
          }
       }
    }

    return true;
}

//-----------------------------------------------------------------------------
// ・ｽW・ｽ・ｽ・ｽC・ｽX・ｽe・ｽB・ｽb・ｽN・ｽﾌ鯉ｿｽ・ｽﾝのキ・ｽ[・ｽ・ｽ・ｽ・ｽ・ｽ`・ｽF・ｽb・ｽN(・ｽ・ｽ・ｽﾚデ・ｽ[・ｽ^)
// 
// ・ｽ・ｽ・ｽ・ｽ    DWORD  kcode  ・ｽ`・ｽF・ｽb・ｽN・ｽ・ｽ・ｽ・ｽL・ｽ[・ｽR・ｽ[・ｽh
//        int       nSum   ・ｽW・ｽ・ｽ・ｽC・ｽX・ｽe・ｽB・ｽb・ｽN・ｽﾔ搾ｿｽ・ｽB・ｽﾈ暦ｿｽ・ｽl・ｽ・ｽJOY_PLAYER1
// 
// ・ｽﾟゑｿｽl true:・ｽ`・ｽF・ｽb・ｽN・ｽn・ｽj・ｽ@false:・ｽ`・ｽF・ｽb・ｽN・ｽm・ｽf
//-----------------------------------------------------------------------------
bool CDirectInput::CheckJoyImm(DWORD kcode, int nSum)
{
    if (DIJ_LEFT > kcode) {
       return m_js[nSum].rgbButtons[kcode] & 0x80;
    } else {
       switch (kcode) {   // ・ｽ繪ｺ・ｽ・ｽ・ｽE・ｽﾌ、・ｽf・ｽW・ｽ^・ｽ・ｽ・ｽiDIJ_VOLUME・ｽ・ｽ・ｽE・ｽj・ｽﾅ費ｿｽ・ｽf
       case DIJ_LEFT :       // DIF_LEFT・ｽ・ｽ・ｽ・ｽ・ｽ・ｽR・ｽ[・ｽh
          return m_js[nSum].lX < (-1 * DIJ_VOLUME);
       case DIJ_RIGHT :   // DIF_RIGHT・ｽ・ｽ・ｽ・ｽ・ｽ・ｽR・ｽ[・ｽh
          return m_js[nSum].lX > DIJ_VOLUME;
       case DIJ_UP :     // DIF_UP・ｽ・ｽ・ｽ・ｽ・ｽ・ｽR・ｽ[・ｽh
          return m_js[nSum].lY < (-1 * DIJ_VOLUME);
       case DIJ_DOWN :       // DIF_DOWN・ｽ・ｽ・ｽ・ｽ・ｽ・ｽR・ｽ[・ｽh
          return m_js[nSum].lY > DIJ_VOLUME;
       default :
          MessageBox(nullptr, _T("DInput.cpp : CheckJoystickImm() : kcode・ｽﾌ指・ｽ閧ｪ・ｽﾔ茨ｿｽ・ｽ・ｽﾄゑｿｽ・ｽﾜゑｿｽ・ｽB"), nullptr, MB_OK);
          return false;
       }
    }
}
//-----------------------------------------------------------------------------
// ・ｽW・ｽ・ｽ・ｽC・ｽX・ｽe・ｽB・ｽb・ｽN・ｽﾌ擾ｿｽﾔを得ゑｿｽ
// 
// ・ｽﾟゑｿｽl true:・ｽ・ｽ・ｽ・ｽ・ｽ@false:・ｽ・ｽ・ｽs
//-----------------------------------------------------------------------------
bool CDirectInput::GetJoy(void)
{
    for (int i=0; i<m_nJoySum; i++) {
       if (!m_pJoy[i]) return false;
       m_pJoy[i]->Poll();
       m_pJoy[i]->GetDeviceState(sizeof(DIJOYSTATE2), &m_js[i]);

       m_BufferRestJoy = BufferSize;
       m_hr = m_pJoy[i]->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), m_pBufferJoy[i], &m_BufferRestJoy, 0);
       if (FAILED(m_hr)) {             // ・ｽ・ｽ・ｽs・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ1・ｽx・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽg・ｽ・ｽ・ｽC
          m_BufferRestJoy = BufferSize;
          m_pJoy[i]->Acquire();
          m_hr = m_pJoy[i]->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), m_pBufferJoy[i], &m_BufferRestJoy, 0);
          if (FAILED(m_hr)) {
             //MessageBox(nullptr,_T("DInput.cpp : GetJoy() : GetDeviceData・ｽ・ｽ・ｽﾙ擾ｿｽI・ｽ・ｽ・ｽ・ｽ・ｽﾜゑｿｽ・ｽ・ｽ・ｽB"),nullptr,MB_OK);
             return false;
          }
       }
       m_BufferRestBackupJoy[i] = m_BufferRestJoy;    // ・ｽﾇみ搾ｿｽ・ｽｾバ・ｽb・ｽt・ｽ@・ｽ・ｽ・ｽ・ｽﾞ費ｿｽ
    }
    return true;
}
//-----------------------------------------------------------------------------
// Joystick ・ｽ繪ｺ・ｽ・ｽ・ｽE・ｽﾌ撰ｿｽ・ｽ・ｽ`・ｽF・ｽb・ｽN(joystick)
// 
// ・ｽ・ｽ・ｽ・ｽ    int       kmode  ・ｽ`・ｽF・ｽb・ｽN・ｽ・ｽ・ｽ驛ゑｿｽ[・ｽh
//        DWORD  kcode  ・ｽ`・ｽF・ｽb・ｽN・ｽ・ｽ・ｽ・ｽL・ｽ[・ｽR・ｽ[・ｽh・ｽ@
//        int       nSum   ・ｽW・ｽ・ｽ・ｽC・ｽX・ｽe・ｽB・ｽb・ｽN・ｽﾔ搾ｿｽ・ｽB・ｽﾈ暦ｿｽ・ｽl・ｽ・ｽJOY_PLAYER1
// 
// ・ｽﾟゑｿｽl true:・ｽ`・ｽF・ｽb・ｽN・ｽn・ｽj・ｽ@false:・ｽ`・ｽF・ｽb・ｽN・ｽm・ｽf
//-----------------------------------------------------------------------------
bool CDirectInput::CheckUpDownLeftRight(int kmode, DWORD kcode, int nSum)
{
    if (m_didodJoy[nSum]->dwOfs == 4) {    // Y・ｽ・ｽ(・ｽ繪ｺ)?
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
       if (m_didodJoy[nSum]->dwOfs == 0) {    // X・ｽ・ｽ(・ｽ・ｽ・ｽE)?
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
// ・ｽL・ｽ[・ｽ・ｽ・ｽ・ｽ・ｽ`・ｽF・ｽb・ｽN(joystick) 
// 
// ・ｽ・ｽ・ｽ・ｽ    const int&    kmode  ・ｽ`・ｽF・ｽb・ｽN・ｽ・ｽ・ｽ驛ゑｿｽ[・ｽh
//        const DWORD&   kcode  ・ｽ`・ｽF・ｽb・ｽN・ｽ・ｽ・ｽ・ｽL・ｽ[・ｽR・ｽ[・ｽh・ｽ@
//        int       nSum   ・ｽW・ｽ・ｽ・ｽC・ｽX・ｽe・ｽB・ｽb・ｽN・ｽﾔ搾ｿｽ・ｽB・ｽﾈ暦ｿｽ・ｽl・ｽ・ｽJOY_PLAYER1
// 
// ・ｽﾟゑｿｽl true:・ｽ`・ｽF・ｽb・ｽN・ｽn・ｽj・ｽ@false:・ｽ`・ｽF・ｽb・ｽN・ｽm・ｽf
//-----------------------------------------------------------------------------
bool CDirectInput::CheckJoy(const int& kmode, const DWORD& kcode, int nSum)
{
    if (!m_bInputActive || !m_pJoy[nSum]) return false;


    switch (kmode) {
    case KD_DAT :                 // ・ｽ・ｽ・ｽﾚデ・ｽ[・ｽ^(・ｽ・ｽ・ｽﾝのキ・ｽ[・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ)
       return CheckJoyImm(kcode, nSum);
    case KD_TRG :                 // ・ｽL・ｽ[・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ・ｽu・ｽ・ｽ
    case KD_UTRG :                // ・ｽL・ｽ[・ｽ｣ゑｿｽ・ｽ・ｽ・ｽu・ｽ・ｽ
       m_BufferRestJoy = m_BufferRestBackupJoy[nSum]; // ・ｽﾇみ搾ｿｽ・ｽｾバ・ｽb・ｽt・ｽ@・ｽ・ｽ・ｽﾌ包ｿｽ・ｽ・ｽ
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
             if (m_didodJoy[nSum]->dwOfs == (DIJOFS_BUTTON(kcode))) {   //・ｽL・ｽ[・ｽﾌ趣ｿｽ・ｽ
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
       MessageBox(nullptr, _T("DInput.cpp : CheckJoy() : ・ｽw・ｽ閧ｵ・ｽ・ｽ・ｽL・ｽ[・ｽ・ｽ・ｽ[・ｽh・ｽ・ｽ・ｽﾔ茨ｿｽ・ｽ・ｽﾄゑｿｽ・ｽﾜゑｿｽ・ｽB"), nullptr, MB_OK);
       return false;
    }
}
//-----------------------------------------------------------------------------
// ・ｽt・ｽH・ｽ[・ｽX・ｽt・ｽB・ｽ[・ｽh・ｽo・ｽb・ｽN・ｽf・ｽB・ｽo・ｽC・ｽX・ｽ・ｽ・ｽﾇゑｿｽ・ｽ・ｽ・ｽ・ｽ・ｽ`・ｽF・ｽb・ｽN・ｽ・ｽ・ｽ・ｽ(joystick) 
//
// ・ｽ・ｽ・ｽ・ｽ    int nSum      ・ｽW・ｽ・ｽ・ｽC・ｽX・ｽe・ｽB・ｽb・ｽN・ｽﾔ搾ｿｽ・ｽB・ｽﾈ暦ｿｽ・ｽl・ｽ・ｽJOY_PLAYER1
//
// ・ｽﾟゑｿｽl true:・ｽt・ｽH・ｽ[・ｽX・ｽt・ｽB・ｽ[・ｽh・ｽo・ｽb・ｽN・ｽf・ｽB・ｽo・ｽC・ｽX・ｽ@false:・ｽ・ｽﾊデ・ｽB・ｽo・ｽC・ｽX
//-----------------------------------------------------------------------------
bool CDirectInput::IfJoyFF(int nSum)
{
    if(!m_pJoy[nSum]) return false;
    if( m_bJoyFF[nSum] ){  // ForceFeedback Joystick・ｽ・ｽ・ｽﾇゑｿｽ・ｽ・ｽ
       return true;
    }else{
       return false;
    }
}
//-----------------------------------------------------------------------------
// ・ｽW・ｽ・ｽ・ｽC・ｽX・ｽe・ｽB・ｽb・ｽN・ｽﾌス・ｽe・ｽ[・ｽg・ｽｾゑｿｽ
//
// ・ｽ・ｽ・ｽ・ｽ    int nSum      ・ｽW・ｽ・ｽ・ｽC・ｽX・ｽe・ｽB・ｽb・ｽN・ｽﾔ搾ｿｽ・ｽB・ｽﾈ暦ｿｽ・ｽl・ｽ・ｽJOY_PLAYER1
//
// ・ｽﾟゑｿｽl DIJOYSTATE2・ｽ\・ｽ・ｽ・ｽﾌの値・ｽB・ｽg・ｽ・ｽ・ｽ@・ｽ\・ｽ・ｽ・ｽ・ｽ・ｽﾂジ・ｽ・ｽ・ｽC・ｽX・ｽe・ｽB・ｽb・ｽN ・ｽf・ｽo・ｽC・ｽX・ｽﾌ擾ｿｽﾔゑｿｽ\・ｽ・ｽ・ｽB
//        ・ｽ・ｽ・ｽﾉ、LONG lX;・ｽ@LONG lY;・ｽ@・ｽﾍ包ｿｽ・ｽ・ｽ・ｽL・ｽ[・ｽﾌア・ｽi・ｽ・ｽ・ｽO・ｽl・ｽ・ｽ\・ｽ・ｽ・ｽB
//-----------------------------------------------------------------------------
DIJOYSTATE2 CDirectInput::GetJoyState(int nSum)
{
    return m_js[nSum];
}
//-----------------------------------------------------------------------------
// ・ｽﾚ托ｿｽ・ｽ・ｽ・ｽ・ｽﾄゑｿｽ・ｽ・ｽW・ｽ・ｽ・ｽC・ｽX・ｽe・ｽB・ｽb・ｽN・ｽﾌ撰ｿｽ・ｽｾゑｿｽ              // -- 2018.8.27
//
// ・ｽ・ｽ・ｽ・ｽ    ・ｽﾈゑｿｽ
//
// ・ｽﾟゑｿｽl int ・ｽﾚ托ｿｽ・ｽ・ｽ・ｽ・ｽﾄゑｿｽ・ｽ・ｽW・ｽ・ｽ・ｽC・ｽX・ｽe・ｽB・ｽb・ｽN・ｽﾌ撰ｿｽ
//-----------------------------------------------------------------------------
int CDirectInput::GetJoyNum(void)
{
    return m_nJoySum;
}

/*
//-----------------------------------------------------------------------------
// ・ｽW・ｽ・ｽ・ｽ・ｽ・ｽﾊの撰ｿｽ・ｽ・ｽ (for ForceFeedback Joystick)
//                                                             !! OLD  !!
// ・ｽiEF_BOUNCE・ｽEF_FIRE・ｽEF_EXPLODE・ｽﾌ３・ｽﾂの鯉ｿｽ・ｽﾊを生撰ｿｽ・ｽ・ｽ・ｽ・ｽj
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
    diEffect.dwSamplePeriod = 0;      // use default sample period
    diEffect.dwTriggerButton = DIEB_NOTRIGGER;
    diEffect.dwTriggerRepeatInterval = 0;
    diEffect.rgdwAxes = rgdwAxes;
    diEffect.rglDirection = rglDirections;
    diEffect.dwGain = FF_BODYBUILDER;

    for (i = 0; i<m_nJoySum; i++) {
       if (!m_pJoy[i]) continue;
       if (!m_bJoyFF[i]) continue;    // ・ｽﾊ擾ｿｽW・ｽ・ｽ・ｽC・ｽX・ｽe・ｽB・ｽb・ｽN・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ


       // BOUNCE・ｽG・ｽt・ｽF・ｽN・ｽg・ｽﾌ撰ｿｽ・ｽ・ｽ
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
          MessageBox(nullptr, _T("DInput.cpp : CreateEffect() : CreateEffect(Bounce)・ｽ・ｽ・ｽﾙ擾ｿｽI・ｽ・ｽ・ｽ・ｽ・ｽﾜゑｿｽ・ｽ・ｽ・ｽB"), nullptr, MB_OK);
          return false;
       }

       // FIRE・ｽG・ｽt・ｽF・ｽN・ｽg・ｽﾌ撰ｿｽ・ｽ・ｽ
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
          MessageBox(nullptr, _T("DInput.cpp : CreateEffect() : CreateEffect(Fire)・ｽ・ｽ・ｽﾙ擾ｿｽI・ｽ・ｽ・ｽ・ｽ・ｽﾜゑｿｽ・ｽ・ｽ・ｽB"), nullptr, MB_OK);
          return false;
       }

       // EXPLODE・ｽG・ｽt・ｽF・ｽN・ｽg・ｽﾌ撰ｿｽ・ｽ・ｽ
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
          MessageBox(nullptr, _T("DInput.cpp : CreateEffect() : CreateEffect(Explode)・ｽ・ｽ・ｽﾙ擾ｿｽI・ｽ・ｽ・ｽ・ｽ・ｽﾜゑｿｽ・ｽ・ｽ・ｽB"), nullptr, MB_OK);
          return false;
       }

       m_nJoyEFSum = 3;   // ・ｽW・ｽ・ｽ・ｽ・ｽ・ｽﾊゑｿｽ・ｽR・ｽﾂ登・ｽ^・ｽ・ｽ・ｽ黷ｽ

    }

    return true;
}
*/

//-----------------------------------------------------------------------------
// ・ｽW・ｽ・ｽ・ｽ・ｽ・ｽﾊの撰ｿｽ・ｽ・ｽ (for ForceFeedback Joystick)                  // -- 2018.8.27
//
// ・ｽiEF_BOUNCE・ｽEF_FIRE・ｽEF_EXPLODE・ｽﾌ３・ｽﾂの鯉ｿｽ・ｽﾊを生撰ｿｽ・ｽ・ｽ・ｽ・ｽj
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
       if( !m_bJoyFF[i] ) continue;   // ・ｽﾊ擾ｿｽW・ｽ・ｽ・ｽC・ｽX・ｽe・ｽB・ｽb・ｽN・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ

       // ・ｽG・ｽt・ｽF・ｽN・ｽg・ｽﾌ擾ｿｽ・ｽ・ｽ・ｽ・ｽ
       ZeroMemory(&diEffect, sizeof(DIEFFECT));
       ZeroMemory(&diEnvelope, sizeof(DIENVELOPE));

       // these fields are the same for all effects we will be creating
       diEffect.dwSize = sizeof(DIEFFECT);
       diEffect.dwSamplePeriod = 0;      // use default sample period
       diEffect.dwTriggerButton = DIEB_NOTRIGGER;
       diEffect.dwTriggerRepeatInterval = 0;
       diEffect.rgdwAxes = rgdwAxes;           // rgdwAxes・ｽz・ｽ・ｽﾌア・ｽh・ｽ・ｽ・ｽX・ｽﾝ抵ｿｽ
       diEffect.rglDirection = rglDirections;  // rglDirections・ｽz・ｽ・ｽﾌア・ｽh・ｽ・ｽ・ｽX・ｽﾝ抵ｿｽ
       //diEffect.dwGain = FF_BODYBUILDER;    // ・ｽ・ｽ・ｽ・ｽ・ｽ@100%
       diEffect.dwGain = FF_ADULT;    // ・ｽ・ｽ・ｽ・ｽ・ｽ@75%                    // -- 2023.1.3

       // BOUNCE・ｽG・ｽt・ｽF・ｽN・ｽg・ｽﾌ撰ｿｽ・ｽ・ｽ
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
          MessageBox(nullptr, _T("DInput.cpp : CreateEffect() : CreateEffect(Bounce)・ｽ・ｽ・ｽﾙ擾ｿｽI・ｽ・ｽ・ｽ・ｽ・ｽﾜゑｿｽ・ｽ・ｽ・ｽB"), nullptr, MB_OK);
          return false;
       }

       // FIRE・ｽG・ｽt・ｽF・ｽN・ｽg・ｽﾌ撰ｿｽ・ｽ・ｽ
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
          MessageBox(nullptr, _T("DInput.cpp : CreateEffect() : CreateEffect(Fire)・ｽ・ｽ・ｽﾙ擾ｿｽI・ｽ・ｽ・ｽ・ｽ・ｽﾜゑｿｽ・ｽ・ｽ・ｽB"), nullptr, MB_OK);
          return false;
       }
       
       // EXPLODE・ｽG・ｽt・ｽF・ｽN・ｽg・ｽﾌ撰ｿｽ・ｽ・ｽ
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
          MessageBox(nullptr, _T("DInput.cpp : CreateEffect() : CreateEffect(Explode)・ｽ・ｽ・ｽﾙ擾ｿｽI・ｽ・ｽ・ｽ・ｽ・ｽﾜゑｿｽ・ｽ・ｽ・ｽB"), nullptr, MB_OK);
          return false;
       }

       m_nJoyEFSum = 3;   // ・ｽW・ｽ・ｽ・ｽ・ｽ・ｽﾊゑｿｽ・ｽR・ｽﾂ登・ｽ^・ｽ・ｽ・ｽ黷ｽ

    }

    return true;
}
//-----------------------------------------------------------------------------
// ForceFeedback・ｽ・ｽ・ｽﾊ列挙のコ・ｽ[・ｽ・ｽ・ｽo・ｽb・ｽN・ｽﾖ撰ｿｽ  (for ForceFeedback Joystick)
//-----------------------------------------------------------------------------
BOOL CALLBACK EnumEffectsInFileProc(LPCDIFILEEFFECT lpdife, LPVOID pvRef )
{
    
    return g_pDI->EnumEffectsInFileCb( lpdife,  pvRef );

}
//-----------------------------------------------------------------------------
// ForceFeedback・ｽ・ｽ・ｽﾊ列挙のコ・ｽ[・ｽ・ｽ・ｽo・ｽb・ｽN(for ForceFeedback Joystic)・ｽﾎ会ｿｽ・ｽﾌ・ｿｽ・ｽ\・ｽb・ｽh  
//-----------------------------------------------------------------------------
BOOL CDirectInput::EnumEffectsInFileCb(LPCDIFILEEFFECT lpdife, LPVOID pvRef )
{
    HRESULT hr;

    hr = m_pJoy[m_nJoyEFI]->CreateEffect( lpdife->GuidEffect, lpdife->lpDiEffect,
                                     &m_pJoyEffect[m_nJoyEFI][m_nJoyEFSum], nullptr );
    if( DI_OK != hr){
       MessageBox(nullptr, _T("DInput.cpp : EnumEffectsInFileProc() : CreateEffect()・ｽ・ｽ・ｽﾙ擾ｿｽI・ｽ・ｽ・ｽ・ｽ・ｽﾜゑｿｽ・ｽ・ｽ・ｽB"), nullptr, MB_OK);
       return DIENUM_CONTINUE;    // ・ｽ・ｽ・ｽﾌデ・ｽo・ｽC・ｽX・ｽ・ｽ・ｽ
    }

    if( ++m_nJoyEFSum >=  JOY_EF_COUNT) return DIENUM_STOP;
    return DIENUM_CONTINUE;

}

//-----------------------------------------------------------------------------
// ForceFeedback・ｽﾌ鯉ｿｽ・ｽﾊの読み搾ｿｽ・ｽ・ｽ (for ForceFeedback Joy)
// 
// ・ｽ・ｽ・ｽ・ｽ    TCHAR* szFName ・ｽ・ｽ・ｽﾊフ・ｽ@・ｽC・ｽ・ｽ・ｽ・ｽ
//        int nEffectNo  ・ｽ・ｽ・ｽﾊ番搾ｿｽ・ｽi・ｽﾟゑｿｽl・ｽj・ｽ@
//        int nNum      ・ｽ・ｽﾂの鯉ｿｽ・ｽﾊの個撰ｿｽ・ｽi・ｽﾟゑｿｽl・ｽj
// 
// ・ｽﾟゑｿｽl true:・ｽ・ｽ・ｽ・ｽ・ｽ@false:・ｽ・ｽ・ｽs
//-----------------------------------------------------------------------------
bool CDirectInput::ReadJoyEffect(const TCHAR* szFName, int &nEffectNo, int &nNum)
{
    int  i, nWSum, nMaxSum;

    nMaxSum = nWSum = m_nJoyEFSum;
    nEffectNo = 0;
    nNum   = 0;

    for ( i=0; i<m_nJoySum; i++) {
       if( !m_pJoy[i] ) continue;
       if( !m_bJoyFF[i] )    continue;       // ・ｽﾊ擾ｿｽW・ｽ・ｽ・ｽC・ｽX・ｽe・ｽB・ｽb・ｽN・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ

       m_nJoyEFSum = nWSum;
       m_nJoyEFI   = i;

       if( m_pJoy[i]->EnumEffectsInFile( szFName, EnumEffectsInFileProc, nullptr, DIFEF_MODIFYIFNEEDED ) != DI_OK ){
          MessageBox(nullptr, _T("DInput.cpp : ReadJoyEffect() : EnumEffectsInFile()・ｽ・ｽ・ｽﾙ擾ｿｽI・ｽ・ｽ・ｽ・ｽ・ｽﾜゑｿｽ・ｽ・ｽ・ｽB"), nullptr, MB_OK);
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
// ForceFeedback・ｽﾌ鯉ｿｽ・ｽﾊ会ｿｽ・ｽt
// 
// ・ｽ・ｽ・ｽ・ｽ    int nEffectNo  ・ｽ・ｽ・ｽﾊ番搾ｿｽ(ReadJoyEffect・ｽﾖ撰ｿｽ・ｽﾌ戻ゑｿｽl・ｽ・ｽ・ｽg・ｽp・ｽ・ｽ・ｽ・ｽ)・ｽ@
//        int nNum      ・ｽ・ｽﾂの鯉ｿｽ・ｽﾊの個撰ｿｽ(ReadJoyEffect・ｽﾖ撰ｿｽ・ｽﾌ戻ゑｿｽl・ｽ・ｽ・ｽg・ｽp・ｽ・ｽ・ｽ・ｽ)・ｽB・ｽﾈ暦ｿｽ・ｽl・ｽﾍ１
//        int nSum      ・ｽW・ｽ・ｽ・ｽC・ｽX・ｽe・ｽB・ｽb・ｽN・ｽﾔ搾ｿｽ・ｽB・ｽﾈ暦ｿｽ・ｽl・ｽ・ｽJOY_PLAYER1
// 
// ・ｽﾟゑｿｽl true:・ｽ・ｽ・ｽ・ｽ・ｽ@false:・ｽ・ｽ・ｽs
//-----------------------------------------------------------------------------
bool CDirectInput::PlayJoyEffect(int nEffectNo, int nNum, int nSum)
{
    int  i;

    if( !m_bJoyFF[nSum] ) return false;    // ・ｽﾊ擾ｿｽW・ｽ・ｽ・ｽC・ｽX・ｽe・ｽB・ｽb・ｽN・ｽ・ｽ・ｽ・ｽ・ｽ・ｽ

    for( i = 0; i < nNum; i++ ){
       if (m_pJoyEffect[nSum][nEffectNo+i]) {
          if (m_pJoyEffect[nSum][nEffectNo+i]->Start(1, 0) != DI_OK) {
             MessageBox(nullptr, _T("DInput.cpp : PlayJoyEffect() : Start(Joy effect)・ｽ・ｽ・ｽﾙ擾ｿｽI・ｽ・ｽ・ｽ・ｽ・ｽﾜゑｿｽ・ｽ・ｽ・ｽB"), nullptr, MB_OK);
             return false;
          }
       }
    }
    return true;
}