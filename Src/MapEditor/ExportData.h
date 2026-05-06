#pragma once
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include "../Stage/StageCollision.h"
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
    nlohmann::json TransformToJson(const std::string& modelName, const Transform& transform, const StageColl& c);

    /// <summary>ステージ上の全オブジェクトをJSON配列にシリアライズしてファイルに書き出す</summary>
    /// <param name="fileName">出力ファイル名（拡張子なし）</param>
    /// <param name="modelList">エクスポートするオブジェクトのリスト</param>
    void AllModelsInfo(const std::string& fileName, const std::vector<StageDataInfo>& modelList);

    ///<summary>ステージ上の全オブジェクトのパスをJSON配列にシリアライズしてファイルに書き出す</summary>
    ///<param name="fileName">出力ファイル名（拡張子なし）</param>
    void AllModelsPath(const std::string& fileName);
};
