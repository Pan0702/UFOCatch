//-----------------------------------------------------------------------------
// DInput.h : DirectInput繧堤ｰ｡蜊倥↓蛻ｩ逕ｨ縺吶ｋ縺溘ａ縺ｮ繝ｩ繧､繝悶Λ繝ｪ(繝倥ャ繝繝ｼ)
//
//                                               ver 3.3        2024.3.23
//
// Copyright (c) 2019 髱吝ｲ｡逕｣讌ｭ謚陦灘ｰる摩蟄ｦ譬｡ 繧ｲ繝ｼ繝繧ｯ繝ｪ繧ｨ繧､繝育ｧ・All rights reserved.
//-----------------------------------------------------------------------------

#ifndef _DINPUT_H_
#define _DINPUT_H_

#define DIRECTINPUT_VERSION 0x0800

#include <dinput.h>
#include <tchar.h>

// 蠢・ｦ√↑繝ｩ繧､繝悶Λ繝ｪ繝輔ぃ繧､繝ｫ縺ｮ繝ｭ繝ｼ繝・
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dinput8.lib")

//-----------------------------------------------------------------------------
// 繧ｭ繝ｼ蛻､螳壹Δ繝ｼ繝・(ALL)
//-----------------------------------------------------------------------------
#define KD_DAT  0                 // 逕溘ョ繝ｼ繧ｿ・育樟蝨ｨ縺ｮ繧ｭ繝ｼ縺ｮ迥ｶ諷具ｼ・
#define KD_TRG  1                 // 繧ｭ繝ｼ縺梧款縺輔ｌ縺溽椪髢・
#define KD_UTRG 2                 // 繧ｭ繝ｼ縺碁屬縺輔ｌ縺溽椪髢・

#define BufferSize  20
//-----------------------------------------------------------------------------
// 繝槭え繧ｹ逕ｨ螳夂ｾｩ
//-----------------------------------------------------------------------------
#define DIM_LBUTTON 0
#define DIM_RBUTTON 1
#define DIM_MBUTTON 2

#define DIM_LEFT    10
#define DIM_RIGHT   11
#define DIM_UP      12
#define DIM_DOWN    13

//-----------------------------------------------------------------------------
// 繧ｸ繝ｧ繧､繧ｹ繝・ぅ繝・け (譛螟ｧ蜷梧凾謗･邯壽焚)
//-----------------------------------------------------------------------------
#define JOYSTICK_COUNT  (4)
#define JOY_PLAYER1    (0)
#define JOY_PLAYER2    (1)
#define JOY_PLAYER3    (2)
#define JOY_PLAYER4    (3)

//-----------------------------------------------------------------------------
// 繝輔か繝ｼ繧ｹ繝輔ぅ繝ｼ繝峨ヰ繝・け 繧ｸ繝ｧ繧､繧ｹ繝・ぅ繝・け 繧ｨ繝輔ぉ繧ｯ繝域怙螟ｧ謨ｰ
//-----------------------------------------------------------------------------
#define JOY_EF_COUNT    32  

#define DIJ_VOLUME  850
//-----------------------------------------------------------------------------
// DirectInput 繧ｹ繧ｭ繝｣繝ｳ繧ｳ繝ｼ繝・(Microsoft SideWinder 繧ｲ繝ｼ繝繝代ャ繝画ｺ匁侠)
//-----------------------------------------------------------------------------
#define DIJ_A       0
#define DIJ_B       1
#define DIJ_C       2
#define DIJ_X       3
#define DIJ_Y       4
#define DIJ_Z       5
#define DIJ_L       6
#define DIJ_R       7
#define DIJ_START   8
#define DIJ_M       9
//-----------------------------------------------------------------------------
// 譁ｹ蜷代く繝ｼ (for Microsoft SideWinder 繧ｲ繝ｼ繝繝代ャ繝画ｺ匁侠)
//-----------------------------------------------------------------------------
#define DIJ_LEFT    10
#define DIJ_RIGHT   11
#define DIJ_UP      12
#define DIJ_DOWN    13
//-----------------------------------------------------------------------------
// DirectInput 繧ｹ繧ｭ繝｣繝ｳ繧ｳ繝ｼ繝・(Microsoft 繝輔か繝ｼ繧ｹ繝輔ぅ繝ｼ繝峨ヰ繝・け 繧ｸ繝ｧ繧､繧ｹ繝・ぅ繝・け逕ｨ)
//-----------------------------------------------------------------------------
#define DIF_FIRE       0
#define DIF_SHIELD     1
#define DIF_STOP       2
#define DIF_STOPD      3           // STOP繝懊ち繝ｳ縺ｮ荳九・繝懊ち繝ｳ
#define DIF_A          4
#define DIF_B          5
#define DIF_C          6
#define DIF_D          7
#define DIF_RESERVED   8
#define DIF_ARROW      9           // 蜊∝ｭ励・繧ｿ繝ｳ
#define DIF_THROW      -1          // 譛ｪ螳夂ｾｩ

