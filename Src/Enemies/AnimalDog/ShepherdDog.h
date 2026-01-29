#pragma once
#include "Dog.h"
class CAShepherdDog : public CADog
{
public:
    CAShepherdDog();
    ~CAShepherdDog();
    
    void Update() override;
    void Draw() override;
};
