//-----------------------------------------------------------------------------
// DInput.h : DirectInputを簡単に利用するためのライブラリ(ヘッダー)
//
//                                               ver 3.3        2024.3.23
//
// Copyright (c) 2019 静岡産業技術専門学校 ゲームクリエイト科 All rights reserved.
//-----------------------------------------------------------------------------

#ifndef _DINPUT_H_

#define DIRECTINPUT_VERSION 0x0800

#include <cstdint>
#include <dinput.h>

// 必要なライブラリファイルのロード
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dinput8.lib")

//-----------------------------------------------------------------------------
// キー判定モード (ALL)
//-----------------------------------------------------------------------------
#define KD_DAT  0                 // 生データ（現在のキーの状態）
#define KD_TRG  1                 // キーが押された瞬間
#define KD_UTRG 2                 // キーが離された瞬間

#define BufferSize  20
//-----------------------------------------------------------------------------
// マウス用定義
//-----------------------------------------------------------------------------
#define DIM_LBUTTON 0
#define DIM_RBUTTON 1
#define DIM_MBUTTON 2

#define DIM_LEFT    10
#define DIM_RIGHT   11
#define DIM_UP      12
#define DIM_DOWN    13

//-----------------------------------------------------------------------------
// ジョイスティック (最大同時接続数)
//-----------------------------------------------------------------------------
#define JOYSTICK_COUNT  (4)
#define JOY_PLAYER1    (0)
#define JOY_PLAYER2    (1)
#define JOY_PLAYER3    (2)
#define JOY_PLAYER4    (3)

//-----------------------------------------------------------------------------
// フォースフィードバック ジョイスティック エフェクト最大数
//-----------------------------------------------------------------------------
#define JOY_EF_COUNT    32

#define DIJ_VOLUME  850
//-----------------------------------------------------------------------------
// DirectInput スキャンコード (Microsoft SideWinder ゲームパッド準拠)
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
// 方向キー (for Microsoft SideWinder ゲームパッド準拠)
//-----------------------------------------------------------------------------
#define DIJ_LEFT    10
#define DIJ_RIGHT   11
#define DIJ_UP      12
#define DIJ_DOWN    13
//-----------------------------------------------------------------------------
// DirectInput スキャンコード (Microsoft フォースフィードバック ジョイスティック用)
//-----------------------------------------------------------------------------
#define DIF_FIRE       0
#define DIF_SHIELD     1
#define DIF_STOP       2
#define DIF_STOPD      3           // STOPボタンの下のボタン
#define DIF_A          4
#define DIF_B          5
#define DIF_C          6
#define DIF_D          7
#define DIF_RESERVED   8
#define DIF_ARROW      9           // 十字ボタン
#define DIF_THROW      (-1)          // 未定義

#define DIF_LEFT       10
#define DIF_RIGHT      11
#define DIF_UP         12
#define DIF_DOWN       13
//-----------------------------------------------------------------------------
// デバイス初期化フラグ
//-----------------------------------------------------------------------------
#define INIT_KEYBOARD   1
#define INIT_MOUSE      2
#define INIT_JOYSTICK   4

#define DIMOFS_BUTTON(n) ((FIELD_OFFSET(DIMOUSESTATE, rgbButtons) + (n)))

#define DEADZONE       (2500)         // スティックの遊び（25%）
#define RANGE_MAX      (1000)         // 軸の最大値
#define RANGE_MIN      (-1000)        // 軸の最小値
#define FF_CHILD       (5000)         // 振動強度 50%
#define FF_ADULT       (7500)         // 振動強度 75%
#define FF_BODYBUILDER (10000)        // 振動強度 100%
//-----------------------------------------------------------------------------
// 内部エフェクト識別子
//-----------------------------------------------------------------------------
#define EF_BOUNCE   0
#define EF_EXPLODE  1
#define EF_FIRE     2

/// <summary>ButtonAction で扱う状態や種別を表す列挙型</summary>
enum ButtonAction : int8_t
{
    UP = -1,
    DOWN = 1,
    NONE = 0
};

