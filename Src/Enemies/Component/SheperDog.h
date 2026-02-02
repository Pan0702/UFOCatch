#pragma once
#include "ComponentBase.h"
#include "../AnimalDog/ShepherdDog.h"

class CCollectiong : public CComponentBase
{
public:
    CCollectiong(CAShepherdDog* dog, float speed);
    void Enter() override;
    void Update() override;


private:
    CAShepherdDog* m_pOwner;
    VECTOR3 m_targetPos;
    float m_moveSpeed;
   
};

class CDriving : public CComponentBase
{
public:
    CDriving(CAShepherdDog* dog, float speed);
    void Enter() override;
    void Update() override;

private:
    CAShepherdDog* m_pOwner;
    VECTOR3 m_targetPos = { 0,0,0 };
    float m_moveSpeed;
    float m_interceptDistance = 15.0f;
};