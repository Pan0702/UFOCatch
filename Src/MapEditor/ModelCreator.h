#pragma once
#include <vector>

#include "../Utils/FbxMesh.h"


///Object3Dを継承しないクラスとして定義
class ModelCreator
{
private:
    /// <summary>ステージエディタで使う Model Info の情報と処理をまとめる型</summary>
    struct ModelInfo
    {
        std::string name;
        CFbxMesh* mesh;
    };

    std::vector<ModelInfo> m_models;

public:
    /// Model を作成する
    /// @param path パス
    void CreateModel(const std::string& path);

    /// Convert And Load の処理を行う
    /// @param fbxPath パス
    void ConvertAndLoad(const std::string& fbxPath);
};
