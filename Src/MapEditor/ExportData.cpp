#include "ExportData.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <Windows.h>

#include "Import.h"
#include "StageData.h"
#include "../Stage/StageCollision.h"

using json = nlohmann::json;

namespace
{
    // Windows ANSI(CP932/Shift-JIS) → UTF-8 変換
    // nlohmann::json の dump() はUTF-8を要求するため必要
    std::string ToUtf8(const std::string& ansi)
    {
        int wlen = MultiByteToWideChar(CP_ACP, 0, ansi.c_str(), -1, nullptr, 0);
        if (wlen <= 0) return ansi;
        std::wstring wstr(wlen, L'\0');
        MultiByteToWideChar(CP_ACP, 0, ansi.c_str(), -1, wstr.data(), wlen);
        int len = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
        if (len <= 0) return ansi;
        std::string utf8(len, '\0');
        WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, utf8.data(), len, nullptr, nullptr);
        utf8.resize(utf8.size() - 1); // null終端を除く
        return utf8;
    }
}

// モデル名とTransformをJSONオブジェクトに変換して返す
json ExportData::TransformToJson(const std::string& modelName, const Transform& transform, const StageColl& c)
{
    json j;
    const char* p = ResourceManager::GetPath(modelName.c_str());
    j["path"] = ToUtf8(p ? p : "");
    j["model_name"] = ToUtf8(modelName);

    // position / rotation / scale それぞれを x,y,z のオブジェクトとして格納
    j["transform"]["position"] = {
        {"x", transform.position.x}, {"y", transform.position.y}, {"z", transform.position.z}
    };
    j["transform"]["rotation"] = {
        {"x", transform.rotation.x}, {"y", transform.rotation.y}, {"z", transform.rotation.z}
    };
    j["transform"]["scale"] = {{"x", transform.scale.x}, {"y", transform.scale.y}, {"z", transform.scale.z}};

    j["useOBB"] = c.useOBB;
    j["useHitGround"] = c.useHitGround;
    return j;
}

// ステージ上の全オブジェクトをJSON配列にシリアライズしてファイルに書き出す
void ExportData::AllModelsInfo(const std::string& fileName,
                               const std::vector<StageDataInfo>& modelList)
{
    json root = json::array(); // 全体を配列として定義

    for (const auto& item : modelList)
    {
        root.push_back(TransformToJson(item.modelName, item.transform, item.c));
    }

    std::ofstream file(fileName);
    if (file.is_open())
    {
        // JSON ダンプ時のインデント幅（スペース数）
        constexpr int kJsonIndent = 4;
        file << root.dump(kJsonIndent); // インデント付きで保存
    }
}

void ExportData::AllModelsPath(const std::string& fileName)
{
    json root = json::array();
    std::vector<std::string> models = ResourceManager::GetAllModelPath();
    for (auto model : models)
    {
        json item;
        item["path"] = ToUtf8(model);
        root.push_back(item);
    }

    std::ofstream file(fileName);
    if (file.is_open())
    {
        constexpr int kJsonIndent = 4;
        file << root.dump(kJsonIndent);
    }
}
