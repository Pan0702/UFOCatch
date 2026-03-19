#include "MeshWriter.h"

#include <fstream>

#include <windows.h>

// 頂点・インデックスデータを .mesh フォーマットでファイルに書き出す
// フォーマットは FbxMesh.cpp の Load 関数から逆算
bool MeshWriter::Write(
    const std::string&             path,
    const std::string&             textureName,
    const std::vector<MeshVertex>& verts,
    const std::vector<uint32_t>&   indices)
{
    std::ofstream file(path, std::ios::binary);
    if (!file)
    {
        MessageBoxA(nullptr, (".mesh ファイルを書き込めません: " + path).c_str(), "MeshWriter", MB_OK | MB_ICONERROR);
        return false;
    }

    // ---- Magic "MESH2010" (WCHAR[8] = 16 bytes) ----
    // Head[6] = '1' -> static mesh として認識される
    WCHAR magic[] = L"MESH2010";  // 9要素（ヌル終端含む）だが書き込みは8文字分のみ
    file.write(reinterpret_cast<const char*>(magic), 8 * sizeof(WCHAR));

    // ---- テクスチャ数 ----
    const int texCount = 1;
    file.write(reinterpret_cast<const char*>(&texCount), sizeof(int));

    // ---- テクスチャ名 (WCHAR[128] = 256 bytes, null 埋め) ----
    WCHAR texBuf[128] = {};
    const size_t copyLen = (std::min)(textureName.size(), static_cast<size_t>(127));
    for (size_t i = 0; i < copyLen; ++i)
        texBuf[i] = static_cast<WCHAR>(static_cast<unsigned char>(textureName[i]));
    file.write(reinterpret_cast<const char*>(texBuf), 128 * sizeof(WCHAR));

    // ---- メッシュ数 ----
    const int meshCount = 1;
    file.write(reinterpret_cast<const char*>(&meshCount), sizeof(int));

    // ---- メッシュ名 (WCHAR[128] = 256 bytes, null 埋め) ----
    WCHAR meshName[128] = {};
    file.write(reinterpret_cast<const char*>(meshName), 128 * sizeof(WCHAR));

    // ---- 頂点数 ----
    const int vertexCount = static_cast<int>(verts.size());
    file.write(reinterpret_cast<const char*>(&vertexCount), sizeof(int));

    // ---- 頂点データ (StaticVertex と同レイアウト: position + normal + uv = 32 bytes) ----
    file.write(reinterpret_cast<const char*>(verts.data()), sizeof(MeshVertex) * verts.size());

    // ---- インデックス数 ----
    const int indexCount = static_cast<int>(indices.size());
    file.write(reinterpret_cast<const char*>(&indexCount), sizeof(int));

    // ---- インデックス配列 (DWORD = 4 bytes each) ----
    file.write(reinterpret_cast<const char*>(indices.data()), sizeof(DWORD) * indices.size());

    // ---- このメッシュのテクスチャ数 + テクスチャ番号インデックス ----
    const int texNumforMesh = 1;
    const int texIndex        = 0;
    file.write(reinterpret_cast<const char*>(&texNumforMesh), sizeof(int));
    file.write(reinterpret_cast<const char*>(&texIndex),        sizeof(int));

    return true;
}
