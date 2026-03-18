#pragma once
#include "../../Common/Object3D.h"
#include "../MouseRay.h"
#include <d3d11.h>

enum class Axis : uint8_t { None, X, Y, Z };

class TRSBase : public Object3D
{
protected:
    struct Info
    {
        std::unique_ptr<CFbxMesh> mesh;
        std::unique_ptr<MeshCollider> coll;
    };

    Info xInfo;
    Info yInfo;
    Info zInfo;

private:
    ID3D11DepthStencilState* m_pDepthOffState = nullptr;

public:
    /// <summary>繧ｮ繧ｺ繝｢縺ｮ陦ｨ遉ｺ菴咲ｽｮ繧呈欠螳壼ｺｧ讓吶↓險ｭ螳壹☆繧・/summary>
    /// <param name="pos">險ｭ螳壹☆繧九Ρ繝ｼ繝ｫ繝牙ｺｧ讓・/param>
    virtual void SetPosition(const VECTOR3& pos);

    /// <summary>繧ｫ繝｡繝ｩ霍晞屬縺ｫ豈比ｾ九＠縺ｦ繧ｮ繧ｺ繝｢縺ｮ繧ｹ繧ｱ繝ｼ繝ｫ繧呈峩譁ｰ縺励∝ｸｸ縺ｫ蜷後§隕九°縺代し繧､繧ｺ縺ｫ縺吶ｋ</summary>
    /// <param name="camPos">繧ｫ繝｡繝ｩ縺ｮ繝ｯ繝ｼ繝ｫ繝牙ｺｧ讓・/param>
    void UpdateScaleByCamera(const VECTOR3& camPos);

public:
    TRSBase();
    ~TRSBase();

    /// <summary>X/Y/Z霆ｸ縺ｮ繧ｮ繧ｺ繝｢繝｡繝・す繝･繧偵☆縺ｹ縺ｦ謠冗判縺吶ｋ</summary>
    virtual void Render();
    

    /// <summary>繝ｬ繧､縺ｨX/Y/Z繧ｮ繧ｺ繝｢縺ｮ繧ｳ繝ｩ繧､繝繝ｼ繧貞愛螳壹＠縲∝ｽ薙◆縺｣縺溯ｻｸ繧定ｿ斐☆</summary>
    /// <param name="ray">蛻､螳壹↓菴ｿ逕ｨ縺吶ｋ繝ｬ繧､</param>
    /// <param name="length">繝ｬ繧､縺ｮ髟ｷ縺包ｼ医ョ繝輔か繝ｫ繝・000・・/param>
    /// <returns>蠖薙◆縺｣縺溯ｻｸ・・/Y/Z・峨∝ｽ薙◆繧峨↑縺代ｌ縺ｰNone</returns>
    Axis RayHitTest(const Ray& ray, float length = 1000.0f);
};

