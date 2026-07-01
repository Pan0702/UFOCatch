#pragma once
#include <algorithm>


template <class T, size_t MAXSIZE>
class PoolAllocator
{
public:
    // コンストラクタ
    /// Pool Allocator を返す
    PoolAllocator()
    {
        for (size_t i = 0; i < MAXSIZE; i++)
        {
            element* elems = reinterpret_cast<element*>(buffer_);
            elems[i].next = (i + 1 < MAXSIZE) ? &elems[i + 1] : nullptr;
        }
        freelist_ = reinterpret_cast<element*>(buffer_);
    }

    /// PoolAllocator の終了処理を行う
    ~PoolAllocator() = default;

    /// Alloc を返す
    /// @return 対象のポインタ
    T* Alloc()
    {
        if (!freelist_)return nullptr;
        void* result = reinterpret_cast<void*>(freelist_);
        freelist_ = freelist_->next;
        return static_cast<T*>(result);
    }

    /// Free の処理を行う
    /// @param addr addr に渡す値
    void Free(T* addr)
    {
        if (addr)
        {
            element* e = reinterpret_cast<element*>(addr);
            e->next = freelist_;
            freelist_ = e;
        }
    }

private:
    union element
    {
        alignas(T) char data[sizeof(T)];
        element* next;
    };

    /// alignas を返す
    /// @param element element に渡す値
    alignas(element) char buffer_[MAXSIZE * sizeof(element)];
    element* freelist_;
};
