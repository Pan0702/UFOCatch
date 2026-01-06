#pragma once
#include "../05_CommonFile/Object3D.h"
class CGround : public Object3D
{
public:
    CGround(const char* mesh);

private:
    ~CGround();
    void Update();
    void Draw() override;
   // bool Init(const char* filePath);
};
