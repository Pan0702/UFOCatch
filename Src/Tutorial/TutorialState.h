#pragma once

class CTutorial;

// チュートリアルのステート基底クラス //
class CTutorialState
{
public:
    // コンストラクタ
    // @param pT チュートリアルオブジェクトへのポインタ //
    CTutorialState(CTutorial* pT);

    virtual void Enter(){}
    virtual void Update(){}
    virtual void Exit(){}

    // チュートリアルのステート種別 //
    enum class State
    {
        None,       // 未設定 //
        Move,       // 移動操作 //
        Suction,    // 吸い込み操作 //
        Expands,    // 拡大操作（複数捕獲） //
        Discovery,  // 発見される体験 //
        Play,       // 実践プレイ //
    };

protected:
    CTutorial* m_pTutorial;  // チュートリアルオブジェクトへのポインタ //

    // プレイヤーに捕獲された動物を削除する //
    void RemoveCaughtAnimals();
};

// 移動操作を教えるステート //
class CMoveState : public CTutorialState
{
public:
    // @param pT チュートリアルオブジェクトへのポインタ //
    CMoveState(CTutorial* pT);

    void Enter() override{}
    void Update() override;
    void Exit() override;
};

// 吸い込み操作を教えるステート //
class CSuctionState : public CTutorialState
{
public:
    // @param pT チュートリアルオブジェクトへのポインタ //
    CSuctionState(CTutorial* pT);

    void Enter() override{}
    void Update() override;
    void Exit() override;
};

// 拡大操作（応用）を教えるステート //
class CExpands : public CTutorialState
{
public:
    // @param pT チュートリアルオブジェクトへのポインタ //
    CExpands(CTutorial* pT);

    void Enter() override;
    void Update() override;
    void Exit() override;
};

// 発見されることを教えるステート //
class CDiscoveryState : public CTutorialState
{
public:
    // @param pT チュートリアルオブジェクトへのポインタ //
    CDiscoveryState(CTutorial* pT);

    void Enter() override;
    void Update() override;
    void Exit() override;
};

// 実践プレイステート //
class CPlayState : public CTutorialState
{
public:
    // @param pT チュートリアルオブジェクトへのポインタ //
    CPlayState(CTutorial* pT);

    void Enter() override;
    void Update() override;
    void Exit() override{}
};