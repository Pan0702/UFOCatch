//=============================================================================
//		３Ｄゲームプログラム　　　　　　               ver 3.2        2023.1.31
//
//		ウィンドウズの主処理
//																Main.cpp
//=============================================================================
#define _CRTDBG_MAPALLOC
#include <crtdbg.h>
#include <locale.h>
#include "MainControl.h"
#pragma warning(disable : 28251)

#include "GameMain.h"


//グローバル変数(コールバック関数のため必要)
static CMain*	g_pMain = nullptr;		// メインクラス

//------------------------------------------------------------------------
//
//	アプリケーションのエントリー関数 	
//
//  HINSTANCE hInstance      // 現在のインスタンスのハンドル
//  HINSTANCE hPrevInstance  // 以前のインスタンスのハンドル
//  LPTSTR    lpCmdLine          // コマンドライン
//  INT       nCmdShow             // 表示状態
//
//	戻り値 INT
//		関数がメッセージループに入る前に終了する場合は、0 
//
//------------------------------------------------------------------------
INT WINAPI _tWinMain(HINSTANCE hInstance,HINSTANCE,LPTSTR,INT)
{
	// メモリリークのチェックを行う　－－－－－－    
	// 全てのレポート出力をディバッグウィンドウに送る
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
	_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_DEBUG);
	// プログラムの最初でコールすることによって、終了時に_CrtDumpMemoryLeaks関数をコールする
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// ロケールの設定
	// ワイド文字の場合、ロケールを指定しないと、正しい文字コードにならない
	_tsetlocale(LC_ALL, _T(""));

	CMain*	pMain = new CMain;	// メインクラスの生成
	g_pMain = pMain;			// コーバック関数のためにグローバルにメインクラスのポインタを置く

	if(SUCCEEDED(pMain->InitWindow(hInstance,0,0,WINDOW_WIDTH, WINDOW_HEIGHT,APP_NAME))) // Windowの初期化
	{
		if (SUCCEEDED(pMain->Init()))		// 初期化処理
		{
			pMain->MessageLoop();			// メッセージループ処理
		}
	}

	// 終了処理
	pMain->Quit();
	delete pMain;

	return 0;
}

//------------------------------------------------------------------------
//
//	OSが呼び出すウィンドウプロシージャー
//	（実際の処理はCMainクラスのMsgProcプロシージャーで処理）	
//
//  HWND hWnd         ウィンドウハンドル
//  UINT iMsg         メッセージ
//  WPARAM wParam     WPARAM
//  LPARAM lParam     LPARAM
//
//	戻り値 LRESULT
//		DefWindowProcの戻り値
//
//------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	return g_pMain->MsgProc(hWnd,uMsg,wParam,lParam);
}

// ============================================================================================
//
// CMain メインクラスの処理
//
// ============================================================================================
//------------------------------------------------------------------------
//
//	CMain メインクラスのコンストラクタ	
//
//  引数　なし
//
//------------------------------------------------------------------------
CMain::CMain()
{
	ZeroMemory(this, sizeof(CMain));

	m_bLoopFlag = true;		// falseになると、ループを抜けてプログラム終了
	m_MainLoopTime = 1000000.0 / 60;  // ループスピード 1/60Sec(マイクロ秒1000000.0/60)
	//m_MainLoopTime = 0;  // ループスピード 1/60Sec(マイクロ秒1000000.0/60)

}

//------------------------------------------------------------------------
//
//	CMain メインクラスのデストラクタ	
//
//------------------------------------------------------------------------
CMain::~CMain()
{
	SAFE_DELETE(m_pGMain);  // ゲームメインクラスの削除
}

