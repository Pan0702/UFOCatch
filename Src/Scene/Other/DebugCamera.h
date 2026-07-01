#pragma once
#include "../../Common/Object3D.h"

/// <summary>シーンで使う Debug Camera の情報と処理をまとめる型</summary>
class CDebugCamera : public Object3D
{
public:
    /// CDebugCamera を初期化する
    CDebugCamera();
    /// CDebugCamera の終了処理を行う
    ~CDebugCamera();

private:
    /// 毎フレームの状態を更新する
    void Update() override;
    /// 移動する
    void Move();
    /// 回転する
    void Rotate();
    /// Zoom の処理を行う
    void Zoom();
};
