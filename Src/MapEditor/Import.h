#pragma once
#include <iosfwd>
#include <string>
#include <vector>
#include "../Common/Object3D.h"
#include "../Stage/StageCollision.h"

/// <summary>ステージエディタで使う Info の情報と処理をまとめる型</summary>
struct Info
{
    std::string modelName;
    std::string modelPath;
    Transform transform;
    StageColl soc;
};

namespace Import
{
    /// Stage Info を返す
    /// @param path パス
    /// @return 取得した要素一覧
    std::vector<Info> StageInfo(const std::string& path);

    /// Model Path を返す
    /// @param path パス
    /// @return 取得した要素一覧
    std::vector<std::string> ModelPath(const std::string& path);
};
