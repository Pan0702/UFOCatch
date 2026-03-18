#include "UndoManager.h"

#include "TRSObject/TRS.h"

UndoManager::UndoManager()
{
    m_pData = ObjectManager::FindGameObject<StageData>();
}

// 現在選択中のオブジェクトのTransformをUndoスタックに積む
void UndoManager::Push()
{
    int index = m_pData->GetSelectIndex();
    UndoState state;
    state.index = index;
    state.trans = *m_pData->GetSelectedTransform();;
    m_undoStack.push(state);
    // 新しい操作をしたら Redo 履歴は無効になるためクリア
    m_redoStack = std::stack<UndoState>();
}

void UndoManager::Push(Transform* target)
{
    if (target == nullptr) return;
    UndoState state;
    state.index  = -2;         // StageData管理外のフラグ
    state.trans  = *target;
    state.target = target;
    m_undoStack.push(state);
    m_redoStack = std::stack<UndoState>();
}

// 現在選択中のオブジェクトのTransformをUndoスタックに積む
void UndoManager::DeleteObjectPush()
{
    int index = m_pData->GetSelectIndex();
    UndoState state;
    state.index = -1;
    state.trans = *m_pData->GetSelectedTransform();
    state.modelName = m_pData->GetStageDataInfo()[index].modelName;
    m_undoStack.push(state);
    // 新しい操作をしたら Redo 履歴は無効になるためクリア
    m_redoStack = std::stack<UndoState>();
}

// 直前の操作を取り消し、現在状態をRedoスタックに退避する
void UndoManager::Undo()
{
    if (m_undoStack.empty()) return;
    UndoState prev_state = m_undoStack.top();
    // Redo できるよう、Undoを適用する前に現在状態を Redo スタックへ退避
    UndoState state;
    if (prev_state.index >= 0)
    {
        state.index = m_pData->GetSelectIndex();
        state.trans = *m_pData->GetSelectedTransform();
        state.modelName = "n";
        m_redoStack.push(state);
    }


    // Undo スタックの先頭を取り出して適用
    m_undoStack.pop();

    if (prev_state.index >= 0)
    {
        m_pData->SetSelectedTransform(prev_state.index, prev_state.trans);
    }
    else
    {
        m_pData->AddModel(prev_state.trans, prev_state.modelName);
        state.index = m_pData->GetSelectIndex();
        state.modelName = "d";
        m_redoStack.push(state);
    }
}

// 取り消した操作をやり直す
void UndoManager::Redo()
{
    if (m_redoStack.empty()) return;
    UndoState state = m_redoStack.top();
    if (state.modelName == "d")
    {
        m_pData->DeleteModel(state.index);
    }
    else
    {
        m_pData->SetSelectedTransform(state.index, state.trans);
    }
    m_redoStack.pop();
}
