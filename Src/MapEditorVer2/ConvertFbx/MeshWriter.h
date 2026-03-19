#pragma once
#include <cstdint>
#include <string>
#include <vector>

#include "FbxParser.h"

// メッシュデータを .mesh フォーマットでファイルに書き出すクラス
class MeshWriter
{
public:
    /// <summary>
    /// 頂点・インデックスデータを .mesh フォーマットでファイルに書き出す
    /// </summary>
    /// <param name="path">出力ファイルパス</param>
    /// <param name="textureName">テクスチャ名（ASCII）</param>
    /// <param name="verts">頂点配列</param>
    /// <param name="indices">インデックス配列</param>
    /// <returns>成功で true</returns>
    static bool Write(
        const std::string&             path,
        const std::string&             textureName,
        const std::vector<MeshVertex>& verts,
        const std::vector<uint32_t>&   indices);
};