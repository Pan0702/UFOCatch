#include "Buttom.h"
#include "Controller.h"
#include "FileDialog.h"
#include "Import.h"
#include "StageData.h"
#include "../ModelStorage.h"
struct ImageButtonData;
class CFbxMesh;

Button::Button()
{
    m_worldPosition = VECTOR3(0.0f, 0.0f, 0.0f);

    // CModelStorage 縺九ｉ繝｡繝・す繝･繧貞叙蠕励＠縺ｦ1蝗槭□縺第緒逕ｻ縺・SRV 縺ｫ辟ｼ縺堺ｻ倥￠繧・
    CModelStorage* storage = ObjectManager::FindGameObject<CModelStorage>();
    m_pData = ObjectManager::FindGameObject<StageData>();
    if (storage)
    {
        AddButton("Plane", storage->GetModel("Plane"));
        AddButton("Curve", storage->GetModel("Curve"));
        AddButton("GoalLine", storage->GetModel("GoalLine"));
    }

    m_pModelCreator = new ModelCreator();
    m_pGridDraw = new GridDraw();
}

Button::~Button()
{
    ReleaseAllTextures();
    ReleaseModelPreviews();
}

void Button::Update()
{
    DebugImGui();
}

// 謖・ｮ唔D縺ｮ繝懊ち繝ｳ繧偵Μ繧ｹ繝医↓霑ｽ蜉縺吶ｋ縲ゅΓ繝・す繝･縺梧ｸ｡縺輔ｌ縺溷ｴ蜷医・繝励Ξ繝薙Η繝ｼ繝・け繧ｹ繝√Ε繧堤函謌舌☆繧・
void Button::AddButton(const std::string& buttonId, CFbxMesh* pMesh, const ImVec2& size)
{
    ImageButtonData data(buttonId, size);
    if (pMesh)
    {
        ModelPreviewRT rt = CreateModelPreviewRT(pMesh);
        m_modelpreviews.push_back(rt);
        data.pTexture = rt.pSRV;
        data.isLoaded = (rt.pSRV != nullptr);
        data.isRenderTexture = true;
    }
    m_imageButtons.push_back(data);
}

// isRenderTexture縺ｧ縺ｪ縺・・繧ｿ繝ｳ縺ｮ繝・け繧ｹ繝√Ε繧偵☆縺ｹ縺ｦ隗｣謾ｾ縺励※繝ｪ繧ｹ繝医ｒ繧ｯ繝ｪ繧｢縺吶ｋ
void Button::ReleaseAllTextures()
{
    for (auto& button : m_imageButtons)
    {
        if (button.pTexture && !button.isRenderTexture)
        {
            button.pTexture->Release();
            button.pTexture = nullptr;
        }
    }
    m_imageButtons.clear();
}