#define DIF_LEFT       10
#define DIF_RIGHT      11
#define DIF_UP         12
#define DIF_DOWN       13
//-----------------------------------------------------------------------------
// 繝・ヰ繧､繧ｹ蛻晄悄蛹悶ヵ繝ｩ繧ｰ
//-----------------------------------------------------------------------------
#define INIT_KEYBOARD   1
#define INIT_MOUSE      2
#define INIT_JOYSTICK   4

#define DIMOFS_BUTTON(n) (FIELD_OFFSET(DIMOUSESTATE, rgbButtons) + (n))

#define DEADZONE       2500         // 繧ｹ繝・ぅ繝・け縺ｮ驕翫・・・5%・・
#define RANGE_MAX      1000         // 霆ｸ縺ｮ譛螟ｧ蛟､
#define RANGE_MIN      -1000        // 霆ｸ縺ｮ譛蟆丞､
#define FF_CHILD       5000         // 謖ｯ蜍募ｼｷ蠎ｦ 50%
#define FF_ADULT       7500         // 謖ｯ蜍募ｼｷ蠎ｦ 75%
#define FF_BODYBUILDER 10000        // 謖ｯ蜍募ｼｷ蠎ｦ 100%
//-----------------------------------------------------------------------------
// 蜀・Κ繧ｨ繝輔ぉ繧ｯ繝郁ｭ伜挨蟄・
//-----------------------------------------------------------------------------
#define EF_BOUNCE   0
#define EF_EXPLODE  1
#define EF_FIRE     2

// CDirectInput : 蜈･蜉帙ｒ荳諡ｬ邂｡逅・☆繧九け繝ｩ繧ｹ
class CDirectInput
{
private:
    //-----------------------------------------------------------------------------
    // 蜈ｱ騾・
    //-----------------------------------------------------------------------------
    HWND           m_hWnd;
    HRESULT        m_hr;
    float          m_ViewWidth;          // 逕ｻ髱｢繧ｵ繧､繧ｺ・壼ｹ・
    float          m_ViewHeight;         // 逕ｻ髱｢繧ｵ繧､繧ｺ・夐ｫ倥＆
    LPDIRECTINPUT8 m_pDI8;               // DirectInput 繧ｪ繝悶ず繧ｧ繧ｯ繝・
    BYTE           m_diKeyState[256];    // 繧ｭ繝ｼ繝懊・繝峨・迥ｶ諷・
    DIMOUSESTATE   m_dims;               // 繝槭え繧ｹ縺ｮ迥ｶ諷・
    DIJOYSTATE2    m_js[JOYSTICK_COUNT]; // 繧ｸ繝ｧ繧､繧ｹ繝・ぅ繝・け縺ｮ迥ｶ諷・
    bool           m_bInputActive;
    
    //-----------------------------------------------------------------------------
    // 繧ｭ繝ｼ繝懊・繝臥畑繝｡繝ｳ繝・
    //-----------------------------------------------------------------------------
    LPDIRECTINPUTDEVICE8 m_pKey;
    DWORD m_BufferRestKey;
    DWORD m_BufferRestBackupKey;
    DIDEVICEOBJECTDATA* m_pBufferKey;
    DIDEVICEOBJECTDATA* m_pBufferPositionKey;
    LPDIDEVICEOBJECTDATA m_didodKey;
    
