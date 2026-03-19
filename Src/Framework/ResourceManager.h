#pragma once
#include <string>
#include "../Utils/FbxMesh.h"
#include "../Utils/MeshCollider.h"

namespace ResourceManager
{
    void Init();
    void Reset();
    CFbxMesh* LoadFbx(const std::string& filename);
    void LoadFbx(const char* name, const char* path);
    void LoadFbx(const char* name);
    CFbxMesh* GetModel(const char* name);
    std::vector<std::string> GetModelNames();
    MeshCollider* GetColl(const char* name);
    const char* GetPath(const char* name);
    
}

struct ModelInfo
{
    const char* name;
    const char* path;
    std::shared_ptr<CFbxMesh> mesh;
    std::shared_ptr<MeshCollider> coll;

    ModelInfo(const char* name, const char* path)
        : name(name), path(path), mesh(nullptr), coll(nullptr)
    {
    }
    ModelInfo(const char* name)
        : name(name), path(nullptr), mesh(nullptr), coll(nullptr)
    {
    }
};

