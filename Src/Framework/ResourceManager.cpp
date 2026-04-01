#include "ResourceManager.h"
#include "../Utils/MyLib.h"
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
    for (auto& f : modelInfos)
    {
        if (f.name == name) return;
    }
    ModelInfo m(name, path);
    m.mesh = std::make_unique<CFbxMesh>();
    m.coll = std::make_unique<MeshCollider>();
    m.mesh->Load(path);
    m.coll->MakeFromMesh(m.mesh.get());
    modelInfos.push_back(m);
}

void ResourceManager::LoadFbx(const std::vector<std::string>& paths)
{
    for (std::string path : paths)
    {
        LoadFbx(path.c_str());
    }
}

CFbxMesh* ResourceManager::LoadFbx(const char* path)
{
    ModelInfo m(path);
    m.mesh = std::make_unique<CFbxMesh>();
    m.coll = std::make_unique<MeshCollider>();
    std::string n = MyLib::ChangePathToName(path);
    for (auto& f : modelInfos)
    {
        if (f.name == n)
        {
            return f.mesh.get();
        }
    }
    m.mesh->Load(path);
    m.name = n.c_str();
    modelInfos.push_back(m);
    return m.mesh.get();
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

std::vector<std::string> ResourceManager::GetAllModelPath()
{
    std::vector<std::string> paths;
    for (auto& m : modelInfos)
    {
        paths.push_back(m.path);
    }
    return paths; 
}


