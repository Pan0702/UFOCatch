#pragma once
#include "../../Common/Object3D.h"

/// <summary>敵AIで使う Animal Factor の情報と処理をまとめる型</summary>
class CAnimalFactor : public Object3D
{
public:
    /// CAnimalFactor を初期化する
    /// @param sizeX サイズ
    /// @param sizeZ サイズ
    /// @param type type に渡す値
    CAnimalFactor(float sizeX, float sizeZ, int type);
    /// CAnimalFactor の終了処理を行う
    ~CAnimalFactor();

private:
    /// Normal の処理を行う
    /// @param sizeX サイズ
    /// @param sizeZ サイズ
    static void Normal(float sizeX, float sizeZ);
};
