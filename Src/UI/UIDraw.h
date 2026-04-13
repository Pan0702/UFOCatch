#pragma once
#include "../Utils/Sprite3D.h"

/// @brief CUIDrawが管理する1つの描画要素
struct UIImageInfo
{
    CSpriteImage* pTexture; // ImageRegistryから取得したポインタ（所有しない）
    VECTOR2 position;
    VECTOR4 imageSize; // (srcX, srcY, srcWidth, srcHeight)
    float alpha;
    int layer;

    UIImageInfo(CSpriteImage* tex, const VECTOR2& pos, const VECTOR4&
                size, float a = 1.0f, int l = 0)
        : pTexture(tex), position(pos),
          imageSize(size), alpha(a), layer(l)
    {
    }
    UIImageInfo() = default;
};

/// @brief ウィジェット階層を使わずに画像をシンプルに描画する軽量UIクラス
/// @details AddElement()で要素を登録し、Draw()でlayer順に一括描画する。
///          インデックスで各要素の位置・透明度・レイヤーを後から変更できる。
class CUIDraw
{
public:
    /// @brief 描画要素を追加してインデックスを返す
    /// @param name ImageRegistryに登録済みのテクスチャ名
    /// @return 追加された要素のインデックス（テクスチャが見つからない場合は-1）
    int AddElement(const char* name, const VECTOR2& pos, const VECTOR4& size, float alpha = 1.0f, int layer = 0);
    void Clear();
    void SetPos(int index, const VECTOR2& pos);
    void SetAlpha(int index, float alpha);
    void SetLayer(int index, int layer);
    void Draw();
    int GetElementCount();
    UIImageInfo GetElement(int index) const;

private:
    std::vector<UIImageInfo> m_elements;

};