// 繝｡繝・す繝･繧偵が繝輔せ繧ｯ繝ｪ繝ｼ繝ｳ縺ｫ1蝗樊緒逕ｻ縺励！mGui繝懊ち繝ｳ逕ｨ縺ｮSRV繧貞性繧繝ｬ繝ｳ繝繝ｼ繧ｿ繝ｼ繧ｲ繝・ヨ繧定ｿ斐☆
ModelPreviewRT Button::CreateModelPreviewRT(CFbxMesh* pMesh)
{
    ModelPreviewRT rt = {};
    if (!pMesh) return rt;

    auto* pD3D = GameDevice()->m_pD3D;
    auto* pDevice = pD3D->m_pDevice;
    auto* pCtx = pD3D->m_pDeviceContext;

    // 繝・け繧ｹ繝√Ε蜈ｱ騾夊ｨｭ螳・
    D3D11_TEXTURE2D_DESC td = {};
    td.Width = td.Height = PREVIEW_SIZE;
    td.MipLevels = td.ArraySize = 1;
    td.SampleDesc.Count = 1;

    // 繧ｫ繝ｩ繝ｼ繝・け繧ｹ繝√Ε縺ｨ繝薙Η繝ｼ縺ｮ菴懈・
    td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    td.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    ID3D11Texture2D* pTex = nullptr;
    pDevice->CreateTexture2D(&td, nullptr, &pTex);
    pDevice->CreateRenderTargetView(pTex, nullptr, &rt.pRTV);
    pDevice->CreateShaderResourceView(pTex, nullptr, &rt.pSRV);
    pTex->Release();

    // 豺ｱ蠎ｦ繝舌ャ繝輔ぃ縺ｮ菴懈・
    td.Format = DXGI_FORMAT_D32_FLOAT;
    td.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    pDevice->CreateTexture2D(&td, nullptr, &pTex);
    pDevice->CreateDepthStencilView(pTex, nullptr, &rt.pDSV);
    pTex->Release();

    // 迴ｾ蝨ｨ縺ｮ繝薙Η繝ｼ繝昴・繝医ｒ菫晏ｭ假ｼ域緒逕ｻ蠕後↓蠕ｩ蜈・☆繧九◆繧・ｼ・
    D3D11_VIEWPORT oldVP;
    UINT numVP = 1;
    pCtx->RSGetViewports(&numVP, &oldVP);

    // 繝励Ξ繝薙Η繝ｼ逕ｨ縺ｫ繝薙Η繝ｼ繝昴・繝医ｒ PREVIEW_SIZE 縺ｮ豁｣譁ｹ蠖｢縺ｫ險ｭ螳・
    D3D11_VIEWPORT vp = {0, 0, static_cast<float>(PREVIEW_SIZE), static_cast<float>(PREVIEW_SIZE), 0.0f, 1.0f};
    pCtx->RSSetViewports(1, &vp);

    // 繝ｬ繝ｳ繝繝ｪ繝ｳ繧ｰ繧ｿ繝ｼ繧ｲ繝・ヨ縺ｮ險ｭ螳壹→繧ｯ繝ｪ繧｢
    pD3D->SetRenderTarget(rt.pRTV, rt.pDSV);
    constexpr float CLEAR_R= 0.12f;
    constexpr float CLEAR_G = 0.12f;    
    constexpr float CLEAR_B = 0.12f;
    constexpr float CLEAR_A = 1.0f;

    float clearColor[4] = {CLEAR_R, CLEAR_G, CLEAR_B, CLEAR_A};
    pD3D->ClearRenderTarget(clearColor);

    // 繝｢繝・Ν縺ｮ AABB 縺九ｉ繝舌え繝ｳ繝・ぅ繝ｳ繧ｰ繧ｹ繝輔ぅ繧｢繧定ｨ育ｮ励＠縺ｦ繧ｫ繝｡繝ｩ霍晞屬繧定・蜍戊ｪｿ謨ｴ
    XMVECTOR center = XMVectorSet(
        (pMesh->m_vMin.x + pMesh->m_vMax.x) * 0.5f,
        (pMesh->m_vMin.y + pMesh->m_vMax.y) * 0.5f,
        (pMesh->m_vMin.z + pMesh->m_vMax.z) * 0.5f, 0.0f);
    XMVECTOR extents = XMVectorSet(
        pMesh->m_vMax.x - pMesh->m_vMin.x,
        pMesh->m_vMax.y - pMesh->m_vMin.y,
        pMesh->m_vMax.z - pMesh->m_vMin.z, 0.0f);
    float radius = XMVectorGetX(XMVector3Length(extents)) * 0.5f;
    constexpr float PREVIEW_CAM_MARGIN = 1.3f; // 繝｢繝・Ν縺悟庶縺ｾ繧九ｈ縺・↓縺吶ｋ菴咏區菫よ焚
    constexpr float PREVIEW_FOV_DEG = 45.0f;
    float distance = (radius / tanf(XMConvertToRadians(PREVIEW_FOV_DEG) * 0.5f)) * PREVIEW_CAM_MARGIN;

    constexpr float PREVIEW_CAM_DIR_X = 5.0f; // 繧ｫ繝｡繝ｩ譁ｹ蜷代・繧ｯ繝医Ν・域ｭ｣隕丞喧蜑搾ｼ・
    constexpr float PREVIEW_CAM_DIR_Y = 5.0f;
    constexpr float PREVIEW_CAM_DIR_Z = -8.0f;
    XMVECTOR cam_dir = XMVector3Normalize(XMVectorSet(PREVIEW_CAM_DIR_X, PREVIEW_CAM_DIR_Y, PREVIEW_CAM_DIR_Z, 0.0f));
    XMVECTOR eye = XMVectorAdd(center, XMVectorScale(cam_dir, distance));

    float near_clip = distance * 0.01f;
    float far_clip = distance * 10.0f;

    XMMATRIX mView = XMMatrixLookAtLH(eye, center, XMVectorSet(0, 1, 0, 0));
    constexpr float PREVIEW_ASPECT = 1.0f; // 豁｣譁ｹ蠖｢繝・け繧ｹ繝√Ε縺ｪ縺ｮ縺ｧ繧｢繧ｹ繝壹け繝域ｯ・//

    XMMATRIX mProj = XMMatrixPerspectiveFovLH(
        XMConvertToRadians(PREVIEW_FOV_DEG), PREVIEW_ASPECT, near_clip, far_clip);

    XMFLOAT3 eye_f;
    XMStoreFloat3(&eye_f, eye);
    const VECTOR3 vEye = VECTOR3(eye_f.x, eye_f.y, eye_f.z);
    // 繝励Ξ繝薙Η繝ｼ逕ｨ繝ｩ繧､繝域婿蜷代→隕也せ菴咲ｽｮ・医Λ繧､繝郁ｨ育ｮ礼畑・・/
    const VECTOR3 PEVIEW_LIGHT_DIR = VECTOR3(0.5f, 1.0f, -1.0f);
    pMesh->Render(XMMatrixIdentity(), mView, mProj, PEVIEW_LIGHT_DIR, vEye);

    // 謠冗判蜈医→繝薙Η繝ｼ繝昴・繝医ｒ蜈・↓謌ｻ縺・
    pD3D->SetRenderTarget(nullptr, nullptr);
    pCtx->RSSetViewports(1, &oldVP);

    return rt;
}

