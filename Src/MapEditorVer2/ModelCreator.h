#pragma once
#include <vector>

#include "../Utils/FbxMesh.h"

class ModelCreator
{
private:
    struct ModelInfo
    {
        std::string name;
        CFbxMesh* mesh;
    };
    std::vector<ModelInfo> m_models;
public:
    /// <summary>
    /// 指定パスのメッシュをロードし、ボタンリストとモデルストレージに登録する
    /// </summary>
    /// <param name="path">ロードするメッシュファイルのパス</param>
    void CreateModel(const std::string& path);

    /// <summary>
    /// FBX ファイルを .mesh に変換してからロードする。
    /// 出力先は FBX と同じフォルダ・同名の .mesh ファイル。
    /// </summary>
    /// <param name="fbxPath">変換する FBX ファイルのパス</param>
    void ConvertAndLoad(const std::string& fbxPath);

};
