#include "UndoManager.h"

#include "TRSObject/TRS.h"

UndoManager::UndoManager()
{
    m_pData = ObjectManager::FindGameObject<StageData>();
}

// 迴ｾ蝨ｨ驕ｸ謚樔ｸｭ縺ｮ繧ｪ繝悶ず繧ｧ繧ｯ繝医・Transform繧旦ndo繧ｹ繧ｿ繝・け縺ｫ遨阪・
void UndoManager::Push()
{
    int index = m_pData->GetSelectIndex();
    UndoState state;
    state.index = index;
    state.trans = *m_pData->GetSelectedTransform();;
    m_undoStack.push(state);
    // 譁ｰ縺励＞謫堺ｽ懊ｒ縺励◆繧・Redo 螻･豁ｴ縺ｯ辟｡蜉ｹ縺ｫ縺ｪ繧九◆繧√け繝ｪ繧｢
    m_redoStack = std::stack<UndoState>();
}

void UndoManager::Push(Transform* target)
{
    if (target == nullptr) return;
    UndoState state;
    state.index  = -2;         // StageData邂｡逅・､悶・繝輔Λ繧ｰ
    state.trans  = *target;
    state.target = target;
    m_undoStack.push(state);
    m_redoStack = std::stack<UndoState>();
}

// 迴ｾ蝨ｨ驕ｸ謚樔ｸｭ縺ｮ繧ｪ繝悶ず繧ｧ繧ｯ繝医・Transform繧旦ndo繧ｹ繧ｿ繝・け縺ｫ遨阪・
void UndoManager::DeleteObjectPush()
{
    int index = m_pData->GetSelectIndex();
    UndoState state;
    state.index = -1;
    state.trans = *m_pData->GetSelectedTransform();
    state.modelName = m_pData->GetStageDataInfo()[index].modelName;
    m_undoStack.push(state);
    // 譁ｰ縺励＞謫堺ｽ懊ｒ縺励◆繧・Redo 螻･豁ｴ縺ｯ辟｡蜉ｹ縺ｫ縺ｪ繧九◆繧√け繝ｪ繧｢
    m_redoStack = std::stack<UndoState>();
}

// 逶ｴ蜑阪・謫堺ｽ懊ｒ蜿悶ｊ豸医＠縲∫樟蝨ｨ迥ｶ諷九ｒRedo繧ｹ繧ｿ繝・け縺ｫ騾驕ｿ縺吶ｋ
void UndoManager::Undo()
{
    if (m_undoStack.empty()) return;
    UndoState prev_state = m_undoStack.top();
    // Redo 縺ｧ縺阪ｋ繧医≧縲ゞndo繧帝←逕ｨ縺吶ｋ蜑阪↓迴ｾ蝨ｨ迥ｶ諷九ｒ Redo 繧ｹ繧ｿ繝・け縺ｸ騾驕ｿ
    UndoState state;
    if (prev_state.index >= 0)
    {
        state.index = m_pData->GetSelectIndex();
        state.trans = *m_pData->GetSelectedTransform();
        state.modelName = "n";
        m_redoStack.push(state);
    }


    // Undo 繧ｹ繧ｿ繝・け縺ｮ蜈磯ｭ繧貞叙繧雁・縺励※驕ｩ逕ｨ
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

// 蜿悶ｊ豸医＠縺滓桃菴懊ｒ繧・ｊ逶ｴ縺・
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

