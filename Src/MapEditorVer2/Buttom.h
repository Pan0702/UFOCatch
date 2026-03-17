#pragma once
#include <unordered_map>

#include "GridDraw.h"
#include "ModelCreator.h"
#include "StageData.h"
#include "../Object3D.h"

static constexpr int PREVIEW_SIZE = 128;

// モデルプレビュー用レンダーターゲット（D3Dリソースのみ所有、メッシュは所有しない）
struct ModelPreviewRT
{
    ID3D11RenderTargetView*   pRTV = nullptr;
    ID3D11DepthStencilView*   pDSV = nullptr;
    ID3D11ShaderResourceView* pSRV = nullptr; // ImGuiに渡す

    /// <summary>保持しているD3Dリソースをすべて解放する</summary>
    void Release()
    {
        if (pRTV) { pRTV->Release(); pRTV = nullptr; }
        if (pDSV) { pDSV->Release(); pDSV = nullptr; }
        if (pSRV) { pSRV->Release(); pSRV = nullptr; }
    }
};

// ボタン情報を管理する構造体
struct ImageButtonData
{
    std::string buttonID;
    ID3D11ShaderResourceView* pTexture;
    ImVec2 size;
    int state;
    bool isLoaded;
    bool isRenderTexture; // trueの場合ReleaseAllTexturesで解放しない

    ImageButtonData(const std::string& id, const ImVec2& buttonSize, int buttonState = 0)
        : buttonID(id), size(buttonSize), state(buttonState), pTexture(nullptr), isLoaded(false), isRenderTexture(false)
    {
    }
};

class Button : public Object3D
{

private:
    VECTOR3 world_position_;
    std::vector<ImageButtonData> image_buttons_;
    std::vector<ModelPreviewRT>  model_previews_;
    ModelCreator* model_creator_;
    GridDraw* grid_draw_;
    StageData* data_;
    int select_id;
    

public:
    Button();
    ~Button();
    void Update() override;

    /// <summary>指定IDのボタンをリストに追加する。メッシュが渡された場合はプレビューテクスチャを生成する</summary>
    /// <param name="button_ID">ボタンの識別ID（モデル名）</param>
    /// <param name="mesh">プレビュー用メッシュ（省略可）</param>
    /// <param name="size">ボタンサイズ（デフォルト64x64）</param>
    void AddButton(const std::string& button_ID, CFbxMesh* mesh = nullptr,const ImVec2& size = ImVec2(64, 64));

private:
    /// <summary>初期ボタン群を生成する（未使用）</summary>
    void InitializeButtons();

    /// <summary>isRenderTextureでないボタンのテクスチャをすべて解放してリストをクリアする</summary>
    void ReleaseAllTextures();

    /// <summary>ImageButtonDataの内容に応じてImGuiのボタンを1つ描画する</summary>
    /// <param name="buttonData">描画するボタンのデータ</param>
    void CreateImageButton(const ImageButtonData& buttonData);

    /// <summary>ボタンがクリックされたとき、対応モデルをステージに追加する</summary>
    /// <param name="buttonID">クリックされたボタンのID</param>
    static void HandleButtonClick(const std::string& buttonID);

    /// <summary>エディタUIのImGuiウィンドウ（モデル追加・エクスポート・グリッド設定）を描画する</summary>
    void DebugImGui();

    // pMesh は CModelStorage が所有するので delete しない
    /// <summary>
    /// メッシュをオフスクリーンに1回描画し、ImGuiボタン用のSRVを返す
    /// メッシュの所有権はCModelStorageが持つため、ここではdeleteしない
    /// </summary>
    /// <param name="pMesh">描画するメッシュ</param>
    /// <returns>生成したレンダーターゲット情報</returns>
    static ModelPreviewRT CreateModelPreviewRT(CFbxMesh* pMesh);

    /// <summary>全モデルプレビューのD3Dリソースを解放してリストをクリアする</summary>
    void ReleaseModelPreviews();

    void DrawHierarchy();

};
