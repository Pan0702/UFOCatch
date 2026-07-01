#pragma once
#include "TRSBase.h"

/// <summary>ステージエディタで使う Rotation の情報と処理をまとめる型</summary>
class CRotation : public TRSBase
{
private:
    /// Meshes を初期化する
    void InitMeshes();

public:
    /// CRotation を初期化する
    CRotation();
    /// CRotation の終了処理を行う
    ~CRotation();
    /// 描画する
    void Draw() override
    {
    }
};
