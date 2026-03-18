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
    /// 謖・ｮ壹ヱ繧ｹ縺ｮ繝｡繝・す繝･繧偵Ο繝ｼ繝峨＠縲√・繧ｿ繝ｳ繝ｪ繧ｹ繝医→繝｢繝・Ν繧ｹ繝医Ξ繝ｼ繧ｸ縺ｫ逋ｻ骭ｲ縺吶ｋ
    /// </summary>
    /// <param name="path">繝ｭ繝ｼ繝峨☆繧九Γ繝・す繝･繝輔ぃ繧､繝ｫ縺ｮ繝代せ</param>
    void CreateModel(const std::string& path);

    /// <summary>
    /// FBX 繝輔ぃ繧､繝ｫ繧・.mesh 縺ｫ螟画鋤縺励※縺九ｉ繝ｭ繝ｼ繝峨☆繧九・
    /// 蜃ｺ蜉帛・縺ｯ FBX 縺ｨ蜷後§繝輔か繝ｫ繝繝ｻ蜷悟錐縺ｮ .mesh 繝輔ぃ繧､繝ｫ縲・
    /// </summary>
    /// <param name="fbxPath">螟画鋤縺吶ｋ FBX 繝輔ぃ繧､繝ｫ縺ｮ繝代せ</param>
    void ConvertAndLoad(const std::string& fbxPath);

};

