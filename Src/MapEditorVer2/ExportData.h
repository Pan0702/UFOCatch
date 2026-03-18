#pragma once
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

#include "StageData.h"
class Transform;

class ExportData
{
public:
    /// <summary>蜊倅ｸ繧ｪ繝悶ず繧ｧ繧ｯ繝医・Transform繧谷SON繝輔ぃ繧､繝ｫ縺ｫ繧ｨ繧ｯ繧ｹ繝昴・繝医☆繧・/summary>
    /// <param name="name">繝輔ぃ繧､繝ｫ蜷搾ｼ域僑蠑ｵ蟄舌↑縺暦ｼ・/param>
    /// <param name="t">繧ｨ繧ｯ繧ｹ繝昴・繝医☆繧亀ransform</param>
    /// <returns>謌仙粥縺ｧtrue</returns>
    bool Export(const std::string& name, const Transform& t);

    /// <summary>繝｢繝・Ν蜷阪→Transform繧谷SON繧ｪ繝悶ず繧ｧ繧ｯ繝医↓螟画鋤縺励※霑斐☆</summary>
    /// <param name="modelName">繝｢繝・Ν縺ｮ蜷榊燕</param>
    /// <param name="transform">螟画鋤縺吶ｋTransform</param>
    /// <returns>螟画鋤蠕後・JSON繧ｪ繝悶ず繧ｧ繧ｯ繝・/returns>
    static nlohmann::json TransformToJson(const std::string& modelName, const Transform& transform);

    /// <summary>繧ｹ繝・・繧ｸ荳翫・蜈ｨ繧ｪ繝悶ず繧ｧ繧ｯ繝医ｒJSON驟榊・縺ｫ繧ｷ繝ｪ繧｢繝ｩ繧､繧ｺ縺励※繝輔ぃ繧､繝ｫ縺ｫ譖ｸ縺榊・縺・/summary>
    /// <param name="file_name">蜃ｺ蜉帙ヵ繧｡繧､繝ｫ蜷搾ｼ域僑蠑ｵ蟄舌↑縺暦ｼ・/param>
    /// <param name="modelList">繧ｨ繧ｯ繧ｹ繝昴・繝医☆繧九が繝悶ず繧ｧ繧ｯ繝医・繝ｪ繧ｹ繝・/param>
    static void ExportAllModels(const std::string& file_name, const std::vector<StageDataInfo>& modelList);
};

