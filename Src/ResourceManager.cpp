#include "ResourceManager.h"
#include <unordered_map>
#include <fstream>

namespace {
    std::unordered_map<std::string, CFbxMesh*> fbxFiles;
};

void ResourceManager::Init()
{
    fbxFiles.clear();
}

void ResourceManager::Reset()
{
    for (auto f : fbxFiles) {
        delete f.second;
    }
    fbxFiles.clear();
}

CFbxMesh* ResourceManager::LoadFbx(std::string filename)
{
    if (fbxFiles.find(filename) == fbxFiles.end()) {
        CFbxMesh* mesh = new CFbxMesh();
        assert(mesh);
        mesh->Load(filename.c_str());
        fbxFiles[filename] = mesh;
    }
    return fbxFiles[filename];
}
