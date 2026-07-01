#pragma once

/// <summary>チュートリアルで使う Tutorial の情報と処理をまとめる型</summary>
class CTutorial;

// チュートリアルのステート基底クラス //
class CTutorialState
{
public:
    // コンストラクタ
    // @param pT チュートリアルオブジェクトへのポインタ //
    /// CTutorialState を初期化する
    /// @param pT pT に渡す値
    CTutorialState(CTutorial* pT);

    /// Enter の処理を行う
    virtual void Enter()
    {
    }

    /// 毎フレームの状態を更新する
    virtual void Update()
    {
    }

    /// Exit の処理を行う
    virtual void Exit()
    {
    }

    // チュートリアルのステート種別 //
    enum class State
    {
        None, // 未設定 //
        Move, // 移動操作 //
        Suction, // 吸い込み操作 //
        Expands, // 拡大操作（複数捕獲） //
        Discovery, // 発見される体験 //
        Play, // 実践プレイ //
    };

protected:
    CTutorial* m_pTutorial; // チュートリアルオブジェクトへのポインタ //

    // プレイヤーに捕獲された動物を削除する //
    /// Caught Animals を削除する
    void RemoveCaughtAnimals();
};

// 移動操作を教えるステート //
class CMoveState : public CTutorialState
{
public:
    // @param pT チュートリアルオブジェクトへのポインタ //
    /// CMoveState を初期化する
    /// @param pT pT に渡す値
    CMoveState(CTutorial* pT);

    /// Enter の処理を行う
    void Enter() override
    {
    }

    /// 毎フレームの状態を更新する
    void Update() override;
    /// Exit の処理を行う
    void Exit() override;
};

// 吸い込み操作を教えるステート //
class CSuctionState : public CTutorialState
{
public:
    // @param pT チュートリアルオブジェクトへのポインタ //
    /// CSuctionState を初期化する
    /// @param pT pT に渡す値
    CSuctionState(CTutorial* pT);

    /// Enter の処理を行う
    void Enter() override
    {
    }

    /// 毎フレームの状態を更新する
    void Update() override;
    /// Exit の処理を行う
    void Exit() override;
};

// 拡大操作（応用）を教えるステート //
class CExpands : public CTutorialState
{
public:
    // @param pT チュートリアルオブジェクトへのポインタ //
    /// CExpands を初期化する
    /// @param pT pT に渡す値
    CExpands(CTutorial* pT);

    /// Enter の処理を行う
    void Enter() override;
    /// 毎フレームの状態を更新する
    void Update() override;
    /// Exit の処理を行う
    void Exit() override;
};

// 発見されることを教えるステート //
class CDiscoveryState : public CTutorialState
{
public:
    // @param pT チュートリアルオブジェクトへのポインタ //
    /// CDiscoveryState を初期化する
    /// @param pT pT に渡す値
    CDiscoveryState(CTutorial* pT);

    /// Enter の処理を行う
    void Enter() override;
    /// 毎フレームの状態を更新する
    void Update() override;
    /// Exit の処理を行う
    void Exit() override;
};

// 実践プレイステート //
class CPlayState : public CTutorialState
{
public:
    // @param pT チュートリアルオブジェクトへのポインタ //
    /// CPlayState を初期化する
    /// @param pT pT に渡す値
    CPlayState(CTutorial* pT);

    /// Enter の処理を行う
    void Enter() override;
    /// 毎フレームの状態を更新する
    void Update() override;
    /// Exit の処理を行う
    void Exit() override
    {
    }
};
