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
    world_position_ = VECTOR3(0.0f, 0.0f, 0.0f);

    // CModelStorage からメッシュを取得して1回だけ描画し SRV に焼き付ける
    CModelStorage* storage = ObjectManager::FindGameObject<CModelStorage>();
    data_ = ObjectManager::FindGameObject<StageData>();
    if (storage)
    {
        AddButton("Plane", storage->GetModel("Plane"));
        AddButton("Curve", storage->GetModel("Curve"));
        AddButton("GoalLine", storage->GetModel("GoalLine"));
    }

    model_creator_ = new ModelCreator();
    grid_draw_ = new GridDraw();
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

// 指定IDのボタンをリストに追加する。メッシュが渡された場合はプレビューテクスチャを生成する
void Button::AddButton(const std::string& button_ID, CFbxMesh* mesh, const ImVec2& size)
{
    ImageButtonData data(button_ID, size);
    if (mesh)
    {
        ModelPreviewRT rt = CreateModelPreviewRT(mesh);
        model_previews_.push_back(rt);
        data.pTexture = rt.pSRV;
        data.isLoaded = (rt.pSRV != nullptr);
        data.isRenderTexture = true;
    }
    image_buttons_.push_back(data);
}

// isRenderTextureでないボタンのテクスチャをすべて解放してリストをクリアする
void Button::ReleaseAllTextures()
{
    for (auto& button : image_buttons_)
    {
        if (button.pTexture && !button.isRenderTexture)
        {
            button.pTexture->Release();
            button.pTexture = nullptr;
        }
    }
    image_buttons_.clear();
}

// メッシュをオフスクリーンに1回描画し、ImGuiボタン用のSRVを含むレンダーターゲットを返す
ModelPreviewRT Button::CreateModelPreviewRT(CFbxMesh* pMesh)
{
    ModelPreviewRT rt = {};
    if (!pMesh) return rt;

    auto* pD3D = GameDevice()->m_pD3D;
    auto* pDevice = pD3D->m_pDevice;
    auto* pCtx = pD3D->m_pDeviceContext;

    // テクスチャ共通設定
    D3D11_TEXTURE2D_DESC td = {};
    td.Width = td.Height = PREVIEW_SIZE;
    td.MipLevels = td.ArraySize = 1;
    td.SampleDesc.Count = 1;

    // カラーテクスチャとビューの作成
    td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    td.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    ID3D11Texture2D* pTex = nullptr;
    pDevice->CreateTexture2D(&td, nullptr, &pTex);
    pDevice->CreateRenderTargetView(pTex, nullptr, &rt.pRTV);
    pDevice->CreateShaderResourceView(pTex, nullptr, &rt.pSRV);
    pTex->Release();

    // 深度バッファの作成
    td.Format = DXGI_FORMAT_D32_FLOAT;
    td.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    pDevice->CreateTexture2D(&td, nullptr, &pTex);
    pDevice->CreateDepthStencilView(pTex, nullptr, &rt.pDSV);
    pTex->Release();

    // 現在のビューポートを保存（描画後に復元するため）
    D3D11_VIEWPORT oldVP;
    UINT numVP = 1;
    pCtx->RSGetViewports(&numVP, &oldVP);

    // プレビュー用にビューポートを PREVIEW_SIZE の正方形に設定
    D3D11_VIEWPORT vp = {0, 0, static_cast<float>(PREVIEW_SIZE), static_cast<float>(PREVIEW_SIZE), 0.0f, 1.0f};
    pCtx->RSSetViewports(1, &vp);

    // レンダリングターゲットの設定とクリア
    pD3D->SetRenderTarget(rt.pRTV, rt.pDSV);
    constexpr float kClearR = 0.12f;
    constexpr float kClearG = 0.12f;
    constexpr float kClearB = 0.12f;
    constexpr float kClearA = 1.0f;

    float clearColor[4] = {kClearR, kClearG, kClearB, kClearA};
    pD3D->ClearRenderTarget(clearColor);

    // モデルの AABB からバウンディングスフィアを計算してカメラ距離を自動調整
    XMVECTOR center = XMVectorSet(
        (pMesh->m_vMin.x + pMesh->m_vMax.x) * 0.5f,
        (pMesh->m_vMin.y + pMesh->m_vMax.y) * 0.5f,
        (pMesh->m_vMin.z + pMesh->m_vMax.z) * 0.5f, 0.0f);
    XMVECTOR extents = XMVectorSet(
        pMesh->m_vMax.x - pMesh->m_vMin.x,
        pMesh->m_vMax.y - pMesh->m_vMin.y,
        pMesh->m_vMax.z - pMesh->m_vMin.z, 0.0f);
    float radius = XMVectorGetX(XMVector3Length(extents)) * 0.5f;
    constexpr float kPreviewCamMargin = 1.3f; // モデルが収まるようにする余白係数
    constexpr float kPreviewFovDeg = 45.0f;
    float distance = (radius / tanf(XMConvertToRadians(kPreviewFovDeg) * 0.5f)) * kPreviewCamMargin;

    constexpr float kPreviewCamDirX = 5.0f; // カメラ方向ベクトル（正規化前）
    constexpr float kPreviewCamDirY = 5.0f;
    constexpr float kPreviewCamDirZ = -8.0f;
    XMVECTOR cam_dir = XMVector3Normalize(XMVectorSet(kPreviewCamDirX, kPreviewCamDirY, kPreviewCamDirZ, 0.0f));
    XMVECTOR eye = XMVectorAdd(center, XMVectorScale(cam_dir, distance));

    float near_clip = distance * 0.01f;
    float far_clip = distance * 10.0f;

    XMMATRIX mView = XMMatrixLookAtLH(eye, center, XMVectorSet(0, 1, 0, 0));
    constexpr float kPreviewAspect = 1.0f; // 正方形テクスチャなのでアスペクト比1//

    XMMATRIX mProj = XMMatrixPerspectiveFovLH(
        XMConvertToRadians(kPreviewFovDeg), kPreviewAspect, near_clip, far_clip);

    XMFLOAT3 eye_f;
    XMStoreFloat3(&eye_f, eye);
    const VECTOR3 light_eye = VECTOR3(eye_f.x, eye_f.y, eye_f.z);
    // プレビュー用ライト方向と視点位置（ライト計算用）//
    const VECTOR3 kPreviewLightDir = VECTOR3(0.5f, 1.0f, -1.0f);
    pMesh->Render(XMMatrixIdentity(), mView, mProj, kPreviewLightDir, light_eye);

    // 描画先とビューポートを元に戻す
    pD3D->SetRenderTarget(nullptr, nullptr);
    pCtx->RSSetViewports(1, &oldVP);

    return rt;
}

