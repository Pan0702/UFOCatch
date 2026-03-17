#include "Import.h"

#include <fstream>
#include <nlohmann/json.hpp>

#include "StageData.h"
#include "Buttom.h"
#include "../ModelStorage.h"

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
    auto* storage    = ObjectManager::FindGameObject<CModelStorage>();
    auto* button     = ObjectManager::FindGameObject<Button>();

    if (!stage_data || !storage) return;

    for (const auto& item : root)
    {
        std::string model_name = item["model_name"];
        std::string model_path = item["path"];

        // モデルがストレージに未登録なら読み込んでボタンにも追加する
        if (storage->GetModel(model_name) == nullptr)
        {
            storage->AddModel(model_name.c_str(), model_path.c_str());
            if (button)
            {
                button->AddButton(model_name, storage->GetModel(model_name));
            }
        }

        // JSON から Transform を復元
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

        stage_data->AddModelWithTransform(model_name, transform);
    }
}