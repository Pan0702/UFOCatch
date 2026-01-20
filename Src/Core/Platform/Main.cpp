//=============================================================================
//		�R�c�Q�[���v���O�����@�@�@�@�@�@               ver 3.2        2023.1.31
//
//		�E�B���h�E�Y�̎又��
//																Main.cpp
//=============================================================================
#define _CRTDBG_MAPALLOC
#include <crtdbg.h>
#include <locale.h>
#include "MainControl.h"
#pragma warning(disable : 28251)

#include "../03_GameMain/GameMain.h"


//�O���[�o���ϐ�(�R�[���o�b�N�֐��̂��ߕK�v)
static CMain*	g_pMain = nullptr;		// ���C���N���X

//------------------------------------------------------------------------
//
//	�A�v���P�[�V�����̃G���g���[�֐� 	
//
//  HINSTANCE hInstance      // ���݂̃C���X�^���X�̃n���h��
//  HINSTANCE hPrevInstance  // �ȑO�̃C���X�^���X�̃n���h��
//  LPTSTR    lpCmdLine          // �R�}���h���C��
//  INT       nCmdShow             // �\�����
//
//	�߂�l INT
//		�֐������b�Z�[�W���[�v�ɓ���O�ɏI������ꍇ�́A0 
//
//------------------------------------------------------------------------
INT WINAPI _tWinMain(HINSTANCE hInstance,HINSTANCE,LPTSTR,INT)
{
	// ���������[�N�̃`�F�b�N���s���@�|�|�|�|�|�|    
	// �S�Ẵ��|�[�g�o�͂��f�B�o�b�O�E�B���h�E�ɑ���
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
	_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_DEBUG);
	// �v���O�����̍ŏ��ŃR�[�����邱�Ƃɂ���āA�I������_CrtDumpMemoryLeaks�֐����R�[������
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// ���P�[���̐ݒ�
	// ���C�h�����̏ꍇ�A���P�[�����w�肵�Ȃ��ƁA�����������R�[�h�ɂȂ�Ȃ�
	_tsetlocale(LC_ALL, _T(""));

	CMain*	pMain = new CMain;	// ���C���N���X�̐���
	g_pMain = pMain;			// �R�[�o�b�N�֐��̂��߂ɃO���[�o���Ƀ��C���N���X�̃|�C���^��u��

	if(SUCCEEDED(pMain->InitWindow(hInstance,0,0,WINDOW_WIDTH, WINDOW_HEIGHT,APP_NAME))) // Window�̏�����
	{
		if (SUCCEEDED(pMain->Init()))		// ����������
		{
			pMain->MessageLoop();			// ���b�Z�[�W���[�v����
		}
	}

	// �I������
	pMain->Quit();
	delete pMain;

	return 0;
}

void CMain::GameThreadFunc()
{
}

//------------------------------------------------------------------------
//
//	OS���Ăяo���E�B���h�E�v���V�[�W���[
//	�i���ۂ̏�����CMain�N���X��MsgProc�v���V�[�W���[�ŏ����j	
//
//  HWND hWnd         �E�B���h�E�n���h��
//  UINT iMsg         ���b�Z�[�W
//  WPARAM wParam     WPARAM
//  LPARAM lParam     LPARAM
//
//	�߂�l LRESULT
//		DefWindowProc�̖߂�l
//
//------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	return g_pMain->MsgProc(hWnd,uMsg,wParam,lParam);
}

// ============================================================================================
//
// CMain ���C���N���X�̏���
//
// ============================================================================================
//------------------------------------------------------------------------
//
//	CMain ���C���N���X�̃R���X�g���N�^	
//
//  �����@�Ȃ�
//
//------------------------------------------------------------------------
CMain::CMain()
{
	ZeroMemory(this, sizeof(CMain));

	m_bLoopFlag = true;		// false�ɂȂ�ƁA���[�v�𔲂��ăv���O�����I��
	m_MainLoopTime = 1000000.0 / 60;  // ���[�v�X�s�[�h 1/60Sec(�}�C�N���b1000000.0/60)
	//m_MainLoopTime = 0;  // ���[�v�X�s�[�h 1/60Sec(�}�C�N���b1000000.0/60)

}

//------------------------------------------------------------------------
//
//	CMain ���C���N���X�̃f�X�g���N�^	
//
//------------------------------------------------------------------------
CMain::~CMain()
{
	SAFE_DELETE(m_pGMain);  // �Q�[�����C���N���X�̍폜
}

