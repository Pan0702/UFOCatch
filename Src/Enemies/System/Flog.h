#pragma once
#include "../../Common/Object3D.h"
#include "../AnimalDog/ShepherdDog.h"
#include "../AnimalSheep/Sheep.h"

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
    FlogInfo CalcFlogInfo(const std::vector<CSheep*>& manySheep) const;
    const std::vector<CSheep*>& GetAllSheeps() const { return m_allSheep; }

    // 群れの中心点と半径の取得
    const VECTOR3& GetFlockCenter() const { return m_flockCenter; }
    float GetFlockRadius() const { return m_flockRadius; }

private:
    void RemoveFromArray(const CSheep* sheep);

    std::vector<CSheep*> m_allSheep;
    std::vector<CAShepherdDog*> m_shepherdDogs;

    // 群れの中心点と半径
    VECTOR3 m_flockCenter;
    float m_flockRadius;
};
