#pragma once
#include "ComponentBase.h"

class CSuction : public CComponentBase
{
public:
    CSuction(CEnemyBase* e);
    void Enter() override;
    void Update() override;
    
    bool IsFinishSuction() const{return m_isFinishSuction;}
private:
    bool m_isFinishSuction;
};
