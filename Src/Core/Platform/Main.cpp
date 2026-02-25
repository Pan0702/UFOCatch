//=============================================================================
//     3Dゲームプログラム                     ver 3.2        2023.1.31
//
//     ウィンドウ系の処理
//                                               Main.cpp
//=============================================================================
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <locale.h>
#include "MainControl.h"
#pragma warning(disable : 28251)

#include "../Game/GameMain.h"
#include "../../Utils/resource.h"


// グローバル変数 (コールバック関数のために必要)
static CMain* g_pMain = nullptr;    // メインクラスのインスタンス

//------------------------------------------------------------------------
//
//  アプリケーションのエントリポイント関数     
//
//  HINSTANCE hInstance      // 現在のインスタンスのハンドル
//  HINSTANCE hPrevInstance  // 以前のインスタンスのハンドル
//  LPTSTR    lpCmdLine      // コマンドライン引数
//  INT       nCmdShow       // 表示設定
//
//  戻り値 INT
//     メッセージループに入る前に終了した場合は 0
//
//------------------------------------------------------------------------
INT WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR, INT)
{
    // メモリリークのチェックを行う設定 -----------------------
    // すべてのレポート出力をデバッグウィンドウに送る
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
    _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
    _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_DEBUG);
    // プログラムの最初で設定することで、終了時に _CrtDumpMemoryLeaks を自動で呼び出す
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    // ロケールの設定
    // ワイド文字列を使用する場合、ロケールを指定しないと日本語が正しくコード変換されない
    _tsetlocale(LC_ALL, _T(""));

    CMain* pMain = new CMain; // メインクラスの生成
    g_pMain = pMain;         // コールバック関数のためにグローバルにポインタを置く

    if(SUCCEEDED(pMain->InitWindow(hInstance, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, APP_NAME))) // ウィンドウの初期化
    {
       if (SUCCEEDED(pMain->Init()))     // ゲーム初期化
       {
          pMain->MessageLoop();         // メッセージループ開始
       }
    }

    // 終了処理
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
//  (実際の処理は CMainクラスの MsgProc で行います)  
//
//  HWND hWnd         ウィンドウハンドル
//  UINT iMsg         メッセージ
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
// CMain メインクラスの処理
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

    m_bLoopFlag = true;              // falseになるとループを抜けてプログラム終了
    m_MainLoopTime = 1000000.0 / 60; // ループスピード 1/60Sec (マイクロ秒単位)
}

//------------------------------------------------------------------------
//
//  CMain デストラクタ    
//
//------------------------------------------------------------------------
CMain::~CMain()
{
    SAFE_DELETE(m_pGMain);  // ゲームメインクラスの削除
}

//------------------------------------------------------------------------
//
//  ウィンドウの作成  
//
//------------------------------------------------------------------------
HRESULT CMain::InitWindow(HINSTANCE hInstance,
                             INT iX, INT iY, INT iWidth, INT iHeight, LPCTSTR WindowName)
{
     m_hInstance = hInstance;

    // ウィンドウクラスの定義
    WNDCLASSEX  wc;
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

    // ウィンドウサイズの計算（クライアント領域を指定サイズにする）
    RECT rc = { 0, 0, iWidth, iHeight };
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, false);

    // ウィンドウの作成
    m_hWnd = CreateWindowEx(0, WindowName, WindowName,
       WS_OVERLAPPEDWINDOW,
       CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top,
       (HWND)nullptr, (HMENU)nullptr,
       hInstance, (LPVOID)nullptr);

    if(!m_hWnd) 
    {
       MessageBox(0, _T("ウィンドウを作成できませんでした"), nullptr, MB_OK);
       return E_FAIL;
    }
    // ウィンドウの表示
    ShowWindow(m_hWnd, SW_SHOW);
    UpdateWindow(m_hWnd);

    return S_OK;
}
//------------------------------------------------------------------------
//
//  ウィンドウプロシージャ（メッセージ処理）
//
//------------------------------------------------------------------------
LRESULT CMain::MsgProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    switch(iMsg)
    {
       case WM_KEYDOWN:
       switch((char)wParam)
       {
          case VK_ESCAPE: // ESCキーで終了
                PostQuitMessage(0);
                break;
       }
       break;
       case WM_DESTROY:
             PostQuitMessage(0);
             break;
    }

    // ImGui のメッセージ処理
    ImGui_ImplWin32_WndProcHandler(hWnd, iMsg, wParam, lParam);

    return DefWindowProc (hWnd, iMsg, wParam, lParam);
}