// 全モデルプレビューのD3Dリソースを解放してリストをクリアする
void Button::ReleaseModelPreviews()
{
    for (auto& rt : model_previews_)
    {
        rt.Release();
    }
    model_previews_.clear();
}


// ImageButtonDataの内容に応じてImGuiのボタンを1つ描画する
void Button::CreateImageButton(const ImageButtonData& buttonData)
{
    if (buttonData.isLoaded && buttonData.pTexture)
    {
        // プレビューテクスチャがある場合は画像ボタンとして描画
        ImGui::PushID(buttonData.buttonID.c_str());
        if (ImGui::ImageButton((ImTextureID)buttonData.pTexture, buttonData.size))
            HandleButtonClick(buttonData.buttonID);
        ImGui::PopID();
    }
    else
    {
        // テクスチャがない場合はテキストボタンにフォールバック
        if (ImGui::Button(buttonData.buttonID.c_str(), buttonData.size))
            HandleButtonClick(buttonData.buttonID);
    }
}

// ボタンがクリックされたとき、対応モデルを原点にステージへ追加する
void Button::HandleButtonClick(const std::string& buttonID)
{
    VECTOR3 init_pos = {0, 0, 0};
    ObjectManager::FindGameObject<StageData>()->AddModel(init_pos, buttonID);
}

// エディタUIのImGuiウィンドウ（モデル追加・エクスポート・グリッド設定）を描画する
void Button::DebugImGui()
{
    // UI ウィンドウのレイアウト定数
    constexpr float kEditorToolsWindowY = 10.0f;
    // 画面右端からの距離
    constexpr float kEditorToolsOffsetFromRight = 320.0f; 
    // モデル追加・ボタン一覧ウィンドウ（画面右端に固定）
    ImGui::SetNextWindowPos(
        ImVec2(ImGui::GetIO().DisplaySize.x - kEditorToolsOffsetFromRight, kEditorToolsWindowY),
        ImGuiCond_FirstUseEver);
    ImGui::Begin("Editor Tools");

    ImGui::Separator();
    //モデルの読み込み
    ImGui::Text("Add Model");
    if (ImGui::Button("OpenModel"))
    {
        std::string path = Platform::OpenFileDialog(L"*.mesh;*.fbx");
        if (!path.empty())
        {
            // 拡張子で .mesh / .fbx を振り分ける
            size_t dot = path.find_last_of('.');
            std::string ext = (dot != std::string::npos) ? path.substr(dot) : "";
            for (auto& c : ext) c = static_cast<char>(tolower(c));

            if (ext == ".fbx")
                model_creator_->ConvertAndLoad(path);
            else
                model_creator_->CreateModel(path);
        }
    }
    ImGui::Separator();
    ImGui::BeginGroup();
    {
        for (size_t i = 0; i < image_buttons_.size(); i++)
        {
            CreateImageButton(image_buttons_[i]);
            // 3列ごとに改行する
            if ((i + 1) % 3 != 0 && i < image_buttons_.size() - 1) ImGui::SameLine();
        }
    }
    ImGui::EndGroup();
    ImGui::Separator();
    ImGui::End();

    constexpr float kSettingWindowY = 300.0f;
    // 設定ウィンドウ（グリッド表示切替・エクスポート）
    ImGui::SetNextWindowPos(
        ImVec2(ImGui::GetIO().DisplaySize.x - kEditorToolsOffsetFromRight, kSettingWindowY));
    ImGui::Begin("Setting", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    bool flag = grid_draw_->GetDrawFlag();
    if (ImGui::Checkbox("Grid", &flag))
    {
        grid_draw_->SetDrawFlag(flag);
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

    const std::vector<StageDataInfo>& stage_data = data_->GetStageDataInfo();

    std::unordered_map<std::string, int> nameCounts;
    for (int i = 0; i < stage_data.size(); i++)
    {
        bool is_select = (select_id == i);
        std::string name = stage_data[i].model_name_;

        // すでに同じ名前が登録されているか確認
        if (nameCounts.find(name) != nameCounts.end())
        {
            // 2回目以降の出現なので、カウントを増やして名前に反映
            nameCounts[name]++;
            name += "_" + std::to_string(nameCounts[name]);
        }
        else
        {
            // 初めて出てくる名前なら、カウントを1に設定
            nameCounts[name] = 0;
        }
        if (ImGui::Selectable(name.c_str(), is_select))
        {
            data_->SetModel(i);
            ObjectManager::FindGameObject<Controller>()->SetCatchFlag(true);
            select_id = i;
        }
    }
    ImGui::End();
}
