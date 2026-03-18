#pragma once
#include "Camera.h"
#include "RandomPlacer.h"
#include "UndoManager.h"
#include "../Common/Object3D.h"
#include "TRSObject/TRS.h"

class Controller : public Object3D
{
private:
    Camera*                         m_pCamera;
    TRS*                            m_pTrs;
    CDirectInput*                   m_pInput;
    StageData*                      m_pStageData;
    std::unique_ptr<UndoManager>    m_pUndoManager;
    std::unique_ptr<RandomPlacer>   m_pRandomPlacer = nullptr;
    bool                            m_isCatch = false;
    bool                            m_isRandomPlacer = false;
    int                             m_copyObjectIndex = -1;

private:
    void Update() override;
    void Draw() override;

    /// <summary>W/E/R/Q繧ｭ繝ｼ縺ｧ繧｢繧ｯ繝・ぅ繝悶↑繧ｮ繧ｺ繝｢繝｢繝ｼ繝峨ｒ蛻・ｊ譖ｿ縺医ｋ</summary>
    void TRSControl() const;

    /// <summary>蜿ｳ繧ｯ繝ｪ繝・け荳ｭ縺ｮ繝槭え繧ｹ遘ｻ蜍輔・繧ｭ繝ｼ蜈･蜉帙〒繧ｫ繝｡繝ｩ繧呈桃菴懊☆繧・/summary>
    void CameraControl() const;

    /// <summary>蟾ｦ繧ｯ繝ｪ繝・け譎ゅ↓TRS繧ｮ繧ｺ繝｢縺ｾ縺溘・繧ｹ繝・・繧ｸ繧ｪ繝悶ず繧ｧ繧ｯ繝医∈縺ｮ繝ｬ繧､蛻､螳壹ｒ陦後≧</summary>
    void HandleLeftClick();

    /// <summary>Ctrl+Z/Ctrl+Y縺ｧUndo/Redo繧貞ｮ溯｡後☆繧・/summary>
    void HandleUndoRedo() const;
    void Random();


public:
    Controller();
    ~Controller() = default;
    
    void SetCatchFlag(bool f);
};