//------------------------------------------------------------------------
//
//  メッセージループ
//
//  アプリケーションのメインループを実行。
//  1/60秒ごとに更新を行うためにウェイト処理を入れることが可能。
//
//  ① アプリケーション側でウェイトをかける方法
//     QueryPerformanceCounter を使用してマイクロ秒単位で待機。
//  ② アプリケーション側でウェイトをかけない方法 (Vsync同期)
//     描画（Present）時に垂直同期を待つ設定にする。
//
//------------------------------------------------------------------------
void CMain::MessageLoop()
{
    // 現在のコード構成では、MainControl::RefreshTimer() の設定により動作が変わる
#if 1
    MSG  msg = { 0 };
    ZeroMemory(&msg, sizeof(msg));
    LARGE_INTEGER StartTime, EndTime, Frq;
    QueryPerformanceFrequency(&Frq);   // 周波数の取得
    double MicroSec = 1000000.0 / (double)Frq.QuadPart;   // 1カウントあたりのマイクロ秒
    double ElapsedTime;
    QueryPerformanceCounter(&StartTime);      // ループ開始時間

    while (msg.message != WM_QUIT && m_bLoopFlag)
    {
       // ゲームの更新と描画
       m_pGMain->Update();
       m_pGMain->Draw();

       // FPS表示
       DispFps();

       float timer = MainControl::RefreshTimer();
       if (timer > 0.0f) {
          // 自前でウェイトをかける処理（タイマー指定時）
          do {
             if (!MessageProcess(&msg)) 
             {
                m_bLoopFlag = false; // WM_QUIT受信
                break;
             }
             QueryPerformanceCounter(&EndTime);
             ElapsedTime = (EndTime.QuadPart - StartTime.QuadPart) * MicroSec;
          } while (ElapsedTime < timer * 1000000.0f); // 設定時間までループ
          StartTime = EndTime;
       }
       else {
          // Vsync 等に任せる場合、またはメッセージ主導
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
//  メッセージプロセス（メッセージが溜まっている間の処理）
//
//------------------------------------------------------------------------
bool CMain::MessageProcess(MSG* pMsg)
{
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
    else {
       return true;
    }
}

//------------------------------------------------------------------------
//
//  初期化処理
//
//------------------------------------------------------------------------
HRESULT CMain::Init()
{
    m_pGMain = new CGameMain( this ); // ゲームメインクラスの生成
    m_pGMain->Init();                 // ゲーム初期化

    return S_OK;
}

//------------------------------------------------------------------------
//
//  メインループ処理（簡易呼び出し用）
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
//  終了処理
//
//------------------------------------------------------------------------
void CMain::Quit()
{
    m_pGMain->Quit();
}

//------------------------------------------------------------------------
//
//  タイトルバーにFPSを表示
//
//------------------------------------------------------------------------
void CMain::DispFps()
{
    static DWORD time = 0;
    static int frame = 0;
    frame++;
    TCHAR str[50];
    _stprintf_s(str, _T("    fps=%d"), frame);
    if (timeGetTime() - time > 1000) {
       time = timeGetTime();
       frame = 0;
       TCHAR AppName[256] = { 0 };
       GetClassName(m_hWnd, AppName, sizeof(AppName) / sizeof(TCHAR));
       _tcscat_s(AppName, str);
       SetWindowText(m_hWnd, AppName);
    }
}