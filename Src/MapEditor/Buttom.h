#pragma once
#include <unordered_map>

#include "GridDraw.h"
#include "ModelCreator.h"
#include "StageData.h"
#include "../Common/Object3D.h"

static constexpr int PREVIEW_SIZE = 128;

// モデルプレビュー用レンダーターゲット（3Dリソースのみ所有、メッシュは所有しない）
struct ModelPreviewRT
{
    ID3D11RenderTargetView* pRTV = nullptr;
    ID3D11DepthStencilView* pDSV = nullptr;
    ID3D11ShaderResourceView* pSRV = nullptr; // ImGuiに渡す

    /// 解放する
    void Release()
    {
        if (pRTV)
        {
            pRTV->Release();
            pRTV = nullptr;
        }
        if (pDSV)
        {
            pDSV->Release();
            pDSV = nullptr;
        }
        if (pSRV)
        {
            pSRV->Release();
            pSRV = nullptr;
        }
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

    /// ImageButtonData を初期化する
    /// @param id id に渡す値
    /// @param buttonSize サイズ
    /// @param buttonState 状態
    ImageButtonData(const std::string& id, const ImVec2& buttonSize, int buttonState = 0)
        : buttonID(id), size(buttonSize), state(buttonState), pTexture(nullptr), isLoaded(false), isRenderTexture(false)
    {
    }
};

/// <summary>ステージエディタで使う Button の情報と処理をまとめる型</summary>
class Button : public Object3D
{
private:
    VECTOR3 m_worldPosition;
    std::vector<ImageButtonData> m_imageButtons;
    std::vector<ModelPreviewRT> m_modelpreviews;
    ModelCreator* m_pModelCreator;
    GridDraw* m_pGridDraw;
    StageData* m_pData;
    int m_pSelectId;

public:
    /// Button を初期化する
    Button();
    /// Button の終了処理を行う
    ~Button();

    /// Button を追加する
    /// @param buttonId buttonId に渡す値
    /// @param pMesh pMesh に渡す値
    /// @param size サイズ
    void AddButton(const std::string& buttonId, CFbxMesh* pMesh = nullptr, const ImVec2& size = ImVec2(64, 64));

    /// Editor Tools Panel を描画する
    void DrawEditorToolsPanel();

    /// Setting Panel を描画する
    void DrawSettingPanel();

    /// Hierarchy Panel を描画する
    void DrawHierarchyPanel();

private:
    /// All Textures を解放する
    void ReleaseAllTextures();

    /// Image Button を作成する
    /// @param buttonData buttonData に渡す値
    static void CreateImageButton(const ImageButtonData& buttonData);

    /// Button Click を処理する
    /// @param buttonId buttonId に渡す値
    static void HandleButtonClick(const std::string& buttonId);

    // pMesh は CModelStorage が所有するので delete しない
    /// Model Preview RT を作成する
    /// @param pMesh pMesh に渡す値
    /// @return 処理結果
    static ModelPreviewRT CreateModelPreviewRT(CFbxMesh* pMesh);

    /// Model Previews を解放する
    void ReleaseModelPreviews();
};
