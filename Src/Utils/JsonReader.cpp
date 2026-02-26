#include "JsonReader.h"

#include "../Common/Object3D.h"
#include <fstream>
#include <nlohmann/json.hpp>


//
// using json = nlohmann::json;
// // JSONファイルを読み込み、記録されたモデルをステージに復元する
// void JsonReader::ImportFromFile(const std::string& path)
// {
//     // ファイルを開く
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
//         // JSON から Transform を復元
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