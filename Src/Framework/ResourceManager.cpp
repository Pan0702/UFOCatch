#include "ResourceManager.h"
#include <unordered_map>
#include <fstream>

namespace {
    std::unordered_map<std::string, std::unique_ptr<CFbxMesh>> fbxFiles;
};

void ResourceManager::Init()
{
    fbxFiles.clear();
}

void ResourceManager::Reset()
{
    for (auto& f : fbxFiles) {
        f.second.reset();
    }
    fbxFiles.clear();
}

CFbxMesh* ResourceManager::LoadFbx(const std::string& filename)
{
    if (fbxFiles.find(filename) == fbxFiles.end()) {
        auto mesh = std::make_unique<CFbxMesh>();
        assert(mesh);
        mesh->Load(filename.c_str());
        fbxFiles[filename] = std::move(mesh);
    }
    return fbxFiles[filename].get();
}
