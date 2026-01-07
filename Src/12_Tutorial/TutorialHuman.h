#pragma once
#include "TutorialAnimal.h"

class CTutorialHuman : public Object3D
{
public:
    CTutorialHuman(const VECTOR3& pos);
    ~CTutorialHuman();

private:
    void Update() override;
    
    bool m_inSight;
};
