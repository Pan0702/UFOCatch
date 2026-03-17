#include "MeshWriter.h"

#include <fstream>

#include <windows.h>

// 頂点・インデックスデータを .mesh フォーマットでファイルに書き出す
// フォーマットは FbxMesh.cpp の Load 関数から逆算
bool MeshWriter::Write(
    const std::string&             path,
    const std::string&             texture_name,
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
    // Head[6] = '1' → static mesh として認識される
    WCHAR magic[] = L"MESH2010";  // 9要素（ヌル終端含む）だが書き込みは8文字分のみ
    file.write(reinterpret_cast<const char*>(magic), 8 * sizeof(WCHAR));

    // ---- テクスチャ数 ----
    int tex_count = 1;
    file.write(reinterpret_cast<const char*>(&tex_count), sizeof(int));

    // ---- テクスチャ名 (WCHAR[128] = 256 bytes, null 埋め) ----
    WCHAR tex_buf[128] = {};
    size_t copy_len = (std::min)(texture_name.size(), static_cast<size_t>(127));
    for (size_t i = 0; i < copy_len; ++i)
        tex_buf[i] = static_cast<WCHAR>(static_cast<unsigned char>(texture_name[i]));
    file.write(reinterpret_cast<const char*>(tex_buf), 128 * sizeof(WCHAR));

    // ---- メッシュ数 ----
    int mesh_count = 1;
    file.write(reinterpret_cast<const char*>(&mesh_count), sizeof(int));

    // ---- メッシュ名 (WCHAR[128] = 256 bytes, null 埋め) ----
    WCHAR mesh_name[128] = {};
    file.write(reinterpret_cast<const char*>(mesh_name), 128 * sizeof(WCHAR));

    // ---- 頂点数 ----
    int vertex_count = static_cast<int>(verts.size());
    file.write(reinterpret_cast<const char*>(&vertex_count), sizeof(int));

    // ---- 頂点データ (StaticVertex と同レイアウト: position + normal + uv = 32 bytes) ----
    file.write(reinterpret_cast<const char*>(verts.data()), sizeof(MeshVertex) * verts.size());

    // ---- インデックス数 ----
    int index_count = static_cast<int>(indices.size());
    file.write(reinterpret_cast<const char*>(&index_count), sizeof(int));

    // ---- インデックス配列 (DWORD = 4 bytes each) ----
    file.write(reinterpret_cast<const char*>(indices.data()), sizeof(DWORD) * indices.size());

    // ---- このメッシュのテクスチャ数 + テクスチャ番号インデックス ----
    int tex_num_for_mesh = 1;
    int tex_index        = 0;
    file.write(reinterpret_cast<const char*>(&tex_num_for_mesh), sizeof(int));
    file.write(reinterpret_cast<const char*>(&tex_index),        sizeof(int));

    return true;
}