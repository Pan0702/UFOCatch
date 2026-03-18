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
        std::string modelName = {};
        Transform*  target     = nullptr;
    };
    std::stack<UndoState> m_undoStack;
    std::stack<UndoState> m_redoStack;
    StageData* m_pData;


public:
    UndoManager();
    ~UndoManager() = default;

    /// <summary>迴ｾ蝨ｨ驕ｸ謚樔ｸｭ縺ｮ繧ｪ繝悶ず繧ｧ繧ｯ繝医・Transform繧旦ndo繧ｹ繧ｿ繝・け縺ｫ遨阪・</summary>
    void Push();
    void Push(Transform* target);
    void DeleteObjectPush();

    /// <summary>逶ｴ蜑阪・謫堺ｽ懊ｒ蜿悶ｊ豸医＠縲ヽedo繧ｹ繧ｿ繝・け縺ｫ迴ｾ蝨ｨ迥ｶ諷九ｒ騾驕ｿ縺吶ｋ</summary>
    void Undo();

    /// <summary>蜿悶ｊ豸医＠縺滓桃菴懊ｒ繧・ｊ逶ｴ縺・/summary>
    void Redo();
};

