#include "UIDraw.h"
#include "ImageRegistry.h"

int UIDraw::AddElement(const char* name, const VECTOR2& pos, const VECTOR4& size, float alpha, int layer)
{
    UIImageInfo info;
    info.pTexture = ImageRegistry::GetTexture(name);
    if (info.pTexture == nullptr)
    {
        assert("Not Texture");
        return -1;
    }
    info.position = pos;
    info.imageSize = size;
    info.alpha = alpha;
    info.layer = layer;
    
    m_elements.push_back(info);
    return static_cast<int>(m_elements.size()) - 1;
}

void UIDraw::Clear()
{
    m_elements.clear();
}

void UIDraw::SetPos(int index, const VECTOR2& pos)
{
    m_elements[index].position = pos;
}

void UIDraw::SetAlpha(int index, float alpha)
{
    m_elements[index].alpha = alpha;
}

void UIDraw::SetLayer(int index, int layer)
{
    m_elements[index].layer = layer;
}

void UIDraw::Draw()
{
    Object3D::Draw();
}

int UIDraw::GetElementCount()
{
    return static_cast<int>(m_elements.size());
}

UIImageInfo UIDraw::GetElement(int index) const
{
    return m_elements[index];
}
