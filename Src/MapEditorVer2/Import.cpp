#include "Import.h"

#include <fstream>
#include <nlohmann/json.hpp>

#include "StageData.h"
#include "Buttom.h"

using json = nlohmann::json;

// JSON繝輔ぃ繧､繝ｫ繧定ｪｭ縺ｿ霎ｼ縺ｿ縲∬ｨ倬鹸縺輔ｌ縺溘Δ繝・Ν繧偵せ繝・・繧ｸ縺ｫ蠕ｩ蜈・☆繧・
void Import::ImportFromFile(const std::string& path)
{
    // 繝輔ぃ繧､繝ｫ繧帝幕縺・
    std::ifstream file(path);
    if (!file.is_open()) return;

    json root;
    file >> root;

    auto* stage_data = ObjectManager::FindGameObject<StageData>();
    auto* button     = ObjectManager::FindGameObject<Button>();

    if (!stage_data ) return;

    for (const auto& item : root)
    {
        std::string modelName = item["model_name"];
        std::string modelPath = item["path"];

        // 繝｢繝・Ν縺後せ繝医Ξ繝ｼ繧ｸ縺ｫ譛ｪ逋ｻ骭ｲ縺ｪ繧芽ｪｭ縺ｿ霎ｼ繧薙〒繝懊ち繝ｳ縺ｫ繧りｿｽ蜉縺吶ｋ
        if (ResourceManager::GetModel(modelName.c_str()) == nullptr)
        {
            ResourceManager::LoadFbx(modelName.c_str(), modelPath.c_str());
            if (button)
            {
                button->AddButton(modelName, ResourceManager::GetModel(modelName.c_str()));
            }
        }

        // JSON 縺九ｉ Transform 繧貞ｾｩ蜈・
        Transform transform;
        transform.position = VECTOR3(
            item["transform"]["position"]["x"],
            item["transform"]["position"]["y"],
            item["transform"]["position"]["z"]);
        transform.rotation = VECTOR3(
            item["transform"]["rotation"]["x"],
            item["transform"]["rotation"]["y"],
            item["transform"]["rotation"]["z"]);
        transform.scale = VECTOR3(
            item["transform"]["scale"]["x"],
            item["transform"]["scale"]["y"],
            item["transform"]["scale"]["z"]);

        stage_data->AddModelWithTransform(modelName, transform);
    }
}
