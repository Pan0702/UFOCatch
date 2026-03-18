//=============================================================================
//     3D繧ｲ繝ｼ繝繝励Ο繧ｰ繝ｩ繝                     ver 3.2        2023.1.31
//
//     繧ｲ繝ｼ繝縺ｮ繝｡繧､繝ｳ蜃ｦ逅・
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
// CGameMain 繧ｲ繝ｼ繝繝｡繧､繝ｳ繧ｯ繝ｩ繧ｹ縺ｮ蜃ｦ逅・
//
// ============================================================================================
//------------------------------------------------------------------------
//
//  CGameMain 繧ｳ繝ｳ繧ｹ繝医Λ繧ｯ繧ｿ  
//
//  蠑墓焚・壹↑縺・
//
//------------------------------------------------------------------------
CGameMain::CGameMain(CMain* pMain)
{
    gameMainInstance = this;
    m_pMain = pMain;      // 繝｡繧､繝ｳ繧ｯ繝ｩ繧ｹ・・indowsOS螻､・・

    m_pD3D = nullptr;        // Direct3D繧ｪ繝悶ず繧ｧ繧ｯ繝・
    m_pXAudio = nullptr;      // XAudio2繧ｪ繝悶ず繧ｧ繧ｯ繝・
    m_pDI = nullptr;         // DirectInput繧ｪ繝悶ず繧ｧ繧ｯ繝・
    m_pFont = nullptr;       // 繝輔か繝ｳ繝医ユ繧ｯ繧ｹ繝√Ε
    m_pShader = nullptr;      // 繧ｷ繧ｧ繝ｼ繝繝ｼ邂｡逅・
    m_pFbxMeshCtrl = nullptr;  // FBX繝｡繝・す繝･繧ｳ繝ｳ繝医Ο繝ｼ繝ｫ繧ｯ繝ｩ繧ｹ

    // 繧ｫ繝｡繝ｩ繝ｻ繝ｩ繧､繝医・繝薙Η繝ｼ縺ｮ蛻晄悄蛹・
    m_vEyePt = VECTOR3(0,0,0);    // 繧ｫ繝｡繝ｩ・郁ｦ也せ・我ｽ咲ｽｮ
    m_vLookatPt = VECTOR3(0,0,0);  // 豕ｨ隕也せ菴咲ｽｮ
    m_mView = XMMatrixIdentity();  // 繝薙Η繝ｼ陦悟・
    m_mProj = XMMatrixIdentity();  // 繝励Ο繧ｸ繧ｧ繧ｯ繧ｷ繝ｧ繝ｳ陦悟・
    m_vLightDir = VECTOR3(0,0,0);  // 繝・ぅ繝ｬ繧ｯ繧ｷ繝ｧ繝翫Ν繝ｩ繧､繝医・譁ｹ蜷・
    m_vLightIntensity = VECTOR4(1,1,1,1);  // 繝ｩ繧､繝医・蠑ｷ蠎ｦ(繝・ヵ繧ｩ繝ｫ繝医・騾壼ｸｸ)
}
//------------------------------------------------------------------------
//
//  CGameMain 繝・せ繝医Λ繧ｯ繧ｿ    
//
//------------------------------------------------------------------------
CGameMain::~CGameMain()
{
    // 蜷・・繝阪・繧ｸ繝｣繝ｼ縺ｮ隗｣謾ｾ蜃ｦ逅・
    SceneManager::Release();
    ObjectManager::Release();
    ResourceManager::Reset();
    MyImgui::ImguiQuit();          // -- 2020.11.15    // MyImgui縺ｮ邨ゆｺ・・逅・

    SAFE_DELETE(m_pFbxMeshCtrl);    // -- 2021.2.4
    SAFE_DELETE(m_pShader);
    SAFE_DELETE(m_pFont);
    SAFE_DELETE(m_pDI);
    SAFE_DELETE(m_pXAudio);
    SAFE_DELETE(m_pD3D);

    CoUninitialize();   // COM繝ｩ繧､繝悶Λ繝ｪ縺ｮ邨ゆｺ・
}

