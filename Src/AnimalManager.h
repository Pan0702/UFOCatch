#pragma once
#include "Object3D.h"

class CAnimalManager:public Object3D
{
public:
    CAnimalManager();
    ~CAnimalManager();
     VECTOR3 GetObjectSize(MeshCollider* meshColl) const;
private:
    void Update() override;
    void Draw() override;
    
};
