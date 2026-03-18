#pragma once
#include <cstdint>
#include <string>
#include <vector>

#include "FbxParser.h"

// 繝｡繝・す繝･繝・・繧ｿ繧・.mesh 繝輔か繝ｼ繝槭ャ繝医〒繝輔ぃ繧､繝ｫ縺ｫ譖ｸ縺榊・縺吶け繝ｩ繧ｹ
class MeshWriter
{
public:
    /// <summary>
    /// 鬆らせ繝ｻ繧､繝ｳ繝・ャ繧ｯ繧ｹ繝・・繧ｿ繧・.mesh 繝輔か繝ｼ繝槭ャ繝医〒繝輔ぃ繧､繝ｫ縺ｫ譖ｸ縺榊・縺・
    /// </summary>
    /// <param name="path">蜃ｺ蜉帙ヵ繧｡繧､繝ｫ繝代せ</param>
    /// <param name="textureName">繝・け繧ｹ繝√Ε蜷搾ｼ・SCII・・/param>
    /// <param name="verts">鬆らせ驟榊・</param>
    /// <param name="indices">繧､繝ｳ繝・ャ繧ｯ繧ｹ驟榊・</param>
    /// <returns>謌仙粥縺ｧ true</returns>
    static bool Write(
        const std::string&             path,
        const std::string&             textureName,
        const std::vector<MeshVertex>& verts,
        const std::vector<uint32_t>&   indices);
};
