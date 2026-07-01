#pragma once
#include "Camera.h"
#include "RandomPlacer.h"
#include "UndoManager.h"
#include "../Common/Object3D.h"
#include "TRSObject/TRS.h"

/// <summary>ステージエディタで使う Controller の情報と処理をまとめる型</summary>
class Controller : public Object3D
{
private:
    TRS* m_pTrs;
    CDirectInput* m_pInput;
    StageData* m_pStageData;
    std::unique_ptr<CUndoManager> m_pUndoManager;
    std::unique_ptr<RandomPlacer> m_pRandomPlacer;
    bool m_isCatch = false;
    bool m_isRandomPlacer = false;
    int m_copyObjectIndex = -1;

private:
    /// 毎フレームの状態を更新する
    void Update() override;
    /// 描画する
    void Draw() override;

    /// TRSControl の処理を行う
    void TRSControl() const;

    /// Camera Control の処理を行う
    void CameraControl() const;

    /// Left Click を処理する
    void HandleLeftClick();

    /// Undo Redo を処理する
    void HandleUndoRedo() const;
    /// Random の処理を行う
    void Random();

public:
    /// Controller を初期化する
    Controller();
    /// Controller の終了処理を行う
    ~Controller() = default;

    /// Catch Flag を設定する
    /// @param f f に渡す値
    void SetCatchFlag(bool f);

    /// Collision Box を描画する
    void DrawCollisionBox();
    /// Setting Panel を描画する
    void DrawSettingPanel();

    /// Transform Panel を描画する
    void DrawTransformPanel();

    /// Selected Object を保持しているか判定する
    /// @return 成功または条件を満たす場合 true
    bool HasSelectedObject() const;
};
