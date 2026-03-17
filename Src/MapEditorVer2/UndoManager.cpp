#include "UndoManager.h"

#include "TRSObject/TRS.h"

UndoManager::UndoManager()
{
    data_ = ObjectManager::FindGameObject<StageData>();
}

// 現在選択中のオブジェクトのTransformをUndoスタックに積む
void UndoManager::Push()
{
    int index = data_->GetSelectIndex();
    UndoState state;
    state.index = index;
    state.trans = *data_->GetSelectedTransform();;
    undo_stack_.push(state);
    // 新しい操作をしたら Redo 履歴は無効になるためクリア
    redo_stack_ = std::stack<UndoState>();
}

void UndoManager::Push(Transform* target)
{
    if (target == nullptr) return;
    UndoState state;
    state.index  = -2;         // StageData管理外のフラグ
    state.trans  = *target;
    state.target = target;
    undo_stack_.push(state);
    redo_stack_ = std::stack<UndoState>();
}

// 現在選択中のオブジェクトのTransformをUndoスタックに積む
void UndoManager::DeleteObjectPush()
{
    int index = data_->GetSelectIndex();
    UndoState state;
    state.index = -1;
    state.trans = *data_->GetSelectedTransform();
    state.model_name = data_->GetStageDataInfo()[index].model_name_;
    undo_stack_.push(state);
    // 新しい操作をしたら Redo 履歴は無効になるためクリア
    redo_stack_ = std::stack<UndoState>();
}

// 直前の操作を取り消し、現在状態をRedoスタックに退避する
void UndoManager::Undo()
{
    if (undo_stack_.empty()) return;
    UndoState prev_state = undo_stack_.top();
    // Redo できるよう、Undoを適用する前に現在状態を Redo スタックへ退避
    UndoState state;
    if (prev_state.index >= 0)
    {
        state.index = data_->GetSelectIndex();
        state.trans = *data_->GetSelectedTransform();
        state.model_name = "n";
        redo_stack_.push(state);
    }


    // Undo スタックの先頭を取り出して適用
    undo_stack_.pop();

    if (prev_state.index >= 0)
    {
        data_->SetSelectedTransform(prev_state.index, prev_state.trans);
    }
    else
    {
        data_->AddModel(prev_state.trans, prev_state.model_name);
        state.index = data_->GetSelectIndex();
        state.model_name = "d";
        redo_stack_.push(state);
    }
}

// 取り消した操作をやり直す
void UndoManager::Redo()
{
    if (redo_stack_.empty()) return;
    UndoState state = redo_stack_.top();
    if (state.model_name == "d")
    {
        data_->DeleteModel(state.index);
    }
    else
    {
        data_->SetSelectedTransform(state.index, state.trans);
    }
    redo_stack_.pop();
}
