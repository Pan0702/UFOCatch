#pragma once
#include "../../Framework/SceneBase.h"

/// <summary>シーンで使う Normal Scene の情報と処理をまとめる型</summary>
class CNormalScene : public SceneBase
{
public:
    /// CNormalScene を初期化する
    CNormalScene();
    /// CNormalScene の終了処理を行う
    ~CNormalScene();
    /// 毎フレームの状態を更新する
    void Update() override;
    /// 描画する
    void Draw() override;

private:
};
