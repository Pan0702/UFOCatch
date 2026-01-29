#pragma once
#include "ComponentBase.h"

class CDestroy : public CComponentBase
{
public:
    CDestroy(CEnemyBase* e, int score, float exp);

    void Enter() override;private:
    int m_score;
    float m_exp;
};
