#pragma once
#include "../05_CommonFile/Object3D.h"

class CTutorial;

class CTutorialState 
{
public:
    CTutorialState(CTutorial* pT);

    virtual void Enter(){}
    virtual void Update(){}
    virtual void Exit(){}

    enum class State
    {
        None,
        Move,
        Suction,
        Expands,
        Discovery,
        Play,
    };

protected:
    CTutorial* m_pTutorial;
    void RemoveCaughtAnimals();
};

class CMoveState : public CTutorialState
{
public:
    CMoveState(CTutorial* pT);

    void Enter() override{}
    void Update() override;
    void Exit() override{}
};

class CSuctionState : public CTutorialState
{
public:
    CSuctionState(CTutorial* pT);
    
    void Enter() override{}
    void Update() override;
    void Exit() override{}
};

class CExpands : public CTutorialState
{
public:
    CExpands(CTutorial* pT);

    void Enter() override;
    void Update() override;
    void Exit() override{}
};

class CDiscoveryState : public CTutorialState
{
public:
    CDiscoveryState(CTutorial* pT);

    void Enter() override;
    void Update() override;
    void Exit() override{}
};

class CPlayState : public CTutorialState
{
public:
    CPlayState(CTutorial* pT);
    
    void Enter() override;
    void Update() override;
    void Exit() override{}
};