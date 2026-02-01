#pragma once
#include "../../Common/Object3D.h"
#include "../AnimalSheep/Sheep.h"
#include "../AnimalDog//ShepherdDog.h"

//群れに関する情報
struct FlogInfo
{
    VECTOR3 centroid;       //群れの重心
    CSheep* furthestSheep;  //一番遠くにいる羊のptr;
    float maxDistance;      //中心から一番遠くにいる動物までの距離
};
class Flog : public Object3D
{
public:
    Flog();
    ~Flog();
    void Update() override;
    FlogInfo CalcFlogInfo(std::vector<CSheep*> sheeps) const;
private:

    void Spawn(int a);
    void RemoveFromArray(CSheep* sheep);

    std::vector<CSheep*> allSheeps;
    std::vector<CAShepherdDog*> shepherdDogs;
};