//------------------------------------------------------------------------
//
//	�E�B���h�E�̍쐬	
//
//  HINSTANCE hInstance    �C���X�^���X�n���h��
//  INT iX                 �E�B���h�E����ʒu�@�w���W(��ɂO)
//  INT iY                 �E�B���h�E����ʒu�@�x���W(��ɂO)
//  INT iWidth             �E�B���h�E�̕�
//  INT iHeight            �E�B���h�E�̍���
//  LPCTSTR WindowName     �E�B���h�E��
//
//	�߂�l HRESULT
//		S_OK	= ����
//		E_FAIL	= �ُ�
//
//------------------------------------------------------------------------
HRESULT CMain::InitWindow(HINSTANCE hInstance,
		                      INT iX,INT iY,INT iWidth,INT iHeight,LPCTSTR WindowName)
{
	 m_hInstance = hInstance;

	// �E�B���h�E�̒�`
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

	// �E�B���h�E�̑傫�����擾(���j���[�͊܂߂Ȃ�)
	RECT rc = { 0, 0, iWidth, iHeight };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, false);

	// �E�B���h�E�̍쐬(�E�B���h�E���[�h)
	m_hWnd = CreateWindowEx(0, WindowName, WindowName,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top,
		(HWND)nullptr, (HMENU)nullptr,
		hInstance, (LPVOID)nullptr);

	if(!m_hWnd) 
	{
		MessageBox(0, _T("Window���쐬�ł��܂���"), nullptr, MB_OK);
		return E_FAIL;
	}
	//�E�C���h�E�̕\��
	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd) ;

	return S_OK;
}
//------------------------------------------------------------------------
//
//	�E�B���h�E�v���V�[�W���[	
//
//  HWND hWnd         �E�B���h�E�n���h��
//  UINT iMsg         ���b�Z�[�W
//  WPARAM wParam     WPARAM
//  LPARAM lParam     LPARAM
//
//	�߂�l LRESULT
//		DefWindowProc�̖߂�l
//
//------------------------------------------------------------------------
LRESULT CMain::MsgProc(HWND hWnd,UINT iMsg,WPARAM wParam,LPARAM lParam)
{
	switch(iMsg)
	{
		case WM_KEYDOWN:
		switch((char)wParam)
		{
			case VK_ESCAPE://ESC�L�[�ŏI��
					PostQuitMessage(0);
					break;

		}
		break;
		case WM_DESTROY:
				PostQuitMessage(0);
				break;
	}

	// imGui�̑���
	ImGui_ImplWin32_WndProcHandler(hWnd, iMsg, wParam, lParam);           // -- 2020.8.7

	return DefWindowProc (hWnd, iMsg, wParam, lParam);
}