// 蜈ｨ繝｢繝・Ν繝励Ξ繝薙Η繝ｼ縺ｮD3D繝ｪ繧ｽ繝ｼ繧ｹ繧定ｧ｣謾ｾ縺励※繝ｪ繧ｹ繝医ｒ繧ｯ繝ｪ繧｢縺吶ｋ
void Button::ReleaseModelPreviews()
{
    for (auto& rt : m_modelpreviews)
    {
        rt.Release();
    }
    m_modelpreviews.clear();
}


// ImageButtonData縺ｮ蜀・ｮｹ縺ｫ蠢懊§縺ｦImGui縺ｮ繝懊ち繝ｳ繧・縺､謠冗判縺吶ｋ
void Button::CreateImageButton(const ImageButtonData& buttonData)
{
    if (buttonData.isLoaded && buttonData.pTexture)
    {
        // 繝励Ξ繝薙Η繝ｼ繝・け繧ｹ繝√Ε縺後≠繧句ｴ蜷医・逕ｻ蜒上・繧ｿ繝ｳ縺ｨ縺励※謠冗判
        ImGui::PushID(buttonData.buttonID.c_str());
        if (ImGui::ImageButton((ImTextureID)buttonData.pTexture, buttonData.size))
            HandleButtonClick(buttonData.buttonID);
        ImGui::PopID();
    }
    else
    {
        // 繝・け繧ｹ繝√Ε縺後↑縺・ｴ蜷医・繝・く繧ｹ繝医・繧ｿ繝ｳ縺ｫ繝輔か繝ｼ繝ｫ繝舌ャ繧ｯ
        if (ImGui::Button(buttonData.buttonID.c_str(), buttonData.size))
            HandleButtonClick(buttonData.buttonID);
    }
}

// 繝懊ち繝ｳ縺後け繝ｪ繝・け縺輔ｌ縺溘→縺阪∝ｯｾ蠢懊Δ繝・Ν繧貞次轤ｹ縺ｫ繧ｹ繝・・繧ｸ縺ｸ霑ｽ蜉縺吶ｋ
void Button::HandleButtonClick(const std::string& buttonId)
{
    VECTOR3 init_pos = {0, 0, 0};
    ObjectManager::FindGameObject<StageData>()->AddModel(init_pos, buttonId);
}

