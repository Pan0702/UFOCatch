//=============================================================================
//     3D繧ｲ繝ｼ繝繝励Ο繧ｰ繝ｩ繝                     ver 3.2        2023.1.31
//
//     繧ｦ繧｣繝ｳ繝峨え邉ｻ縺ｮ蜃ｦ逅・
//                                               Main.cpp
//=============================================================================
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <locale.h>
#include "MainControl.h"
#pragma warning(disable : 28251)

#include "../Game/GameMain.h"
#include "../../Utils/resource.h"


// 繧ｰ繝ｭ繝ｼ繝舌Ν螟画焚 (繧ｳ繝ｼ繝ｫ繝舌ャ繧ｯ髢｢謨ｰ縺ｮ縺溘ａ縺ｫ蠢・ｦ・
static CMain* g_pMain = nullptr;    // 繝｡繧､繝ｳ繧ｯ繝ｩ繧ｹ縺ｮ繧､繝ｳ繧ｹ繧ｿ繝ｳ繧ｹ

//------------------------------------------------------------------------
//
//  繧｢繝励Μ繧ｱ繝ｼ繧ｷ繝ｧ繝ｳ縺ｮ繧ｨ繝ｳ繝医Μ繝昴う繝ｳ繝磯未謨ｰ     
//
//  HINSTANCE hInstance      // 迴ｾ蝨ｨ縺ｮ繧､繝ｳ繧ｹ繧ｿ繝ｳ繧ｹ縺ｮ繝上Φ繝峨Ν
//  HINSTANCE hPrevInstance  // 莉･蜑阪・繧､繝ｳ繧ｹ繧ｿ繝ｳ繧ｹ縺ｮ繝上Φ繝峨Ν
//  LPTSTR    lpCmdLine      // 繧ｳ繝槭Φ繝峨Λ繧､繝ｳ蠑墓焚
//  INT       nCmdShow       // 陦ｨ遉ｺ險ｭ螳・
//
//  謌ｻ繧雁､ INT
//     繝｡繝・そ繝ｼ繧ｸ繝ｫ繝ｼ繝励↓蜈･繧句燕縺ｫ邨ゆｺ・＠縺溷ｴ蜷医・ 0
//
//------------------------------------------------------------------------
INT WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR, INT)
{
    // 繝｡繝｢繝ｪ繝ｪ繝ｼ繧ｯ縺ｮ繝√ぉ繝・け繧定｡後≧險ｭ螳・-----------------------
    // 縺吶∋縺ｦ縺ｮ繝ｬ繝昴・繝亥・蜉帙ｒ繝・ヰ繝・げ繧ｦ繧｣繝ｳ繝峨え縺ｫ騾√ｋ
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
    _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
    _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_DEBUG);
    // 繝励Ο繧ｰ繝ｩ繝縺ｮ譛蛻昴〒險ｭ螳壹☆繧九％縺ｨ縺ｧ縲∫ｵゆｺ・凾縺ｫ _CrtDumpMemoryLeaks 繧定・蜍輔〒蜻ｼ縺ｳ蜃ｺ縺・
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    // 繝ｭ繧ｱ繝ｼ繝ｫ縺ｮ險ｭ螳・
    // 繝ｯ繧､繝画枚蟄怜・繧剃ｽｿ逕ｨ縺吶ｋ蝣ｴ蜷医√Ο繧ｱ繝ｼ繝ｫ繧呈欠螳壹＠縺ｪ縺・→譌･譛ｬ隱槭′豁｣縺励￥繧ｳ繝ｼ繝牙､画鋤縺輔ｌ縺ｪ縺・
    _tsetlocale(LC_ALL, _T(""));

    CMain* pMain = new CMain; // 繝｡繧､繝ｳ繧ｯ繝ｩ繧ｹ縺ｮ逕滓・
    g_pMain = pMain;         // 繧ｳ繝ｼ繝ｫ繝舌ャ繧ｯ髢｢謨ｰ縺ｮ縺溘ａ縺ｫ繧ｰ繝ｭ繝ｼ繝舌Ν縺ｫ繝昴う繝ｳ繧ｿ繧堤ｽｮ縺・

    if(SUCCEEDED(pMain->InitWindow(hInstance, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, APP_NAME))) // 繧ｦ繧｣繝ｳ繝峨え縺ｮ蛻晄悄蛹・
    {
       if (SUCCEEDED(pMain->Init()))     // 繧ｲ繝ｼ繝蛻晄悄蛹・
       {
          pMain->MessageLoop();         // 繝｡繝・そ繝ｼ繧ｸ繝ｫ繝ｼ繝鈴幕蟋・
       }
    }

    // 邨ゆｺ・・逅・
    pMain->Quit();
    delete pMain;

    return 0;
}

void CMain::GameThreadFunc()
{
}

