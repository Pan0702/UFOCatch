#pragma once
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

#include "StageData.h"
class Transform;

namespace ExportData
{
    
    /// <summary>単一オブジェクトのTransformをJSONファイルにエクスポートする</summary>
    /// <param name="name">ファイル名（拡張子なし）</param>
    /// <param name="t">エクスポートするtransform</param>
    /// <returns>成功でtrue</returns>
    //bool Export(const std::string& name, const Transform& t);

    /// <summary>モデル名とTransformをJSONオブジェクトに変換して返す</summary>
    /// <param name="modelName">モデルの名前</param>
    /// <param name="transform">変換するTransform</param>
    /// <returns>変換後のJSONオブジェクト</returns>
    static nlohmann::json TransformToJson(const std::string& modelName, const Transform& transform);

    /// <summary>ステージ上の全オブジェクトをJSON配列にシリアライズしてファイルに書き出す</summary>
    /// <param name="file_name">出力ファイル名（拡張子なし）</param>
    /// <param name="modelList">エクスポートするオブジェクトのリスト</param>
    static void ExportAllModels(const std::string& file_name, const std::vector<StageDataInfo>& modelList);
};