//------------------------------------------------------------------------
//
//  繧｢繝励Μ繧ｱ繝ｼ繧ｷ繝ｧ繝ｳ縺ｮ蛻晄悄蛹門・逅・   
//
//  蠑墓焚・壹↑縺・
//
//  謌ｻ繧雁､ HRESULT
//     S_OK   = 謌仙粥
//     E_FAIL = 逡ｰ蟶ｸ
//
//------------------------------------------------------------------------
HRESULT CGameMain::Init()
{
    // COM繝ｩ繧､繝悶Λ繝ｪ縺ｮ蛻晄悄蛹・
    CoInitialize(nullptr);

    // Direct3D縺ｮ蛻晄悄蛹・
    m_pD3D = new CDirect3D;
    if (FAILED(m_pD3D->InitD3D(m_pMain->m_hWnd, WINDOW_WIDTH, WINDOW_HEIGHT))) {
       MessageBox(0, _T("Direct3D縺ｮ蛻晄悄蛹悶↓螟ｱ謨励＠縺ｾ縺励◆"), nullptr, MB_OK);
       return E_FAIL;
    }

    // 繧ｷ繧ｧ繝ｼ繝繝ｼ縺ｮ蛻晄悄蛹・
    m_pShader = new CShader(m_pD3D);
    if (FAILED(m_pShader->InitShader())) {
       MessageBox(0, _T("Shader縺ｮ逕滓・繝ｻ蛻晄悄蛹悶↓螟ｱ謨励＠縺ｾ縺励◆"), nullptr, MB_OK);
       return E_FAIL;
    }

    // XAudio2・医が繝ｼ繝・ぅ繧ｪ・峨・蛻晄悄蛹・
    m_pXAudio = new CXAudio;
    if (FAILED(m_pXAudio->InitAudio(m_pMain->m_hWnd))) {
       MessageBox(0, _T("XAudio2縺ｮ蛻晄悄蛹悶↓螟ｱ謨励＠縺ｾ縺励◆"), nullptr, MB_OK);
       return E_FAIL;
    }

    // DirectInput・亥・蜉帙ョ繝舌う繧ｹ・峨・蛻晄悄蛹・
    m_pDI = new CDirectInput;
    if (!(m_pDI->StartDirectInput(m_pMain->m_hInstance, m_pMain->m_hWnd, 
                    INIT_KEYBOARD | INIT_MOUSE | INIT_JOYSTICK, WINDOW_WIDTH, WINDOW_HEIGHT))) {
       MessageBox(0, _T("DirectInput縺ｮ蛻晄悄蛹悶↓螟ｱ謨励＠縺ｾ縺励◆"), nullptr, MB_OK);
       return E_FAIL;
    }

    // 繝輔か繝ｳ繝医ユ繧ｯ繧ｹ繝√Ε縺ｮ蛻晄悄蛹・
    m_pFont = new CFontTexture(m_pShader);

    // MyImgui・医ョ繝舌ャ繧ｰUI・峨・蛻晄悄蛹・
    MyImgui::ImguiInit(m_pMain->m_hWnd, m_pD3D, WINDOW_WIDTH, WINDOW_HEIGHT);

    // FBX繝｡繝・す繝･繧ｳ繝ｳ繝医Ο繝ｼ繝ｫ繧ｯ繝ｩ繧ｹ縺ｮ逕滓・
    m_pFbxMeshCtrl = new CFbxMeshCtrl(m_pShader);

    // ----------------------------------------------------------------------------------------
    // 蜷・ｨｮ繝ｬ繝ｳ繝繝ｪ繝ｳ繧ｰ螟画焚縺ｮ蛻晄悄蛟､險ｭ螳・
    m_vLightDir = normalize( VECTOR3(0.8f, 1, -1) );  // 繝ｩ繧､繝域婿蜷代・蛻晄悄蛟､・域ｭ｣隕丞喧・・

    // 繝励Ο繧ｸ繧ｧ繧ｯ繧ｷ繝ｧ繝ｳ陦悟・・磯剰ｦ門､画鋤・峨・蛻晄悄蛹・
    m_mProj = XMMatrixPerspectiveFovLH(XMConvertToRadians(38.0f), (FLOAT)WINDOW_WIDTH / (FLOAT)WINDOW_HEIGHT, 0.1f, 1000.0);

    // 繝薙Η繝ｼ陦悟・・郁ｦ也せ螟画鋤・峨・蛻晄悄蛹・
    VECTOR3 vUpVec(0.0f, 1.0f, 0.0f); // 荳頑婿蜷・
    m_vEyePt = VECTOR3(0.0f, 4.0f, -5.0f);
    m_vLookatPt = VECTOR3(0.0f, 2.0f, 0.0f);
    m_mView = XMMatrixLookAtLH(m_vEyePt, m_vLookatPt, vUpVec);

    // 蜷・・繝阪・繧ｸ繝｣繝ｼ縺ｮ髢句ｧ句・逅・
    SceneManager::Start();
    ObjectManager::Start();

    // 繝｡繧､繝ｳ蛻ｶ蠕｡・・
    MainControl::UseRefreshMessage();

    return S_OK;
}

