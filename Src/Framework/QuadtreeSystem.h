#pragma once
#include "ObjectManager.h"

/// <summary>ゲーム共通基盤で使う Quadtree System の情報と処理をまとめる型</summary>
class CQuadtreeSystem
{
public:
    /// CQuadtreeSystem の終了処理を行う
    virtual ~CQuadtreeSystem() = default;
};

/// Instance QTree を返す
/// @return 対象のポインタ
template <class C>
C* InstanceQTree()
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
