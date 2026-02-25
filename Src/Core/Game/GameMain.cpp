//=============================================================================
//     3Dゲームプログラム                     ver 3.2        2023.1.31
//
//     ゲームのメイン処理
//                                               GameMain.cpp
//=============================================================================
#include "GameMain.h"
#include "../../Framework/SceneManager.h"
#include "../../Framework/objectManager.h"
#include "../../Framework/ResourceManager.h"

namespace {
    CGameMain* gameMainInstance;
}

// ============================================================================================
//
// CGameMain ゲームメインクラスの処理
//
// ============================================================================================
//------------------------------------------------------------------------
//
//  CGameMain コンストラクタ  
//
//  引数：なし
//
//------------------------------------------------------------------------
CGameMain::CGameMain(CMain* pMain)
{
    gameMainInstance = this;
    m_pMain = pMain;      // メインクラス（WindowsOS層）

    m_pD3D = nullptr;        // Direct3Dオブジェクト
    m_pXAudio = nullptr;      // XAudio2オブジェクト
    m_pDI = nullptr;         // DirectInputオブジェクト
    m_pFont = nullptr;       // フォントテクスチャ
    m_pShader = nullptr;      // シェーダー管理
    m_pFbxMeshCtrl = nullptr;  // FBXメッシュコントロールクラス

    // カメラ・ライト・ビューの初期化
    m_vEyePt = VECTOR3(0,0,0);    // カメラ（視点）位置
    m_vLookatPt = VECTOR3(0,0,0);  // 注視点位置
    m_mView = XMMatrixIdentity();  // ビュー行列
    m_mProj = XMMatrixIdentity();  // プロジェクション行列
    m_vLightDir = VECTOR3(0,0,0);  // ディレクショナルライトの方向
    m_vLightIntensity = VECTOR4(1,1,1,1);  // ライトの強度(デフォルトは通常)
}
//------------------------------------------------------------------------
//
//  CGameMain デストラクタ    
//
//------------------------------------------------------------------------
CGameMain::~CGameMain()
{
    // 各マネージャーの解放処理
    SceneManager::Release();
    ObjectManager::Release();
    ResourceManager::Reset();
    MyImgui::ImguiQuit();          // -- 2020.11.15    // MyImguiの終了処理

    SAFE_DELETE(m_pFbxMeshCtrl);    // -- 2021.2.4
    SAFE_DELETE(m_pShader);
    SAFE_DELETE(m_pFont);
    SAFE_DELETE(m_pDI);
    SAFE_DELETE(m_pXAudio);
    SAFE_DELETE(m_pD3D);

    CoUninitialize();   // COMライブラリの終了
}

//------------------------------------------------------------------------
//
//  アプリケーションの初期化処理    
//
//  引数：なし
//
//  戻り値 HRESULT
//     S_OK   = 成功
//     E_FAIL = 異常
//
//------------------------------------------------------------------------
HRESULT CGameMain::Init()
{
    // COMライブラリの初期化
    CoInitialize(nullptr);

    // Direct3Dの初期化
    m_pD3D = new CDirect3D;
    if (FAILED(m_pD3D->InitD3D(m_pMain->m_hWnd, WINDOW_WIDTH, WINDOW_HEIGHT))) {
       MessageBox(0, _T("Direct3Dの初期化に失敗しました"), nullptr, MB_OK);
       return E_FAIL;
    }

    // シェーダーの初期化
    m_pShader = new CShader(m_pD3D);
    if (FAILED(m_pShader->InitShader())) {
       MessageBox(0, _T("Shaderの生成・初期化に失敗しました"), nullptr, MB_OK);
       return E_FAIL;
    }

    // XAudio2（オーディオ）の初期化
    m_pXAudio = new CXAudio;
    if (FAILED(m_pXAudio->InitAudio(m_pMain->m_hWnd))) {
       MessageBox(0, _T("XAudio2の初期化に失敗しました"), nullptr, MB_OK);
       return E_FAIL;
    }

    // DirectInput（入力デバイス）の初期化
    m_pDI = new CDirectInput;
    if (!(m_pDI->StartDirectInput(m_pMain->m_hInstance, m_pMain->m_hWnd, 
                    INIT_KEYBOARD | INIT_MOUSE | INIT_JOYSTICK, WINDOW_WIDTH, WINDOW_HEIGHT))) {
       MessageBox(0, _T("DirectInputの初期化に失敗しました"), nullptr, MB_OK);
       return E_FAIL;
    }

    // フォントテクスチャの初期化
    m_pFont = new CFontTexture(m_pShader);

    // MyImgui（デバッグUI）の初期化
    MyImgui::ImguiInit(m_pMain->m_hWnd, m_pD3D, WINDOW_WIDTH, WINDOW_HEIGHT);

    // FBXメッシュコントロールクラスの生成
    m_pFbxMeshCtrl = new CFbxMeshCtrl(m_pShader);

    // ----------------------------------------------------------------------------------------
    // 各種レンダリング変数の初期値設定
    m_vLightDir = normalize( VECTOR3(0.8f, 1, -1) );  // ライト方向の初期値（正規化）

    // プロジェクション行列（透視変換）の初期化
    m_mProj = XMMatrixPerspectiveFovLH(XMConvertToRadians(38.0f), (FLOAT)WINDOW_WIDTH / (FLOAT)WINDOW_HEIGHT, 0.1f, 1000.0);

    // ビュー行列（視点変換）の初期化
    VECTOR3 vUpVec(0.0f, 1.0f, 0.0f); // 上方向
    m_vEyePt = VECTOR3(0.0f, 4.0f, -5.0f);
    m_vLookatPt = VECTOR3(0.0f, 2.0f, 0.0f);
    m_mView = XMMatrixLookAtLH(m_vEyePt, m_vLookatPt, vUpVec);

    // 各マネージャーの開始処理
    SceneManager::Start();
    ObjectManager::Start();

    // メイン制御：フレームレートの固定設定 (1/60s)
    MainControl::UseFrameTimer(1.0f / 60.0f);

    return S_OK;
}

