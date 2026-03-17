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
        APPROACH_SHEEP, // 繝輔ぉ繝ｼ繧ｺ1: 鄒翫↓霑代▼縺・
        GUIDE_TO_CENTER // 繝輔ぉ繝ｼ繧ｺ2: 驥榊ｿ・∪縺ｧ隱伜ｰ・
    };

    CAShepherdDog* m_pOwner;
    CSheep* m_targetSheep = nullptr; // 謨大勧蟇ｾ雎｡縺ｮ鄒・
    VECTOR3 m_centroid = {0, 0, 0}; // 鄒､繧後・驥榊ｿ・
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