//------------------------------------------------------------------------
//
//  OS縺九ｉ蜻ｼ縺ｳ蜃ｺ縺輔ｌ繧九え繧｣繝ｳ繝峨え繝励Ο繧ｷ繝ｼ繧ｸ繝｣
//  (螳滄圀縺ｮ蜃ｦ逅・・ CMain繧ｯ繝ｩ繧ｹ縺ｮ MsgProc 縺ｧ陦後＞縺ｾ縺・  
//
//  HWND hWnd         繧ｦ繧｣繝ｳ繝峨え繝上Φ繝峨Ν
//  UINT iMsg         繝｡繝・そ繝ｼ繧ｸ
//  WPARAM wParam     WPARAM
//  LPARAM lParam     LPARAM
//
//  謌ｻ繧雁､ LRESULT
//     DefWindowProc 縺ｮ謌ｻ繧雁､
//
//------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    return g_pMain->MsgProc(hWnd, uMsg, wParam, lParam);
}

// ============================================================================================
//
// CMain 繝｡繧､繝ｳ繧ｯ繝ｩ繧ｹ縺ｮ蜃ｦ逅・
//
// ============================================================================================
//------------------------------------------------------------------------
//
//  CMain 繧ｳ繝ｳ繧ｹ繝医Λ繧ｯ繧ｿ  
//
//------------------------------------------------------------------------
CMain::CMain()
{
    ZeroMemory(this, sizeof(CMain));

    m_bLoopFlag = true;              // false縺ｫ縺ｪ繧九→繝ｫ繝ｼ繝励ｒ謚懊￠縺ｦ繝励Ο繧ｰ繝ｩ繝邨ゆｺ・
    m_MainLoopTime = 1000000.0 / 60; // 繝ｫ繝ｼ繝励せ繝斐・繝・1/60Sec (繝槭う繧ｯ繝ｭ遘貞腰菴・
}

//------------------------------------------------------------------------
//
//  CMain 繝・せ繝医Λ繧ｯ繧ｿ    
//
//------------------------------------------------------------------------
CMain::~CMain()
{
    SAFE_DELETE(m_pGMain);  // 繧ｲ繝ｼ繝繝｡繧､繝ｳ繧ｯ繝ｩ繧ｹ縺ｮ蜑企勁
}

//------------------------------------------------------------------------
//
//  繧ｦ繧｣繝ｳ繝峨え縺ｮ菴懈・  
//
//------------------------------------------------------------------------
HRESULT CMain::InitWindow(HINSTANCE hInstance,
                             INT iX, INT iY, INT iWidth, INT iHeight, LPCTSTR WindowName)
{
     m_hInstance = hInstance;

    // 繧ｦ繧｣繝ｳ繝峨え繧ｯ繝ｩ繧ｹ縺ｮ螳夂ｾｩ
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

    // 繧ｦ繧｣繝ｳ繝峨え繧ｵ繧､繧ｺ縺ｮ險育ｮ暦ｼ医け繝ｩ繧､繧｢繝ｳ繝磯伜沺繧呈欠螳壹し繧､繧ｺ縺ｫ縺吶ｋ・・
    RECT rc = { 0, 0, iWidth, iHeight };
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, false);

    // 繧ｦ繧｣繝ｳ繝峨え縺ｮ菴懈・
    m_hWnd = CreateWindowEx(0, WindowName, WindowName,
       WS_OVERLAPPEDWINDOW,
       CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top,
       (HWND)nullptr, (HMENU)nullptr,
       hInstance, (LPVOID)nullptr);

    if(!m_hWnd) 
    {
       MessageBox(0, _T("繧ｦ繧｣繝ｳ繝峨え繧剃ｽ懈・縺ｧ縺阪∪縺帙ｓ縺ｧ縺励◆"), nullptr, MB_OK);
       return E_FAIL;
    }
    // 繧ｦ繧｣繝ｳ繝峨え縺ｮ陦ｨ遉ｺ
    ShowWindow(m_hWnd, SW_SHOW);
    UpdateWindow(m_hWnd);

    return S_OK;
}
//------------------------------------------------------------------------
//
//  繧ｦ繧｣繝ｳ繝峨え繝励Ο繧ｷ繝ｼ繧ｸ繝｣・医Γ繝・そ繝ｼ繧ｸ蜃ｦ逅・ｼ・
//
//------------------------------------------------------------------------
LRESULT CMain::MsgProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    switch(iMsg)
    {
       case WM_KEYDOWN:
       switch((char)wParam)
       {
          case VK_ESCAPE: // ESC繧ｭ繝ｼ縺ｧ邨ゆｺ・
                PostQuitMessage(0);
                break;
       }
       break;
       case WM_DESTROY:
             PostQuitMessage(0);
             break;
    }

    // ImGui 縺ｮ繝｡繝・そ繝ｼ繧ｸ蜃ｦ逅・
    ImGui_ImplWin32_WndProcHandler(hWnd, iMsg, wParam, lParam);

    return DefWindowProc (hWnd, iMsg, wParam, lParam);
}

