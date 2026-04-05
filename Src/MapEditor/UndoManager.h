#pragma once
#include <stack>
#include "StageData.h"

//Object3Dを継承しないクラスとして定義
class UndoManager
{
private:
    struct UndoState
    {
        int index = -1;
        Transform trans = {};
        std::string modelName = {};
        Transform* target = nullptr;
    };

    std::stack<UndoState> m_undoStack;
    std::stack<UndoState> m_redoStack;
    StageData* m_pData;

public:
    UndoManager();
    ~UndoManager() = default;

    /// <summary>現在選択中のオブジェクトのTransformをUndoスタックに積む</summary>
    void Push();
    void Push(Transform* target);
    void DeleteObjectPush();

    /// <summary>取り消した操作をやり直し前の状態をRedoスタックに入れる</summary>
    void Undo();

    /// <summary>取り消した操作をやり直す</summary>
    void Redo();
};

struct ICommande
{
    virtual ~ICommande() = default;
    virtual void Undo() = 0;
    virtual void Redo() = 0;
};

class CTransformCommand : public ICommande
{
public:
    CTransformCommand();
    void Undo() override;
    void Redo() override;

private:
    int m_index;
    Transform m_befor;
    Transform m_after;
    StageData* m_pData;
};

class CAddCommand : public ICommande
{
public:
    CAddCommand();
    void Undo() override;
    void Redo() override;

private:
    Transform m_trans;
    StageData* m_pData;
    std::string m_modelName;
};

class CDeleteCommand : public ICommande
{
public:
    CDeleteCommand();
    void Undo() override;
    void Redo() override;
private:
    Transform m_trans;
    StageData* m_pData;
    std::string m_modelName;
};

class CUndoManager
{
public:
    void Push(std::unique_ptr<ICommande> cmd);
    void Undo();
    void Redo();
    static CUndoManager& GetInstance();
private:
    std::stack<std::unique_ptr<ICommande>> m_undoStack;
    std::stack<std::unique_ptr<ICommande>> m_redoStack;
};
