//-----------------------------------------------------------------------------
// 
//                                          ver 3.3        2024.3.23
//
//-----------------------------------------------------------------------------
#include <stdio.h>
#include "DInput.h"

#include <tchar.h>

#include "../../Common/Constants.h"
#include "../../Framework/AudioManager.h"


static CDirectInput* g_pDI = nullptr;

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
CDirectInput::CDirectInput(void)
{
    ZeroMemory(this, sizeof(CDirectInput));
    m_bInputActive = true;

    g_pDI = this;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
CDirectInput::~CDirectInput()
{
    EndDirectInput();
}

//-----------------------------------------------------------------------------
// 
// 
//-----------------------------------------------------------------------------
bool CDirectInput::StartDirectInput(HINSTANCE hInst, HWND hWnd, int flag, DWORD dwWidth, DWORD dwHeight)
{
    m_hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&m_pDI8, nullptr);
    if (DI_OK != m_hr)
    {
        MessageBox(nullptr, _T("DInput.cpp : StartDirectInput() : DirectInput : 処理に失敗しました。"), nullptr, MB_OK);
        return false;
    }
    m_hWnd = hWnd;

    if (flag & INIT_KEYBOARD) InitKey(hWnd);
    if (flag & INIT_MOUSE) InitMouse(hWnd);
    if (flag & INIT_JOYSTICK) InitJoy(hWnd);

    m_ViewWidth = (float)dwWidth;
    m_ViewHeight = (float)dwHeight;

    return true;
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
bool CDirectInput::EndDirectInput(void)
{
    if (m_pBufferKey)
    {
        delete []m_pBufferKey;
        m_pBufferKey = nullptr;
    }
    if (m_pKey)
    {
        m_pKey->Unacquire();
        m_pKey->Release();
        m_pKey = nullptr;
    }

    if (m_pBufferMouse)
    {
        delete []m_pBufferMouse;
        m_pBufferMouse = nullptr;
    }
    if (m_pMouse)
    {
        m_pMouse->Unacquire();
        m_pMouse->Release();
        m_pMouse = nullptr;
    }

    int i, j;
    for (i = 0; i < m_nJoySum; i++)
    {
        if (m_bJoyFF[i])
        {
            for (j = 0; j < m_nJoyEFSum; j++)
            {
                if (m_pJoyEffect[i][j])
                {
                    m_pJoyEffect[i][j]->Unload();
                    m_pJoyEffect[i][j]->Release();
                    m_pJoyEffect[i][j] = nullptr;
                }
            }
        }

        if (m_pBufferJoy[i])
        {
            delete []m_pBufferJoy[i];
            m_pBufferJoy[i] = nullptr;
        }

        if (m_pJoy[i])
        {
            m_pJoy[i]->Unacquire();
            m_pJoy[i]->Release();
            m_pJoy[i] = nullptr;
        }
    }

    if (m_pDI8)
    {
        m_pDI8->Release();
        m_pDI8 = nullptr;
    }

    return true;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CDirectInput::SetAcquire(void)
{
    int i;

    if (m_bInputActive)
    {
        if (m_pKey) m_pKey->Acquire();
        if (m_pMouse) m_pMouse->Acquire();
        for (i = 0; i < m_nJoySum; i++) if (m_pJoy[i]) m_pJoy[i]->Acquire();
    }
    else
    {
        if (m_pKey) m_pKey->Unacquire();
        if (m_pMouse) m_pMouse->Unacquire();
        for (i = 0; i < m_nJoySum; i++) if (m_pJoy[i]) m_pJoy[i]->Unacquire();
    }
}

//-----------------------------------------------------------------------------
// 
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
// 
// 
//-----------------------------------------------------------------------------
bool CDirectInput::InitKey(HWND hWnd)
{
    //-----------------------------------------------------------------------------
    m_hr = m_pDI8->CreateDevice(GUID_SysKeyboard, &m_pKey, nullptr);
    if (DI_OK != m_hr)
    {
        MessageBox(nullptr, _T("DInput.cpp : StartDirectInput() : DirectInputDeviceEx(keyboard) : 処理に失敗しました。"), nullptr,
                   MB_OK);
        return false;
    }
    m_pKey->SetDataFormat(&c_dfDIKeyboard);
    if (DI_OK != m_hr)
    {
        MessageBox(nullptr, _T("DInput.cpp : StartDirectInput() : DirectInput : 処理に失敗しました。"), nullptr,MB_OK);
        return false;
    }
    m_pKey->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
    if (DI_OK != m_hr)
    {
        MessageBox(nullptr, _T("DInput.cpp : StartDirectInput() : keyboard : 処理に失敗しました。"), nullptr,MB_OK);
        return false;
    }
    return SetPropertyKey();
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
bool CDirectInput::SetPropertyKey(void)
{
    m_BufferRestKey = 0;
    m_pBufferPositionKey = nullptr;
    m_pBufferKey = new DIDEVICEOBJECTDATA[BufferSize];
    DIPROPDWORD dipdw;
    ZeroMemory(&dipdw, sizeof(dipdw));
    dipdw.diph.dwSize = sizeof(DIPROPDWORD);
    dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    dipdw.diph.dwObj = 0;
    dipdw.diph.dwHow = DIPH_DEVICE;
    dipdw.dwData = BufferSize;
    m_pKey->Unacquire();
    m_hr = m_pKey->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);
    m_pKey->Acquire();
    return !FAILED(m_hr);
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
bool CDirectInput::GetKey(void)
{
    if (!m_pKey) return false;
    if (m_pKey->GetDeviceState(sizeof(m_diKeyState), m_diKeyState) != DI_OK)
    {
        m_pKey->Acquire();
        if (FAILED(m_pKey->GetDeviceState(sizeof(m_diKeyState), m_diKeyState)))
        {
            return false;
        }
    }

    m_BufferRestKey = BufferSize;
    if (FAILED(m_pKey->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), m_pBufferKey, &m_BufferRestKey, 0)))
    {
        m_BufferRestKey = BufferSize;
        m_pKey->Acquire();
        if (FAILED(m_pKey->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), m_pBufferKey, &m_BufferRestKey, 0)))
        {
            return false;
        }
    }
    m_BufferRestBackupKey = m_BufferRestKey;

    return true;
}

