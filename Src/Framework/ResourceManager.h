#pragma once
#include <string>
#include <string_view>
#include "../Utils/FbxMesh.h"

class MeshCollider;

namespace ResourceManager
{
    void Init();
    void Reset();
    CFbxMesh* LoadFbx(const std::string& filename);
    void LoadFbx(const char* name, const char* path);
    void LoadFbx( const char* name);
    CFbxMesh* GetMesh(const char* name);
    MeshCollider* GetColl(const char* name);
}

struct ModelInfo
{
    const char* name;
    const char* path;
    std::unique_ptr<CFbxMesh> mesh;
    std::unique_ptr<MeshCollider> coll;

    ModelInfo(const char* name, const char* path)
        : name(name), path(path), mesh(nullptr), coll(nullptr)
    {
    }
    ModelInfo(const char* name)
        : name(name), path(nullptr), mesh(nullptr), coll(nullptr)
    {
    }
};

