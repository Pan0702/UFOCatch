#include "UndoManager.h"

#include "TRSObject/TRS.h"

ICommande::~ICommande()
{
    SAFE_DELETE(m_pData);
}

CTransformCommand::CTransformCommand()
{
    m_pData = ObjectManager::FindGameObject<StageData>();
    m_index = m_pData->GetSelectIndex();
    m_befor = *m_pData->GetSelectedTransform();
}

void CTransformCommand::Undo()
{
    m_after = *m_pData->GetSelectedTransform();
    m_pData->SetSelectedTransform(m_index, m_befor);
}

void CTransformCommand::Redo()
{
    m_pData->SetSelectedTransform(m_index, m_after);
}

CAddCommand::CAddCommand()
{
    m_pData = ObjectManager::FindGameObject<StageData>();
    m_modelName = m_pData->GetModelName();
    m_trans = m_pData->GetTrans();
    m_index = m_pData->GetSelectIndex();
}

void CAddCommand::Undo()
{
    m_pData->DeleteModel(m_index);
}

void CAddCommand::Redo()
{
    m_pData->AddModel(m_trans,m_modelName);
    m_index = m_pData->GetSelectIndex();
}

CDeleteCommand::CDeleteCommand()
{
    m_pData = ObjectManager::FindGameObject<StageData>();
    m_modelName = m_pData->GetModelName();
    m_trans = m_pData->GetTrans();
}

void CDeleteCommand::Undo()
{
    m_index = m_pData->AddModel(m_trans,m_modelName);
}

void CDeleteCommand::Redo()
{
    m_pData->DeleteModel(m_index);
}

CRawTransformCommand::CRawTransformCommand(Transform* target)
    : m_target(target), m_befor(*target)
{
}

void CRawTransformCommand::SetAfter()
{
    m_after = m_befor;
    m_befor = *m_target;
}

void CRawTransformCommand::Undo()
{
    *m_target = m_befor;
}

void CRawTransformCommand::Redo()
{
    *m_target = m_after;
}

void CUndoManager::Push(std::unique_ptr<ICommande> cmd)
{
    m_redoStack = {};
    m_undoStack.push(std::move(cmd));
}

void CUndoManager::Undo()
{
    if (m_undoStack.empty()) return;
    auto u = std::move(m_undoStack.top());
    m_undoStack.pop(); 
    u->Undo();
    m_redoStack.push(std::move(u)); 
}

void CUndoManager::Redo()
{
    if (m_redoStack.empty()) return;
    auto r = std::move(m_redoStack.top());
    m_redoStack.pop();
    r->Redo();
    m_undoStack.push(std::move(r));
}

