#pragma once
#include <unordered_map>

#include "GridDraw.h"
#include "ModelCreator.h"
#include "StageData.h"
#include "../Common/Object3D.h"

static constexpr int PREVIEW_SIZE = 128;

// 繝｢繝・Ν繝励Ξ繝薙Η繝ｼ逕ｨ繝ｬ繝ｳ繝繝ｼ繧ｿ繝ｼ繧ｲ繝・ヨ・・3D繝ｪ繧ｽ繝ｼ繧ｹ縺ｮ縺ｿ謇譛峨√Γ繝・す繝･縺ｯ謇譛峨＠縺ｪ縺・ｼ・
struct ModelPreviewRT
{
    ID3D11RenderTargetView*   pRTV = nullptr;
    ID3D11DepthStencilView*   pDSV = nullptr;
    ID3D11ShaderResourceView* pSRV = nullptr; // ImGui縺ｫ貂｡縺・

    /// <summary>菫晄戟縺励※縺・ｋD3D繝ｪ繧ｽ繝ｼ繧ｹ繧偵☆縺ｹ縺ｦ隗｣謾ｾ縺吶ｋ</summary>
    void Release()
    {
        if (pRTV) { pRTV->Release(); pRTV = nullptr; }
        if (pDSV) { pDSV->Release(); pDSV = nullptr; }
        if (pSRV) { pSRV->Release(); pSRV = nullptr; }
    }
};

// 繝懊ち繝ｳ諠・ｱ繧堤ｮ｡逅・☆繧区ｧ矩菴・
struct ImageButtonData
{
    std::string buttonID;
    ID3D11ShaderResourceView* pTexture;
    ImVec2 size;
    int state;
    bool isLoaded;
    bool isRenderTexture; // true縺ｮ蝣ｴ蜷・eleaseAllTextures縺ｧ隗｣謾ｾ縺励↑縺・

    ImageButtonData(const std::string& id, const ImVec2& buttonSize, int buttonState = 0)
        : buttonID(id), size(buttonSize), state(buttonState), pTexture(nullptr), isLoaded(false), isRenderTexture(false)
    {
    }
};

class Button : public Object3D
{

private:
    VECTOR3 m_worldPosition;
    std::vector<ImageButtonData> m_imageButtons;
    std::vector<ModelPreviewRT>  m_modelpreviews;
    ModelCreator* m_pModelCreator;
    GridDraw* m_pGridDraw;
    StageData* m_pData;
    int m_pSelectId;
    

public:
    Button();
    ~Button();
    void Update() override;

    /// <summary>謖・ｮ唔D縺ｮ繝懊ち繝ｳ繧偵Μ繧ｹ繝医↓霑ｽ蜉縺吶ｋ縲ゅΓ繝・す繝･縺梧ｸ｡縺輔ｌ縺溷ｴ蜷医・繝励Ξ繝薙Η繝ｼ繝・け繧ｹ繝√Ε繧堤函謌舌☆繧・/summary>
    /// <param name="buttonId">繝懊ち繝ｳ縺ｮ隴伜挨ID・医Δ繝・Ν蜷搾ｼ・/param>
    /// <param name="pMesh">繝励Ξ繝薙Η繝ｼ逕ｨ繝｡繝・す繝･・育怐逡･蜿ｯ・・/param>
    /// <param name="size">繝懊ち繝ｳ繧ｵ繧､繧ｺ・医ョ繝輔か繝ｫ繝・4x64・・/param>
    void AddButton(const std::string& buttonId, CFbxMesh* pMesh = nullptr,const ImVec2& size = ImVec2(64, 64));

private:
    /// <summary>isRenderTexture縺ｧ縺ｪ縺・・繧ｿ繝ｳ縺ｮ繝・け繧ｹ繝√Ε繧偵☆縺ｹ縺ｦ隗｣謾ｾ縺励※繝ｪ繧ｹ繝医ｒ繧ｯ繝ｪ繧｢縺吶ｋ</summary>
    void ReleaseAllTextures();

    /// <summary>ImageButtonData縺ｮ蜀・ｮｹ縺ｫ蠢懊§縺ｦImGui縺ｮ繝懊ち繝ｳ繧・縺､謠冗判縺吶ｋ</summary>
    /// <param name="buttonData">謠冗判縺吶ｋ繝懊ち繝ｳ縺ｮ繝・・繧ｿ</param>
    static void CreateImageButton(const ImageButtonData& buttonData);

    /// <summary>繝懊ち繝ｳ縺後け繝ｪ繝・け縺輔ｌ縺溘→縺阪∝ｯｾ蠢懊Δ繝・Ν繧偵せ繝・・繧ｸ縺ｫ霑ｽ蜉縺吶ｋ</summary>
    /// <param name="buttonId">繧ｯ繝ｪ繝・け縺輔ｌ縺溘・繧ｿ繝ｳ縺ｮID</param>
    static void HandleButtonClick(const std::string& buttonId);

    /// <summary>繧ｨ繝・ぅ繧ｿUI縺ｮImGui繧ｦ繧｣繝ｳ繝峨え・医Δ繝・Ν霑ｽ蜉繝ｻ繧ｨ繧ｯ繧ｹ繝昴・繝医・繧ｰ繝ｪ繝・ラ險ｭ螳夲ｼ峨ｒ謠冗判縺吶ｋ</summary>
    void DebugImGui();

    // pMesh 縺ｯ CModelStorage 縺梧園譛峨☆繧九・縺ｧ delete 縺励↑縺・
    /// <summary>
    /// 繝｡繝・す繝･繧偵が繝輔せ繧ｯ繝ｪ繝ｼ繝ｳ縺ｫ1蝗樊緒逕ｻ縺励！mGui繝懊ち繝ｳ逕ｨ縺ｮSRV繧定ｿ斐☆
    /// 繝｡繝・す繝･縺ｮ謇譛画ｨｩ縺ｯCModelStorage縺梧戟縺､縺溘ａ縲√％縺薙〒縺ｯdelete縺励↑縺・
    /// </summary>
    /// <param name="pMesh">謠冗判縺吶ｋ繝｡繝・す繝･</param>
    /// <returns>逕滓・縺励◆繝ｬ繝ｳ繝繝ｼ繧ｿ繝ｼ繧ｲ繝・ヨ諠・ｱ</returns>
    static ModelPreviewRT CreateModelPreviewRT(CFbxMesh* pMesh);

    /// <summary>蜈ｨ繝｢繝・Ν繝励Ξ繝薙Η繝ｼ縺ｮD3D繝ｪ繧ｽ繝ｼ繧ｹ繧定ｧ｣謾ｾ縺励※繝ｪ繧ｹ繝医ｒ繧ｯ繝ｪ繧｢縺吶ｋ</summary>
    void ReleaseModelPreviews();

    void DrawHierarchy();

};

