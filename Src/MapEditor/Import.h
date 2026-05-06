#pragma once
#include <iosfwd>
#include <string>
#include <vector>
#include "../Common/Object3D.h"
#include "../Stage/StageCollision.h"

struct Info
{
    std::string modelName;
    std::string modelPath;
    Transform transform;
    StageColl soc;
};

namespace Import
{
    /// <summary>
    /// JSONファイルを読み込み、記録されたモデルをステージに復元する。
    /// 未ロードのモデルは自動的にロードしてボタンにも追加する。
    /// </summary>
    /// <param name="path">インポートするJSONファイルのパス</param>
    std::vector<Info> StageInfo(const std::string& path);

    ///<summary>JSONファイルからモデルのパスを読み込む</summary>
    ///<param name="path">インポートするJSONファイルのパス</param>
    std::vector<std::string> ModelPath(const std::string& path);
};
