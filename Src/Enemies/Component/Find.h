#pragma once
#include "ComponentBase.h"
#include "../Human/Human.h"

class CFind : public CComponentBase
{
public:
    CFind(CHuman* human);
    ~CFind() = default;

    void Enter() override;
    void Update() override;


private:
    CHuman* m_pOwner;
};

