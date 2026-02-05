#pragma once
#include "../../Common/Object3D.h"

class CFunShape : public Object3D
{
public:
    CFunShape();
    ~CFunShape();
    void PosSet(const VECTOR3& pos, const float angle)
    {
        transform.position = pos;
        transform.rotation.y = angle;
    };
};
