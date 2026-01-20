#pragma once
//ƒ}ƒNƒ’è‹`
#define SAFE_RELEASE(x) if(x){x->Release(); x=0;}
#define SAFE_DELETE(x) if(x){delete x; x=0;}
#define SAFE_DELETE_ARRAY(p){ if(p){ delete[] (p);   (p)=nullptr;}}

inline void safe_delete(void*& ptr) {
	if (ptr != nullptr) {
		delete ptr;
		ptr = nullptr;
	}
}

inline void safe_delete_array(void*& ptr) {
	if (ptr != nullptr) {
		delete[] ptr;
		ptr = nullptr;
	}
}