#pragma once
#include "Dog.h"
#include "../AnimalSheep/Sheep.h"
#include "../System/Flog.h"

class CAShepherdDog : public CADog
{
public:
    CAShepherdDog();
    ~CAShepherdDog();
    
    void Update() override;
    void ChangeStateHerded(const CSheep* sheep) const;
    void Draw() override;
    void ChangeStateHerded(const CSheep* sheep);
    void Collecting(const FlogInfo& flogInfo);
    void Driving(const FlogInfo& flogInfo);
    void StartHerding();
    
private:
    std::vector<CSheep*> m_sheeps;
    LerpValue m_lerpValue;
    bool m_isHerding = false;
};
    