//------------------------------------------------------------------------
//
//  繧ｲ繝ｼ繝縺ｮ譖ｴ譁ｰ蜃ｦ逅・ｼ域ｯ弱ヵ繝ｬ繝ｼ繝螳溯｡鯉ｼ・ 
//
//------------------------------------------------------------------------
void CGameMain::Update()
{
    MyImgui::ImguiNewFrame();   // ImGui繝輔Ξ繝ｼ繝髢句ｧ句・逅・

    m_pDI->GetInput();          // 蜈･蜉帙ョ繝舌う繧ｹ諠・ｱ縺ｮ蜿門ｾ・
    m_pFont->Refresh();         // 蜍慕噪繝輔か繝ｳ繝育ｮ｡逅・・譖ｴ譁ｰ

    // F4繧ｭ繝ｼ縺ｫ繧医ｋ逕ｻ髱｢繝｢繝ｼ繝会ｼ医え繧｣繝ｳ繝峨え/繝輔Ν繧ｹ繧ｯ繝ｪ繝ｼ繝ｳ・牙・繧頑崛縺・
    if (m_pDI->CheckKey(KD_TRG, DIK_F4)) ChangeScreenMode();

    SceneManager::Update();     // 繧ｷ繝ｼ繝ｳ驕ｷ遘ｻ縺ｨ迴ｾ蝨ｨ縺ｮ繧ｷ繝ｼ繝ｳ縺ｮ譖ｴ譁ｰ
    ObjectManager::Update();    // 蜈ｨ繧ｲ繝ｼ繝繧ｪ繝悶ず繧ｧ繧ｯ繝医・譖ｴ譁ｰ
}

//------------------------------------------------------------------------
//
//  繧ｲ繝ｼ繝縺ｮ繝｡繧､繝ｳ繝ｭ繧ｸ繝・け・郁・逕ｱ險倩ｿｰ逕ｨ・・
//
//------------------------------------------------------------------------
void CGameMain::GameMain()
{
}

