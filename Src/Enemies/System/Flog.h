#pragma once
#include "../../Common/Object3D.h"
#include "../AnimalSheep/Sheep.h"
#include "../AnimalDog//ShepherdDog.h"

//群れに関する情報
struct FlogInfo
{
    VECTOR3 centroid; //群れの重心
    CSheep* furthestSheep; //一番遠くにいる羊のptr;
    float maxDistance; //中心から一番遠くにいる動物までの距離
};

class CFlog : public Object3D
{
public:
    CFlog();
    ~CFlog();
    void Initialize();  // 羊と犬を生成して紐付け
    void Update() override;
    FlogInfo CalcFlogInfo(const std::vector<CSheep*>& sheeps) const;
    const std::vector<CSheep*>& GetAllSheeps() const { return m_allSheeps; }

private:
    void Spawn(int a);
    void RemoveFromArray(const CSheep* sheep);

    std::vector<CSheep*> m_allSheeps;
    std::vector<CAShepherdDog*> m_shepherdDogs;
};
