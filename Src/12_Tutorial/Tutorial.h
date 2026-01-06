#pragma once
#include "../05_CommonFile/Object3D.h"

class CTutorial : public Object3D
{
public:
    CTutorial();
private:
    ~CTutorial();
    void Update() override;
};