    //-----------------------------------------------------------------------------
    // 繝槭え繧ｹ逕ｨ繝｡繝ｳ繝・
    //-----------------------------------------------------------------------------
    LPDIRECTINPUTDEVICE8 m_pMouse;
    DWORD m_BufferRestMouse;
    DWORD m_BufferRestBackupMouse;
    DIDEVICEOBJECTDATA* m_pBufferMouse;
    DIDEVICEOBJECTDATA* m_pBufferPositionMouse;
    LPDIDEVICEOBJECTDATA m_didodMouse;
    
    //-----------------------------------------------------------------------------
    // 繧ｸ繝ｧ繧､繧ｹ繝・ぅ繝・け逕ｨ繝｡繝ｳ繝・
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
    // 蝓ｺ譛ｬ謫堺ｽ・
    //-----------------------------------------------------------------------------
    CDirectInput(void);
    ~CDirectInput();

    bool StartDirectInput(HINSTANCE, HWND, int, DWORD, DWORD);
    bool EndDirectInput(void);
    void SetAcquire(void); // 繝・ヰ繧､繧ｹ縺ｮ讓ｩ髯舌ｒ蜿門ｾ・
    bool GetInput(void);   // 蜈ｨ繝・ヰ繧､繧ｹ縺ｮ蜈･蜉帙ｒ譖ｴ譁ｰ
    
    //-----------------------------------------------------------------------------
    // 繧ｭ繝ｼ繝懊・繝・
    //-----------------------------------------------------------------------------
    bool GetKey(void);
    bool CheckKey(const int& nKey, const DWORD& nMode);
    bool InitKey(HWND);
    bool SetPropertyKey(void);
    
    //-----------------------------------------------------------------------------
    // 繝槭え繧ｹ
    //-----------------------------------------------------------------------------
    bool GetMouse(void);
    bool CheckMouse(const int& nButton, const DWORD& nMode);
    DIMOUSESTATE GetMouseState(void);
    float GetMouseWheel() const;
    bool IsMouseMove() const;
    bool IsMoveInput();
    POINT GetMousePos(); // 繝槭え繧ｹ縺ｮ迴ｾ蝨ｨ縺ｮ蠎ｧ讓吶ｒ蜿門ｾ・
    void ShowMouseCursor(bool bFlag);
    bool InitMouse(HWND);
    bool SetPropertyMouse(void);
    
    //-----------------------------------------------------------------------------
    // 繧ｸ繝ｧ繧､繧ｹ繝・ぅ繝・け
    //-----------------------------------------------------------------------------
    bool GetJoy(void);
    bool IfJoyFF(int nSum=JOY_PLAYER1); // 謖ｯ蜍募ｯｾ蠢懊°遒ｺ隱・
    DIJOYSTATE2 GetJoyState(int nSum=JOY_PLAYER1);
    int  GetJoyNum(void);
    bool CheckJoy(const int& nButton, const DWORD& nMode, int nSum=JOY_PLAYER1);
    bool CheckUpDownLeftRight(int nDir, DWORD nMode, int nSum=JOY_PLAYER1);
    bool InitJoy(HWND);
    bool InitJoyNormal(HWND);
    bool InitJoyFF(HWND);
    bool SetPropertyJoy(void);
    bool CheckJoyImm(DWORD nButton, int nSum=JOY_PLAYER1);
    bool PlayJoyEffect(int nEffectNo, int nNum=1, int nSum=JOY_PLAYER1);
    bool CreateJoyEffectStandard(void);
    bool ReadJoyEffect(const TCHAR* szFName, int &nEffectNo, int &nNum);

    //-----------------------------------------------------------------------------
    // 繧ｳ繝ｼ繝ｫ繝舌ャ繧ｯ髢｢謨ｰ
    //-----------------------------------------------------------------------------
    BOOL EnumJoysticksCb(LPCDIDEVICEINSTANCE pInst, LPVOID lpvContext);
    BOOL EnumEffectsInFileCb(LPCDIFILEEFFECT lpdife, LPVOID pvRef);

};

#endif