//------------------------------------------------------------------------
//
//  繧ｲ繝ｼ繝縺ｮ謠冗判蜃ｦ逅・ｼ域ｯ弱ヵ繝ｬ繝ｼ繝螳溯｡鯉ｼ・ 
//
//------------------------------------------------------------------------
void CGameMain::Draw()
{
    // 繝ｬ繝ｳ繝繝ｼ繧ｿ繝ｼ繧ｲ繝・ヨ縺ｮ繧ｯ繝ｪ繧｢・磯ｻ定牡縺ｧ蝪励ｊ縺､縺ｶ縺暦ｼ・
    float ClearColor[4] = { 0, 0, 0, 1 }; // RGBA
    m_pD3D->ClearRenderTarget(ClearColor);

    // 蜷・ｦ∫ｴ縺ｮ繝ｬ繝ｳ繝繝ｪ繝ｳ繧ｰ
    SceneManager::Draw();           // 迴ｾ蝨ｨ縺ｮ繧ｷ繝ｼ繝ｳ繧呈緒逕ｻ
    ObjectManager::Draw();          // 蜈ｨ繧ｪ繝悶ず繧ｧ繧ｯ繝医ｒ謠冗判
    SceneManager::DrawTransition(); // 繧ｷ繝ｼ繝ｳ蛻・ｊ譖ｿ縺域凾縺ｮ繝輔ぉ繝ｼ繝臥ｭ峨ｒ謠冗判

    MyImgui::ImguiRender();         // ImGui縺ｮ謠冗判螳溯｡・

    // 繝舌ャ繧ｯ繝舌ャ繝輔ぃ繧堤判髱｢縺ｫ陦ｨ遉ｺ・医ヵ繝ｪ繝・・・・
    m_pD3D->m_pSwapChain->Present(1, 0); // Vsync・亥桙逶ｴ蜷梧悄・峨ｒ蠕・▽
}

//------------------------------------------------------------------------
//
//  繧｢繝励Μ繧ｱ繝ｼ繧ｷ繝ｧ繝ｳ縺ｮ邨ゆｺ・・逅・
//
//------------------------------------------------------------------------
void CGameMain::Quit()
{
    // 蠢・ｦ√↓蠢懊§縺ｦ霑ｽ蜉縺ｮ邨ゆｺ・・逅・ｒ險倩ｿｰ
}

//------------------------------------------------------------------------
//
//  繧ｦ繧｣繝ｳ繝峨え繝｢繝ｼ繝峨→繝輔Ν繧ｹ繧ｯ繝ｪ繝ｼ繝ｳ繝｢繝ｼ繝峨ｒ蛻・ｊ譖ｿ縺医ｋ蜃ｦ逅・   
//
//  蠑墓焚・嗜Mode  0:繧ｦ繧｣繝ｳ繝峨え 1:繝輔Ν繧ｹ繧ｯ繝ｪ繝ｼ繝ｳ -1:繝医げ繝ｫ(蜿崎ｻ｢)
//
//------------------------------------------------------------------------
HRESULT CGameMain::ChangeScreenMode(int nMode)
{
    HRESULT Ret = S_OK;
    BOOL bFullScreen;

    // 迴ｾ蝨ｨ縺ｮ繧ｹ繧ｯ繝ｪ繝ｼ繝ｳ繝｢繝ｼ繝峨ｒ蜿門ｾ・
    m_pD3D->m_pSwapChain->GetFullscreenState(&bFullScreen, nullptr);

    switch (nMode)
    {
    case 0:    // 繧ｦ繧｣繝ｳ繝峨え繝｢繝ｼ繝峨∈
       if( bFullScreen ) Ret = m_pD3D->m_pSwapChain->SetFullscreenState(false, nullptr);
       break;
    case 1:    // 繝輔Ν繧ｹ繧ｯ繝ｪ繝ｼ繝ｳ繝｢繝ｼ繝峨∈
       if( !bFullScreen ) Ret = m_pD3D->m_pSwapChain->SetFullscreenState(true, nullptr);
       break; 
    default:   // 迴ｾ蝨ｨ縺ｮ迥ｶ諷九ｒ蜿崎ｻ｢
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
//  繝ｩ繧､繝医・蠑ｷ蠎ｦ繧定ｨｭ螳夲ｼ・GBA・・
//
//  蠑墓焚・喃loat r, g, b, a  (0.0f・・.0f)
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

// 繝・ヰ繧､繧ｹ繧｢繧ｯ繧ｻ繧ｹ縺ｮ縺溘ａ縺ｮ繧ｰ繝ｭ繝ｼ繝舌Ν縺ｪ蜿門ｾ鈴未謨ｰ
CGameMain* GameDevice()
{
    return gameMainInstance;
}
