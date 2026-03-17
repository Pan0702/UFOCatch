#include "JsonReader.h"

#include "../Common/Object3D.h"
#include <fstream>
#include <nlohmann/json.hpp>


//
// using json = nlohmann::json;
// // JSON繝輔ぃ繧､繝ｫ繧定ｪｭ縺ｿ霎ｼ縺ｿ縲∬ｨ倬鹸縺輔ｌ縺溘Δ繝・Ν繧偵せ繝・・繧ｸ縺ｫ蠕ｩ蜈・☆繧・
// void JsonReader::ImportFromFile(const std::string& path)
// {
//     // 繝輔ぃ繧､繝ｫ繧帝幕縺・
//     std::ifstream file(path);
//     if (!file.is_open()) return;
//
//     json root;
//     file >> root;
//     
//     
//
//     for (const auto& item : root)
//     {
//         std::string model_name = item["model_name"];
//         std::string model_path = item["path"];
//         
//
//         // JSON 縺九ｉ Transform 繧貞ｾｩ蜈・
//         Transform transform;
//         transform.position = VECTOR3(
//             item["transform"]["position"]["x"],
//             item["transform"]["position"]["y"],
//             item["transform"]["position"]["z"]);
//         transform.rotation = VECTOR3(
//             item["transform"]["rotation"]["x"],
//             item["transform"]["rotation"]["y"],
//             item["transform"]["rotation"]["z"]);
//         transform.scale = VECTOR3(
//             item["transform"]["scale"]["x"],
//             item["transform"]["scale"]["y"],
//             item["transform"]["scale"]["z"]);
//
//         stage_data->AddModelWithTransform(model_name, transform);
//     }
// }