//------------------------------------------------------------------------ // -- 2020.1.15
//
//	���b�Z�[�W���[�v
//
//	�A�v���P�[�V�����̃��[�v�������s��
//	���̂Ƃ��A���[�v�̎��ԊǗ������邽�߂ɃE�F�C�g�������s��(1/60Sec)
//	�Ȃ��A���L�̂悤�ɃE�F�C�g�������@��I���ł���
//
//	?@�@�A�v���P�[�V�����ŃE�F�C�g���|������@
//		QueryPerformanceCounterty�֐����g�p���ă}�C�N���b�P�ʂ̃E�F�C�g���|����
//		MessageProcess���\�b�h��m_MainLoopTime�����o���g�p����
//		GameMain.cpp����Present���\�b�h�̈�����(0, 0)�Ƃ���
//
//	?A�@�A�v���P�[�V�����ł̓E�F�C�g���|���Ȃ����@
//		�A�v���P�[�V�����ł̓E�F�C�g���|�����APresent���\�b�h��Vsync�ɓ���������B
//		���̏ꍇ�AMessageProcess���\�b�h��m_MainLoopTime�����o�͎g�p���Ȃ�)
//		GameMain.cpp����Present���\�b�h�̈�����(1, 0)�Ƃ���(Vsync�Ή�)
//
// �����@�Ȃ�
//
//	�߂�l �Ȃ�
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
	// �A�v���P�[�V�����ŃE�F�C�g���|������@
	// 
	MSG  msg = { 0 };
	ZeroMemory(&msg, sizeof(msg));
	LARGE_INTEGER StartTime, EndTime, Frq;
	QueryPerformanceFrequency(&Frq);   // �J�E���g�A�b�v���g���̎擾
	double MicroSec = 1000000.0 / (double)Frq.QuadPart;   // �J�E���g���ԁi�}�C�N���b�P�ʁj�̎擾
	double ElapsedTime;
	QueryPerformanceCounter(&StartTime);		// ���[�v�J�n����

	while (msg.message != WM_QUIT && m_bLoopFlag)	// ���b�Z�[�W���擾���Ă���ԌJ��Ԃ�
	{
		//imgui	�A�b�v�f�[�g�̍ŏ��ɌĂ�
		//ImGui_ImplDX11_NewFrame();
		//ImGui_ImplWin32_NewFrame();
		//ImGui::NewFrame();

		m_pGMain->Update();
		m_pGMain->Draw();

		DispFps();	 // ��������\��

		float timer = MainControl::RefreshTimer();
		if (timer > 0.0f) {
			do {										// ���[�v���ԑҋ@�i�Œ��x�͒ʂ�j
				if (!MessageProcess(&msg))				// ���b�Z�[�W�̏���
				{
					m_bLoopFlag = false;				// WM_QUIT ���b�Z�[�W���擾�����ꍇ
					break;
				}
				QueryPerformanceCounter(&EndTime);		// ���[�v�I������
				ElapsedTime = (EndTime.QuadPart - StartTime.QuadPart) * MicroSec;  // �o�ߎ���(�}�C�N���b)
			} while (ElapsedTime < timer*1000000.0f);		// �S�̂̃E�F�C�g����(�}�C�N���b)
			StartTime = EndTime;
		}
		else {
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			QueryPerformanceCounter(&StartTime);		// ���[�v�J�n����
		}

		//imgui �`��
		//ImGui::Render();
		//ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	}
#endif


#if 0	
	// 
	// �A�v���P�[�V�����ŃE�F�C�g���|������@
	// 
	MSG  msg = { 0 };
	ZeroMemory(&msg, sizeof(msg));
	LARGE_INTEGER StartTime, EndTime, Frq;
	QueryPerformanceFrequency(&Frq);   // �J�E���g�A�b�v���g���̎擾
	double MicroSec = 1000000.0 / (double)Frq.QuadPart;   // �J�E���g���ԁi�}�C�N���b�P�ʁj�̎擾
	double ElapsedTime;

	while (msg.message != WM_QUIT && m_bLoopFlag)	// ���b�Z�[�W���擾���Ă���ԌJ��Ԃ�
	{
		QueryPerformanceCounter(&StartTime);		// ���[�v�J�n����

		Loop();										// �A�v���P�[�V�����̃��[�v����

		do {										// ���[�v���ԑҋ@�i�Œ��x�͒ʂ�j
			if (!MessageProcess(&msg))				// ���b�Z�[�W�̏���
			{
				m_bLoopFlag = false;				// WM_QUIT ���b�Z�[�W���擾�����ꍇ
				break;
			}
			QueryPerformanceCounter(&EndTime);		// ���[�v�I������
			ElapsedTime = (EndTime.QuadPart - StartTime.QuadPart) * MicroSec;  // �o�ߎ���(�}�C�N���b)
		} while (ElapsedTime < m_MainLoopTime);		// �S�̂̃E�F�C�g����(�}�C�N���b)
	}
#endif

#if 1
	//
	// �A�v���P�[�V�����ł̓E�F�C�g���|���Ȃ����@
	//
	MSG  msg = { 0 };
	ZeroMemory(&msg, sizeof(msg));

	while (msg.message != WM_QUIT && m_bLoopFlag)	// ���b�Z�[�W���擾���Ă���ԌJ��Ԃ�
	{
		m_pGMain->Update();
		m_pGMain->Draw();

		DispFps();	 // ��������\��

		if( PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) )
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else{
//			Loop();									// �A�v���P�[�V�����̃��[�v����
		}
	}
#endif
}

//------------------------------------------------------------------------
//
//	���b�Z�[�W����
//
// MSG* pMsg      ���b�Z�[�W
//
//	�߂�l bool
//     WM_QUIT �ȊO�̃��b�Z�[�W���擾�����ꍇ:true
//     WM_QUIT ���b�Z�[�W���擾�����ꍇ      :false
//
//------------------------------------------------------------------------
bool CMain::MessageProcess(MSG* pMsg)
{
	while ((pMsg->message != WM_QUIT && m_bLoopFlag) &&
		PeekMessage(pMsg, nullptr, 0, 0, PM_REMOVE))    // ���b�Z�[�W���擾���Ă���ԌJ��Ԃ�
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
//	�A�v���P�[�V�����̏���������
//
// �����@�Ȃ�
//
//	�߂�l HRESULT
//		S_OK	= ����
//		E_FAIL	= �ُ�
//
//------------------------------------------------------------------------
HRESULT CMain::Init()
{
	m_pGMain = new CGameMain( this ); // �Q�[�����C���N���X�̐���
	m_pGMain->Init();                 // �Q�[�����C���N���X�̏�����

	return S_OK;
}

//------------------------------------------------------------------------
//
//	�A�v���P�[�V�����̃��C�����[�v����
//
// �����@�Ȃ�
//
//	�߂�l �Ȃ�
//
//------------------------------------------------------------------------
void CMain::Loop()
{
	m_pGMain->Draw();
	m_pGMain->Update();

	// ��������\��
	DispFps();
}

//------------------------------------------------------------------------
//
//	�A�v���P�[�V�����̏I������
//
// �����@�Ȃ�
//
//	�߂�l �Ȃ�
//
//------------------------------------------------------------------------
void CMain::Quit()
{
	m_pGMain->Quit();

}

//------------------------------------------------------------------------
//
//	�^�C�g���o�[�ɂ�������\��
//
// �����@�Ȃ�
//
//	�߂�l �Ȃ�
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