//------------------------------------------------------------------------
//
//  繝｡繝・そ繝ｼ繧ｸ繝ｫ繝ｼ繝・
//
//  繧｢繝励Μ繧ｱ繝ｼ繧ｷ繝ｧ繝ｳ縺ｮ繝｡繧､繝ｳ繝ｫ繝ｼ繝励ｒ螳溯｡後・
//  1/60遘偵＃縺ｨ縺ｫ譖ｴ譁ｰ繧定｡後≧縺溘ａ縺ｫ繧ｦ繧ｧ繧､繝亥・逅・ｒ蜈･繧後ｋ縺薙→縺悟庄閭ｽ縲・
//
//  竭 繧｢繝励Μ繧ｱ繝ｼ繧ｷ繝ｧ繝ｳ蛛ｴ縺ｧ繧ｦ繧ｧ繧､繝医ｒ縺九￠繧区婿豕・
//     QueryPerformanceCounter 繧剃ｽｿ逕ｨ縺励※繝槭う繧ｯ繝ｭ遘貞腰菴阪〒蠕・ｩ溘・
//  竭｡ 繧｢繝励Μ繧ｱ繝ｼ繧ｷ繝ｧ繝ｳ蛛ｴ縺ｧ繧ｦ繧ｧ繧､繝医ｒ縺九￠縺ｪ縺・婿豕・(Vsync蜷梧悄)
//     謠冗判・・resent・画凾縺ｫ蝙ら峩蜷梧悄繧貞ｾ・▽險ｭ螳壹↓縺吶ｋ縲・
//
//------------------------------------------------------------------------
void CMain::MessageLoop()
{
    // 迴ｾ蝨ｨ縺ｮ繧ｳ繝ｼ繝画ｧ区・縺ｧ縺ｯ縲｀ainControl::RefreshTimer() 縺ｮ險ｭ螳壹↓繧医ｊ蜍穂ｽ懊′螟峨ｏ繧・
#if 1
    MSG  msg = { 0 };
    ZeroMemory(&msg, sizeof(msg));
    LARGE_INTEGER StartTime, EndTime, Frq;
    QueryPerformanceFrequency(&Frq);   // 蜻ｨ豕｢謨ｰ縺ｮ蜿門ｾ・
    double MicroSec = 1000000.0 / (double)Frq.QuadPart;   // 1繧ｫ繧ｦ繝ｳ繝医≠縺溘ｊ縺ｮ繝槭う繧ｯ繝ｭ遘・
    double ElapsedTime;
    QueryPerformanceCounter(&StartTime);      // 繝ｫ繝ｼ繝鈴幕蟋区凾髢・

    while (msg.message != WM_QUIT && m_bLoopFlag)
    {
       // 繧ｲ繝ｼ繝縺ｮ譖ｴ譁ｰ縺ｨ謠冗判
       m_pGMain->Update();
       m_pGMain->Draw();

       // FPS陦ｨ遉ｺ
       DispFps();

       float timer = MainControl::RefreshTimer();
       if (timer > 0.0f) {
          // 閾ｪ蜑阪〒繧ｦ繧ｧ繧､繝医ｒ縺九￠繧句・逅・ｼ医ち繧､繝槭・謖・ｮ壽凾・・
          do {
             if (!MessageProcess(&msg)) 
             {
                m_bLoopFlag = false; // WM_QUIT蜿嶺ｿ｡
                break;
             }
             QueryPerformanceCounter(&EndTime);
             ElapsedTime = (EndTime.QuadPart - StartTime.QuadPart) * MicroSec;
          } while (ElapsedTime < timer * 1000000.0f); // 險ｭ螳壽凾髢薙∪縺ｧ繝ｫ繝ｼ繝・
          StartTime = EndTime;
       }
       else {
          // Vsync 遲峨↓莉ｻ縺帙ｋ蝣ｴ蜷医√∪縺溘・繝｡繝・そ繝ｼ繧ｸ荳ｻ蟆・
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
//  繝｡繝・そ繝ｼ繧ｸ繝励Ο繧ｻ繧ｹ・医Γ繝・そ繝ｼ繧ｸ縺梧ｺ懊∪縺｣縺ｦ縺・ｋ髢薙・蜃ｦ逅・ｼ・
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
//  蛻晄悄蛹門・逅・
//
//------------------------------------------------------------------------
HRESULT CMain::Init()
{
    m_pGMain = new CGameMain( this ); // 繧ｲ繝ｼ繝繝｡繧､繝ｳ繧ｯ繝ｩ繧ｹ縺ｮ逕滓・
    m_pGMain->Init();                 // 繧ｲ繝ｼ繝蛻晄悄蛹・

    return S_OK;
}

//------------------------------------------------------------------------
//
//  繝｡繧､繝ｳ繝ｫ繝ｼ繝怜・逅・ｼ育ｰ｡譏灘他縺ｳ蜃ｺ縺礼畑・・
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
//  邨ゆｺ・・逅・
//
//------------------------------------------------------------------------
void CMain::Quit()
{
    m_pGMain->Quit();
}

//------------------------------------------------------------------------
//
//  繧ｿ繧､繝医Ν繝舌・縺ｫFPS繧定｡ｨ遉ｺ
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