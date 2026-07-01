//=============================================================================
//
//																Main.h
//=============================================================================

#pragma once

#include <atomic>
#include <stdio.h>
#include <thread>
#include <windows.h>

//#include "GameMain.h"

// #define WINDOW_HEIGHT  1080 //鬮倥＆

#define WINDOW_WIDTH   1366
#define WINDOW_HEIGHT  768

#define APP_NAME _T("UFOAbducition")

/// <summary>ゲーム全体の初期化、更新、描画、終了処理をまとめる中枢クラス</summary>
class CGameMain;

//  ---------------------------------------------------------
//  ---------------------------------------------------------
class CMain
{
public:
    CGameMain* m_pGMain;
    HINSTANCE m_hInstance;
    HWND m_hWnd;
    bool m_bLoopFlag;
    double m_MainLoopTime;

public:
    /// Window を初期化する
    /// @param INT INT に渡す値
    /// @param INT INT に渡す値
    /// @param INT INT に渡す値
    /// @param INT INT に渡す値
    /// @return 処理結果
    HRESULT InitWindow(HINSTANCE, INT, INT, INT, INT, LPCTSTR);
    /// Msg Proc を返す
    /// @param WPARAM WPARAM に渡す値
    /// @param LPARAM LPARAM に渡す値
    /// @return メッセージ処理結果
    LRESULT MsgProc(HWND, UINT, WPARAM, LPARAM);
    /// 初期化する
    /// @return 処理結果
    HRESULT Init();
    /// Message Loop の処理を行う
    void MessageLoop();
    /// Message Process を返す
    /// @param msg msg に渡す値
    /// @return 成功または条件を満たす場合 true
    bool MessageProcess(MSG* msg);
    /// Loop の処理を行う
    void Loop();
    /// Disp Fps の処理を行う
    void DispFps();
    /// 終了処理を行う
    void Quit();
    /// CMain を初期化する
    CMain();
    /// CMain の終了処理を行う
    ~CMain();

private:
    std::thread m_GameThread;
    std::atomic<bool> m_RunGame{false};

    /// Game Thread Func の処理を行う
    void GameThreadFunc();
};

/// Wnd Proc を返す
/// @param hWnd hWnd に渡す値
/// @param uMsg uMsg に渡す値
/// @param wParam wParam に渡す値
/// @param lParam lParam に渡す値
/// @return メッセージ処理結果
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

/// Im Gui Impl Win32 Wnd Proc Handler を返す
/// @param hWnd hWnd に渡す値
/// @param iMsg iMsg に渡す値
/// @param wParam wParam に渡す値
/// @param lParam lParam に渡す値
/// @return メッセージ処理結果
LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
