#pragma once
#include "../05_CommonFile/Object3D.h"

class CTutorialAnimal : public Object3D
{
public:
    CTutorialAnimal(const VECTOR3& pos);
    //　Scoreを足してオブジェクトをDestroy//
    void Destroy();

private:
    ~CTutorialAnimal();
    
    void Update() override;
    
};