//------------------------------------------------------------------------
//
//  ゲームの更新処理（毎フレーム実行）  
//
//------------------------------------------------------------------------
void CGameMain::Update()
{
    MyImgui::ImguiNewFrame();   // ImGuiフレーム開始処理

    m_pDI->GetInput();          // 入力デバイス情報の取得
    m_pFont->Refresh();         // 動的フォント管理の更新

    // F4キーによる画面モード（ウィンドウ/フルスクリーン）切り替え
    if (m_pDI->CheckKey(KD_TRG, DIK_F4)) ChangeScreenMode();

    SceneManager::Update();     // シーン遷移と現在のシーンの更新
    ObjectManager::Update();    // 全ゲームオブジェクトの更新
}

//------------------------------------------------------------------------
//
//  ゲームのメインロジック（自由記述用）
//
//------------------------------------------------------------------------
void CGameMain::GameMain()
{
}

//------------------------------------------------------------------------
//
//  ゲームの描画処理（毎フレーム実行）  
//
//------------------------------------------------------------------------
void CGameMain::Draw()
{
    // レンダーターゲットのクリア（黒色で塗りつぶし）
    float ClearColor[4] = { 0, 0, 0, 1 }; // RGBA
    m_pD3D->ClearRenderTarget(ClearColor);

    // 各要素のレンダリング
    SceneManager::Draw();           // 現在のシーンを描画
    ObjectManager::Draw();          // 全オブジェクトを描画
    SceneManager::DrawTransition(); // シーン切り替え時のフェード等を描画

    MyImgui::ImguiRender();         // ImGuiの描画実行

    // バックバッファを画面に表示（フリップ）
    m_pD3D->m_pSwapChain->Present(1, 0); // Vsync（垂直同期）を待つ
}

//------------------------------------------------------------------------
//
//  アプリケーションの終了処理
//
//------------------------------------------------------------------------
void CGameMain::Quit()
{
    // 必要に応じて追加の終了処理を記述
}

//------------------------------------------------------------------------
//
//  ウィンドウモードとフルスクリーンモードを切り替える処理    
//
//  引数：nMode  0:ウィンドウ 1:フルスクリーン -1:トグル(反転)
//
//------------------------------------------------------------------------
HRESULT CGameMain::ChangeScreenMode(int nMode)
{
    HRESULT Ret = S_OK;
    BOOL bFullScreen;

    // 現在のスクリーンモードを取得
    m_pD3D->m_pSwapChain->GetFullscreenState(&bFullScreen, nullptr);

    switch (nMode)
    {
    case 0:    // ウィンドウモードへ
       if( bFullScreen ) Ret = m_pD3D->m_pSwapChain->SetFullscreenState(false, nullptr);
       break;
    case 1:    // フルスクリーンモードへ
       if( !bFullScreen ) Ret = m_pD3D->m_pSwapChain->SetFullscreenState(true, nullptr);
       break; 
    default:   // 現在の状態を反転
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
//  ライトの強度を設定（RGBA）
//
//  引数：float r, g, b, a  (0.0f～1.0f)
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

// デバイスアクセスのためのグローバルな取得関数
CGameMain* GameDevice()
{
    return gameMainInstance;
}