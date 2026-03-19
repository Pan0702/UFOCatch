#include "ResourceManager.h"
#include <unordered_map>
#include <fstream>
#include "../Utils/MeshCollider.h"
#include "GameObject.h"

namespace {
    std::vector<ModelInfo> modelInfos;
};

void ResourceManager::Init()
{
}

void ResourceManager::Reset()
{
    
    modelInfos.clear();
}

void ResourceManager::LoadFbx(const char* name, const char* path)
{
    
    ModelInfo m(name, path);
    m.mesh = std::make_unique<CFbxMesh>();
    m.coll = std::make_unique<MeshCollider>();
    m.mesh->Load(path);
    m.coll->MakeFromMesh(m.mesh.get());
    for (auto& f : modelInfos)
    {
        if (f.name == name)return;
    }
    modelInfos.push_back(m);
    
}

CFbxMesh* ResourceManager::GetModel(const char* name)
{
    for (auto& m : modelInfos)
    {
        if (m.name == name) return m.mesh.get();
    }
    return nullptr;
}

std::vector<std::string> ResourceManager::GetModelNames()
{
    std::vector<std::string> names;
    for (auto& m : modelInfos)
    {
        names.push_back(m.name);
    }
    return names;
}

MeshCollider* ResourceManager::GetColl(const char* name)
{
    for (auto& m : modelInfos)
    {
        if (m.name == name) return m.coll.get();
    }
    return nullptr;
}

const char* ResourceManager::GetPath(const char* name)
{
    for (auto& m : modelInfos)
    {
        if (m.name == name) return m.path;
    }
    assert("ModelのPathが見つかりません");
    return nullptr;
}


