#pragma once
#include "TRSBase.h"

/// <summary>ステージエディタで使う Translation の情報と処理をまとめる型</summary>
class CTranslation : public TRSBase
{
private:
    /// Meshes を初期化する
    void InitMeshes();
    /// 描画する
    void Draw() override
    {
    }

public:
    /// CTranslation を初期化する
    CTranslation();
};
