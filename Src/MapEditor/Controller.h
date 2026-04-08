#pragma once
#include "Camera.h"
#include "RandomPlacer.h"
#include "UndoManager.h"
#include "../Common/Object3D.h"
#include "TRSObject/TRS.h"

class Controller : public Object3D
{
private:
    TRS*                            m_pTrs;
    CDirectInput*                   m_pInput;
    StageData*                      m_pStageData;
    std::unique_ptr<CUndoManager>    m_pUndoManager;
    std::unique_ptr<RandomPlacer>   m_pRandomPlacer;
    bool                            m_isCatch = false;
    bool                            m_isRandomPlacer = false;
    int                             m_copyObjectIndex = -1;

private:
    void Update() override;
    void Draw() override;

    /// <summary>W/E/R/Qキーでアクティブなギズモモードを切り替える</summary>
    void TRSControl() const;

    /// <summary>右クリック中のマウス移動・キー入力でカメラを操作する</summary>
    void CameraControl() const;

    /// <summary>左クリック時にTRSギズモまたはステージオブジェクトへのレイ判定を行う</summary>
    void HandleLeftClick();

    /// <summary>Ctrl+Z/Ctrl+YでUndo/Redoを実行する</summary>
    void HandleUndoRedo() const;
    void Random();


public:
    Controller();
    ~Controller() = default;

    void SetCatchFlag(bool f);

    /// <summary>"Setting"ウィンドウのRandom Placer部分を描画する（Begin/Endなし）</summary>
    void DrawSettingPanel();

    /// <summary>"Transform"ウィンドウの中身を描画する（Begin/Endなし）</summary>
    void DrawTransformPanel();

    /// <summary>オブジェクトが選択されていてTransformウィンドウを表示すべきか返す</summary>
    bool HasSelectedObject() const;
};
