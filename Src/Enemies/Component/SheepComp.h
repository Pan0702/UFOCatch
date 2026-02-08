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
    VECTOR3 CalculateBoundaryForce() const;  // 中心点への引き寄せと半径制約
    VECTOR3 CalculateWandering();  // ランダムな徘徊行動

    // Wandering用のパラメータ
    VECTOR3 m_wanderTarget;
    float m_wanderTimer;

    // 移動時間管理（一定時間後にIDLEに戻る）
    float m_walkDuration;
    float m_walkTimer;

    // 回転の滑らかさ
    float m_currentRotation;
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