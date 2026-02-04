#pragma once
#include "ComponentBase.h"
#include "Destroy.h"
#include "../AnimalDog/ShepherdDog.h"

class CCollecting : public CComponentBase
{
public:
    CCollecting(CAShepherdDog* dog, float speed);
    void Enter() override;
    void Update() override;

private:
    CAShepherdDog* m_pOwner;
    VECTOR3 m_targetPos = {0, 0, 0};
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
    VECTOR3 m_targetPos = {0, 0, 0};
    VECTOR3 m_destination = {0, 0, 0};
    float m_moveSpeed;
};

class CRescue : public CComponentBase
{
public:
    CRescue(CAShepherdDog* dog);
    void Enter() override;
    void Update() override;

private:
    enum Phase
    {
        APPROACH_SHEEP, // フェーズ1: 羊に近づく
        GUIDE_TO_CENTER // フェーズ2: 重心まで誘導
    };

    CAShepherdDog* m_pOwner;
    CSheep* m_targetSheep = nullptr; // 救助対象の羊
    VECTOR3 m_centroid = {0, 0, 0}; // 群れの重心
    Phase m_phase;
};

class CDestroyShepherdDog : public CDestroy
{
public:
    CDestroyShepherdDog(CAShepherdDog* dog, int score, float exp);
    void Enter() override;

private:
    CAShepherdDog* m_pDog;
};
