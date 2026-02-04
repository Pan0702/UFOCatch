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

class CRescue : public CComponentBase
{
public:
    CRescue(CAShepherdDog* dog);
    void Enter() override;
    void Update() override;

private:
    enum Phase {
        APPROACH_SHEEP,   // フェーズ1: 羊に近づく
        GUIDE_TO_CENTER   // フェーズ2: 重心まで誘導
    };
    
    CAShepherdDog* m_pOwner;
    CSheep* m_targetSheep;      // 救助対象の羊
    VECTOR3 m_centroid;         // 群れの重心
    Phase m_phase;
    
    float m_moveSpeed = 2.5f;
    float m_approachDistance = 2.0f;  // 羊に近づく距離
    float m_arrivalDistance = 1.0f;   // 重心到達判定距離
};

class CDestroyShepherdDog : public CDestroy
{
public:
    CDestroyShepherdDog(CAShepherdDog* dog, int score, float exp);
    void Enter() override;
    
    
private:
    CAShepherdDog* m_pDog;
};