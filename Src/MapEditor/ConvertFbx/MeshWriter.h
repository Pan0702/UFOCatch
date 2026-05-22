#pragma once
#include <cstdint>
#include <string>
#include <vector>

#include "FbxParser.h"

// メッシュデータを .mesh フォーマットでファイルに書き出すクラス
class MeshWriter
{
public:
    /// Write を返す
    /// @param path パス
    /// @param textureName 名前
    /// @param verts verts に渡す値
    /// @param indices indices に渡す値
    /// @return 成功または条件を満たす場合 true
    static bool Write(
        const std::string& path,
        const std::string& textureName,
        const std::vector<MeshVertex>& verts,
        const std::vector<uint32_t>& indices);
};
