#pragma once
#include "ComponentBase.h"
#include "../Human/Human.h"

class CFind : public CComponentBase
{
public:
    CFind(CHuman* human);
    ~CFind();

    void Enter() override;
    void Update() override;

    void Exit() override
    {
    }

private:
    CHuman* m_pOwner;
};
