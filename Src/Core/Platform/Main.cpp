//=============================================================================
//     3Dゲームプログラム                     ver 3.2        2023.1.31
//
//                                               Main.cpp
//=============================================================================
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <locale.h>
#include "MainControl.h"
#pragma warning(disable : 28251)

#include "../Game/GameMain.h"
#include "../../Utils/resource.h"


static CMain* g_pMain = nullptr; // メインクラスのインスタンス

//------------------------------------------------------------------------
//
//  アプリケーションのエントリポイント関数     
//
//  HINSTANCE hInstance      // 現在のインスタンスのハンドル
//  LPTSTR    lpCmdLine      // コマンドライン引数
//  INT       nCmdShow       // 表示設定
//
//  戻り値 INT
//
//------------------------------------------------------------------------
INT WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR, INT)
{
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
    _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
    _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_DEBUG);
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    // ロケールの設定
    _tsetlocale(LC_ALL, _T(""));

    CMain* pMain = new CMain;
    g_pMain = pMain; // コールバック関数から参照できるように保持する

    if (SUCCEEDED(pMain->InitWindow(hInstance, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, APP_NAME)))
    {
        if (SUCCEEDED(pMain->Init()))
        {
            pMain->MessageLoop();
        }
    }

    pMain->Quit();
    delete pMain;

    return 0;
}

void CMain::GameThreadFunc()
{
}

//------------------------------------------------------------------------
//
//  OSから呼び出されるウィンドウプロシージャ
//
//  HWND hWnd         ウィンドウハンドル
//  WPARAM wParam     WPARAM
//  LPARAM lParam     LPARAM
//
//  戻り値 LRESULT
//     DefWindowProc の戻り値
//
//------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    return g_pMain->MsgProc(hWnd, uMsg, wParam, lParam);
}

// ============================================================================================
//
//
// ============================================================================================
//------------------------------------------------------------------------
//
//  CMain コンストラクタ  
//
//------------------------------------------------------------------------
CMain::CMain()
{
    ZeroMemory(this, sizeof(CMain));

    m_bLoopFlag = true;
    m_MainLoopTime = 1000000.0 / 60;
}

//------------------------------------------------------------------------
//
//
//------------------------------------------------------------------------
CMain::~CMain()
{
    SAFE_DELETE(m_pGMain); // ゲームメインクラスの削除
}

//------------------------------------------------------------------------
//
//
//------------------------------------------------------------------------
HRESULT CMain::InitWindow(HINSTANCE hInstance,
                          INT iX, INT iY, INT iWidth, INT iHeight, LPCTSTR WindowName)
{
    m_hInstance = hInstance;

    // ウィンドウクラスの定義
    WNDCLASSEX wc;
    ZeroMemory(&wc, sizeof(wc));
    wc.cbSize = sizeof(wc);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAIN_ICON));
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
    wc.lpszClassName = WindowName;
    wc.lpszMenuName = nullptr;
    RegisterClassEx(&wc);

    RECT rc = {0, 0, iWidth, iHeight};
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, false);

    m_hWnd = CreateWindowEx(0, WindowName, WindowName,
                            WS_OVERLAPPEDWINDOW,
                            CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top,
                            (HWND)nullptr, (HMENU)nullptr,
                            hInstance, (LPVOID)nullptr);

    if (!m_hWnd)
    {
        MessageBox(0, _T("処理に失敗しました。"), nullptr, MB_OK);
        return E_FAIL;
    }
    // ウィンドウの表示
    ShowWindow(m_hWnd, SW_SHOW);
    UpdateWindow(m_hWnd);

    return S_OK;
}

//------------------------------------------------------------------------
//
//
//------------------------------------------------------------------------
LRESULT CMain::MsgProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    switch (iMsg)
    {
    case WM_KEYDOWN:
        switch ((char)wParam)
        {
        case VK_ESCAPE:
            PostQuitMessage(0);
            break;
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }

    ImGui_ImplWin32_WndProcHandler(hWnd, iMsg, wParam, lParam);

    return DefWindowProc(hWnd, iMsg, wParam, lParam);
}

//------------------------------------------------------------------------
//
//
//  アプリケーションのメインループを実行、E
//
//  ① アプリケーション側でウェイトをかける方況E
//
//------------------------------------------------------------------------
void CMain::MessageLoop()
{
#if 1
    MSG msg = {0};
    ZeroMemory(&msg, sizeof(msg));
    LARGE_INTEGER StartTime, EndTime, Frq;
    QueryPerformanceFrequency(&Frq);
    double MicroSec = 1000000.0 / (double)Frq.QuadPart;
    double ElapsedTime;
    QueryPerformanceCounter(&StartTime);

    while (msg.message != WM_QUIT && m_bLoopFlag)
    {
        // ゲームの更新と描画
        m_pGMain->Update();
        m_pGMain->Draw();

        // FPS表示
        DispFps();

        float timer = MainControl::RefreshTimer();
        if (timer > 0.0f)
        {
            do
            {
                if (!MessageProcess(&msg))
                {
                    m_bLoopFlag = false; // WM_QUIT蜿嶺ｿ｡
                    break;
                }
                QueryPerformanceCounter(&EndTime);
                ElapsedTime = (EndTime.QuadPart - StartTime.QuadPart) * MicroSec;
            }
            while (ElapsedTime < timer * 1000000.0f);
            StartTime = EndTime;
        }
        else
        {
            if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
            QueryPerformanceCounter(&StartTime);
        }
    }
#endif
}

//------------------------------------------------------------------------
//
//
//------------------------------------------------------------------------
bool CMain::MessageProcess(MSG* pMsg)
{
    // 対象範囲を順に処理しながら必要な更新や判定を行う。
    while ((pMsg->message != WM_QUIT && m_bLoopFlag) &&
        PeekMessage(pMsg, nullptr, 0, 0, PM_REMOVE))
    {
        TranslateMessage(pMsg);
        DispatchMessage(pMsg);
    }

    if (pMsg->message == WM_QUIT || m_bLoopFlag == false)
    {
        return false;
    }
    else
    {
        return true;
    }
}

//------------------------------------------------------------------------
//
//
//------------------------------------------------------------------------
HRESULT CMain::Init()
{
    m_pGMain = new CGameMain(this);
    m_pGMain->Init();

    return S_OK;
}

//------------------------------------------------------------------------
//
//
//------------------------------------------------------------------------
void CMain::Loop()
{
    m_pGMain->Draw();
    m_pGMain->Update();
    DispFps();
}

//------------------------------------------------------------------------
//
//
//------------------------------------------------------------------------
void CMain::Quit()
{
    m_pGMain->Quit();
}

//------------------------------------------------------------------------
//
//
//------------------------------------------------------------------------
void CMain::DispFps()
{
    static DWORD time = 0;
    static int frame = 0;
    frame++;
    TCHAR str[50];
    _stprintf_s(str, _T("    fps=%d"), frame);
    if (timeGetTime() - time > 1000)
    {
        time = timeGetTime();
        frame = 0;
        TCHAR AppName[256] = {0};
        GetClassName(m_hWnd, AppName, sizeof(AppName) / sizeof(TCHAR));
        _tcscat_s(AppName, str);
        SetWindowText(m_hWnd, AppName);
    }
}
