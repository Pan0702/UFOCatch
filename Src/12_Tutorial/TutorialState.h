#pragma once
#include "../05_CommonFile/Object3D.h"

class CTutorial;

// チュートリアルのステート基底クラス //
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

// 移動操作を教えるステート //
class CMoveState : public CTutorialState
{
public:
    CMoveState(CTutorial* pT);

    void Enter() override{}
    void Update() override;
    void Exit() override{}
};

// 吸い込み操作を教えるステート //
class CSuctionState : public CTutorialState
{
public:
    CSuctionState(CTutorial* pT);

    void Enter() override{}
    void Update() override;
    void Exit() override{}
};

// 拡大操作を教えるステート //
class CExpands : public CTutorialState
{
public:
    CExpands(CTutorial* pT);

    void Enter() override;
    void Update() override;
    void Exit() override{}
};

// 発見されることを教えるステート //
class CDiscoveryState : public CTutorialState
{
public:
    CDiscoveryState(CTutorial* pT);

    void Enter() override;
    void Update() override;
    void Exit() override{}
};

// 実践プレイステート //
class CPlayState : public CTutorialState
{
public:
    CPlayState(CTutorial* pT);

    void Enter() override;
    void Update() override;
    void Exit() override{}
};