//-----------------------------------------------------------------------------
// 
// 
//-----------------------------------------------------------------------------
bool CDirectInput::CheckKey(const int& kmode, const DWORD& kcode)
{
    if (!m_bInputActive || !m_pKey) return false;
    switch (kmode)
    {
    case KD_DAT:
        return m_diKeyState[kcode] & 0x80;
    case KD_TRG:
    case KD_UTRG:
        m_BufferRestKey = m_BufferRestBackupKey;
        m_pBufferPositionKey = m_pBufferKey;
        while (m_BufferRestKey > 0)
        {
            m_BufferRestKey--;
            m_didodKey = m_pBufferPositionKey;
            m_pBufferPositionKey++;
            if (m_didodKey->dwOfs == kcode)
            {
                if (kmode == KD_TRG)
                {
                    if (m_didodKey->dwData) return true;
                }
                else
                {
                    if (!(m_didodKey->dwData)) return true;
                }
            }
        }
        return false;
    default:
        MessageBox(nullptr, _T("DInput.cpp : CheckKey() : : 処理に失敗しました。"), nullptr, MB_OK);
        return false;
    }
}

int CDirectInput::IsPushUpKey()
{
    if (CheckKey(KD_TRG,DIK_UP) || CheckKey(KD_TRG,DIK_W))
    {
        AudioManager::Play(_T(Sound::Key::SELECT_SE), false);
        return -1;
    }
    return 0;
}

int CDirectInput::IsPushDownKey()
{
    if (CheckKey(KD_TRG,DIK_DOWN) || CheckKey(KD_TRG,DIK_S))
    {
        AudioManager::Play(_T(Sound::Key::SELECT_SE), false);
        return 1;
    }
    return 0;
}

bool CDirectInput::IsPushEnter()
{
    AudioManager::Play(_T(Sound::Key::DECIDE_SE), false);
    return CheckKey(KD_TRG,DIK_RETURN);
}

//*****************************************************************************
//*** Mouse                                                                 ***
//*****************************************************************************
//-----------------------------------------------------------------------------
// 
// 
//-----------------------------------------------------------------------------
bool CDirectInput::InitMouse(HWND hWnd)
{
    //-----------------------------------------------------------------------------
    m_hr = m_pDI8->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr);
    if (DI_OK != m_hr)
    {
        MessageBox(nullptr, _T("DInput.cpp : StartDirectInput() : DirectInputDeviceEx(mouse)"), nullptr,MB_OK);
        return false;
    }
    if (m_pMouse->SetDataFormat(&c_dfDIMouse) != DI_OK)
    {
        MessageBox(nullptr, _T("DInput.cpp : StartDirectInput() : SetDataFormat"), nullptr, MB_OK);
        return false;
    }
    if (m_pMouse->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE) != DI_OK)
    {
        MessageBox(nullptr, _T("DInput.cpp : StartDirectInput() : SetCooperativeLevel"), nullptr, MB_OK);
        return false;
    }

    return SetPropertyMouse();
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
bool CDirectInput::SetPropertyMouse(void)
{
    m_BufferRestMouse = 0;
    m_pBufferPositionMouse = nullptr;
    m_pBufferMouse = new DIDEVICEOBJECTDATA[BufferSize];
    DIPROPDWORD dipdw;
    ZeroMemory(&dipdw, sizeof(dipdw));
    dipdw.diph.dwSize = sizeof(DIPROPDWORD);
    dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    dipdw.diph.dwObj = 0;
    dipdw.diph.dwHow = DIPH_DEVICE;
    dipdw.dwData = BufferSize;
    m_pMouse->Unacquire();
    m_hr = m_pMouse->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);
    m_pMouse->Acquire();
    return !FAILED(m_hr);
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
bool CDirectInput::GetMouse(void)
{
    if (!m_pMouse) return false;
    if (m_pMouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_dims) != DI_OK)
    {
        m_pMouse->Acquire();
        if (FAILED(m_pMouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_dims)))
        {
            return false;
        }
    }

    m_BufferRestMouse = BufferSize;
    if (FAILED(m_pMouse->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), m_pBufferMouse, &m_BufferRestMouse, 0)))
    {
        m_BufferRestMouse = BufferSize;
        m_pMouse->Acquire();
        if (FAILED(m_pMouse->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), m_pBufferMouse, &m_BufferRestMouse, 0)))
        {
            return false;
        }
    }
    m_BufferRestBackupMouse = m_BufferRestMouse;

    return true;
}

