#pragma once
#include "../Common/Object3D.h"

namespace Camera
{
    /// <summary>選択オブジェクトにカメラをフォーカスする</summary>
    void Focus();

    /// <summary>WASDキーでカメラを前後左右に平行移動する</summary>
    void Move();

    /// <summary>マウス移動量に応じてカメラの注視点を回転する</summary>
    void Rotate();

    /// <summary>マウスホイールでカメラを前後方向にズームする</summary>
    void Zoom();

    /// <summary>Alt+左ドラッグで注視点を中心にカメラを回転する</summary>
    void Orbit();

    /// <summary>Alt+中ドラッグでカメラを平行移動する</summary>
    void Pan();

    /// <summary>Alt+右ドラッグでカメラを前後移動する</summary>
    void Dolly();
};
