#pragma once
#define SAFE_RELEASE(x) if(x){x->Release(); x=0;}
#define SAFE_DELETE(x) if(x){delete x; x=0;}
#define SAFE_DELETE_ARRAY(p){ if(p){ delete[] (p);   (p)=nullptr;}}

/// safe delete の処理を行う
/// @param ptr ptr に渡す値
inline void safe_delete(void*& ptr)
{
    if (ptr != nullptr)
    {
        delete ptr;
        ptr = nullptr;
    }
}

/// safe delete array の処理を行う
/// @param ptr ptr に渡す値
inline void safe_delete_array(void*& ptr)
{
    if (ptr != nullptr)
    {
        delete[] ptr;
        ptr = nullptr;
    }
}
