#pragma once
#include "../05_CommonFile/Object3D.h"
class CGround : public Object3D
{
public:
    //.meshのPathとmeshの大きさを渡す//
    //　初期状態は１０×１０,中心点は5,0,5//
    CGround(const char* mesh, const VECTOR3& scale = VECTOR3(1.0f, 1.0f, 1.0f));

private:
    ~CGround();
   // bool Init(const char* filePath);
};
