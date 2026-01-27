#pragma once
#include "ComponentBase.h"

class Suction : public CComponentBase
{
public:
    Suction();
    Suction(CEnemyBase* e);
    ~Suction();
    void Enter() override;
    void Update() override;
    void Exit() override;
private:
};
