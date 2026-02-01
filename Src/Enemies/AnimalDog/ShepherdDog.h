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
    void Draw() override;
    void ChangeStateHerded(CSheep* sheep);
    void Collecting(const FlogInfo& flogInfo);
    void Driving(const FlogInfo& flogInfo);
    
private:

    std::vector<CSheep*> sheeps;
};
    