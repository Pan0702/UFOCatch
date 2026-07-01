#pragma once
#include "../../Common/Object3D.h"

/// <summary>敵AIで使う Fun Shape の情報と処理をまとめる型</summary>
class CFunShape : public Object3D
{
public:
    /// CFunShape を初期化する
    CFunShape();
    /// CFunShape の終了処理を行う
    ~CFunShape();
    /// Pos Set の処理を行う
    /// @param pos 座標
    /// @param angle 角度
    void PosSet(const VECTOR3& pos, const float angle);
};
