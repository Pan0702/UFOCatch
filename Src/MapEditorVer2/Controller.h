#pragma once
#include "Camera.h"
#include "RandomPlacer.h"
#include "UndoManager.h"
#include "../Object3D.h"
#include "TRSObject/TRS.h"

class Controller : public Object3D
{
private:
    Camera*                         camera_;
    TRS*                            trs_;
    CDirectInput*                   input_;
    UndoManager*                    undo_manager_;
    StageData*                      stage_data_;
    RandomPlacer*                   random_placer_ = nullptr;
    bool                            is_catch_ = false;
    bool                            is_random_placer_ = false;
    int                             copy_object_index_ = -1;

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
};
