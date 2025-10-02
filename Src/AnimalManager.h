#pragma once
#include <chrono>

#include "Object3D.h"

class CAnimalManager:public Object3D
{
public:
    CAnimalManager();
    ~CAnimalManager();
protected:
    //各座標の最大値を返却
     VECTOR3 GetObjectSize(MeshCollider* meshColl) const;
    //オブジェクトをUFOに向かって動かす
     void MoveForUFO(const VECTOR3& animalPos, const VECTOR3& distanceFromObjectToUFO);
private:
    void Update() override;
    void Draw() override;
    
};
