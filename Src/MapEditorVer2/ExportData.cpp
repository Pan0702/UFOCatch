#include "ExportData.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include "StageData.h"

using json = nlohmann::json;

// 繝｢繝・Ν蜷阪→Transform繧谷SON繧ｪ繝悶ず繧ｧ繧ｯ繝医↓螟画鋤縺励※霑斐☆
json ExportData::TransformToJson(const std::string& modelName, const Transform& transform)
{
    json j;
    const char* p = ResourceManager::GetPath(modelName.c_str());
    j["path"]       = p;
    j["model_name"] = modelName;

    // position / rotation / scale 縺昴ｌ縺槭ｌ繧・x,y,z 縺ｮ繧ｪ繝悶ず繧ｧ繧ｯ繝医→縺励※譬ｼ邏・
    j["transform"]["position"] = { {"x", transform.position.x}, {"y", transform.position.y}, {"z", transform.position.z} };
    j["transform"]["rotation"] = { {"x", transform.rotation.x}, {"y", transform.rotation.y}, {"z", transform.rotation.z} };
    j["transform"]["scale"]    = { {"x", transform.scale.x},    {"y", transform.scale.y},    {"z", transform.scale.z} };

    return j;
}

// 繧ｹ繝・・繧ｸ荳翫・蜈ｨ繧ｪ繝悶ず繧ｧ繧ｯ繝医ｒJSON驟榊・縺ｫ繧ｷ繝ｪ繧｢繝ｩ繧､繧ｺ縺励※繝輔ぃ繧､繝ｫ縺ｫ譖ｸ縺榊・縺・
void ExportData::ExportAllModels(const std::string& file_name,
                                 const std::vector<StageDataInfo>& model_list)
{
    json root = json::array(); // 蜈ｨ菴薙ｒ驟榊・縺ｨ縺励※螳夂ｾｩ

    for (const auto& item : model_list)
    {
        root.push_back(TransformToJson(item.modelName, item.transform));
    }
    
    std::ofstream file(file_name);
    if (file.is_open())
    {
        // JSON 繝繝ｳ繝玲凾縺ｮ繧､繝ｳ繝・Φ繝亥ｹ・ｼ医せ繝壹・繧ｹ謨ｰ・・
        constexpr int  kJsonIndent = 4;
        file << root.dump(kJsonIndent); // 繧､繝ｳ繝・Φ繝井ｻ倥″縺ｧ隕九ｄ縺吶￥菫晏ｭ・
    }
    MessageBox(nullptr, _T("Export Success"), nullptr, MB_OK);
}

