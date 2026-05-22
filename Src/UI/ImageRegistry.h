#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include "../Utils/MyMath.h"
#include "../Utils/Sprite3D.h"

/// @brief テクスチャを名前付きで一元管理するレジストリ
/// @details ロード済みテクスチャをキャッシュし、重複ロードを防ぐ。
///          テクスチャの所有権はレジストリ内部が持ち、ポインタは借用として返す。
namespace ImageRegistry
{
    /// Texture を読み込む
    /// @param textureName 名前
    /// @param path パス
    /// @return 対象のポインタ
    CSpriteImage* LoadTexture(const char* textureName, const char* path);

    /// Texture を読み込む
    /// @param path パス
    /// @return 対象のポインタ
    CSpriteImage* LoadTexture(const std::string& path);

    /// Texture を取得する
    /// @param textureName 名前
    /// @return 対象のポインタ
    CSpriteImage* GetTexture(const char* textureName);
}

/// @brief レジストリ内部で使うテクスチャ情報（パスと実体を保持）
struct ImageInfo
{
    const char* path;
    std::unique_ptr<CSpriteImage> pTexture;

    /// ImageInfo を初期化する
    /// @param path パス
    ImageInfo(const char* path)
        : path(path), pTexture(nullptr)
    {
        pTexture = std::make_unique<CSpriteImage>(path);
    }
};
