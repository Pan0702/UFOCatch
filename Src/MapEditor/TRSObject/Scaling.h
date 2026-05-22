#pragma once
#include "TRSBase.h"

/// <summary>ステージエディタで使う Scaling の情報と処理をまとめる型</summary>
class CScaling : public TRSBase
{
private:
    /// Meshes を初期化する
    void InitMeshes();

public:
    /// CScaling を初期化する
    CScaling();
    /// CScaling の終了処理を行う
    ~CScaling();
    /// 描画する
    void Draw() override
    {
    }
};
