#include "Import.h"

#include <fstream>
#include <nlohmann/json.hpp>

#include "StageData.h"
#include "Buttom.h"

using json = nlohmann::json;


// JSONファイルを読み込み、記録されたモデルをステージに復元する
std::vector<Info> Import::ImportFromFile(const std::string& path)
{
    // ファイルを開く
    std::ifstream file(path);
    if (!file.is_open()) return {};

    json root;
    file >> root;

    std::vector<Info> infos;
    for (const auto& item : root)
    {
        Info info{};
        info.modelName = item["model_name"];
        info.modelPath = item["path"];


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
        info.transform = transform;
        infos.push_back(info);
    }
    return infos;
}
