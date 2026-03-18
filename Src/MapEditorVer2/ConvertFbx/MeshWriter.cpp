#include "MeshWriter.h"

#include <fstream>

#include <windows.h>

// 鬆らせ繝ｻ繧､繝ｳ繝・ャ繧ｯ繧ｹ繝・・繧ｿ繧・.mesh 繝輔か繝ｼ繝槭ャ繝医〒繝輔ぃ繧､繝ｫ縺ｫ譖ｸ縺榊・縺・
// 繝輔か繝ｼ繝槭ャ繝医・ FbxMesh.cpp 縺ｮ Load 髢｢謨ｰ縺九ｉ騾・ｮ・
bool MeshWriter::Write(
    const std::string&             path,
    const std::string&             textureName,
    const std::vector<MeshVertex>& verts,
    const std::vector<uint32_t>&   indices)
{
    std::ofstream file(path, std::ios::binary);
    if (!file)
    {
        MessageBoxA(nullptr, (".mesh 繝輔ぃ繧､繝ｫ繧呈嶌縺崎ｾｼ繧√∪縺帙ｓ: " + path).c_str(), "MeshWriter", MB_OK | MB_ICONERROR);
        return false;
    }

    // ---- Magic "MESH2010" (WCHAR[8] = 16 bytes) ----
    // Head[6] = '1' 竊・static mesh 縺ｨ縺励※隱崎ｭ倥＆繧後ｋ
    WCHAR magic[] = L"MESH2010";  // 9隕∫ｴ・医レ繝ｫ邨らｫｯ蜷ｫ繧・峨□縺梧嶌縺崎ｾｼ縺ｿ縺ｯ8譁・ｭ怜・縺ｮ縺ｿ
    file.write(reinterpret_cast<const char*>(magic), 8 * sizeof(WCHAR));

    // ---- 繝・け繧ｹ繝√Ε謨ｰ ----
    const int texCount = 1;
    file.write(reinterpret_cast<const char*>(&texCount), sizeof(int));

    // ---- 繝・け繧ｹ繝√Ε蜷・(WCHAR[128] = 256 bytes, null 蝓九ａ) ----
    WCHAR texBuf[128] = {};
    const size_t copyLen = (std::min)(textureName.size(), static_cast<size_t>(127));
    for (size_t i = 0; i < copyLen; ++i)
        texBuf[i] = static_cast<WCHAR>(static_cast<unsigned char>(textureName[i]));
    file.write(reinterpret_cast<const char*>(texBuf), 128 * sizeof(WCHAR));

    // ---- 繝｡繝・す繝･謨ｰ ----
    const int meshCount = 1;
    file.write(reinterpret_cast<const char*>(&meshCount), sizeof(int));

    // ---- 繝｡繝・す繝･蜷・(WCHAR[128] = 256 bytes, null 蝓九ａ) ----
    WCHAR meshName[128] = {};
    file.write(reinterpret_cast<const char*>(meshName), 128 * sizeof(WCHAR));

    // ---- 鬆らせ謨ｰ ----
    const int vertexCount = static_cast<int>(verts.size());
    file.write(reinterpret_cast<const char*>(&vertexCount), sizeof(int));

    // ---- 鬆らせ繝・・繧ｿ (StaticVertex 縺ｨ蜷後Ξ繧､繧｢繧ｦ繝・ position + normal + uv = 32 bytes) ----
    file.write(reinterpret_cast<const char*>(verts.data()), sizeof(MeshVertex) * verts.size());

    // ---- 繧､繝ｳ繝・ャ繧ｯ繧ｹ謨ｰ ----
    const int indexCount = static_cast<int>(indices.size());
    file.write(reinterpret_cast<const char*>(&indexCount), sizeof(int));

    // ---- 繧､繝ｳ繝・ャ繧ｯ繧ｹ驟榊・ (DWORD = 4 bytes each) ----
    file.write(reinterpret_cast<const char*>(indices.data()), sizeof(DWORD) * indices.size());

    // ---- 縺薙・繝｡繝・す繝･縺ｮ繝・け繧ｹ繝√Ε謨ｰ + 繝・け繧ｹ繝√Ε逡ｪ蜿ｷ繧､繝ｳ繝・ャ繧ｯ繧ｹ ----
    const int texNumforMesh = 1;
    const int texIndex        = 0;
    file.write(reinterpret_cast<const char*>(&texNumforMesh), sizeof(int));
    file.write(reinterpret_cast<const char*>(&texIndex),        sizeof(int));

    return true;
}
