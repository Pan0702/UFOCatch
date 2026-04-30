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
    void LoadFbx(const std::vector<std::string>& paths);
    CFbxMesh* LoadFbx(const char* path);
    CFbxMesh* GetModel(const char* name);
    std::vector<std::string> GetModelNames();
    MeshCollider* GetColl(const char* name);
    MeshCollider* GetColl(const CFbxMesh* mesh);
    const char* GetPath(const char* name);
    std::vector<std::string> GetAllModelPath();
}

struct ModelInfo
{
    std::string name;
    std::string path;
    std::shared_ptr<CFbxMesh> mesh;
    std::shared_ptr<MeshCollider> coll;

    ModelInfo(const char* name, const char* path)
        : name(name ? name : ""), path(path ? path : ""), mesh(nullptr), coll(nullptr)
    {
    }

    ModelInfo(const char* name)
        : name(name ? name : ""), path(""), mesh(nullptr), coll(nullptr)
    {
    }
};
