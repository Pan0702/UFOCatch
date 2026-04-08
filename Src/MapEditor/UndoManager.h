#pragma once
#include <stack>
#include "StageData.h"

//Object3Dを継承しないクラスとして定義

struct ICommande
{
    virtual ~ICommande();
    virtual void Undo() = 0;
    virtual void Redo() = 0;

    StageData* m_pData;
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
};

class CAddCommand : public ICommande
{
public:
    CAddCommand();
    void Undo() override;
    void Redo() override;

private:
    Transform m_trans;
    std::string m_modelName;
    int m_index;
};

class CDeleteCommand : public ICommande
{
public:
    CDeleteCommand();
    void Undo() override;
    void Redo() override;

private:
    Transform m_trans;
    std::string m_modelName;
    int m_index;
};

class CRawTransformCommand : public ICommande
{
public:
    explicit CRawTransformCommand(Transform* target);
    void SetAfter();
    void Undo() override;
    void Redo() override;

private:
    Transform* m_target;
    Transform m_befor;
    Transform m_after;
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