//-----------------------------------------------------------------------------
// 
// 
//-----------------------------------------------------------------------------
bool CDirectInput::CheckMouse(const int& kmode, const DWORD& kcode)
{
    if (!m_bInputActive || !m_pMouse) return false;

    // 複数の状態や境界条件をまとめて判定する。
    if (kcode == DIM_LEFT || kcode == DIM_RIGHT || kcode == DIM_UP || kcode == DIM_DOWN)
    {
        if (m_dims.lX > 0 && kcode == DIM_RIGHT)
        {
            return true;
        }
        else if (m_dims.lX < 0 && kcode == DIM_LEFT)
        {
            return true;
        }
        else if (m_dims.lY < 0 && kcode == DIM_UP)
        {
            return true;
        }
        else if (m_dims.lY > 0 && kcode == DIM_DOWN)
        {
            return true;
        }
        return false;
    }
    else
    {
        switch (kmode)
        {
        case KD_DAT:
            return m_dims.rgbButtons[kcode] & 0x80;
        case KD_TRG:
        case KD_UTRG:
            m_BufferRestMouse = m_BufferRestBackupMouse;
            m_pBufferPositionMouse = m_pBufferMouse;
            while (m_BufferRestMouse > 0)
            {
                m_BufferRestMouse--;
                m_didodMouse = m_pBufferPositionMouse;
                m_pBufferPositionMouse++;
                if (m_didodMouse->dwOfs == DIMOFS_BUTTON(kcode))
                {
                    if (kmode == KD_TRG)
                    {
                        if (m_didodMouse->dwData) return true;
                    }
                    else
                    {
                        if (!(m_didodMouse->dwData)) return true;
                    }
                }
            }
            return false;
        default:
            MessageBox(nullptr, _T("DInput.cpp : CheckMouse() : : 処理に失敗しました。"), nullptr, MB_OK);
            return false;
        }
    }
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
DIMOUSESTATE CDirectInput::GetMouseState(void)
{
    return m_dims;
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
//
//
//-----------------------------------------------------------------------------
POINT CDirectInput::GetMousePos(void)
{
    POINT pt;
    RECT rc;
    float scaleX, scaleY;
    GetCursorPos(&pt);
    ScreenToClient(m_hWnd, &pt);

    GetClientRect(m_hWnd, &rc);
    scaleX = m_ViewWidth / (rc.right - rc.left);
    scaleY = m_ViewHeight / (rc.bottom - rc.top);

    pt.x = (long)(pt.x * scaleX);
    pt.y = (long)(pt.y * scaleY);

    return pt;
}

//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------
void CDirectInput::ShowMouseCursor(bool bFlag)
{
    ShowCursor(bFlag);
}

//*****************************************************************************
//*****************************************************************************
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
BOOL CALLBACK EnumJoysticksCallback(LPCDIDEVICEINSTANCE pInst, LPVOID lpvContext)
{
    return g_pDI->EnumJoysticksCb(pInst, lpvContext);
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
BOOL CDirectInput::EnumJoysticksCb(LPCDIDEVICEINSTANCE pInst, LPVOID lpvContext)
{
    LPDIRECTINPUTDEVICE8 pDevice = nullptr;
    DIDEVCAPS diDevCaps = {0};

    m_hr = m_pDI8->CreateDevice(pInst->guidInstance, &pDevice, nullptr);
    if (DI_OK != m_hr) return DIENUM_CONTINUE;

    diDevCaps.dwSize = sizeof(DIDEVCAPS);
    m_hr = pDevice->GetCapabilities(&diDevCaps);
    if (FAILED(m_hr))
    {
        pDevice->Release();
        pDevice = nullptr;
        return DIENUM_CONTINUE;
    }

    m_pJoy[m_nJoySum] = pDevice;

    //if( diDevCaps.dwFlags == DIDC_FORCEFEEDBACK ){
    if (pInst->guidFFDriver != GUID_NULL)
    {
        m_bJoyFF[m_nJoySum] = true;
        m_nJoyFFNum++;
    }
    else
    {
        m_bJoyFF[m_nJoySum] = false;
    }

    if (++m_nJoySum >= JOYSTICK_COUNT) return DIENUM_STOP;
    return DIENUM_CONTINUE;
}


//-----------------------------------------------------------------------------
// 
// 
//-----------------------------------------------------------------------------
bool CDirectInput::InitJoy(HWND hWnd)
{
    //-----------------------------------------------------------------------------
    m_pDI8->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback, nullptr, DIEDFL_ATTACHEDONLY);
    if (!m_pJoy[0])
    {
        return false;
    }
    for (int i = 0; i < m_nJoySum; i++)
    {
        if (!m_pJoy[i]) continue;

        m_hr = m_pJoy[i]->SetDataFormat(&c_dfDIJoystick2);
        if (DI_OK != m_hr)
        {
            MessageBox(nullptr, _T("DInput.cpp : StartDirectInput()"), nullptr,MB_OK);
            return false;
        }

        if (m_nJoyFFNum > 0)
        {
            m_hr = m_pJoy[i]->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
        }
        else
        {
            m_hr = m_pJoy[i]->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
        }
        if (DI_OK != m_hr)
        {
            MessageBox(nullptr, _T("DInput.cpp : StartDirectInput()"), nullptr,MB_OK);
            return false;
        }
    }

    InitJoyNormal(hWnd);
    InitJoyFF(hWnd);

    return true;
}

//-----------------------------------------------------------------------------
// 
// 
//-----------------------------------------------------------------------------
bool CDirectInput::InitJoyNormal(HWND hWnd)
{
    int i;

    DIPROPRANGE diprg;
    diprg.diph.dwSize = sizeof(diprg);
    diprg.diph.dwHeaderSize = sizeof(diprg.diph);
    diprg.diph.dwHow = DIPH_BYOFFSET;
    diprg.lMin = RANGE_MIN;
    diprg.lMax = RANGE_MAX;

    for (i = 0; i < m_nJoySum; i++)
    {
        if (!m_pJoy[i]) continue;
        if (m_bJoyFF[i]) continue;

        diprg.diph.dwObj = DIJOFS_X;
        m_hr = m_pJoy[i]->SetProperty(DIPROP_RANGE, &diprg.diph);
        if (DI_OK != m_hr)
        {
            MessageBox(nullptr, _T("DInput.cpp : StartDirectInput() "), nullptr,MB_OK);
            return false;
        }

        diprg.diph.dwObj = DIJOFS_Y;
        m_hr = m_pJoy[i]->SetProperty(DIPROP_RANGE, &diprg.diph);
        if (DI_OK != m_hr)
        {
            MessageBox(nullptr, _T("DInput.cpp : StartDirectInput()"), nullptr,MB_OK);
            return false;
        }
    }

    return SetPropertyJoy();
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
bool CDirectInput::SetPropertyJoy(void)
{
    m_BufferRestJoy = 0;
    m_pBufferPositionJoy = nullptr;
    for (int i = 0; i < m_nJoySum; i++)
    {
        if (!m_pJoy[i]) continue;
        if (m_bJoyFF[i]) continue;

        m_pBufferJoy[i] = new DIDEVICEOBJECTDATA[BufferSize];
        DIPROPDWORD dipdw;
        ZeroMemory(&dipdw, sizeof(dipdw));
        dipdw.diph.dwSize = sizeof(DIPROPDWORD);
        dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
        dipdw.diph.dwObj = 0;
        dipdw.diph.dwHow = DIPH_DEVICE;
        dipdw.dwData = BufferSize;
        m_pJoy[i]->Unacquire();
        m_hr = m_pJoy[i]->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);
        m_pJoy[i]->Acquire();
        if (FAILED(m_hr)) return !FAILED(m_hr);
    }
    return true;
}

//-----------------------------------------------------------------------------
// 
// 
//-----------------------------------------------------------------------------
bool CDirectInput::InitJoyFF(HWND hWnd)
{
    int i;

    DIPROPRANGE dipr;
    dipr.diph.dwSize = sizeof(DIPROPRANGE);
    dipr.diph.dwHeaderSize = sizeof(dipr.diph);
    dipr.diph.dwHow = DIPH_BYOFFSET;
    dipr.lMin = RANGE_MIN; // negative to the left/top
    dipr.lMax = RANGE_MAX; // positive to the right/bottom

    for (i = 0; i < m_nJoySum; i++)
    {
        if (!m_pJoy[i]) continue;
        if (!m_bJoyFF[i]) continue;

        m_pJoy[i]->Unacquire();

        dipr.diph.dwObj = DIJOFS_X;
        if (m_pJoy[i]->SetProperty(DIPROP_RANGE, &dipr.diph) != DI_OK)
        {
            MessageBox(nullptr, _T("DInput.cpp : StartDirectInput() : SetProperty(X : 処理に失敗しました。"), nullptr, MB_OK);
            return false;
        }

        dipr.diph.dwObj = DIJOFS_Y;
        if (m_pJoy[i]->SetProperty(DIPROP_RANGE, &dipr.diph) != DI_OK)
        {
            MessageBox(nullptr, _T("DInput.cpp : StartDirectInput() : SetProperty(Y : 処理に失敗しました。"), nullptr, MB_OK);
            return false;
        }
    }

    DIPROPDWORD dipdw;
    dipdw.diph.dwSize = sizeof(DIPROPDWORD);
    dipdw.diph.dwHeaderSize = sizeof(dipdw.diph);
    dipdw.diph.dwHow = DIPH_BYOFFSET;
    dipdw.dwData = DEADZONE;

    for (i = 0; i < m_nJoySum; i++)
    {
        if (!m_pJoy[i]) continue;
        if (!m_bJoyFF[i]) continue;

        dipdw.diph.dwObj = DIJOFS_X;
        if (m_pJoy[i]->SetProperty(DIPROP_DEADZONE, &dipdw.diph) != DI_OK)
        {
            MessageBox(nullptr, _T("DInput.cpp : StartDirectInput() : SetProperty(X : 処理に失敗しました。"), nullptr, MB_OK);
            return false;
        }

        dipdw.diph.dwObj = DIJOFS_Y;
        if (m_pJoy[i]->SetProperty(DIPROP_DEADZONE, &dipdw.diph) != DI_OK)
        {
            MessageBox(nullptr, _T("DInput.cpp : StartDirectInput() : SetProperty(Y : 処理に失敗しました。"), nullptr, MB_OK);
            return false;
        }
    }

    for (i = 0; i < m_nJoySum; i++)
    {
        if (!m_pJoy[i]) continue;
        if (!m_bJoyFF[i]) continue;

        dipdw.diph.dwSize = sizeof(DIPROPDWORD);
        dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
        dipdw.diph.dwObj = 0;
        dipdw.diph.dwHow = DIPH_DEVICE;
        //dipdw.dwData = DIPROPAUTOCENTER_ON;
        dipdw.dwData = DIPROPAUTOCENTER_OFF;
        m_hr = m_pJoy[i]->SetProperty(DIPROP_AUTOCENTER, &dipdw.diph);
        if (FAILED(m_hr))
        {
            MessageBox(nullptr, _T("DInput.cpp : StartDirectInput() : SetProperty( : 処理に失敗しました。"), nullptr,MB_OK);
            return false;
        }
    }

    m_BufferRestJoy = 0;
    m_pBufferPositionJoy = nullptr;

    for (i = 0; i < m_nJoySum; i++)
    {
        if (!m_pJoy[i]) continue;
        if (!m_bJoyFF[i]) continue;

        m_pBufferJoy[i] = new DIDEVICEOBJECTDATA[BufferSize];
        ZeroMemory(&dipdw, sizeof(dipdw));
        dipdw.diph.dwSize = sizeof(DIPROPDWORD);
        dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
        dipdw.diph.dwObj = 0;
        dipdw.diph.dwHow = DIPH_DEVICE;
        dipdw.dwData = BufferSize;
        m_pJoy[i]->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);

        if (m_pJoy[i]->Acquire() != DI_OK)
        {
            ;
        }

        // get the device capabilities
        DIDEVCAPS didc;
        didc.dwSize = sizeof(DIDEVCAPS);
        if (m_pJoy[i]->GetCapabilities(&didc) != DI_OK)
        {
            MessageBox(nullptr, _T(
                           "DInput.cpp : StartDirectInput() : GetCapabilities(ForceFeedback Joystick) : 処理に失敗しました。"),
                       nullptr, MB_OK);
            return false;
        }

        if (didc.dwFlags & DIDC_FORCEFEEDBACK)
        {
            //MessageBox(nullptr,_T("DInput.cpp : StartDirectInput() : ForceFeedback device found.\n"), nullptr, MB_OK); // -- 2018.8.27
            if (!CreateJoyEffectStandard())
            {
                MessageBox(nullptr,
                           _T("DInput.cpp : StartDirectInput() : CreateEffect(ForceFeedback Joystick) : 処理に失敗しました。"),
                           nullptr, MB_OK);
                return false;
            }
        }
    }

    return true;
}

//-----------------------------------------------------------------------------
// 
// 
//-----------------------------------------------------------------------------
bool CDirectInput::CheckJoyImm(DWORD kcode, int nSum)
{
    if (DIJ_LEFT > kcode)
    {
        return m_js[nSum].rgbButtons[kcode] & 0x80;
    }
    else
    {
        switch (kcode)
        {
        case DIJ_LEFT:
            return m_js[nSum].lX < (-1 * DIJ_VOLUME);
        case DIJ_RIGHT:
            return m_js[nSum].lX > DIJ_VOLUME;
        case DIJ_UP:
            return m_js[nSum].lY < (-1 * DIJ_VOLUME);
        case DIJ_DOWN:
            return m_js[nSum].lY > DIJ_VOLUME;
        default:
            MessageBox(nullptr, _T("DInput.cpp : CheckJoystickImm() : kcode : 処理に失敗しました。"), nullptr, MB_OK);
            return false;
        }
    }
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
bool CDirectInput::GetJoy(void)
{
    for (int i = 0; i < m_nJoySum; i++)
    {
        if (!m_pJoy[i]) return false;
        m_pJoy[i]->Poll();
        m_pJoy[i]->GetDeviceState(sizeof(DIJOYSTATE2), &m_js[i]);

        m_BufferRestJoy = BufferSize;
        m_hr = m_pJoy[i]->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), m_pBufferJoy[i], &m_BufferRestJoy, 0);
        if (FAILED(m_hr))
        {
            m_BufferRestJoy = BufferSize;
            m_pJoy[i]->Acquire();
            m_hr = m_pJoy[i]->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), m_pBufferJoy[i], &m_BufferRestJoy, 0);
            if (FAILED(m_hr))
            {
                return false;
            }
        }
        m_BufferRestBackupJoy[i] = m_BufferRestJoy;
    }
    return true;
}

