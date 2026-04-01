#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include "../Utils/MyMath.h"
#include "../Utils/Sprite3D.h"

///<summary>モデルの管理</summary>
namespace ImageRegistry
{
    ///<summary>テクスチャのロード</summary>
    ///<@param textureName = テクスチャの識別名(キー)
    ///<@param path = テクスチャファイルのパス
    CSpriteImage* LoadTexture(const char* textureName, const char* path);
    
    ///<summary>テクスチャのロード</summary>
    ///<para>std::string型のパスを指定してテクスチャをロードする</para>
    ///<@param path = テクスチャファイルのパス(ファイル名が自動的に識別名として使用される)
    CSpriteImage* LoadTexture(const std::string& path);
    
    ///<summary>既にロード済みのテクスチャを取得</summary>
    ///<@param textureName = テクスチャの識別名(キー)
    ///<@return 指定された識別名のテクスチャ、見つからない場合はnullptr
    CSpriteImage* GetTexture(const char* textureName);
}

///<summary>モデルの管理</summary>
struct ImageInfo
{
    const char* path;
    std::unique_ptr<CSpriteImage> pTexture;
    
    ImageInfo( const char* path)
    :  path(path),pTexture(nullptr)
    {
        pTexture = std::make_unique<CSpriteImage>(path);
    }
    
};
