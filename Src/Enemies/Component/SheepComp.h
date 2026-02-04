#pragma once
#include "ComponentBase.h"
#include "../AnimalSheep/Sheep.h"
class CHerded : public CComponentBase
{
public:
    CHerded(CSheep* sheep);
    void Enter() override;
    void Update() override;

private:
    CSheep* m_pOwner;
    VECTOR3 CalculateBoids() const;
    VECTOR3 CalculateEscapeFromDog() const;
};


class CPanic : public CComponentBase
{
public:
    CPanic(CSheep* sheep);
    void Enter() override;
    void Update() override;

private:
    CSheep* m_pOwner;
    VECTOR3 m_panicDirection;  // ランダムな逃げる方向
    float m_changeDirectionTimer = 0.0f;

};