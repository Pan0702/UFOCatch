#pragma once
#include <stack>
#include "StageData.h"

//Object3Dを継承しないクラスとして定義

struct ICommande
{
    /// ICommande の終了処理を行う
    virtual ~ICommande();
    /// Undo の処理を行う
    virtual void Undo() = 0;
    /// Redo の処理を行う
    virtual void Redo() = 0;

    StageData* m_pData;
};

/// <summary>ステージエディタで使う Transform Command の情報と処理をまとめる型</summary>
class CTransformCommand : public ICommande
{
public:
    /// CTransformCommand を初期化する
    CTransformCommand();
    /// Undo の処理を行う
    void Undo() override;
    /// Redo の処理を行う
    void Redo() override;

private:
    int m_index;
    Transform m_befor;
    Transform m_after;
};

/// <summary>ステージエディタで使う Add Command の情報と処理をまとめる型</summary>
class CAddCommand : public ICommande
{
public:
    /// CAddCommand を初期化する
    CAddCommand();
    /// Undo の処理を行う
    void Undo() override;
    /// Redo の処理を行う
    void Redo() override;

private:
    Transform m_trans;
    std::string m_modelName;
    int m_index;
};

/// <summary>ステージエディタで使う Delete Command の情報と処理をまとめる型</summary>
class CDeleteCommand : public ICommande
{
public:
    /// CDeleteCommand を初期化する
    CDeleteCommand();
    /// Undo の処理を行う
    void Undo() override;
    /// Redo の処理を行う
    void Redo() override;

private:
    Transform m_trans;
    std::string m_modelName;
    int m_index;
};

/// <summary>ステージエディタで使う Raw Transform Command の情報と処理をまとめる型</summary>
class CRawTransformCommand : public ICommande
{
public:
    /// CRawTransformCommand を初期化する
    /// @param target 対象
    explicit CRawTransformCommand(Transform* target);
    /// After を設定する
    void SetAfter();
    /// Undo の処理を行う
    void Undo() override;
    /// Redo の処理を行う
    void Redo() override;

private:
    Transform* m_target;
    Transform m_befor;
    Transform m_after;
};


/// <summary>ステージエディタで使う Undo Manager の情報と処理をまとめる型</summary>
class CUndoManager
{
public:
    /// 追加する
    /// @param cmd cmd に渡す値
    void Push(std::unique_ptr<ICommande> cmd);
    /// Undo の処理を行う
    void Undo();
    /// Redo の処理を行う
    void Redo();
    /// Instance を取得する
    /// @return 処理結果
    static CUndoManager& GetInstance();

private:
    std::stack<std::unique_ptr<ICommande>> m_undoStack;
    std::stack<std::unique_ptr<ICommande>> m_redoStack;
};
