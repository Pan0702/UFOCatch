#pragma once
#include "../../Common/Object3D.h"

class CDebugCamera : public Object3D
{
public:
    CDebugCamera();
    ~CDebugCamera();

private:
    void Update() override;
    void Move();
    void Rotate();
    void Zoom();
};