//------------------------------------------------------------------------
//
//	ウィンドウの作成	
//
//  HINSTANCE hInstance    インスタンスハンドル
//  INT iX                 ウィンドウ左上位置　Ｘ座標(常に０)
//  INT iY                 ウィンドウ左上位置　Ｙ座標(常に０)
//  INT iWidth             ウィンドウの幅
//  INT iHeight            ウィンドウの高さ
//  LPCTSTR WindowName     ウィンドウ名
//
//	戻り値 HRESULT
//		S_OK	= 正常
//		E_FAIL	= 異常
//
//------------------------------------------------------------------------
HRESULT CMain::InitWindow(HINSTANCE hInstance,
		                      INT iX,INT iY,INT iWidth,INT iHeight,LPCTSTR WindowName)
{
	 m_hInstance = hInstance;

	// ウィンドウの定義
	WNDCLASSEX  wc;
	ZeroMemory(&wc,sizeof(wc));
	wc.cbSize = sizeof(wc);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(hInstance,  MAKEINTRESOURCE(IDI_MAIN_ICON));
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	wc.lpszClassName = WindowName;
	wc.lpszMenuName = nullptr;
	RegisterClassEx(&wc);

	// ウィンドウの大きさを取得(メニューは含めない)
	RECT rc = { 0, 0, iWidth, iHeight };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, false);

	// ウィンドウの作成(ウィンドウモード)
	m_hWnd = CreateWindowEx(0, WindowName, WindowName,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top,
		(HWND)nullptr, (HMENU)nullptr,
		hInstance, (LPVOID)nullptr);

	if(!m_hWnd) 
	{
		MessageBox(0, _T("Windowが作成できません"), nullptr, MB_OK);
		return E_FAIL;
	}
	//ウインドウの表示
	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd) ;

	return S_OK;
}
//------------------------------------------------------------------------
//
//	ウィンドウプロシージャー	
//
//  HWND hWnd         ウィンドウハンドル
//  UINT iMsg         メッセージ
//  WPARAM wParam     WPARAM
//  LPARAM lParam     LPARAM
//
//	戻り値 LRESULT
//		DefWindowProcの戻り値
//
//------------------------------------------------------------------------
LRESULT CMain::MsgProc(HWND hWnd,UINT iMsg,WPARAM wParam,LPARAM lParam)
{
	switch(iMsg)
	{
		case WM_KEYDOWN:
		switch((char)wParam)
		{
			case VK_ESCAPE://ESCキーで終了
					PostQuitMessage(0);
					break;

		}
		break;
		case WM_DESTROY:
				PostQuitMessage(0);
				break;
	}

	// imGuiの操作
	ImGui_ImplWin32_WndProcHandler(hWnd, iMsg, wParam, lParam);           // -- 2020.8.7

	return DefWindowProc (hWnd, iMsg, wParam, lParam);
}

