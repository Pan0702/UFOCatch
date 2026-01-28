#pragma once
#include "ComponentBase.h"

class CSuction : public CComponentBase
{
public:
    CSuction(CEnemyBase* e);
    void Enter() override;
    void Update() override;
    void Exit() override;
private:
};
