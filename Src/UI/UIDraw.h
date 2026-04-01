#pragma once
#include "../Common/Object3D.h"

struct UIImageInfo
{
    CSpriteImage* pTexture; // ImageRegistryから取得したポインタ
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

///<summary>UI描画クラス</summary>
class UIDraw : Object3D
{
public:
    int AddElement(const char* name,const VECTOR2& pos,const VECTOR4& size,float alpha = 1.0f,int layer = 0);
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