//-----------------------------------------------------------------------------
// 
// 
//-----------------------------------------------------------------------------
bool CDirectInput::CheckUpDownLeftRight(int kmode, DWORD kcode, int nSum)
{
    if (m_didodJoy[nSum]->dwOfs == 4)
    {
        switch (kcode)
        {
        case DIJ_UP:
            if (kmode == KD_TRG)
            {
                if ((long)m_didodJoy[nSum]->dwData < (-1 * DIJ_VOLUME)) return true;
            }
            else
            {
                if (!((long)m_didodJoy[nSum]->dwData < (-1 * DIJ_VOLUME))) return true;
            }
            break;
        case DIJ_DOWN:
            if (kmode == KD_TRG)
            {
                if ((long)m_didodJoy[nSum]->dwData > DIJ_VOLUME) return true;
            }
            else
            {
                if (!((long)m_didodJoy[nSum]->dwData > DIJ_VOLUME)) return true;
            }
        }
    }
    else
    {
        if (m_didodJoy[nSum]->dwOfs == 0)
        {
            switch (kcode)
            {
            case DIJ_LEFT:
                if (kmode == KD_TRG)
                {
                    if ((long)m_didodJoy[nSum]->dwData < (-1 * DIJ_VOLUME)) return true;
                }
                else
                {
                    if (!((long)m_didodJoy[nSum]->dwData < (-1 * DIJ_VOLUME))) return true;
                }
                break;
            case DIJ_RIGHT:
                if (kmode == KD_TRG)
                {
                    if ((long)m_didodJoy[nSum]->dwData > DIJ_VOLUME) return true;
                }
                else
                {
                    if (!((long)m_didodJoy[nSum]->dwData > DIJ_VOLUME)) return true;
                }
            }
        }
    }
    return false;
}