// CDirectInput : 入力を一括管理するクラス
class CDirectInput
{
private:
    //-----------------------------------------------------------------------------
    // 共通
    //-----------------------------------------------------------------------------
private:
    HWND m_hWnd;
    LPDIRECTINPUT8 m_pDI8;
    LPDIRECTINPUTDEVICE8 m_pKey;
    DIDEVICEOBJECTDATA* m_pBufferKey;
    DIDEVICEOBJECTDATA* m_pBufferPositionKey;
    LPDIDEVICEOBJECTDATA m_didodKey;
    LPDIRECTINPUTDEVICE8 m_pMouse;
    DIDEVICEOBJECTDATA* m_pBufferMouse;
    DIDEVICEOBJECTDATA* m_pBufferPositionMouse;
    LPDIDEVICEOBJECTDATA m_didodMouse;
    LPDIRECTINPUTDEVICE8 m_pJoy[JOYSTICK_COUNT];
    DIDEVICEOBJECTDATA* m_pBufferJoy[JOYSTICK_COUNT];
    DIDEVICEOBJECTDATA* m_pBufferPositionJoy;
    LPDIDEVICEOBJECTDATA m_didodJoy[JOYSTICK_COUNT];
    LPDIRECTINPUTEFFECT m_pJoyEffect[JOYSTICK_COUNT][JOY_EF_COUNT];

    // --- 2. 4バイト（int, float, DWORD, HRESULT） ---
    HRESULT m_hr;
    float m_ViewWidth;
    float m_ViewHeight;
    DWORD m_BufferRestKey;
    DWORD m_BufferRestBackupKey;
    DWORD m_BufferRestMouse;
    DWORD m_BufferRestBackupMouse;
    DWORD m_BufferRestJoy;
    DWORD m_BufferRestBackupJoy[JOYSTICK_COUNT];
    int m_nJoySum;
    int m_nJoyFFNum;
    int m_nJoyEFSum;
    int m_nJoyEFI;

    // --- 3. 1バイト（BYTE配列, bool） ---
    // 大きな配列を先に置く
    BYTE m_diKeyState[256]; // 256は8の倍数なので安全
    DIMOUSESTATE m_dims; // 内部はlong(4)とBYTE(1)の混合だがここに配置
    DIJOYSTATE2 m_js[JOYSTICK_COUNT]; // 非常に巨大な構造体
    bool m_bInputActive;
    bool m_bJoyFF[JOYSTICK_COUNT];

public:
    //-----------------------------------------------------------------------------
    // 基本操作
    //-----------------------------------------------------------------------------
    /// CDirectInput を初期化する
    CDirectInput(void);
    /// CDirectInput の終了処理を行う
    ~CDirectInput();

    /// Direct Input を開始する
    /// @return 成功または条件を満たす場合 true
    bool StartDirectInput(HINSTANCE, HWND, int, DWORD, DWORD);
    /// Direct Input を終了する
    /// @return 成功または条件を満たす場合 true
    bool EndDirectInput(void);
    /// Acquire を設定する
    void SetAcquire(void); // デバイスの権限を取得
    /// Input を取得する
    /// @return 成功または条件を満たす場合 true
    bool GetInput(void); // 全デバイスの入力を更新

    //-----------------------------------------------------------------------------
    // キーボード
    //-----------------------------------------------------------------------------
    /// Key を取得する
    /// @return 成功または条件を満たす場合 true
    bool GetKey(void);
    /// Key を確認する
    /// @param nKey nKey に渡す値
    /// @param nMode nMode に渡す値
    /// @return 成功または条件を満たす場合 true
    bool CheckKey(const int& nKey, const DWORD& nMode);
    /// Push Up Key を判定する
    /// @return 処理結果の数値
    int IsPushUpKey();
    /// Push Down Key を判定する
    /// @return 処理結果の数値
    int IsPushDownKey();
    /// Push Enter を判定する
    /// @return 成功または条件を満たす場合 true
    bool IsPushEnter();
    /// Key を初期化する
    /// @return 成功または条件を満たす場合 true
    bool InitKey(HWND);
    /// Property Key を設定する
    /// @return 成功または条件を満たす場合 true
    bool SetPropertyKey(void);

