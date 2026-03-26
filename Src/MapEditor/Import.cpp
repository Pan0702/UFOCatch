#include "Import.h"

#include <fstream>
#include <nlohmann/json.hpp>

#include "StageData.h"
#include "Buttom.h"

using json = nlohmann::json;

// JSONファイルを読み込み、記録されたモデルをステージに復元する
void Import::ImportFromFile(const std::string& path)
{
    // ファイルを開く
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

        // モデルが未ロードの場合、自動的にロードしてボタンにも追加する
        if (ResourceManager::GetModel(modelName.c_str()) == nullptr)
        {
            ResourceManager::LoadFbx(modelName.c_str(), modelPath.c_str());
            if (button)
            {
                button->AddButton(modelName, ResourceManager::GetModel(modelName.c_str()));
            }
        }

        // JSON から Transformを読み込む
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