//-----------------------------------------------------------------------------
// 
// 
//-----------------------------------------------------------------------------
bool CDirectInput::CheckJoy(const int& kmode, const DWORD& kcode, int nSum)
{
    if (!m_bInputActive || !m_pJoy[nSum]) return false;


    switch (kmode)
    {
    case KD_DAT:
        return CheckJoyImm(kcode, nSum);
    case KD_TRG:
    case KD_UTRG:
        m_BufferRestJoy = m_BufferRestBackupJoy[nSum];
        m_pBufferPositionJoy = m_pBufferJoy[nSum];
        while (m_BufferRestJoy > 0)
        {
            m_BufferRestJoy--;
            m_didodJoy[nSum] = m_pBufferPositionJoy;
            m_pBufferPositionJoy++;
#if 0
            TCHAR chBuffer[128];
            _stprintf(chBuffer, _T("DInput.cpp : CheckNJoy() : cdwOfs=%ld, dwData=%ld\n"), m_didodJoy[nSum]->dwOfs,
                      m_didodJoy[nSum]->dwData);
            ErrorMessage(chBuffer);
#endif
            if (kcode >= DIJ_LEFT && kcode <= DIJ_DOWN)
            {
                return CheckUpDownLeftRight(kmode, kcode, nSum);
            }
            else
            {
                if (m_didodJoy[nSum]->dwOfs == (DIJOFS_BUTTON(kcode)))
                {
                    if (kmode == KD_TRG)
                    {
                        if (m_didodJoy[nSum]->dwData) return true;
                    }
                    else
                    {
                        if (!(m_didodJoy[nSum]->dwData)) return true;
                    }
                }
            }
        }
        return false;
    default:
        MessageBox(nullptr, _T("DInput.cpp : CheckJoy() : : 処理に失敗しました。"), nullptr, MB_OK);
        return false;
    }
}

