#include "ExportData.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include "StageData.h"

using json = nlohmann::json;

// モデル名とTransformをJSONオブジェクトに変換して返す
json ExportData::TransformToJson(const std::string& modelName, const Transform& transform)
{
    json j;
    const char* p = ResourceManager::GetPath(modelName.c_str());
    j["path"]       = p;
    j["model_name"] = modelName;

    // position / rotation / scale それぞれを x,y,z のオブジェクトとして格納
    j["transform"]["position"] = { {"x", transform.position.x}, {"y", transform.position.y}, {"z", transform.position.z} };
    j["transform"]["rotation"] = { {"x", transform.rotation.x}, {"y", transform.rotation.y}, {"z", transform.rotation.z} };
    j["transform"]["scale"]    = { {"x", transform.scale.x},    {"y", transform.scale.y},    {"z", transform.scale.z} };

    return j;
}

// ステージ上の全オブジェクトをJSON配列にシリアライズしてファイルに書き出す
void ExportData::ExportAllModels(const std::string& file_name,
                                 const std::vector<StageDataInfo>& model_list)
{
    json root = json::array();// 全体を配列として定義

    for (const auto& item : model_list)
    {
        root.push_back(TransformToJson(item.modelName, item.transform));
    }
    
    std::ofstream file(file_name);
    if (file.is_open())
    {
        // JSON ダンプ時のインデント幅（スペース数）
        constexpr int  kJsonIndent = 4;
        file << root.dump(kJsonIndent);// インデント付きで保存
    }
    MessageBox(nullptr, _T("Export Success"), nullptr, MB_OK);
}