// 繧ｨ繝・ぅ繧ｿUI縺ｮImGui繧ｦ繧｣繝ｳ繝峨え・医Δ繝・Ν霑ｽ蜉繝ｻ繧ｨ繧ｯ繧ｹ繝昴・繝医・繧ｰ繝ｪ繝・ラ險ｭ螳夲ｼ峨ｒ謠冗判縺吶ｋ
void Button::DebugImGui()
{
    // UI 繧ｦ繧｣繝ｳ繝峨え縺ｮ繝ｬ繧､繧｢繧ｦ繝亥ｮ壽焚
    constexpr float EDITOR_TOOL_WINDOW_Y = 10.0f;
    // 逕ｻ髱｢蜿ｳ遶ｯ縺九ｉ縺ｮ霍晞屬
    constexpr float EDITOR_TOOLS_OFFSERT_FROM_RIGHT = 320.0f; 
    // 繝｢繝・Ν霑ｽ蜉繝ｻ繝懊ち繝ｳ荳隕ｧ繧ｦ繧｣繝ｳ繝峨え・育判髱｢蜿ｳ遶ｯ縺ｫ蝗ｺ螳夲ｼ・
    ImGui::SetNextWindowPos(
        ImVec2(ImGui::GetIO().DisplaySize.x - EDITOR_TOOLS_OFFSERT_FROM_RIGHT, EDITOR_TOOL_WINDOW_Y),
        ImGuiCond_FirstUseEver);
    ImGui::Begin("Editor Tools");

    ImGui::Separator();
    //繝｢繝・Ν縺ｮ隱ｭ縺ｿ霎ｼ縺ｿ
    ImGui::Text("Add Model");
    if (ImGui::Button("OpenModel"))
    {
        std::string path = Platform::OpenFileDialog(L"*.mesh;*.fbx");
        if (!path.empty())
        {
            // 諡｡蠑ｵ蟄舌〒 .mesh / .fbx 繧呈険繧雁・縺代ｋ
            const size_t dot = path.find_last_of('.');
            std::string ext = (dot != std::string::npos) ? path.substr(dot) : "";
            for (auto& c : ext) c = static_cast<char>(tolower(c));

            if (ext == ".fbx")
                m_pModelCreator->ConvertAndLoad(path);
            else
                m_pModelCreator->CreateModel(path);
        }
    }
    ImGui::Separator();
    ImGui::BeginGroup();
    {
        for (size_t i = 0; i < m_imageButtons.size(); i++)
        {
            CreateImageButton(m_imageButtons[i]);
            // 3蛻励＃縺ｨ縺ｫ謾ｹ陦後☆繧・
            if ((i + 1) % 3 != 0 && i < m_imageButtons.size() - 1) ImGui::SameLine();
        }
    }
    ImGui::EndGroup();
    ImGui::Separator();
    ImGui::End();

    constexpr float SETTING_WINDOW_Y = 300.0f;
    // 險ｭ螳壹え繧｣繝ｳ繝峨え・医げ繝ｪ繝・ラ陦ｨ遉ｺ蛻・崛繝ｻ繧ｨ繧ｯ繧ｹ繝昴・繝茨ｼ・
    ImGui::SetNextWindowPos(
        ImVec2(ImGui::GetIO().DisplaySize.x - EDITOR_TOOLS_OFFSERT_FROM_RIGHT, SETTING_WINDOW_Y));
    ImGui::Begin("Setting", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    bool flag = m_pGridDraw->GetDrawFlag();
    if (ImGui::Checkbox("Grid", &flag))
    {
        m_pGridDraw->SetDrawFlag(flag);
    }
    ImGui::Separator();
    if (ImGui::Button("Export"))
    {
            std::string path = Platform::SaveFileDialog(L"*.json", L"data");
        if (!path.empty())
        {
            ObjectManager::FindGameObject<StageData>()->Export(path);
        }
    }
    ImGui::SameLine();
    if (ImGui::Button("Import"))
    {
        std::string path = Platform::OpenFileDialog(L"*.json");
        if (!path.empty())
        {
            Import::ImportFromFile(path);
        }
    }
    ImGui::End();
    DrawHierarchy();
}

void Button::DrawHierarchy()
{
    ImGui::SetNextWindowPos(ImVec2(10, 180), ImGuiCond_Once);
    ImGui::SetNextWindowSizeConstraints(ImVec2(120, 0), ImVec2(FLT_MAX, FLT_MAX));
    ImGui::Begin("Hierarchy", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    const std::vector<StageDataInfo>& stage_data = m_pData->GetStageDataInfo();

    std::unordered_map<std::string, int> nameCounts;
    for (int i = 0; i < stage_data.size(); i++)
    {
        bool is_select = (m_pSelectId == i);
        std::string name = stage_data[i].modelName;

        // 縺吶〒縺ｫ蜷後§蜷榊燕縺檎匳骭ｲ縺輔ｌ縺ｦ縺・ｋ縺狗｢ｺ隱・
        if (nameCounts.contains(name))
        {
            // 2蝗樒岼莉･髯阪・蜃ｺ迴ｾ縺ｪ縺ｮ縺ｧ縲√き繧ｦ繝ｳ繝医ｒ蠅励ｄ縺励※蜷榊燕縺ｫ蜿肴丐
            nameCounts[name]++;
            name += "_" + std::to_string(nameCounts[name]);
        }
        else
        {
            // 蛻昴ａ縺ｦ蜃ｺ縺ｦ縺上ｋ蜷榊燕縺ｪ繧峨√き繧ｦ繝ｳ繝医ｒ1縺ｫ險ｭ螳・
            nameCounts[name] = 0;
        }
        if (ImGui::Selectable(name.c_str(), is_select))
        {
            m_pData->SetModel(i);
            ObjectManager::FindGameObject<Controller>()->SetCatchFlag(true);
            m_pSelectId = i;
        }
    }
    ImGui::End();
}

