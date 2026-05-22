#pragma once
#include "../../Framework/SceneBase.h"
#include "../../Utils/WipeAnimator.h"

/// <summary>シーンで使う Selection Scene の情報と処理をまとめる型</summary>
class CSelectionScene : public SceneBase
{
public:
    /// CSelectionScene を初期化する
    CSelectionScene();
    /// CSelectionScene の終了処理を行う
    ~CSelectionScene();

private:
    /// 毎フレームの状態を更新する
    void Update() override;
    /// 描画する
    void Draw() override;
};
