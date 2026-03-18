#pragma once
#include "../Common/Object3D.h"

/// <summary>
/// 繝ｯ繝ｼ繝ｫ繝臥ｩｺ髢謎ｸ翫・繝ｬ繧､・亥濠逶ｴ邱夲ｼ・
/// </summary>
struct Ray
{
    VECTOR3 origin;    // 蟋狗せ・医き繝｡繝ｩ菴咲ｽｮ・・
    VECTOR3 direction; // 豁｣隕丞喧貂医∩譁ｹ蜷代・繧ｯ繝医Ν
};

/// <summary>
/// 繝槭え繧ｹ繧ｫ繝ｼ繧ｽ繝ｫ縺九ｉ繝ｬ繧､繧堤函謌舌＠縲√が繝悶ず繧ｧ繧ｯ繝医→縺ｮ蠖薙◆繧雁愛螳壹ｒ陦後≧繝ｦ繝ｼ繝・ぅ繝ｪ繝・ぅ繧ｯ繝ｩ繧ｹ
/// </summary>
class MouseRay
{
public:
    /// <summary>
    /// 迴ｾ蝨ｨ縺ｮ繝槭え繧ｹ繧ｫ繝ｼ繧ｽ繝ｫ菴咲ｽｮ縺九ｉ繝ｯ繝ｼ繝ｫ繝臥ｩｺ髢薙・繝ｬ繧､繧堤函謌舌☆繧・
    /// </summary>
    /// <returns>逕滓・縺輔ｌ縺欒ay</returns>
    static Ray Create();

    /// <summary>
    /// 繝ｬ繧､縺ｨObject3D縺ｮ繝｡繝・す繝･縺ｮ蠖薙◆繧雁愛螳・
    /// </summary>
    /// <param name="ray">蛻､螳壹☆繧九Ξ繧､</param>
    /// <param name="obj">蛻､螳壼ｯｾ雎｡縺ｮObject3D</param>
    /// <param name="collOut">陦晉ｪ∵ュ蝣ｱ縺ｮ蜃ｺ蜉帛・・井ｸ崎ｦ√↑蝣ｴ蜷医・nullptr・・/param>
    /// <param name="rayLength">繝ｬ繧､縺ｮ髟ｷ縺包ｼ医ョ繝輔か繝ｫ繝・000・・/param>
    /// <returns>蠖薙◆縺｣縺ｦ縺・ｌ縺ｰtrue</returns>
    static bool HitTest(const Ray& ray, Object3D* obj,
                        MeshCollider::CollInfo* collOut = nullptr,
                        float rayLength = 1000.0f);

    /// <summary>
    /// 繝ｬ繧､縺ｨ逅・・蠖薙◆繧雁愛螳・
    /// </summary>
    /// <param name="ray">蛻､螳壹☆繧九Ξ繧､</param>
    /// <param name="sphere">蛻､螳壼ｯｾ雎｡縺ｮ逅・さ繝ｩ繧､繝繝ｼ</param>
    /// <returns>蠖薙◆縺｣縺ｦ縺・ｌ縺ｰtrue</returns>
    static bool HitTestSphere(const Ray& ray, const SphereCollider& sphere);
};
