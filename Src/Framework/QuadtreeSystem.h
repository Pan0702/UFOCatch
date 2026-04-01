#pragma once
#include "ObjectManager.h"

class CQuadtreeSystem
{
public:
    virtual ~CQuadtreeSystem() = default;
};
template<class C> C* InstanceQTree()
{
    C* obj = ObjectManager::FindQuadTree<C>();
    if (obj == nullptr)
    {
        auto object = std::make_unique<C>();
        C* raw = object.get();
        ObjectManager::PushTree(std::move(object));
        return raw;
    }
    return obj;
}

