//=============================================================================
//     3Dゲームプログラム                     ver 3.2        2023.1.31
//
//                                               GameMain.cpp
//=============================================================================
#include "GameMain.h"
#include "../../Framework/SceneManager.h"
#include "../../Framework/objectManager.h"
#include "../../Framework/ResourceManager.h"

namespace
{
    CGameMain* gameMainInstance;
}

// ============================================================================================
//
//
// ============================================================================================
//------------------------------------------------------------------------
//
//  CGameMain コンストラクタ  
//
//
//------------------------------------------------------------------------
CGameMain::CGameMain(CMain* pMain)
{
    gameMainInstance = this;
    m_pMain = pMain;

    m_pD3D = nullptr; // Direct3Dオブジェクト
    m_pXAudio = nullptr; // XAudio2オブジェクト
    m_pDI = nullptr; // DirectInputオブジェクト
    m_pFont = nullptr; // フォントテクスチャ
    m_pShader = nullptr;
    m_pFbxMeshCtrl = nullptr;

    m_vEyePt = VECTOR3(0, 0, 0);
    m_vLookatPt = VECTOR3(0, 0, 0); // 注視点位置
    m_mView = XMMatrixIdentity();
    m_mProj = XMMatrixIdentity();
    m_vLightDir = VECTOR3(0, 0, 0);
    m_vLightIntensity = VECTOR4(1, 1, 1, 1);
}

//------------------------------------------------------------------------
//
//
//------------------------------------------------------------------------
CGameMain::~CGameMain()
{
    SceneManager::Release();
    ObjectManager::Release();
    ResourceManager::Reset();
    MyImgui::ImguiQuit();

    SAFE_DELETE(m_pFbxMeshCtrl); // -- 2021.2.4
    SAFE_DELETE(m_pShader);
    SAFE_DELETE(m_pFont);
    SAFE_DELETE(m_pDI);
    SAFE_DELETE(m_pXAudio);
    SAFE_DELETE(m_pD3D);

    CoUninitialize();
}

//------------------------------------------------------------------------
//
//
//
//  戻り値 HRESULT
//     E_FAIL = 逡ｰ蟶ｸ
//
//------------------------------------------------------------------------
HRESULT CGameMain::Init()
{
    CoInitialize(nullptr);

    m_pD3D = new CDirect3D;
    if (FAILED(m_pD3D->InitD3D(m_pMain->m_hWnd, WINDOW_WIDTH, WINDOW_HEIGHT)))
    {
        MessageBox(0, _T("Direct3Dの初期化に失敗しました"), nullptr, MB_OK);
        return E_FAIL;
    }

    m_pShader = new CShader(m_pD3D);
    if (FAILED(m_pShader->InitShader()))
    {
        MessageBox(0, _T("Shader : 処理に失敗しました。"), nullptr, MB_OK);
        return E_FAIL;
    }

    m_pXAudio = new CXAudio;
    if (FAILED(m_pXAudio->InitAudio(m_pMain->m_hWnd)))
    {
        MessageBox(0, _T("XAudio2の初期化に失敗しました"), nullptr, MB_OK);
        return E_FAIL;
    }

    m_pDI = new CDirectInput;
    if (!(m_pDI->StartDirectInput(m_pMain->m_hInstance, m_pMain->m_hWnd,
                                  INIT_KEYBOARD | INIT_MOUSE | INIT_JOYSTICK, WINDOW_WIDTH, WINDOW_HEIGHT)))
    {
        MessageBox(0, _T("DirectInputの初期化に失敗しました"), nullptr, MB_OK);
        return E_FAIL;
    }

    m_pFont = new CFontTexture(m_pShader);

    MyImgui::ImguiInit(m_pMain->m_hWnd, m_pD3D, WINDOW_WIDTH, WINDOW_HEIGHT);

    m_pFbxMeshCtrl = new CFbxMeshCtrl(m_pShader);

    // ----------------------------------------------------------------------------------------
    m_vLightDir = normalize(VECTOR3(0.8f, 1, -1));

    m_mProj = XMMatrixPerspectiveFovLH(XMConvertToRadians(38.0f), (FLOAT)WINDOW_WIDTH / (FLOAT)WINDOW_HEIGHT, 0.1f,
                                       1000.0);

    VECTOR3 vUpVec(0.0f, 1.0f, 0.0f); // 上方向
    m_vEyePt = VECTOR3(0.0f, 4.0f, -5.0f);
    m_vLookatPt = VECTOR3(0.0f, 2.0f, 0.0f);
    m_mView = XMMatrixLookAtLH(m_vEyePt, m_vLookatPt, vUpVec);

    SceneManager::Start();
    ObjectManager::Start();

    MainControl::UseRefreshMessage();

    return S_OK;
}

//------------------------------------------------------------------------
//
//
//------------------------------------------------------------------------
void CGameMain::Update()
{
    MyImgui::ImguiNewFrame();

    m_pDI->GetInput();
    m_pFont->Refresh();

    if (m_pDI->CheckKey(KD_TRG, DIK_F4)) ChangeScreenMode();

    SceneManager::Update(); // シーン遷移と現在のシーンの更新
    ObjectManager::Update();
}

//------------------------------------------------------------------------
//
//
//------------------------------------------------------------------------
void CGameMain::GameMain()
{
}

//------------------------------------------------------------------------
//
//
//------------------------------------------------------------------------
void CGameMain::Draw()
{
    float ClearColor[4] = {0, 0, 0, 1}; // RGBA
    m_pD3D->ClearRenderTarget(ClearColor);

    ObjectManager::Draw(); // 全オブジェクトを描画
    SceneManager::Draw(); // 現在のシーンを描画
    SceneManager::DrawTransition();

    MyImgui::ImguiRender();

    m_pD3D->m_pSwapChain->Present(1, 0);
}

//------------------------------------------------------------------------
//
//
//------------------------------------------------------------------------
void CGameMain::Quit()
{
}

//------------------------------------------------------------------------
//
//
//
//------------------------------------------------------------------------
HRESULT CGameMain::ChangeScreenMode(int nMode)
{
    HRESULT Ret = S_OK;
    BOOL bFullScreen;

    m_pD3D->m_pSwapChain->GetFullscreenState(&bFullScreen, nullptr);

    switch (nMode)
    {
    case 0: // ウィンドウモードへ
        if (bFullScreen) Ret = m_pD3D->m_pSwapChain->SetFullscreenState(false, nullptr);
        break;
    case 1: // フルスクリーンモードへ
        if (!bFullScreen) Ret = m_pD3D->m_pSwapChain->SetFullscreenState(true, nullptr);
        break;
    default: // 現在の状態を反転
        Ret = m_pD3D->m_pSwapChain->SetFullscreenState(!bFullScreen, nullptr);
    }

    return Ret;
}

void CGameMain::SetWindowName(const char* name)
{
    SetWindowText(m_pMain->m_hWnd, name);
}

//------------------------------------------------------------------------
//
//
//
//------------------------------------------------------------------------
void CGameMain::SetLightIntensity(float r, float g, float b, float a)
{
    m_vLightIntensity = VECTOR4(r, g, b, a);
}

void CGameMain::SetLightIntensity(VECTOR4 intensity)
{
    m_vLightIntensity = intensity;
}

CGameMain* GameDevice()
{
    return gameMainInstance;
}
