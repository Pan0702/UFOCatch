#pragma once
#include "../Common/Object3D.h"

namespace Camera
{
    /// Focus の処理を行う
    void Focus();

    /// 移動する
    void Move();

    /// 回転する
    void Rotate();

    /// Zoom の処理を行う
    void Zoom();

    /// Orbit の処理を行う
    void Orbit();

    /// Pan の処理を行う
    void Pan();

    /// Dolly の処理を行う
    void Dolly();
};
