#pragma once
#include "../Base/EnemyBase.h"
class CSheep : public CEnemyBase
{
public:
    CSheep();
    ~CSheep();
    void Update() override;
    void Draw() override;
    
private:
};
