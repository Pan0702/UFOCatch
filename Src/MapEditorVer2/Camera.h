#pragma once
#include "../Common/Object3D.h"
class Camera : public Object3D
{
public:
    Camera();
    ~Camera();
    void Update() override;

    /// <summary>選択オブジェクトにカメラをフォーカスする</summary>
    static void Focus();

    /// <summary>WASDキーでカメラを前後左右に平行移動する</summary>
    static void Move();

    /// <summary>マウス移動量に応じてカメラの注視点を回転する</summary>
    static void Rotate();

    /// <summary>マウスホイールでカメラを前後方向にズームする</summary>
    static void Zoom();
};
