#include "ImageRegistry.h"

#include "../Utils/MyLib.h"

namespace
{
    std::unordered_map<std::string, ImageInfo> m_textures;
}

CSpriteImage* ImageRegistry::LoadTexture(const char* textureName, const char* path)
{
    auto it = m_textures.find(textureName);
    if (it != m_textures.end()) return it->second.pTexture.get();

    auto [newIt, _] = m_textures.emplace(textureName, ImageInfo(path));
    return newIt->second.pTexture.get();
}

CSpriteImage* ImageRegistry::LoadTexture(const std::string& path)
{
    const std::string name = MyLib::ChangePathToName(path);
    return LoadTexture(name.c_str(), path.c_str());
}

CSpriteImage* ImageRegistry::GetTexture(const char* textureName)
{
    auto it = m_textures.find(textureName);
    if (it == m_textures.end()) return nullptr;
    return it->second.pTexture.get();
}

