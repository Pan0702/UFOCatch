#pragma once
#include "../../Common/Object3D.h"
#include "../AnimalSheep/Sheep.h"

//群れに関する情報
//centroid 群れの重心
struct FlogInfo
{
    VECTOR3 centroid; //群れの重心
    CSheep* furthestSheep;
    float maxDistance;
};
class Flog : public Object3D
{
public:
    Flog();
    ~Flog();
    void Update() override;

private:

    void Spawn(int a);
    void RemoveFromArray();
};
