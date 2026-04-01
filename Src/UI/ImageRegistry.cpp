#include "ImageRegistry.h"

#include "../Utils/MyLib.h"

namespace
{
    std::unordered_map<std::string, ImageInfo> m_textures;
}

CSpriteImage* ImageRegistry::LoadTexture(const char* textureName, const char* path)
{
    for (auto& t : m_textures)
    {
        if (t.first == textureName) return t.second.pTexture.get();
    }
    ImageInfo info(path);
    m_textures.insert(std::make_pair(textureName, info));
    return info.pTexture.get();
    
}

CSpriteImage* ImageRegistry::LoadTexture(const std::string& path)
{
    const std::string name = MyLib::ChangePathToName(path);
    return LoadTexture(name.c_str(), path.c_str());
}

CSpriteImage* ImageRegistry::GetTexture(const char* textureName)
{
    for (auto& t : m_textures)
    {
        if (t.first == textureName) return t.second.pTexture.get();
    }
    return nullptr;
}

