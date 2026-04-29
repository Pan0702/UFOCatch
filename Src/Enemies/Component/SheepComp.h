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
    VECTOR3 CalcBoids() const;
    VECTOR3 CalcEscapeFromDog() const;
    VECTOR3 CalcWandering(); // ランダムな徘徊行動
    VECTOR3 CalcRetrunToFlock();


    CSheep* m_pOwner;

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
    void UpdateDirection();
    void UpdateMovement();
    void CheckBoundaryAndTransition();

    CSheep* m_pOwner;
    VECTOR3 m_panicDirection; // ランダムな逃げる方向
    float m_changeDirectionTimer = 0.0f;
    float m_panicDuration = 0.0f;
    float m_panicTimer = 0.0f;
    VECTOR3 m_startPos;
};
