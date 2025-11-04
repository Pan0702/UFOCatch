#pragma once
#include <algorithm>


template<class T, size_t MAXSIZE> class PoolAllocator
{
public:
    // コンストラクタ
    PoolAllocator() {
        for (size_t i = 0; i < MAXSIZE; i++)
        {
            element* elems = reinterpret_cast<element*>(buffer_);
            elems[i].next = (i + 1 < MAXSIZE) ? &elems[i + 1] : nullptr;
        }
        freelist_ = reinterpret_cast<element*>(buffer_);
    }

    // デストラクタ
    ~PoolAllocator() = default;

    // 固定長メモリプールからメモリブロックを割り付ける。
    T* Alloc() {
        if (!freelist_)return nullptr;
        void* result = reinterpret_cast<void*>(freelist_);
        freelist_ = freelist_->next;
        return  static_cast<T*>(result);
    }

    // 固定長メモリプールから割り付けたメモリブロックを解放する。
    void Free(T* addr) {
        if (addr) {
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
    alignas(element) char buffer_[MAXSIZE * sizeof(element)];
    element* freelist_;

};