//-----------------------------------------------------------------------------
//
//
//-----------------------------------------------------------------------------
bool CDirectInput::IfJoyFF(int nSum)
{
    if (!m_pJoy[nSum]) return false;
    if (m_bJoyFF[nSum])
    {
        return true;
    }
    else
    {
        return false;
    }
}

//-----------------------------------------------------------------------------
//
//
//-----------------------------------------------------------------------------
DIJOYSTATE2 CDirectInput::GetJoyState(int nSum)
{
    return m_js[nSum];
}

//-----------------------------------------------------------------------------
//
//
//-----------------------------------------------------------------------------
int CDirectInput::GetJoyNum(void)
{
    return m_nJoySum;
}

/*
//-----------------------------------------------------------------------------
//                                                             !! OLD  !!
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
       if (!m_bJoyFF[i]) continue;


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
          MessageBox(nullptr, _T("DInput.cpp : CreateEffect() : CreateEffect(Bounce) : 処理に失敗しました。"), nullptr, MB_OK);
          return false;
       }

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
          MessageBox(nullptr, _T("DInput.cpp : CreateEffect() : CreateEffect(Fire) : 処理に失敗しました。"), nullptr, MB_OK);
          return false;
       }

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
          MessageBox(nullptr, _T("DInput.cpp : CreateEffect() : CreateEffect(Explode) : 処理に失敗しました。"), nullptr, MB_OK);
          return false;
       }

       m_nJoyEFSum = 3;

    }

    return true;
}
*/

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool CDirectInput::CreateJoyEffectStandard(void)
{
    DIEFFECT diEffect;
    DIENVELOPE diEnvelope;
    DWORD rgdwAxes[2] = {0, 0};
    LONG rglDirections[2] = {0, 0};
    DICONSTANTFORCE dicf = {0};
    DIPERIODIC dipf = {0};
    int i;


    for (i = 0; i < m_nJoySum; i++)
    {
        if (!m_pJoy[i]) continue;
        if (!m_bJoyFF[i]) continue;

        ZeroMemory(&diEffect, sizeof(DIEFFECT));
        ZeroMemory(&diEnvelope, sizeof(DIENVELOPE));

        // these fields are the same for all effects we will be creating
        diEffect.dwSize = sizeof(DIEFFECT);
        diEffect.dwSamplePeriod = 0; // use default sample period
        diEffect.dwTriggerButton = DIEB_NOTRIGGER;
        diEffect.dwTriggerRepeatInterval = 0;
        diEffect.rgdwAxes = rgdwAxes;
        diEffect.rglDirection = rglDirections;
        diEffect.dwGain = FF_ADULT;

        if (m_pJoyEffect[i][EF_BOUNCE])
        {
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

        if (m_pJoy[i]->CreateEffect(GUID_ConstantForce, &diEffect, &m_pJoyEffect[i][EF_BOUNCE], nullptr) != DI_OK)
        {
            MessageBox(nullptr, _T("DInput.cpp : CreateEffect() : CreateEffect(Bounce) : 処理に失敗しました。"), nullptr, MB_OK);
            return false;
        }

        if (m_pJoyEffect[i][EF_FIRE])
        {
            m_pJoyEffect[i][EF_FIRE]->Release();
            m_pJoyEffect[i][EF_FIRE] = nullptr;
        }

        dicf.lMagnitude = 3000; // -- 2018.8.27

        rgdwAxes[0] = DIJOFS_Y;
        rgdwAxes[1] = 0; // -- 2018.8.27
        rglDirections[0] = 1;
        rglDirections[1] = 0; // -- 2018.8.27

        diEffect.dwFlags = DIEFF_OBJECTOFFSETS | DIEFF_CARTESIAN;
        //diEffect.dwDuration = 20000;
        diEffect.dwDuration = 200000; // -- 2023.1.3
        diEffect.cAxes = 2; // -- 2018.8.27
        diEffect.lpEnvelope = nullptr;
        diEffect.cbTypeSpecificParams = sizeof(DICONSTANTFORCE);
        diEffect.lpvTypeSpecificParams = &dicf;

        if (m_pJoy[i]->CreateEffect(GUID_ConstantForce, &diEffect, &m_pJoyEffect[i][EF_FIRE], nullptr) != DI_OK)
        {
            MessageBox(nullptr, _T("DInput.cpp : CreateEffect() : CreateEffect(Fire) : 処理に失敗しました。"), nullptr, MB_OK);
            return false;
        }

        if (m_pJoyEffect[i][EF_EXPLODE])
        {
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
        rgdwAxes[1] = 0; // -- 2018.8.27
        rglDirections[0] = 0;
        rglDirections[1] = 0; // -- 2018.8.27

        diEffect.dwFlags = DIEFF_OBJECTOFFSETS | DIEFF_CARTESIAN;
        //diEffect.dwDuration = 1000000;
        diEffect.dwDuration = 2000000; // -- 2023.1.3
        diEffect.cAxes = 1;
        diEffect.lpEnvelope = &diEnvelope;
        diEffect.cbTypeSpecificParams = sizeof(DIPERIODIC);
        diEffect.lpvTypeSpecificParams = &dipf;

        if (m_pJoy[i]->CreateEffect(GUID_Square, &diEffect, &m_pJoyEffect[i][EF_EXPLODE], nullptr) != DI_OK)
        {
            MessageBox(nullptr, _T("DInput.cpp : CreateEffect() : CreateEffect(Explode) : 処理に失敗しました。"), nullptr, MB_OK);
            return false;
        }

        m_nJoyEFSum = 3;
    }

    return true;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
BOOL CALLBACK EnumEffectsInFileProc(LPCDIFILEEFFECT lpdife, LPVOID pvRef)
{
    return g_pDI->EnumEffectsInFileCb(lpdife, pvRef);
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
BOOL CDirectInput::EnumEffectsInFileCb(LPCDIFILEEFFECT lpdife, LPVOID pvRef)
{
    HRESULT hr;

    hr = m_pJoy[m_nJoyEFI]->CreateEffect(lpdife->GuidEffect, lpdife->lpDiEffect,
                                         &m_pJoyEffect[m_nJoyEFI][m_nJoyEFSum], nullptr);
    if (DI_OK != hr)
    {
        MessageBox(nullptr, _T("DInput.cpp : EnumEffectsInFileProc() : CreateEffect() : 処理に失敗しました。"), nullptr, MB_OK);
        return DIENUM_CONTINUE;
    }

    if (++m_nJoyEFSum >= JOY_EF_COUNT) return DIENUM_STOP;
    return DIENUM_CONTINUE;
}

//-----------------------------------------------------------------------------
// 
// 
//-----------------------------------------------------------------------------
bool CDirectInput::ReadJoyEffect(const TCHAR* szFName, int& nEffectNo, int& nNum)
{
    int i, nWSum, nMaxSum;

    nMaxSum = nWSum = m_nJoyEFSum;
    nEffectNo = 0;
    nNum = 0;

    for (i = 0; i < m_nJoySum; i++)
    {
        if (!m_pJoy[i]) continue;
        if (!m_bJoyFF[i]) continue;

        m_nJoyEFSum = nWSum;
        m_nJoyEFI = i;

        if (m_pJoy[i]->EnumEffectsInFile(szFName, EnumEffectsInFileProc, nullptr, DIFEF_MODIFYIFNEEDED) != DI_OK)
        {
            MessageBox(nullptr, _T("DInput.cpp : ReadJoyEffect() : EnumEffectsInFile() : 処理に失敗しました。"), nullptr, MB_OK);
            return false;
        }
        if (m_nJoyEFSum > nMaxSum) nMaxSum = m_nJoyEFSum;
    }
    m_nJoyEFSum = nMaxSum;
    nEffectNo = nWSum;
    nNum = m_nJoyEFSum - nEffectNo;

    return true;
}

//-----------------------------------------------------------------------------
// 
// 
//-----------------------------------------------------------------------------
bool CDirectInput::PlayJoyEffect(int nEffectNo, int nNum, int nSum)
{
    int i;

    if (!m_bJoyFF[nSum]) return false;

    for (i = 0; i < nNum; i++)
    {
        if (m_pJoyEffect[nSum][nEffectNo + i])
        {
            if (m_pJoyEffect[nSum][nEffectNo + i]->Start(1, 0) != DI_OK)
            {
                MessageBox(nullptr, _T("DInput.cpp : PlayJoyEffect() : Start(Joy effect) : 処理に失敗しました。"), nullptr,
                           MB_OK);
                return false;
            }
        }
    }
    return true;
}
