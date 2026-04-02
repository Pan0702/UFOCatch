#include "UIDraw.h"
#include "ImageRegistry.h"

int CUIDraw::AddElement(const char* name, const VECTOR2& pos, const VECTOR4& size, float alpha, int layer)
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

void CUIDraw::Clear()
{
    m_elements.clear();
}

void CUIDraw::SetPos(int index, const VECTOR2& pos)
{
    m_elements[index].position = pos;
}

void CUIDraw::SetAlpha(int index, float alpha)
{
    m_elements[index].alpha = alpha;
}

void CUIDraw::SetLayer(int index, int layer)
{
    m_elements[index].layer = layer;
}

void CUIDraw::Draw()
{
    std::vector<UIImageInfo*> sorted;
    for (auto& e : m_elements)
        sorted.push_back(&e);
    std::ranges::sort(sorted, [](const UIImageInfo* a, const UIImageInfo* b)
    {
        return a->layer < b->layer;
    });

    CSprite spr;
    for (auto* e : sorted)
    {
        spr.Draw(e->pTexture,
            e->position.x, e->position.y,
            static_cast<DWORD>(e->imageSize.x),
            static_cast<DWORD>(e->imageSize.y),
            static_cast<DWORD>(e->imageSize.z),
            static_cast<DWORD>(e->imageSize.w),
            e->alpha);
    }
}

int CUIDraw::GetElementCount()
{
    return static_cast<int>(m_elements.size());
}

UIImageInfo CUIDraw::GetElement(int index) const
{
    return m_elements[index];
}
