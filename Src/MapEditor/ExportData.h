#pragma once
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include "../Stage/StageCollision.h"
#include "StageData.h"

/// <summary>ステージエディタで使う Transform の情報と処理をまとめる型</summary>
class Transform;

namespace ExportData
{
    /// 単一オブジェクトのTransformをJSONファイルにエクスポートする
    /// @param name ファイル名（拡張子なし）
    /// @param t エクスポートするtransform
    /// @return 成功でtrue
    //bool Export(const std::string& name, const Transform& t);

    /// Transform To Json を返す
    /// @param modelName モデル名
    /// @param transform transform に渡す値
    /// @param c c に渡す値
    /// @return 処理結果
    nlohmann::json TransformToJson(const std::string& modelName, const Transform& transform, const StageColl& c);

    /// All Models Info の処理を行う
    /// @param fileName ファイル名
    /// @param modelList modelList に渡す値
    void AllModelsInfo(const std::string& fileName, const std::vector<StageDataInfo>& modelList);

    /// All Models Path の処理を行う
    /// @param fileName ファイル名
    void AllModelsPath(const std::string& fileName);
};
