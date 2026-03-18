#pragma once
#include <algorithm>


template<class T, size_t MAXSIZE> class PoolAllocator
{
public:
    // 繧ｳ繝ｳ繧ｹ繝医Λ繧ｯ繧ｿ
    PoolAllocator() {
        for (size_t i = 0; i < MAXSIZE; i++)
        {
            element* elems = reinterpret_cast<element*>(buffer_);
            elems[i].next = (i + 1 < MAXSIZE) ? &elems[i + 1] : nullptr;
        }
        freelist_ = reinterpret_cast<element*>(buffer_);
    }

    // 繝・せ繝医Λ繧ｯ繧ｿ
    ~PoolAllocator() = default;

    // 蝗ｺ螳夐聞繝｡繝｢繝ｪ繝励・繝ｫ縺九ｉ繝｡繝｢繝ｪ繝悶Ο繝・け繧貞牡繧贋ｻ倥￠繧九・
    T* Alloc() {
        if (!freelist_)return nullptr;
        void* result = reinterpret_cast<void*>(freelist_);
        freelist_ = freelist_->next;
        return  static_cast<T*>(result);
    }

    // 蝗ｺ螳夐聞繝｡繝｢繝ｪ繝励・繝ｫ縺九ｉ蜑ｲ繧贋ｻ倥￠縺溘Γ繝｢繝ｪ繝悶Ο繝・け繧定ｧ｣謾ｾ縺吶ｋ縲・
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