    //-----------------------------------------------------------------------------
    // マウス
    //-----------------------------------------------------------------------------
    /// Mouse を取得する
    /// @return 成功または条件を満たす場合 true
    bool GetMouse(void);
    /// Mouse を確認する
    /// @param nButton nButton に渡す値
    /// @param nMode nMode に渡す値
    /// @return 成功または条件を満たす場合 true
    bool CheckMouse(const int& nButton, const DWORD& nMode);
    /// Mouse State を取得する
    /// @return 処理結果
    DIMOUSESTATE GetMouseState(void);
    /// Mouse Wheel を取得する
    /// @return 計算結果の値
    float GetMouseWheel() const;
    /// Mouse Move を判定する
    /// @return 成功または条件を満たす場合 true
    bool IsMouseMove() const;
    /// Move Input を判定する
    /// @return 成功または条件を満たす場合 true
    bool IsMoveInput();
    /// Mouse Pos を取得する
    /// @return 座標
    POINT GetMousePos(); // マウスの現在の座標を取得
    /// Show Mouse Cursor の処理を行う
    /// @param bFlag bFlag に渡す値
    void ShowMouseCursor(bool bFlag);
    /// Mouse を初期化する
    /// @return 成功または条件を満たす場合 true
    bool InitMouse(HWND);
    /// Property Mouse を設定する
    /// @return 成功または条件を満たす場合 true
    bool SetPropertyMouse(void);

    //-----------------------------------------------------------------------------
    // ジョイスティック
    //-----------------------------------------------------------------------------
    /// Joy を取得する
    /// @return 成功または条件を満たす場合 true
    bool GetJoy(void);
    /// If Joy FF を返す
    /// @param nSum nSum に渡す値
    /// @return 成功または条件を満たす場合 true
    bool IfJoyFF(int nSum = JOY_PLAYER1); // 振動対応か確認
    /// Joy State を取得する
    /// @param nSum nSum に渡す値
    /// @return 処理結果
    DIJOYSTATE2 GetJoyState(int nSum = JOY_PLAYER1);
    /// Joy Num を取得する
    /// @return 処理結果の数値
    int GetJoyNum(void);
    /// Joy を確認する
    /// @param nButton nButton に渡す値
    /// @param nMode nMode に渡す値
    /// @param nSum nSum に渡す値
    /// @return 成功または条件を満たす場合 true
    bool CheckJoy(const int& nButton, const DWORD& nMode, int nSum = JOY_PLAYER1);
    /// Up Down Left Right を確認する
    /// @param nDir nDir に渡す値
    /// @param nMode nMode に渡す値
    /// @param nSum nSum に渡す値
    /// @return 成功または条件を満たす場合 true
    bool CheckUpDownLeftRight(int nDir, DWORD nMode, int nSum = JOY_PLAYER1);
    /// Joy を初期化する
    /// @return 成功または条件を満たす場合 true
    bool InitJoy(HWND);
    /// Joy Normal を初期化する
    /// @return 成功または条件を満たす場合 true
    bool InitJoyNormal(HWND);
    /// Joy FF を初期化する
    /// @return 成功または条件を満たす場合 true
    bool InitJoyFF(HWND);
    /// Property Joy を設定する
    /// @return 成功または条件を満たす場合 true
    bool SetPropertyJoy(void);
    /// Joy Imm を確認する
    /// @param nButton nButton に渡す値
    /// @param nSum nSum に渡す値
    /// @return 成功または条件を満たす場合 true
    bool CheckJoyImm(DWORD nButton, int nSum = JOY_PLAYER1);
    /// Joy Effect を再生する
    /// @param nEffectNo nEffectNo に渡す値
    /// @param nNum nNum に渡す値
    /// @param nSum nSum に渡す値
    /// @return 成功または条件を満たす場合 true
    bool PlayJoyEffect(int nEffectNo, int nNum = 1, int nSum = JOY_PLAYER1);
    /// Joy Effect Standard を作成する
    /// @return 成功または条件を満たす場合 true
    bool CreateJoyEffectStandard(void);
    /// Joy Effect を読み込む
    /// @param szFName 名前
    /// @param nEffectNo nEffectNo に渡す値
    /// @param nNum nNum に渡す値
    /// @return 成功または条件を満たす場合 true
    bool ReadJoyEffect(const TCHAR* szFName, int& nEffectNo, int& nNum);

    //-----------------------------------------------------------------------------
    // コールバック関数
    //-----------------------------------------------------------------------------
    /// Enum Joysticks Cb を返す
    /// @param pInst pInst に渡す値
    /// @param lpvContext 使用するテクスチャ
    /// @return 処理結果
    BOOL EnumJoysticksCb(LPCDIDEVICEINSTANCE pInst, LPVOID lpvContext);
    /// Enum Effects In File Cb を返す
    /// @param lpdife lpdife に渡す値
    /// @param pvRef pvRef に渡す値
    /// @return 処理結果
    BOOL EnumEffectsInFileCb(LPCDIFILEEFFECT lpdife, LPVOID pvRef);
};

#endif
