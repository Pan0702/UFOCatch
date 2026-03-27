#pragma once
#include <iosfwd>
#include <string>
#include <vector>
#include "../Common/Object3D.h"
struct Info
{
    std::string modelName;
    std::string modelPath;
    Transform transform;
};
namespace Import
{
    /// <summary>
    /// JSONファイルを読み込み、記録されたモデルをステージに復元する。
    /// 未ロードのモデルは自動的にロードしてボタンにも追加する。
    /// </summary>
    /// <param name="path">インポートするJSONファイルのパス</param>
    std::vector<Info> ImportFromFile(const std::string& path);
};