//------------------------------------------------------------------------ // -- 2020.1.15
//
//	メッセージループ
//
//	アプリケーションのループ処理を行う
//	このとき、ループの時間管理をするためにウェイト処理を行う(1/60Sec)
//	なお、下記のようにウェイト処理方法を選択できる
//
//	①　アプリケーションでウェイトを掛ける方法
//		QueryPerformanceCounterty関数を使用してマイクロ秒単位のウェイトを掛ける
//		MessageProcessメソッドとm_MainLoopTimeメンバを使用する
//		GameMain.cpp中のPresentメソッドの引数を(0, 0)とする
//
//	②　アプリケーションではウェイトを掛けない方法
//		アプリケーションではウェイトを掛けず、PresentメソッドのVsyncに同期させる。
//		この場合、MessageProcessメソッドとm_MainLoopTimeメンバは使用しない)
//		GameMain.cpp中のPresentメソッドの引数を(1, 0)とする(Vsync対応)
//
// 引数　なし
//
//	戻り値 なし
//
//------------------------------------------------------------------------
void CMain::MessageLoop()
{

/*
	// ImGui
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;       // Enable Gamepad Controls

		ImGuiStyle& style = ImGui::GetStyle();

		ImGui_ImplWin32_Init(m_hWnd, WINDOW_WIDTH, WINDOW_HEIGHT);
//		ImGui_ImplDX11_Init((ID3D11Device*)DxLib::GetUseDirect3D11Device(), (ID3D11DeviceContext*)DxLib::GetUseDirect3D11DeviceContext());
	}
*/

#if 0
	// 
	// アプリケーションでウェイトを掛ける方法
	// 
	MSG  msg = { 0 };
	ZeroMemory(&msg, sizeof(msg));
	LARGE_INTEGER StartTime, EndTime, Frq;
	QueryPerformanceFrequency(&Frq);   // カウントアップ周波数の取得
	double MicroSec = 1000000.0 / (double)Frq.QuadPart;   // カウント時間（マイクロ秒単位）の取得
	double ElapsedTime;
	QueryPerformanceCounter(&StartTime);		// ループ開始時間

	while (msg.message != WM_QUIT && m_bLoopFlag)	// メッセージを取得している間繰り返す
	{
		//imgui	アップデートの最初に呼ぶ
		//ImGui_ImplDX11_NewFrame();
		//ImGui_ImplWin32_NewFrame();
		//ImGui::NewFrame();

		m_pGMain->Update();
		m_pGMain->Draw();

		DispFps();	 // ｆｐｓを表示

		float timer = MainControl::RefreshTimer();
		if (timer > 0.0f) {
			do {										// ループ時間待機（最低一度は通る）
				if (!MessageProcess(&msg))				// メッセージの処理
				{
					m_bLoopFlag = false;				// WM_QUIT メッセージを取得した場合
					break;
				}
				QueryPerformanceCounter(&EndTime);		// ループ終了時間
				ElapsedTime = (EndTime.QuadPart - StartTime.QuadPart) * MicroSec;  // 経過時間(マイクロ秒)
			} while (ElapsedTime < timer*1000000.0f);		// 全体のウェイト時間(マイクロ秒)
			StartTime = EndTime;
		}
		else {
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			QueryPerformanceCounter(&StartTime);		// ループ開始時間
		}

		//imgui 描画
		//ImGui::Render();
		//ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	}
#endif


#if 0	
	// 
	// アプリケーションでウェイトを掛ける方法
	// 
	MSG  msg = { 0 };
	ZeroMemory(&msg, sizeof(msg));
	LARGE_INTEGER StartTime, EndTime, Frq;
	QueryPerformanceFrequency(&Frq);   // カウントアップ周波数の取得
	double MicroSec = 1000000.0 / (double)Frq.QuadPart;   // カウント時間（マイクロ秒単位）の取得
	double ElapsedTime;

	while (msg.message != WM_QUIT && m_bLoopFlag)	// メッセージを取得している間繰り返す
	{
		QueryPerformanceCounter(&StartTime);		// ループ開始時間

		Loop();										// アプリケーションのループ処理

		do {										// ループ時間待機（最低一度は通る）
			if (!MessageProcess(&msg))				// メッセージの処理
			{
				m_bLoopFlag = false;				// WM_QUIT メッセージを取得した場合
				break;
			}
			QueryPerformanceCounter(&EndTime);		// ループ終了時間
			ElapsedTime = (EndTime.QuadPart - StartTime.QuadPart) * MicroSec;  // 経過時間(マイクロ秒)
		} while (ElapsedTime < m_MainLoopTime);		// 全体のウェイト時間(マイクロ秒)
	}
#endif

#if 1
	//
	// アプリケーションではウェイトを掛けない方法
	//
	MSG  msg = { 0 };
	ZeroMemory(&msg, sizeof(msg));

	while (msg.message != WM_QUIT && m_bLoopFlag)	// メッセージを取得している間繰り返す
	{
		m_pGMain->Update();
		m_pGMain->Draw();

		DispFps();	 // ｆｐｓを表示

		if( PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) )
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else{
//			Loop();									// アプリケーションのループ処理
		}
	}
#endif
}

//------------------------------------------------------------------------
//
//	メッセージ処理
//
// MSG* pMsg      メッセージ
//
//	戻り値 bool
//     WM_QUIT 以外のメッセージを取得した場合:true
//     WM_QUIT メッセージを取得した場合      :false
//
//------------------------------------------------------------------------
bool CMain::MessageProcess(MSG* pMsg)
{
	while ((pMsg->message != WM_QUIT && m_bLoopFlag) &&
		PeekMessage(pMsg, nullptr, 0, 0, PM_REMOVE))    // メッセージを取得している間繰り返す
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
//	アプリケーションの初期化処理
//
// 引数　なし
//
//	戻り値 HRESULT
//		S_OK	= 正常
//		E_FAIL	= 異常
//
//------------------------------------------------------------------------
HRESULT CMain::Init()
{
	m_pGMain = new CGameMain( this ); // ゲームメインクラスの生成
	m_pGMain->Init();                 // ゲームメインクラスの初期化

	return S_OK;
}

//------------------------------------------------------------------------
//
//	アプリケーションのメインループ処理
//
// 引数　なし
//
//	戻り値 なし
//
//------------------------------------------------------------------------
void CMain::Loop()
{
	m_pGMain->Draw();
	m_pGMain->Update();

	// ｆｐｓを表示
	DispFps();
}

//------------------------------------------------------------------------
//
//	アプリケーションの終了処理
//
// 引数　なし
//
//	戻り値 なし
//
//------------------------------------------------------------------------
void CMain::Quit()
{
	m_pGMain->Quit();

}

//------------------------------------------------------------------------
//
//	タイトルバーにｆｐｓを表示
//
// 引数　なし
//
//	戻り値 なし
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

