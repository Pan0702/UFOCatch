#pragma once
#include <stack>
#include "StageData.h"

class UndoManager
{
private:
    struct UndoState
    {
        int         index      = -1;
        Transform   trans      = {};
        std::string model_name = {};
        Transform*  target     = nullptr;
    };
    std::stack<UndoState> undo_stack_;
    std::stack<UndoState> redo_stack_;
    StageData* data_;


public:
    UndoManager();
    ~UndoManager() = default;

    /// <summary>現在選択中のオブジェクトのTransformをUndoスタックに積む</summary>
    void Push();
    void Push(Transform* target);
    void DeleteObjectPush();

    /// <summary>直前の操作を取り消し、Redoスタックに現在状態を退避する</summary>
    void Undo();

    /// <summary>取り消した操作をやり直す</summary>
    void Redo();
};
