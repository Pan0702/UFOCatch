#include "Import.h"

#include <filesystem>
#include <fstream>
#include <Windows.h>
#include <nlohmann/json.hpp>

#include "StageData.h"
#include "Buttom.h"
#include "../Utils/MyLib.h"

using json = nlohmann::json;

namespace
{
    std::filesystem::path ToFilePath(const std::string& utf8Path)
    {
        if (utf8Path.empty()) return {};

        const int wide_size = MultiByteToWideChar(CP_UTF8, 0, utf8Path.c_str(), -1, nullptr, 0);
        if (wide_size <= 0) return std::filesystem::path(utf8Path);

        std::wstring wide_path(wide_size, L'\0');
        MultiByteToWideChar(CP_UTF8, 0, utf8Path.c_str(), -1, wide_path.data(), wide_size);
        wide_path.resize(wide_path.size() - 1);
        return std::filesystem::path(wide_path);
    }
}

// JSONファイルを読み込み、記録されたモデルをステージに復元する
std::vector<Info> Import::StageInfo(const std::string& path)
{
    // ファイルを開く
    std::ifstream file(ToFilePath(path));
    if (!file.is_open()) return {};

    json root;
    file >> root;
    if (!root.is_array()) return {};

    std::vector<Info> infos;
    for (const auto& item : root)
    {
        if (!item.is_object()) continue;
        if (!item.contains("path") || !item.contains("transform")) continue;

        Info info{};
        info.modelName = item.value("model_name", "");
        info.modelPath = item.value("path", "");
        if (info.modelName.empty() && !info.modelPath.empty())
        {
            info.modelName = MyLib::ChangePathToName(info.modelPath);
        }


        // JSON から Transformを読み込む
        const auto& transformJson = item["transform"];
        if (!transformJson.contains("position") || !transformJson.contains("rotation") ||
            !transformJson.contains("scale"))
        {
            continue;
        }

        Transform transform;
        transform.position = VECTOR3(
            transformJson["position"].value("x", 0.0f),
            transformJson["position"].value("y", 0.0f),
            transformJson["position"].value("z", 0.0f));
        transform.rotation = VECTOR3(
            transformJson["rotation"].value("x", 0.0f),
            transformJson["rotation"].value("y", 0.0f),
            transformJson["rotation"].value("z", 0.0f));
        transform.scale = VECTOR3(
            transformJson["scale"].value("x", 1.0f),
            transformJson["scale"].value("y", 1.0f),
            transformJson["scale"].value("z", 1.0f));
        info.transform = transform;
        info.soc.useOBB = item.contains("useOBB")
                              ? item["useOBB"].get<bool>()
                              : true;

        info.soc.useHitGround = item.contains("useHitGround")
                                    ? item["useHitGround"].get<bool>()
                                    : false;
        infos.push_back(info);
    }
    return infos;
}

std::vector<std::string> Import::ModelPath(const std::string& path)
{
    std::vector<std::string> paths;

    std::ifstream file(ToFilePath(path));
    if (!file.is_open()) return {};
    json root;
    file >> root;

    for (const auto& item : root)
    {
        if (item.contains("path"))
            paths.push_back(item["path"]);
    }

    return paths;
}
